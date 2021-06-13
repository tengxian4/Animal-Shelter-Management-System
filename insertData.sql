INSERT INTO Staff VALUES
('S0000','Nigel','Ng','Admin','Admin','123456','012-6953777','Male','nigelNg@gmail.com','2020-10-31','Active');
INSERT INTO Staff VALUES
('S0001','Ali','Haikal','Owner of shelter','Ali','123456','012-6954527','Male','AliHaikal@gmail.com','2014-01-01','Active');
INSERT INTO Staff VALUES
('S0002','Ronie','Ong','Animal Caretaker','Ron','123456','013-7854527','Female','ronieOng@gmail.com','2019-12-31','Active');
INSERT INTO Staff VALUES
('S0003','Mary','Oh','Cleaner','Mary','123456','013-7852233','Female','','2016-12-28','Active');
INSERT INTO Staff VALUES
('S0004','Housen','Teh','Animal Caretaker','House','123456','016-7852534','Male','','2017-12-31','Left');

INSERT INTO Animal VALUES 
('A000001','Cat','Tuxedo Cat','female','no','2014-11-16',NULL,'Available','S0000',NULL);
INSERT INTO Animal VALUES
('A000002','Dog','Domestic shorthair','male','yes','2014-12-31',NULL,'Available','S0000',NULL);

INSERT INTO Animal VALUES
('A000004','Cat','Cyprus','male','yes','2016-01-12',NULL,'Adopted','S0000',NULL);
INSERT INTO Animal VALUES 
('A000005','Cat','Cyprus','female','yes','2017-12-13',NULL,'Adopted','S0000',NULL);
INSERT INTO Animal VALUES 
('A000006','Cat','American Shorthair','male','yes','2018-01-02',NULL,'Adopted','S0001',NULL);

INSERT INTO Animal VALUES 
('A000007','Dog','Shih Tzu','male','no','2018-01-31','2016-10-22','Available','S0001',NULL);
INSERT INTO Animal VALUES
('A000008','Dog','Kombai dog','female','yes','2020-08-15','2020-01-02','Available','S0002',NULL);
INSERT INTO Animal VALUES
('A000009','Cat','Cyprus','female','yes','2020-08-16','2020-01-05','Available','S0002',NULL),
('A000010','Cat','Cyprus','female','no','2020-09-01',NULL,'Available','S0000',NULL);

INSERT INTO OwnerOfAdoptedAnimal VALUES
('W0001','Serene','Ung','991107-14-5230','female','016-5463232','S0001'),
('W0002','Nurul Aina','binti Zairi','851018-56-4228','female','013-4548756','S0001'),
('W0003','William','Teh','871206-14-6679','male','014-8987171','S0002');

INSERT INTO Adoption VALUES 
('AD0001','W0001','2016-02-03','S0000','A000004'),
('AD0002','W0002','2018-02-14','S0000','A000005'),
('AD0003','W0003','2019-08-12','S0001','A000006');

UPDATE Animal SET adoption_Id='AD0001' WHERE animal_Id='A000004';
UPDATE Animal SET adoption_Id='AD0002' WHERE animal_Id='A000005';
UPDATE Animal SET adoption_Id='AD0003' WHERE animal_Id='A000006';

INSERT INTO Medical VALUES
(0,'have ring worm','Valley Pets Veterinary Clinic',50.00,'2014-11-16','A000001','S0001'),
(0,'Healthy','Valley Pets Veterinary Clinic',30.00,'2014-12-31','A000002','S0002'),
 (0,'Healthy','G Veterinary',90.00,'2016-01-12', 'A000004','S0001'),
 (0,'upper respiratory infections','G Veterinary',150.0,'2017-12-13','A000005','S0001'),
 (0,'finished rabies vaccination','G Veterinary',80.00,'2018-01-02','A000006','S0001'),
  (0,'finished parvovirus vaccination','Valley Pets Veterinary Clinic',80.00,'2018-01-31','A000007','S0002'),
  (0,'finished parvovirus vaccination','Valley Pets Veterinary Clinic',80.00,'2020-08-15','A000008','S0002'),
  (0,'have ring worms', 'G Veterinary',50.00,'2020-08-16','A000009','S0002'),
   (0,'Healthy','Valley Pets Veterinary Clinic',30.00,'2020-12-30','A000010','S0002');
  
 INSERT INTO Medicine VALUES
 (0,'clotrimazole','clotrimazole 1%',1),
 (0,'Clavamox','amoxicillin/clavulanic acid',5),
 (0,'clotrimazole','clotrimazole 1%',9);