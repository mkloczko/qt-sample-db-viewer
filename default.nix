{ stdenv
, lib
, cmake
, ninja
, gcc
, gcovr
, qt6
, gtest
}:

stdenv.mkDerivation rec {
  pname = "sample-viewer";
  version = "0.1.0";
  
  src = ./.;

  nativeBuildInputs = [ qt6.wrapQtAppsHook cmake ninja gcc gcovr ];
  buildInputs = [ qt6.qtbase qt6.qtdeclarative qt6.qtlocation gtest ];

  cmakeFlags = [
    "-DENABLE_TESTING=OFF"
    "-DENABLE_INSTALL=ON"
  ];

  meta = with lib; {
    homepage = "https://github.com/mkloczko/???";
    description = ''
      A small app for querying SQL databases.";
    '';
    licencse = licenses.mit;
    platforms = with platforms; linux;
  };
}
