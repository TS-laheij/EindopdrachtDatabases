CREATE TABLE tblLand
(
    ID int AUTO_INCREMENT NOT NULL,
    Land VARCHAR(20) NOT NULL,
    PRIMARY KEY (ID),
    UNIQUE(Land)
);

CREATE TABLE tblStad
(
    ID int AUTO_INCREMENT NOT NULL,
    Stad VARCHAR(30) NOT NULL,
    IDLand int NOT NULL,
    PRIMARY KEY (ID),
    FOREIGN KEY (IDLand) REFERENCES tblLand(ID),
    UNIQUE(Stad)
);

CREATE TABLE tblMerk
(
    ID int AUTO_INCREMENT NOT NULL,
    IDStad int NOT NULL,
    Merk VARCHAR(15) NOT NULL,
    Logo blob,
    PRIMARY KEY (ID),
    FOREIGN KEY (IDStad) REFERENCES tblStad(ID),
    UNIQUE(Merk)
);

CREATE TABLE tblModel
(
    ID int AUTO_INCREMENT NOT NULL,
    Model VARCHAR(10) NOT NULL,
    IDMerk int NOT NULL,
    PRIMARY KEY (ID),
    FOREIGN KEY (IDMerk) REFERENCES tblMerk(ID),
    UNIQUE(Model)
);

CREATE TABLE tblVermogen
(
    ID int AUTO_INCREMENT NOT NULL,
    PK int NOT NULL,
    PRIMARY KEY (ID),
    UNIQUE(PK)
);

CREATE TABLE tblUitvoering
(
    ID int AUTO_INCREMENT NOT NULL,
    Uitvoering VARCHAR(15) NOT NULL,
    PRIMARY KEY (ID),
    UNIQUE(Uitvoering)
);

CREATE TABLE tblType
(
    ID int AUTO_INCREMENT NOT NULL,
    IDModel int NOT NULL,
    IDUitvoering1 int,
    IDUitvoering2 int,
    IDUitvoering3 int,
    IDVermogen int NOT NULL,
    FOREIGN KEY (IDModel) REFERENCES tblModel(ID),
    FOREIGN KEY (IDUitvoering1) REFERENCES tblUitvoering(ID),
    FOREIGN KEY (IDUitvoering2) REFERENCES tblUitvoering(ID),
    FOREIGN KEY (IDUitvoering3) REFERENCES tblUitvoering(ID),
    FOREIGN KEY (IDVermogen) REFERENCES tblVermogen(ID),
    PRIMARY KEY (ID)
);

CREATE TABLE tblDUMP(
ID INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
merk VARCHAR(45),
model VARCHAR(45),
uitvoering1 VARCHAR(45),
uitvoering2 VARCHAR(45),
uitvoering3 VARCHAR(45),
vermogenPK VARCHAR(45),
vermogenKW VARCHAR(45),
stad VARCHAR(45),
land VARCHAR(45)
);

LOAD DATA INFILE 'NFS.csv'
INTO TABLE tbldump
FIELDS TERMINATED BY ','
ENCLOSED BY '"'
LINES TERMINATED BY '\n'
IGNORE 1 ROWS;

INSERT IGNORE INTO tblland(land) 
SELECT DISTINCT land FROM tbldump;

INSERT IGNORE INTO tblstad(IDLand, Stad) 
SELECT DISTINCT tblland.ID, tbldump.stad 
FROM tbldump 
INNER JOIN tblland 
ON tblland.Land = tbldump.land;

INSERT IGNORE INTO tblmerk(IDStad, Merk)
SELECT DISTINCT tblstad.ID, tbldump.merk
FROM tbldump
INNER JOIN tblstad
ON tblstad.stad = tbldump.stad;

INSERT IGNORE INTO tblModel(Model, IDMerk)
SELECT DISTINCT tbldump.Model, tblMerk.ID
FROM tbldump
INNER JOIN tblMerk
ON tblMerk.merk = tblDump.merk;

INSERT IGNORE INTO tblvermogen(PK)
SELECT tbldump.vermogenPK
FROM tbldump;

INSERT IGNORE INTO tblUitvoering(Uitvoering)
SELECT tbldump.uitvoering1
FROM tbldump;

INSERT IGNORE INTO tblUitvoering(Uitvoering)
SELECT tbldump.uitvoering2
FROM tbldump;

INSERT IGNORE INTO tblUitvoering(Uitvoering)
SELECT tbldump.uitvoering3
FROM tbldump;

INSERT INTO tbltype(IDModel, IDUitvoering1, IDUitvoering2, IDUitvoering3, IDVermogen)
SELECT tblmodel.ID, eerste.id, tweede.id, derde.id, tblvermogen.ID
FROM tbldump
INNER JOIN tblmodel
ON tbldump.model = tblmodel.model
INNER JOIN tbluitvoering eerste
ON tbldump.uitvoering1 = eerste.Uitvoering
INNER JOIN tbluitvoering tweede
ON tbldump.uitvoering2 = tweede.uitvoering
INNER JOIN tbluitvoering derde
ON tbldump.uitvoering3 = derde.uitvoering
INNER JOIN tblvermogen
ON tbldump.vermogenPK = tblvermogen.PK;

#This view will show all the car brands in the QT application

CREATE VIEW showMerk AS SELECT tblmerk.Merk FROM tblmerk;

# View to show car model and specifications
# In QT, use this view in combination with:
# WHERE showmodel.merk LIKE "Mercedes-Benz"


CREATE VIEW showModel AS
SELECT tblmerk.merk AS merk, CONCAT(tblmodel.Model, " ", eerste.Uitvoering, " ", tweede.Uitvoering , " ", derde.Uitvoering ) AS "Automodellen"
FROM tblmodel
INNER JOIN tbltype
ON tbltype.IDModel = tblmodel.ID
INNER JOIN tbluitvoering eerste
ON tbltype.IDUitvoering1 = eerste.ID
INNER JOIN tbluitvoering tweede
ON tbltype.IDUitvoering2 = tweede.ID
INNER JOIN tbluitvoering derde
ON tbltype.IDUitvoering3 = derde.ID
LEFT JOIN tblmerk
ON tblmerk.ID = tblmodel.IDMerk;

# View to show location of brand
# In QT, use in combination with:
# WHERE showlocatie.Merk LIKE "audi"

CREATE VIEW showLocatie AS
SELECT tblMerk.Merk, CONCAT("Het hoofdkantoor van ", tblMerk.merk, "  is gevestigd in: ", tblstad.stad, ", ", tblland.land) AS locatieZin
FROM tblMerk
INNER JOIN tblstad
ON tblmerk.IDStad = tblstad.ID
INNER JOIN tblland 
ON tblland.ID = tblstad.IDLand;

# View to show PK or KW of brand
# In QT, use in combination with:
# WHERE showvermogenmodel.Automodellen = "audi"

CREATE VIEW showVermogenModel AS
SELECT CONCAT(tblmodel.Model, " ", eerste.Uitvoering, " ", tweede.Uitvoering , " ", derde.Uitvoering ) AS "Automodellen", tblvermogen.PK
FROM tblmodel
INNER JOIN tbltype
ON tbltype.IDModel = tblmodel.ID
INNER JOIN tbluitvoering eerste
ON tbltype.IDUitvoering1 = eerste.ID
INNER JOIN tbluitvoering tweede
ON tbltype.IDUitvoering2 = tweede.ID
INNER JOIN tbluitvoering derde
ON tbltype.IDUitvoering3 = derde.ID
INNER JOIN tblvermogen
ON tbltype.IDVermogen =tblvermogen.ID







