USE task;

INSERT INTO locations 
VALUES 
    ("8.8.8.8", 30,NULL),
    ("netia.pl", NULL, 50),
    ("polsat.pl", 20,30),
    ("upc.pl", 10, -30),
    ("hey.pl", 20,-10),
    ("yahoo.com", NULL, NULL),
    ("google.pl", 37.419158935547, -122.075408935547),
    ("hostname.pl", 14.000000000000, 55.000000000000),
    ("norsk.no", 55.680511474609, 12.587829589844),
    ("reddit.com", 37.767848968506, -122.392860412598),
    ("woohoo.com", 40.378250122070, -74.662208557129);

SELECT * FROM locations;
