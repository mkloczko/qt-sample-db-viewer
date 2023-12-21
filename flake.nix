{
  inputs = { nixpkgs.url = "github:nixos/nixpkgs/nixos-23.11"; };

  outputs = inputs@{ flake-parts,... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [
        "x86_64-linux" "aarch64-linux" "aarch64-darwin" "x86_64-darwin"
      ];
      perSystem = {config, self', inputs', pkgs, system, ...}: 
        let concatStrings = pkgs.lib.strings.concatStrings;
            dev_pkgs = with pkgs; [mariadb cmake ninja qtcreator ccls gcc gcovr];
            qt_pkgs = with pkgs.qt6; [qtbase qtdeclarative qtlocation];
            maria_pkgs = with pkgs; [mariadb-connector-c gtest];
            isDir = pkgs.top-level.impure.isDir;
        in {
          packages.default = pkgs.callPackage ./default.nix {};
          devShells.default =
            pkgs.mkShell rec { 
                nativeBuildInputs = dev_pkgs;
                inputsFrom = [self'.devShells];
                packages = qt_pkgs ++ maria_pkgs;
                buildInputs = qt_pkgs ++ maria_pkgs;
                # Database related scripts.
                db_path = "/tmp/test-db"; #Could use mktemp
                db_socket = "mariasocket";
                db_socket_path = concatStrings [db_path "/" db_socket];
                db_exists = builtins.pathExists (db_path + "/.");
                createDb_cmd = ''
                    if [ ! -d ${db_path} ];
                    then
                        mariadb-install-db --datadir="${db_path}" --socket="${db_socket_path}"
                    fi
                '';
                connectDb_cmd = ''

                    function shutdownDB(){
                        echo "Turning the database off, calling mysqladmin --skip-password -p shutdown --socket=${db_socket_path}"
                        echo "It seems any password works."
                        mysqladmin --skip-password -p shutdown --socket=${db_socket_path}
                    }

                    function connected(){
                        mysql --socket=${db_socket_path} -e "" &> /dev/null
                        return $?
                    }

                    function start(){
                        trap shutdownDB EXIT
                        sleep 1
                        mariadbd-safe --no-defaults --datadir="${db_path}" --socket=${db_socket} &
                    }

                    if ! connected;
                    then
                        start
                    fi

                '';
                populateDb_cmd = ''
                    if [ ! -d ${db_path} ];
                    then
                        sleep 1
                        mysql --socket "${db_socket_path}" < init_db.sql
                        mysql --socket "${db_socket_path}" < populate_db.sql
                    fi
                '';
                
                joined_cmd = pkgs.lib.strings.concatStrings [createDb_cmd connectDb_cmd populateDb_cmd];
                db_cmd = if db_exists then connectDb_cmd else joined_cmd;

                # Without this mariadb plugin for Qt won't work.
                MARIADB_LIB="${pkgs.mariadb-connector-c}/lib/mariadb";
                shellHook = ''
                    export LD_LIBRARY_PATH=${MARIADB_LIB}:$LD_LIBRARY_PATH
                '' + db_cmd;
            };
        };
      

};

}
