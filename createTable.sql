ALTER TABLE Animal
DROP CONSTRAINT Animal_Adoption_FK;	
ALTER TABLE Adoption
DROP CONSTRAINT Adoption_Animal_FK ;
DROP TABLE Adoption;
DROP TABLE OwnerOfAdoptedAnimal;
DROP TABLE Medicine;
DROP TABLE Medical;
DROP TABLE Animal;
DROP TABLE Staff;

CREATE TABLE Staff
(
	staff_Id VARCHAR(5),
	firstName VARCHAR(255),
	lastName VARCHAR(255),
	position VARCHAR(255) NOT NULL,
	username VARCHAR(255) NOT NULL,
	password VARCHAR(255) NOT NULL,
	hpNumber VARCHAR(20) NOT NULL,
	gender VARCHAR(6),
	email VARCHAR(255),
	dateStartWorking DATE,
	status VARCHAR(255),
	
	PRIMARY KEY(staff_Id),
	CONSTRAINT Staff_username UNIQUE (username)
	
);



CREATE TABLE OwnerOfAdoptedAnimal
(
	owner_Id VARCHAR(5),
	firstName VARCHAR(255),
	lastName VARCHAR(255),
	IcNumber VARCHAR(14) NOT NULL,
	gender VARCHAR(6),
	hpNumber VARCHAR(20) NOT NULL,
	staff_Id VARCHAR(5) NOT NULL,
	
	PRIMARY KEY(owner_Id),
	CONSTRAINT OwnerOfAdoptedAnimal_Staff FOREIGN KEY(staff_Id)
	REFERENCES Staff(staff_Id)
);

CREATE TABLE Adoption 
(
	adoption_Id VARCHAR(6),
	owner_Id VARCHAR(5) NOT NULL,
	
	adoptDate DATE NOT NULL,
	staff_Id VARCHAR(5) NOT NULL,
	
	PRIMARY KEY(adoption_Id),
	CONSTRAINT Adoption_OwnerOfAdoptedAnimal_FK FOREIGN KEY(owner_Id)
	REFERENCES OwnerOfAdoptedAnimal(owner_Id),
	CONSTRAINT Adoption_Staff_FK FOREIGN KEY(staff_Id)
	REFERENCES Staff(staff_Id)
);

CREATE TABLE Animal
(
	animal_Id VARCHAR(7),
	type VARCHAR(3) NOT NULL,
	breed VARCHAR(255),
	gender VARCHAR(6) NOT NULL,
	sterilized VARCHAR(3),
	dateCameIn DATE NOT NULL,
	dateBorn DATE,
	status VARCHAR(255) NOT NULL,
	staff_Id VARCHAR(5)NOT NULL,
	
	PRIMARY KEY (animal_Id),
	CONSTRAINT Animal_Staff_FK FOREIGN KEY(staff_Id)
	REFERENCES Staff(staff_Id)

);

ALTER TABLE Adoption 
ADD (animal_Id VARCHAR(7) NOT NULL),
ADD CONSTRAINT Adoption_Animal_FK FOREIGN KEY(animal_Id)
REFERENCES Animal(animal_Id);

ALTER TABLE Animal
ADD (adoption_Id VARCHAR(6)),
ADD CONSTRAINT Animal_Adoption_FK FOREIGN KEY(adoption_Id)
REFERENCES Adoption(adoption_Id);

CREATE TABLE Medical
(
	medical_Id int AUTO_INCREMENT,
	medicalCondition VARCHAR(255),
	veterinaryPlace VARCHAR(255),
	costs DOUBLE,
	date DATE,
	animal_Id VARCHAR(7) NOT NULL,
	staff_Id VARCHAR(5) NOT NULL,

	PRIMARY KEY(medical_Id),
	CONSTRAINT Medical_Animal_FK FOREIGN KEY (animal_Id) 
	REFERENCES Animal(animal_Id),
	CONSTRAINT Medical_Staff_FK FOREIGN KEY (staff_Id)
	REFERENCES Staff(staff_Id)
);

CREATE TABLE Medicine
(
	medicine_Id int AUTO_INCREMENT,
	medicineName VARCHAR(255) NOT NULL,
	ingredients VARCHAR(255) NOT NULL,
	medical_Id int,
	
	PRIMARY KEY(medicine_Id),
	CONSTRAINT Medical_Medicine_FK FOREIGN KEY(medical_Id)
	REFERENCES Medical(medical_Id)
);
