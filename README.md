# Database-related Qt sample code

A sample code related to using Qt for database management, with additional API callbacks for obtaining data. The project uses `MariaDB`. The database stores hostname/addresses and geographical coordinates.

When using `nix` flakes all the dependencies are obtained for you. From Qt6 we need base, declarative for Qml and location modules. You might also need to compile correct Sql plugins for Qt.

# Building and running

The guide will describe how to use the project with [nix](https://nixos.org) package manager with flakes enabled.

```bash
$ nix develop
```

This will download all the dependencies and run the database. If the command is ran for the first time, the database will be created the help of local scripts. The `trap` command used in nix's devShelll will shutdown the database when exiting the shell.

The database is located at `/tmp/test-db/`, with the socket at `/tmp/test-db/mariasocket`.

You can either build the project using QtCreator or by using `cmake`. You can also run:

```bash
$ nix run
```

This will build the program and run it. If ran without `nix develop`, the Qt Sql plugins might not know where to look for `mariadb-connector-c` libraries. The `nix develop` sets LD_LIBRARY_PATH which fixes the problem.

# Usage

The default connection settings are already set - just click on the **Connect** button to connect to the local database. The program uses `QSqlTableModel` as a model for the Qml `TableView`, which allows us to revert and submit changes between the view and the database. Changes are usually cached and sent only by pressing the **Submit** button, unless **Autoupdate** checkbox was checked.

You will also need an access key from [IpStack](https://ipstack.com) if you intend to get the location data through their API.

Rows can be selected by using the checkboxes under the `Sel.` column. The checkboxes are actually a custom vertical header of the table model. These will go out of sync when doing changes to the location names or when adding or removing new entries. You can deselect all checkboxes by clicking on `Sel.`.
