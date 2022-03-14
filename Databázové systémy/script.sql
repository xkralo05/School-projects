----- xkralo05_xvalko11 -----
-- MAZANIE --
DROP INDEX index_druh_kc;
DROP TABLE Rodina CASCADE CONSTRAINTS;
DROP TABLE Clen CASCADE CONSTRAINTS;
DROP TABLE KriminalnaCinnost CASCADE CONSTRAINTS;
DROP TABLE Uzemie CASCADE CONSTRAINTS;
DROP TABLE StretnutieDonov CASCADE CONSTRAINTS;
DROP TABLE DonBolPozvany CASCADE CONSTRAINTS;
DROP TABLE Vrazda CASCADE CONSTRAINTS;
DROP SEQUENCE noveID ;

-- Vytvorenie Tabuliek --

CREATE TABLE Rodina (
    IDrodiny INT NOT NULL PRIMARY KEY,
    MenoRodiny VARCHAR(255) NOT NULL,
    PocetClenov INT NOT NULL,
    MenoDona VARCHAR(255) NOT NULL,
    VekDona INT NOT NULL
    check(VekDona >=18),
    VelkostTopanok INT NOT NULL
    check(VelkostTopanok>20),
    FarbaOciDona VARCHAR(250) NOT NULL,
    PocetAut INT NOT NULL
    check(PocetAut >= 0),
    PocetNehnutelnosti INT NOT NULL
    check(PocetNehnutelnosti >= 0),
    SumaPenazi NUMBER NOT NULL,
    Spojenci VARCHAR(255)
);
CREATE TABLE Uzemie(
    IDuzemia INT PRIMARY KEY,
    Rozloha NUMBER NOT NULL,
    check(Rozloha>0),
    Dlzka DECIMAL(9,6) NOT NULL,
    Sirka DECIMAL(8,6) NOT NULL,
    Adresa  VARCHAR(255) NOT NULL,
    MajitelUzemia NUMBER,
    CONSTRAINT Vlastni FOREIGN KEY (MajitelUzemia) REFERENCES Rodina(IDrodiny) ON DELETE CASCADE
);
CREATE TABLE KriminalnaCinnost(
    IDkriminalnejCinnosti INT NOT NULL PRIMARY KEY,
    DatumZaciatkuCinnosti DATE NOT NULL,
    DatumKoncaCinnosti DATE NOT NULL,
    Druh VARCHAR(255) NOT NULL,
    IDrodinyV INT NOT NULL,
    IDuzemiaV INT NOT NULL,
    CONSTRAINT Vedie FOREIGN KEY (IDrodinyV) REFERENCES Rodina(IDrodiny) ON DELETE CASCADE,
    CONSTRAINT PrebiehaNa FOREIGN KEY (IDuzemiaV) REFERENCES UZEMIE(IDuzemia) ON DELETE CASCADE
);

CREATE TABLE Clen (
    RCclena CHAR(11) NOT NULL PRIMARY KEY,
  --  CHECK (REGEXP_LIKE(RCclena,'^\d{2}((0[1-9]|1[012])|(5[1-9]|6[012]))(0[1-9]|[12][0-9]|3[01])/([0-9]{2,4})$','i')),
    Vek INT NOT NULL
    check ( Vek > 0 ),
    Meno  VARCHAR(255) NOT NULL,
    Ulica  VARCHAR(255) NOT NULL,
    CisloDomu INT NOT NULL
    check( CisloDomu > 0),
    PSC INT NOT NULL,
    Pohlavie VARCHAR(255) NOT NULL,
    IDrodinyC INT NOT NULL,
    IDkriminalnejCinnostiV INT NOT NULL,
    CONSTRAINT Vykonava FOREIGN KEY (IDkriminalnejCinnostiV) REFERENCES KriminalnaCinnost(IDkriminalnejCinnosti) ON DELETE CASCADE ,
    CONSTRAINT PatriDo FOREIGN KEY (IDrodinyC) REFERENCES Rodina(IDrodiny) ON DELETE CASCADE
);

CREATE TABLE StretnutieDonov(
    IDstretnutia INT NOT NULL PRIMARY KEY,
    Datum DATE NOT NULL ,
    Cas DECIMAL(4,2) NOT NULL,
    Hostitel VARCHAR(255) NOT NULL,
    IDuzemiaP INT NOT NULL ,
    CONSTRAINT StretnutiePrebiehaNa FOREIGN KEY (IDuzemiaP) REFERENCES Uzemie(IDuzemia) ON DELETE CASCADE
);
CREATE TABLE Vrazda(
    IDkriminalnejCinnosti INT NOT NULL PRIMARY KEY REFERENCES KriminalnaCinnost(IDkriminalnejCinnosti) ON DELETE CASCADE,
    Obet VARCHAR(255) NOT NULL
);
CREATE TABLE DonBolPozvany(
    IDrodinyS NUMBER NOT NULL,
    IDstretnutiaD NUMBER NOT NULL,
    Potvrdil VarChar(3) NOT NULL,
    CONSTRAINT StretRodina PRIMARY KEY (IDstretnutiaD,IDrodinyS),
    CONSTRAINT Stretnutie FOREIGN KEY (IDstretnutiaD) REFERENCES StretnutieDonov(IDstretnutia) ON DELETE CASCADE ,
    CONSTRAINT Potvrdenie FOREIGN KEY (IDrodinyS) REFERENCES Rodina(IDrodiny) ON DELETE CASCADE
);

-- triger na automaticke pridelovanie hodnoty primarneho kluca
-- triger prideluje hodnoty ID vacsie ako 100 a pre kazdu novu hodnotu inkrementuje nove ID o 1
CREATE SEQUENCE noveID
START WITH 1000
INCREMENT BY 1;

CREATE OR REPLACE TRIGGER nastavNoveID
    BEFORE INSERT ON Uzemie
    FOR EACH ROW
WHEN (new.IDuzemia is NULL)
    BEGIN
    SELECT noveID.nextval INTO :NEW.IDuzemia FROM dual;
    end;
-- Triger kontroluje spravnost zadaneho rodneho cisla pri vkladani hodnoty do tabulky
CREATE OR REPLACE TRIGGER RodneCisloKontrola
    BEFORE INSERT OR UPDATE OF RCclena ON Clen
    FOR EACH ROW
DECLARE
    rodneCislo Clen.RCclena%TYPE ;
    rok        VARCHAR(2);
    mesiac     VARCHAR(2);
    den        VARCHAR(2);
    koncovka   varchar(4);
    ECP BOOLEAN;
    RCP BOOLEAN;
    Help NUMBER;
BEGIN
    rodneCislo := :NEW.RCclena ;
    rok := substr(rodneCislo,1,2);
    mesiac := substr(rodneCislo,3,2);
    den := substr(rodneCislo,5,2);
IF NOT REGEXP_LIKE(TRIM(rodneCislo),'^[0-9]{6}\/[0-9]*$','i' ) THEN
    raise_application_error(-20203,'V rodnom cisle mozu byt len cislice a znak / ');
end if;

IF ( LENGTH(trim(rodneCislo)) < 10) THEN
    raise_application_error(-20204,'V rodnom cisle mosi byt aspon 9 cislic  / ');
end if;

IF (LENGTH(trim(rodneCislo)) = 10) THEN
    koncovka := substr(rodneCislo,8,3);
    IF(regexp_like(koncovka,'^000$','i'))THEN
        raise_application_error(-20205,'Koncovka pri 9 miestom rodnom cisle nesmie byt 000');
    end if;

end if;

IF (TO_NUMBER(mesiac) > 50) THEN
    mesiac := TO_NUMBER(mesiac) -50 ;
end if;

IF (TO_NUMBER(mesiac) > 20) THEN
    RCP := true;
end if;

IF(TO_NUMBER(den) > 40)THEN
    ECP := true;
    den := TO_NUMBER(den) - 40;
end if;
IF (RCP = TRUE AND ECP = TRUE) THEN
      raise_application_error(-20206,'Neplatny format rodneho cisla');
end if;

IF NOT (REGEXP_LIKE(rodneCislo,'^\d{2}((0[1-9]|1[012])|(5[1-9]|6[012]))(0[1-9]|[12][0-9]|3[01])/([0-9]{2,4})$','i') )THEN
    raise_application_error(-20207,'Neplatny format rodneho cisla');
end if;

IF (LENGTH(rodneCislo) = 10)THEN
    koncovka := substr(rodneCislo,8,3);
    IF(TO_NUMBER(koncovka) <600 AND ECP = TRUE)THEN
        raise_application_error(-20208,'Neplatny format rodneho cisla');
    end if;
end if;

IF (LENGTH(rodneCislo) = 11) THEN
    koncovka := substr(rodneCislo,8,4);
    IF(TO_NUMBER(koncovka) <6000 AND ECP = TRUE )THEN
        raise_application_error(-20209,'Neplatny format rodneho cisla');
    end if;
    Help:= TO_NUMBER(substr(rodneCislo,1,1)) + TO_NUMBER(substr(rodneCislo,2,1)) + TO_NUMBER(substr(rodneCislo,3,1)) + TO_NUMBER(substr(rodneCislo,4,1)) + TO_NUMBER(substr(rodneCislo,5,1)) + TO_NUMBER(substr(rodneCislo,6,1)) + TO_NUMBER(substr(rodneCislo,8,1)) + TO_NUMBER(substr(rodneCislo,9,1)) + TO_NUMBER(substr(rodneCislo,10,1)) + TO_NUMBER(substr(rodneCislo,11,1));
    IF  MOD(Help, 11) != 0 THEN
        raise_application_error(-20210,'rodne cislo musi byt delitelne 11');
    end if;
end if;
end;

/

INSERT INTO Rodina(IDrodiny, MenoRodiny, PocetClenov, MenoDona, VekDona, VelkostTopanok, FarbaOciDona, PocetAut, PocetNehnutelnosti, SumaPenazi,Spojenci)
    VALUES('1','Kralovicovci','20','Kristian Kralovic','20','45','Hneda','15','10','5','Monsignor');
INSERT INTO Rodina(IDrodiny, MenoRodiny, PocetClenov, MenoDona, VekDona, VelkostTopanok, FarbaOciDona, PocetAut, PocetNehnutelnosti, SumaPenazi, Spojenci )
    VALUES('2','Krlovicovci','20','Filip Kralovic','20','45','Hneda','15','0','5','');
INSERT INTO Rodina(IDrodiny, MenoRodiny, PocetClenov, MenoDona, VekDona, VelkostTopanok, FarbaOciDona, PocetAut, PocetNehnutelnosti, SumaPenazi, Spojenci )
    VALUES('3','Valkovci','10','Marek Valko','20','42','Zelena','8','4','50000','');

INSERT INTO Uzemie(IDuzemia, Rozloha, Dlzka, Sirka, Adresa,MajitelUzemia)
    VALUES ('1','8000','53,158472','53,195230','Purkyne','1');
INSERT INTO Uzemie(IDuzemia, Rozloha, Dlzka, Sirka, Adresa,MajitelUzemia)
    VALUES ('2','1500','53,15872','53,19230','Brno-Sever','2');
INSERT INTO Uzemie(IDuzemia, Rozloha, Dlzka, Sirka, Adresa,MajitelUzemia)
    VALUES ('3','1900','53,15872','53,19230','Liptovske Revuce','3');
INSERT INTO Uzemie(IDuzemia, Rozloha, Dlzka, Sirka, Adresa,MajitelUzemia)
    VALUES ('4','1900','58,15872','42,19230','Ruzomberok','');
INSERT INTO Uzemie(IDuzemia, Rozloha, Dlzka, Sirka, Adresa,MajitelUzemia)
    VALUES ('5','2000','73,14872','13,190','Palacak','1');
INSERT INTO Uzemie(IDuzemia, Rozloha, Dlzka, Sirka, Adresa,MajitelUzemia)
    VALUES ('6','123','43,1472','65,13730','Brno-Juh','2');
INSERT INTO Uzemie(IDuzemia, Rozloha, Dlzka, Sirka, Adresa,MajitelUzemia)
    VALUES ('7','1235','33,6572','66,1340','Handlova','3');
INSERT INTO Uzemie(IDuzemia, Rozloha, Dlzka, Sirka, Adresa,MajitelUzemia)
    VALUES (NULL,'1235','33,6572','66,1340','Brezno','3');
INSERT INTO Uzemie(IDuzemia, Rozloha, Dlzka, Sirka, Adresa,MajitelUzemia)
    VALUES (NULL,'1200','30,6572','60,1340','Cervinkova','1');

INSERT INTO KriminalnaCinnost(idkriminalnejcinnosti, datumzaciatkucinnosti, datumkoncacinnosti,Druh,IDrodinyV,IDuzemiaV)
    VALUES ('1',date '2020-07-02',date '2020-09-15','Vydieranie','1','2');
INSERT INTO KriminalnaCinnost(idkriminalnejcinnosti, datumzaciatkucinnosti, datumkoncacinnosti,Druh,IDrodinyV,IDuzemiaV)
    VALUES ('2',date '2021-01-02',date '2021-04-03','Predaj drog','1','1');
INSERT INTO KriminalnaCinnost(idkriminalnejcinnosti, datumzaciatkucinnosti, datumkoncacinnosti,Druh,IDrodinyV,IDuzemiaV)
    VALUES ('3',date '2021-01-03',date '2021-01-03','Vrazda','1','2');
INSERT INTO KriminalnaCinnost(idkriminalnejcinnosti, datumzaciatkucinnosti, datumkoncacinnosti,Druh,IDrodinyV,IDuzemiaV)
    VALUES ('4',date '2021-01-03',date '2021-01-03','Vrazda','2','2');
INSERT INTO KriminalnaCinnost(idkriminalnejcinnosti, datumzaciatkucinnosti, datumkoncacinnosti,Druh,IDrodinyV,IDuzemiaV)
    VALUES ('5',date '2021-04-12',date '2021-04-21','Predaj zbrani','3','3');
INSERT INTO KriminalnaCinnost(idkriminalnejcinnosti, datumzaciatkucinnosti, datumkoncacinnosti,Druh,IDrodinyV,IDuzemiaV)
    VALUES ('6',date '2021-03-20',date '2021-03-25','Predaj zbrani','1','1');
INSERT INTO KriminalnaCinnost(idkriminalnejcinnosti, datumzaciatkucinnosti, datumkoncacinnosti,Druh,IDrodinyV,IDuzemiaV)
    VALUES ('7',date '2021-05-02',date '2021-06-03','Predaj drog','2','2');
INSERT INTO KriminalnaCinnost(idkriminalnejcinnosti, datumzaciatkucinnosti, datumkoncacinnosti,Druh,IDrodinyV,IDuzemiaV)
    VALUES ('8',date '2021-07-12',date '2021-08-13','Predaj drog','3','3');

INSERT INTO StretnutieDonov(IDstretnutia, Datum, Cas, Hostitel,IDuzemiaP)
    VALUES('1',date'2021-05-01','16,30','Kristian Kralovic','1');
INSERT INTO StretnutieDonov(IDstretnutia, Datum, Cas, Hostitel,IDuzemiaP)
    VALUES('2',date'2021-06-01','17,30','Kristian Kralovic','2');
INSERT INTO StretnutieDonov(IDstretnutia, Datum, Cas, Hostitel,IDuzemiaP)
    VALUES('3',date'2021-04-21','12,00','Valkovci','3');

INSERT INTO Clen(RCclena, Vek, Meno, Ulica, CisloDomu, PSC, Pohlavie,IDrodinyC,IDkriminalnejCinnostiV)
    VALUES ('970227/1187','20','Augustino Florencia','Bernolakova','255','61200','Muz','1','1');
INSERT INTO Clen(RCclena, Vek, Meno, Ulica, CisloDomu, PSC, Pohlavie,IDrodinyC,IDkriminalnejCinnostiV)
    VALUES ('000516/9100','34','Vito Florencia','Cervinkova','255','61200','Muz','1','1');
INSERT INTO Clen(RCclena, Vek, Meno, Ulica, CisloDomu, PSC, Pohlavie,IDrodinyC,IDkriminalnejCinnostiV)
    VALUES ('740627/9389','34','Mario Pizza','Karlova','5','61200','trans','1','2');
INSERT INTO Clen(RCclena, Vek, Meno, Ulica, CisloDomu, PSC, Pohlavie,IDrodinyC,IDkriminalnejCinnostiV)
    VALUES ('910715/8454','34','Jozef Strach','Stredna Revuca','587','61200','Muz','3','5');
INSERT INTO Clen(RCclena, Vek, Meno, Ulica, CisloDomu, PSC, Pohlavie,IDrodinyC,IDkriminalnejCinnostiV)
    VALUES ('000515/9110','20','Jan Strach','Stredna Revuca','588','13474','Muz','3','5');
INSERT INTO Clen(RCclena, Vek, Meno, Ulica, CisloDomu, PSC, Pohlavie,IDrodinyC,IDkriminalnejCinnostiV)
    VALUES ('030404/0000','34','Peter Mattoni','Cervinkova','577','61200','Muz','1','6');
INSERT INTO Clen(RCclena, Vek, Meno, Ulica, CisloDomu, PSC, Pohlavie,IDrodinyC,IDkriminalnejCinnostiV)
    VALUES ('640111/8435','54','Maria Mattoni','Cervinkova','577','61200','Zena','1','6');
INSERT INTO Clen(RCclena, Vek, Meno, Ulica, CisloDomu, PSC, Pohlavie,IDrodinyC,IDkriminalnejCinnostiV)
    VALUES ('761221/2779','44','Alexander Narkoman','Drogovy brloh','22','97251','Muz','2','7');
INSERT INTO Clen(RCclena, Vek, Meno, Ulica, CisloDomu, PSC, Pohlavie,IDrodinyC,IDkriminalnejCinnostiV)
    VALUES ('720203/5111','47','Pablo Esco','Za rohom','212','97251','Muz','2','7');


INSERT INTO Vrazda(IDkriminalnejCinnosti, Obet)
    VALUES ('3','Mario Nepizza');
INSERT INTO Vrazda(IDkriminalnejCinnosti, Obet)
    VALUES ('4','Mario Vitali');

INSERT INTO DonBolPozvany(IDrodinyS, IDstretnutiaD, Potvrdil)
    VALUES ('1','1','Ano');
INSERT INTO DonBolPozvany(IDrodinyS, IDstretnutiaD, Potvrdil)
    VALUES ('2','1','Ano');

--spojene dve tabulky
-- zobrazi meno a vek vsetkych muzov ktori predavaju zbrane
SELECT Clen.Meno, Clen.Ulica, Clen.CisloDomu, KriminalnaCinnost.IDrodinyV FROM Clen, KriminalnaCinnost WHERE KriminalnaCinnost.IDkriminalnejCinnosti=Clen.IDkriminalnejCinnostiV AND Clen.Pohlavie='Muz' AND KriminalnaCinnost.Druh='Predaj zbrani';
-- zobrazi druh, datum zaciatku a datum konca kriminalnych cinnosti ktore prebiehali/prebiehaju na uzemi s adresou Brno-Sever
SELECT KriminalnaCinnost.Druh,KriminalnaCinnost.DatumZaciatkuCinnosti,KriminalnaCinnost.DatumKoncaCinnosti FROM KriminalnaCinnost,Uzemie WHERE KriminalnaCinnost.IDuzemiaV=Uzemie.IDuzemia AND Uzemie.Adresa='Brno-Sever';

-- spojene 3 tabulky
-- zobrazi meno a vek clenov ktori predavaju drogy a patria k rodine ktora ma viac ako 10 aut
SELECT Clen.Meno, Clen.Vek, KriminalnaCinnost.DatumKoncaCinnosti, Rodina.MenoRodiny FROM Rodina, Clen, KriminalnaCinnost WHERE KriminalnaCinnost.IDkriminalnejCinnosti=Clen.IDkriminalnejCinnostiV  AND KriminalnaCinnost.Druh='Predaj drog' AND Clen.IDrodinyC=Rodina.IDrodiny AND Rodina.PocetAut>10;

--agregacne funkcie
--zobrazi sumu rozlohy vsetkych uzemi kazdej rodiny vratane neutralnych zoradenu podla velkosti
SELECT MajitelUzemia AS ID_rodiny, SUM(Rozloha) AS ROZLOHA_SPOLU FROM Uzemie GROUP BY Uzemie.MajitelUzemia ORDER BY SUM(Uzemie.Rozloha) DESC;

--zobrazi najvyssi vek clena z kazdej rodiny
SELECT Clen.IDrodinyC AS ID_rodiny, MAX(Clen.Vek) AS Vek FROM Clen GROUP BY Clen.IDrodinyC;

--EXISTS
--zobrazi uzemia na ktorych prebieha/prebiehala kriminalna cinnost s vyuzitim EXISTS
SELECT Adresa FROM Uzemie WHERE EXISTS(SELECT KriminalnaCinnost.IDkriminalnejCinnosti FROM KriminalnaCinnost WHERE KriminalnaCinnost.IDuzemiaV = Uzemie.IDuzemia);

--IN
--zobrazi uzemia na ktorych sa nepredavaju drogy s vyuzitim IN
SELECT Adresa FROM Uzemie WHERE Uzemie.IDuzemia NOT IN (SELECT Uzemie.IDuzemia FROM KriminalnaCinnost,
Uzemie WHERE Uzemie.IDuzemia=KriminalnaCinnost.IDuzemiaV AND KriminalnaCinnost.Druh='Predaj drog');

-- Prava
GRANT ALL ON Rodina TO XVALKO11;
GRANT ALL ON Uzemie TO XVALKO11;
GRANT ALL ON Clen TO XVALKO11;
GRANT ALL ON KriminalnaCinnost TO XVALKO11;
GRANT ALL ON StretnutieDonov TO XVALKO11;
GRANT ALL ON DonBolPozvany TO XVALKO11;
GRANT ALL ON Vrazda TO XVALKO11;
GRANT EXECUTE ON clenovia_z_brna TO XVALKO11;
GRANT EXECUTE ON informacie_rodiny TO XVALKO11;


-- View
DROP MATERIALIZED VIEW ClenoviaVek;

CREATE MATERIALIZED VIEW LOG ON Clen WITH PRIMARY KEY,ROWID(Vek)INCLUDING NEW VALUES;

CREATE MATERIALIZED VIEW ClenoviaVek
CACHE
BUILD IMMEDIATE
REFRESH FAST ON COMMIT
ENABLE QUERY REWRITE
AS SELECT C.vek , count(C.vek) AS ClenoviaVek
FROM Clen C
WHERE C.vek > 50
GROUP BY C.vek;

GRANT ALL ON ClenoviaVek TO XVALKO11;

SELECT * from ClenoviaVek;
INSERT INTO Clen(RCclena, Vek, Meno, Ulica, CisloDomu, PSC, Pohlavie,IDrodinyC,IDkriminalnejCinnostiV)
    VALUES ('841217/8904','70','Diler Drog','Drogovy raj','12','97251','Muz','3','8');
COMMIT ;
SELECT * from ClenoviaVek;


CREATE OR REPLACE PROCEDURE clenovia_z_brna AS
    meno Clen.Meno%type;
    ulica Clen.Ulica%type;
    idrodiny Clen.IDrodinyC%type;
    psc Clen.PSC%type;
    CURSOR crsr is SELECT Meno, Ulica, IDrodinyC, PSC FROM Clen;
BEGIN
    IF crsr %ISOPEN THEN
        CLOSE crsr ;
    END IF;
    OPEN crsr;
    DBMS_OUTPUT.PUT_LINE('Zoznam clenov vsekych rodin ktorych psc je 61200 Brno');
    LOOP
         FETCH crsr INTO meno, ulica, idrodiny, PSC;
         EXIT WHEN crsr%notfound;
         IF psc = 61200 THEN
             DBMS_OUTPUT.put_line(meno ||' z rodiny s id '|| idrodiny ||' byva na ulici '|| ulica ||' v meste s PSC 61200');
         END IF;
    END LOOP;
    CLOSE crsr;
END;

CREATE OR REPLACE PROCEDURE informacie_rodiny(nazov_rodiny Rodina.MenoRodiny%TYPE) AS
    nazov Rodina.MenoRodiny%TYPE;
    clenovia Rodina.PocetClenov%TYPE;
    auta Rodina.PocetAut%TYPE;
    meno_dona Rodina.MenoDona%TYPE;
    nehnutelnosti Rodina.PocetNehnutelnosti%TYPE;
BEGIN
    SELECT  Rodina.MenoRodiny, Rodina.PocetClenov, Rodina.PocetAut, Rodina.PocetNehnutelnosti, Rodina.MenoDona INTO nazov, clenovia, auta, nehnutelnosti, meno_dona FROM Rodina WHERE Rodina.MenoRodiny = nazov_rodiny;
    DBMS_OUTPUT.put_line('Informacie o rodine '|| nazov_rodiny);
    DBMS_OUTPUT.put_line('Pocet clenov : '|| clenovia);
    DBMS_OUTPUT.put_line('Pocet aut : '|| auta);
    DBMS_OUTPUT.put_line('Pocet nehnutelnosti : '|| nehnutelnosti);
    DBMS_OUTPUT.put_line('Meno dona : '|| meno_dona);
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            DBMS_OUTPUT.put_line('Rodina s nazvom ' || nazov_rodiny || 'neexistuje');
END;

CALL clenovia_z_brna();
CALL informacie_rodiny('Kralovicovci');



EXPLAIN PLAN FOR
SELECT K.IDkriminalnejCinnosti,K.Druh,K.DatumZaciatkuCinnosti, K.DatumKoncaCinnosti, COUNT(*) Pocet_drogovych_dilerov
FROM Clen, KriminalnaCinnost K
WHERE K.Druh = 'Predaj drog' AND Clen.PSC = 97251 AND K.IDkriminalnejCinnosti = Clen.IDkriminalnejCinnostiV
GROUP BY K.IDkriminalnejCinnosti, K.Druh, K.DatumZaciatkuCinnosti, K.DatumKoncaCinnosti;
SELECT * FROM TABLE(dbms_xplan.display);

CREATE INDEX index_druh_kc ON KriminalnaCinnost(Druh);
EXPLAIN PLAN FOR
SELECT K.IDkriminalnejCinnosti,K.Druh,K.DatumZaciatkuCinnosti, K.DatumKoncaCinnosti, COUNT(*) Pocet_drogovych_dilerov
FROM Clen, KriminalnaCinnost K
WHERE K.Druh = 'Predaj drog' AND Clen.PSC = 97251 AND K.IDkriminalnejCinnosti = Clen.IDkriminalnejCinnostiV
GROUP BY K.IDkriminalnejCinnosti, K.Druh, K.DatumZaciatkuCinnosti, K.DatumKoncaCinnosti;
SELECT * FROM TABLE(dbms_xplan.display);