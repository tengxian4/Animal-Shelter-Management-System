#include <iostream>
#include <sstream>
#include <algorithm>
using namespace std;

#include <mysql/jdbc.h>
using namespace sql;

#include "DbConn.h"
#include "Animal.h"
#include "AnimalManager.h"
#include "MedicalManager.h"

#include "Utils.h"
#include "Login.h"

 AnimalManager::AnimalManager(string currentUser) {
	this->currentUser = currentUser;
	
}
void AnimalManager::displayAllAnimal() {
	DbConn dbCon;
	PreparedStatement* pstat = dbCon.prepareStatement("SELECT * FROM Animal");

	ResultSet* rs = pstat->executeQuery();

	vector <string> columnName(9);

	columnName = { "Animal Id","Type","Breed             ","Gender","Sterilized","Date Came In","Date Born ","Status   "};

	int a = 1;
	for (int i = 0;i < columnName.size();i++) //find the column size
	{
		a += columnName[i].size();
		a += 1;
	}
	cout << setw(a) << setfill('-') << "-" << " \n" << "|";//need | with a space behind
	
	for (int i = 0;i < columnName.size();i++)//print column name
	{
		cout << setfill(' ') << setw(columnName[i].size()) << left << columnName[i] << "|";
	}

	cout << setw(a) << setfill('-') << "\n" << "\n"; //this setw and setfill is from right side then one \n is for line before 

	while (rs->next())
	{
		int i = 0;
		while (i < columnName.size())//print the data
		{
			
			cout << "|" << setfill(' ') << setw(columnName[i].size()) << left << rs->getString(i + 1);

			i++;
			if (i == columnName.size())
			{
				cout << "|\n";
				//or use this cout << "|";cout << setw(a) << setfill('-') << "\n" << "\n";?
			}
		}
	}
	delete rs;
	delete pstat;


}

void AnimalManager::viewAnimal() {
	
	
	
	Animal animal;
	Utils utils;
	vector <string> items = animal.items();
	vector<string> attribute = animal.attributesInTable();
	
	
	int choice = 0,noItem;
	
	string findItem;
	
	while (choice != (animal.getTotalSearchItem() + 1))
	{
		system("cls");
		cout << "\n\tAnimal Management Page: Submenu 1\n";
		cout << "\tView and Edit Animal Page\n";
		cout << "\nWhich animal you wish to view or edit?\n";
		cout << "Please enter the choice number to select the animal with one of the following specific detail.\n";
		for (int i = 0;i < animal.getTotalSearchItem() ;i++)//first three item
		{
			cout << "\t" << i + 1 << ". " << animal.getSearchItem()[i] << endl;
		}

		cout << "\n\n\t" << animal.getTotalSearchItem()+1 << ". " << "Go Back Previous Page" << endl;

		choice = utils.inputChoice(1, (animal.getTotalSearchItem() + 1),true);
		if (choice == (animal.getTotalSearchItem() + 1))
			continue;
	
		system("cls");
		

		
		//viewColumnData(choice);

		cout << "\nPlease enter the " << animal.getSearchItem()[choice-1] <<" : ";
		getline(cin, findItem);
	
		Animal* foundAnimal=NULL;
		int index = animal.getIndexSItem()[choice - 1];
		foundAnimal = selectAnimal(attribute[index],findItem,noItem);
		
		if (foundAnimal == NULL)
		{
			cout << "Invalid animal's detail\n";
			system("pause");
			system("cls");
		
		}
		else
		{
			Animal* temp = foundAnimal;
			
			while (temp != NULL)
			{

				cout << "\n\nAnimal ID     : " << temp->animal_Id << endl;
				
				cout << "Type          : " << temp->type << endl;
				cout << "Breed         : " << temp->breed << endl;
				cout << "Gender        : " << temp->gender << endl;
				cout << "Sterilized    : " << temp->sterilized << endl;
				cout << "Date Came In  : " << temp->dateCameIn << endl;
				cout << "Date Born     : " << temp->dateBorn << endl;
				cout << "Status        : " << temp->status << endl;
				cout << "Staff ID who  : " << temp->staff_Id << endl << "inserted this \nanimal \n";
				cout << "Adoption ID   : " << temp->adoption_Id << endl;
				temp = temp->next;
			}
			
			
				
			if (noItem == 1)
			{


				MedicalManager medicalManager;
				medicalManager.viewMedical(foundAnimal->animal_Id);

				string checkStatus = foundAnimal->status;
				transform(checkStatus.begin(), checkStatus.end(), checkStatus.begin(), toupper);
				if (checkStatus == "ADOPTED")
				{
					cout << "\n\tThis is adopted animal. No edit or add medical details available\n";
					system("pause");
				}
				else
				{
					cout << "\n\t1. Edit animal details\n";
					cout << "\t2. Add Medical details\n";
					cout << "\t3. Go Back Animal Management Main Page\n";
					int decision = utils.inputChoice(1, 3, true);


					system("cls");


					if (decision == 1)
					{

						int status = editAnimal(foundAnimal->animal_Id);

						if (status != 0)
							cout << "Successfully edited." << endl;
						else
							cout << "Fail to edit details " << endl;
						system("pause");
					}
					else if (decision == 2)
					{
						int status = medicalManager.insertMedical(foundAnimal->animal_Id, currentUser);
						if (status != 0)
							cout << "Successfully inserted medical details." << endl;
						else
							cout << "Fail to insert medical details " << endl;
						system("pause");
					}
					else if (decision == 3) {
						break;
					}
				}
			}
			else {
				cout << "\nTotal " << noItem << " animal found " << endl;
				cout << "\nIf you wish to edit, you need to search it using Animal Id and edit it.\n";
				system("pause");
			}
		}
		
	}
	
}
Animal* AnimalManager::selectAnimal(string attribute,string findItem,int& noItem)
{
	Animal* pNew=NULL;
	Animal* pLast;
	Animal* pHead=NULL;
	noItem = 0;
	DbConn dbCon;
	transform(findItem.begin(), findItem.end(), findItem.begin(), toupper);

	string stat = "SELECT * FROM Animal WHERE UPPER(" + attribute + ") =\'" + findItem+"\'";
	PreparedStatement* pstat = dbCon.prepareStatement(stat);
	/*pstat->setString(1, attribute);
	pstat->setString(2, findItem);*/
	ResultSet* rs = pstat->executeQuery();


	while (rs->next())
	{
		
		pNew = new Animal();
		pNew->animal_Id = rs->getString(1);
		
		pNew->type = rs->getString(2);
		pNew->breed = rs->getString(3);
		pNew->gender = rs->getString(4);
		pNew->sterilized = rs->getString(5);
		pNew->dateCameIn = rs->getString(6);
		pNew->dateBorn = rs->getString(7);
		pNew->status = rs->getString(8);
		pNew->staff_Id = rs->getString(9);
		pNew->adoption_Id = rs->getString(10);
		pNew->next = NULL;

		if (pHead == NULL)
		{
			pHead = pNew;
		}
		else
		{
			pLast = pHead;
			while (pLast->next != NULL)
				pLast = pLast->next;

			pLast->next = pNew;
		}
		
		noItem++;

	}
	
	delete rs;
	delete pstat;
	
	return pHead;

}
int AnimalManager::editAnimal(string animal_Id) {
	
	Utils utils;
	
	Animal animal;
	
	vector<string> editDetail = animal.items();
	vector<string> attribute = animal.attributesInTable();
	cout << "\nWhich details you wish to edit?\n";

	for (int i = 1;i < editDetail.size();i++)//start with 1 because animal Id cannot change
	{
		cout << "\n\t" << i << "." << editDetail[i];
	}

	cout << "\n\n\t" << editDetail.size() << ". " << "Go Back Previous Page" << endl;
	int choice = 0, status;
	choice = utils.inputChoice(1, editDetail.size(),true);

	if (choice == editDetail.size())
		return 0;
	string updateDetail;

	bool date=utils.dateFormat(editDetail[choice]);
	/*cin.clear();
	cin.ignore(INT_MAX, '\n');25.12.2020*/

	cout << "Now we will update " << editDetail[choice] << " for animal with animal Id of " << animal_Id << endl;
	if (!date)
	{
		cout << "Enter the new detail to update : ";
		getline(cin, updateDetail);
		if (editDetail[choice] == "Sterilized")
		{
			string checkSterilized = updateDetail;
			transform(checkSterilized.begin(), checkSterilized.end(), checkSterilized.begin(), toupper);
			if (checkSterilized != "YES" && checkSterilized != "NO") {
				cout << "Sterilized only accept Yes or No"<<endl;
				return 0;
			}
		}
		else if (editDetail[choice] == "Type") {
			string checkCatDog = updateDetail;
			transform(checkCatDog.begin(), checkCatDog.end(), checkCatDog.begin(), toupper);
			if (checkCatDog != "CAT" && checkCatDog != "DOG") {
				cout << "\nThe animal type is cat and dog only." << endl;
				return 0;
			}

		
		}
		else if (editDetail[choice] == "Gender") {
			string checkGender = updateDetail;
			transform(checkGender.begin(), checkGender.end(), checkGender.begin(), toupper);
			if (checkGender != "FEMALE" && checkGender != "MALE") {
				cout << "\nOnly female and male is accepted" << endl	;
				return 0;
			}
		}
		else if (editDetail[choice] == "Status") {
			if (updateDetail == "")
			{
				cout << "\nUpdate detail is required" << endl;
				return 0;
			}
		}

	
	}
	else
	{
		if (editDetail[choice] == "Date born") {
			bool valid;
			for (int i = 0;i < 4;i++)
			{
				cout << "Enter the new detail to update : ";
				getline(cin, updateDetail);
				valid = utils.validDate(updateDetail,false);
				if (valid)
					break;
				else
					cout << "\nPlease enter again. You have " << 3 - i << " times of try left. \n";
			}
			if (!valid) {
				cout << "\nWrong date format. If you want to edit, you can start again the View and Edit Animal\n";
				system("pause");
				return 0;
			}
			DbConn dbConn;
			PreparedStatement* beforeCameIn = dbConn.prepareStatement("SELECT timestampdiff(day,?,dateCameIn) FROM animal WHERE animal_Id=?");
			beforeCameIn->setString(1, updateDetail);
			beforeCameIn->setString(2, animal_Id);

			ResultSet* rs = beforeCameIn->executeQuery();
			rs->next();
			int day = rs->getInt(1);
			delete rs;
			delete beforeCameIn;
			if (day < 0) {
				cout << "\nIt is impossible for the animal to come into the animal shelter first before born." << endl;
				cout << "\nWrong date. If you want to edit, you can start again the View and Edit Animal\n";
				return 0;
			}
		}
		else
		{
			bool valid;
			for (int i = 0;i < 4;i++)
			{
				cout << "Enter the new detail to update : ";
				getline(cin, updateDetail);
				valid = utils.validDate(updateDetail);
				if (valid)
					break;
				else
					cout << "\nPlease enter again. You have " << 3 - i << " times of try left. \n";
			}
			if (!valid) {
				cout << "\nWrong date format. If you want to edit, you can start again the View and Edit Animal\n";
				system("pause");
				return 0;
			}
			DbConn dbConn;
			PreparedStatement* beforeCameIn = dbConn.prepareStatement("SELECT timestampdiff(day,dateBorn,?) FROM animal WHERE animal_Id=?");
			beforeCameIn->setString(1, updateDetail);
			beforeCameIn->setString(2, animal_Id);

			ResultSet* rs = beforeCameIn->executeQuery();
			rs->next();
			int day = rs->getInt(1);
			delete rs;
			delete beforeCameIn;
			if (day < 0) {
				cout << "\nIt is impossible for the animal to come into the animal shelter first before born." << endl;
				cout << "\nWrong date. If you want to edit, you can start again the View and Edit Animal\n";
				return 0;
			}
		}
	}
	DbConn dbCon;

	string stat = "UPDATE Animal SET " + attribute[choice] + " =\'" + updateDetail + "\' WHERE animal_Id=\'" + animal_Id + "\'";
	
	PreparedStatement* pstat = dbCon.prepareStatement(stat);


	status = pstat->executeUpdate();
	delete pstat;

	return status;

}
/*int AnimalManager::editAnimal(string animal_Id,string selectedAttribute, string updateDetail) {
	
	DbConn dbCon;

	string stat = "UPDATE Animal SET " + selectedAttribute + " =\'" + updateDetail + "\' WHERE animal_Id=\'" + animal_Id + "\'";

	PreparedStatement* pstat = dbCon.prepareStatement(stat);


	int status = pstat->executeUpdate();
	delete pstat;

	return status;
}*/
int AnimalManager::insertAnimal() {
	system("cls");
	
	cout << "\n\t" << "Animal Management: Submenu 2";
	cout << "\n\tRegister New Animal Page\n";
	Animal* animal = new Animal(); //maybe can remove the pointer
	cout << "\nPlease enter the animal details" << endl;
	cout << "\'*\' means the detail is required" << endl;
	cout << "Type (Cat or Dog)*: ";
	getline(cin, animal->type);

	cout << "Breed             : ";
	getline(cin, animal->breed);

	cout << "Gender*           : ";
	getline(cin, animal->gender);

	cout << "Sterilized        : ";
	getline(cin, animal->sterilized);

	cout << "Format of date, YYYY-MM-DD" << endl;
	
	bool valid;
	Utils utils;
	for (int i = 0;i < 4;i++)
	{
		cout << "Date Came In*     : ";
		getline(cin, animal->dateCameIn);
		valid = utils.validDate(animal->dateCameIn);
		if (valid)
			break;
		else 
			cout << "\nPlease enter again. You have " << 3 - i << " times of try left. \n";
	}
	if (!valid) {
		cout << "\nWrong date format. If you want to enter, you can start again the Register Animal\n";
		return 0;
	}
	for (int i = 0;i < 4;i++)
	{
		cout << "Date Born         : ";
		getline(cin, animal->dateBorn);
		valid = utils.validDate(animal->dateBorn,false);
		if (valid||animal->dateBorn=="")
			break;
		else
			cout << "\nPlease enter again. You have " << 3 - i << " times of try left. \n";
	}

	if (!valid && animal->dateBorn != "") {
		cout << "\nWrong date format. If you want to enter, you can start again the Register Animal\n";
		return 0;
	}

	DbConn dbConn;
	PreparedStatement* beforeCameIn = dbConn.prepareStatement("SELECT timestampdiff(day,?,?)");
	beforeCameIn->setString(1, animal->dateBorn);
	beforeCameIn->setString(2, animal->dateCameIn);

	ResultSet* rs = beforeCameIn->executeQuery();
	rs->next();
	int day = rs->getInt(1);
	delete rs;
	delete beforeCameIn;
	if (day < 0) {
		cout << "\nIt is impossible to come into the animal shelter first before born." << endl;
		cout << "\nWrong date. If you want to enter, you can start again the Register Animal\n";
		return 0;
	}
	//cout << "Status            :";
	animal->status = "Available"; //just came in will be available to be adopted
	
	animal->staff_Id =currentUser ; //store the staff who have login into this system and using this system
	
	animal->adoption_Id = "NULL"; //havent adopted yet

	//error handling
	if (animal->type == "") {
		cout << "\nAnimal type is required." << endl;
		return 0;
	}
	string checkCatDog = animal->type;
	transform(checkCatDog.begin(), checkCatDog.end(), checkCatDog.begin(), toupper);
	if (checkCatDog != "CAT"&&checkCatDog!="DOG") {
		cout << "\nThe animal type is cat and dog only." << endl;
		return 0;
	}
	if (animal->gender == "") {
		cout << "\nAnimal gender is required." << endl;
		return 0;
	}

	string checkGender = animal->gender;
	transform(checkGender.begin(), checkGender.end(), checkGender.begin(), toupper);
	if (checkGender != "FEMALE" && checkGender != "MALE") {
		cout << "\nOnly female and male is accepted";
		return 0;
	}
	string checkSterilized = animal->sterilized;
	transform(checkSterilized.begin(), checkSterilized.end(), checkSterilized.begin(), toupper);
	if (checkSterilized != "YES" && checkSterilized != "NO") {
		cout << "Sterilized only accept Yes or No" ;
		return 0;
	}

	string last = generateLastId();
	cout << "\nAnimal Id for the new animal is " << last << endl;
	animal->animal_Id = last;
	DbConn dbCon;
	string stat;
	if (!animal->dateBorn.empty())
	{
		 stat = "INSERT INTO Animal VALUES (\'" + animal->animal_Id + "\',\'" + animal->type + "\',\'" + animal->breed + "\',\'" + animal->gender + "\',\'"
			+ animal->sterilized + "\',\'" + animal->dateCameIn + "\',\'" + animal->dateBorn + "\',\'" + animal->status + "\',\'" 
			 + animal->staff_Id + "\'," + animal->adoption_Id + ")";
	}
	else
	{
		animal->dateBorn = "NULL";
		 stat = "INSERT INTO Animal VALUES (\'" + animal->animal_Id + "\',\'" + animal->type + "\',\'" + animal->breed + "\',\'" + animal->gender + "\',\'"
			+ animal->sterilized + "\',\'" + animal->dateCameIn + "\'," + animal->dateBorn + ",\'" + animal->status + "\',\'" 
			 + animal->staff_Id + "\'," + animal->adoption_Id + ")";
	}
	PreparedStatement* pstat = dbCon.prepareStatement(stat);

/*	pstat->setString(1, animal->animal_Id);
	cout << "animal Id is " << animal->animal_Id << endl;
	pstat->setString(2, animal->type);
	cout << "animal type is " << animal->type << endl;
	pstat->setString(3, animal->breed);
	cout << "animal breed is " << animal->breed << endl;
	pstat->setString(4, animal->gender);
	cout << "animal gender is " << animal->gender << endl;
	pstat->setString(5, animal->sterilized);
	cout << "sterilized is " << animal->sterilized << endl;
	pstat->setString(6, animal->dateCameIn);
	cout << "date came in " << animal->dateCameIn << endl;
	pstat->setString(7, animal->dateBorn);
	cout << "date born is " << animal->dateBorn << endl;
	pstat->setString(8, animal->status);
	cout << "status is " << animal->status << endl;
	pstat->setString(9, animal->dateDied);
	cout << "date died is " << animal->dateDied << endl;
	pstat->setString(10, animal->staff_Id);
	cout << "staff id is " << animal->staff_Id << endl;
	pstat->setString(11, animal->adoption_Id);
	cout << "adoption id is " << animal->adoption_Id << endl;*/
	
	int status = pstat->executeUpdate();
	delete pstat;


	system("pause");
	return status;
}
string AnimalManager::generateLastId() {
	int varcharId = 6;//the number in id

	DbConn dbCon;
	PreparedStatement* pstat = dbCon.prepareStatement("SELECT * FROM LastAnimalId");

	ResultSet* rs = pstat->executeQuery();
	rs->next();
	string last=rs->getString(1);

	delete rs;
	delete pstat;

	
	stringstream lastInt(last);
	int lastNumber = 0;
	lastInt.get();//get characters (remove the A)
	lastInt >> lastNumber;
	lastNumber++;
	last.clear();
	string zero = "0";
	int temp = lastNumber;
	int noOfDigits = 0;
	while (temp > 0)
	{
		temp = temp / 10;
		noOfDigits++;
	}
	last.insert(last.end(), 'A');//maybe can change to .append
	for (int i = 0;i < (varcharId - noOfDigits);i++)
		last.append(zero);
	last.append(to_string(lastNumber));


	//update the table 
	PreparedStatement* update = dbCon.prepareStatement("UPDATE LastAnimalId SET id=?");
	update->setString(1, last);
	update->executeUpdate();

	delete update;
	return last;


}
/*string AnimalManager::generateLastId() {
	int varcharId = 6;//the number in id
	
	DbConn dbCon;
	PreparedStatement* pstat = dbCon.prepareStatement("SELECT * FROM Animal");

	ResultSet* rs = pstat->executeQuery();


	string last;
	while (rs->next())
	{
		last = rs->getString(1);
	}
	delete rs;
	delete pstat;

	stringstream lastInt(last);
	int lastNumber = 0;
	lastInt.get();//get characters (remove the A)
	lastInt >> lastNumber;
	lastNumber++;
	last.clear();
	string zero = "0";
	int temp = lastNumber;
	int noOfDigits = 0;
	while (temp > 0)
	{
		temp = temp / 10;
		noOfDigits++;
	}
	last.insert(last.end(), 'A');//maybe can change to .append
	for (int i = 0;i < (varcharId - noOfDigits);i++)
		last.append(zero);
	last.append(to_string(lastNumber));
	
	return last;
}*/

int AnimalManager::deleteAnimal() {
	displayAllAnimal();
	string animal_Id;
	cout << "\n\tAnimal Mangement: Submenu 3";
	cout << "\n\tDelete Animal Page\n";
	//because delete is a serious matter, then only accept animal id which is the primary key and unique
	cout << "Please key in only the animal Id of the animal you wish to delete.\n";

	cout << "\nAnimal ID : ";
	getline(cin, animal_Id);
	
	Utils utils;
	cout << "\nDelete an animal is a serious matter. All the selected animal's details will be deleted."
		<<" Delete only died animal.";
	cout << "\nDo you want to continue? ";
	cout << "\n\t1. Yes\n\t2. No";
	int choice = utils.inputChoice(1, 2,true);
	int noItem;
	

	Animal* animal = NULL;
	animal=selectAnimal("Animal_Id", animal_Id, noItem);
	if (animal != NULL)
	{
		string animalStatus = animal->status;
		transform(animalStatus.begin(), animalStatus.end(), animalStatus.begin(), toupper);
		if (choice == 1)
		{
			if (animalStatus != "ADOPTED")
			{
				DbConn dbCon;

				PreparedStatement* pstatMed = dbCon.prepareStatement("DELETE FROM Medicine WHERE medical_Id IN (SELECT medical_Id From medical WHERE animal_Id=?)");
				pstatMed->setString(1, animal_Id);
				pstatMed->executeUpdate();
				delete pstatMed;

				PreparedStatement* pstatM = dbCon.prepareStatement("DELETE FROM Medical WHERE animal_Id=?");
				pstatM->setString(1, animal_Id);
				pstatM->executeUpdate();
				delete pstatM;

				PreparedStatement* pstat = dbCon.prepareStatement("DELETE FROM Animal WHERE animal_Id=?");
				pstat->setString(1, animal_Id);
				int status = pstat->executeUpdate();
				delete pstat;

				

				PreparedStatement* pUpdate = dbCon.prepareStatement("UPDATE DiedAnimal SET count=count+1");
				int statusUpdate = pUpdate->executeUpdate();
				delete pUpdate;

				return status;
			}
			else {
				cout << "\n\tThe animal is adopted. Cannot be deleted. It has record that link with owner.\n";
				return 0;

			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		cout << "\n\tInvalid animal id." << endl;
		return 0;
	}
}
int AnimalManager::updateStatus(string animal_Id) {
	
	DbConn dbCon;
	PreparedStatement* pstat = dbCon.prepareStatement("SELECT status FROM animal WHERE animal_Id=?");
	pstat->setString(1,animal_Id);
	
	ResultSet* rs = pstat->executeQuery();

	if (rs->next())
	{
		string availability = rs->getString(1);
		transform(availability.begin(), availability.end(), availability.begin(), toupper);

		if (availability != "AVAILABLE") {
			cout << "\n\tThe animal is unavailable to be adopted\n" << endl;
			delete rs;
			delete pstat;
			return 0;
		}
		else
		{
			PreparedStatement* pstat = dbCon.prepareStatement("UPDATE Animal SET status=\'Adopted\' WHERE animal_Id=?");
			pstat->setString(1, animal_Id);

			int status = pstat->executeUpdate();
			delete rs;
			delete pstat;
			return status;

		}
	}
	else
	{
		cout << "\nWrong animal id entered\n";
		return 0;
	}
}
void AnimalManager::viewColumnData(int choice) {

	Animal animal;
	string column = animal.attributesInTable()[choice - 1];
	cout << animal.items()[choice - 1] << "\t\n";
	cout << "column is " << column << endl;
	DbConn dbCon;
	PreparedStatement* pstat = dbCon.prepareStatement("SELECT ? FROM animal");
	pstat->setString(1, column);

	ResultSet* rs = pstat->executeQuery();
	
	while (rs->next()) {
		cout << rs->getString(1) << endl;
	
	}
	delete rs;
	delete pstat;
}