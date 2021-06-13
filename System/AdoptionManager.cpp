#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
using namespace std;


#include <mysql/jdbc.h>
using namespace sql;

#include "DbConn.h"
#include "AdoptionManager.h"
#include "OwnerOfAdoptedAnimal.h"
#include "Adoption.h"
#include "Utils.h"
#include "AnimalManager.h"

AdoptionManager::AdoptionManager(string currentUser) {
	this->currentUser = currentUser;
}
int AdoptionManager::insertOwner() {
	system("cls");
	cout << "\n\tAdoption Management: Submenu 3";
	cout << "\n\tRegister New Owner of Adopted Animal\n";

	OwnerOfAdoptedAnimal owner;
	cout << "\nPlease enter the owner details"<<endl;
	
	cout << "\'*\' means the detail is required" << endl;

	cout << "First Name    : ";
	getline(cin, owner.firstName);

	cout << "Last Name     : ";
	getline(cin, owner.lastName);

	cout << "IC Number/Pass. No*: ";
	getline(cin, owner.IcNumber);

	cout << "Gender*      : ";
	getline(cin, owner.gender);

	cout << "Hp Number*    : ";
	getline(cin, owner.hpNumber);

	owner.staff_Id = currentUser;
	owner.owner_Id = generateOwnerId();
	
	cout << "owner id is " << owner.owner_Id;
	cout << "\nStaff Id who enter the new owner is " << owner.staff_Id<<endl;

	if (owner.IcNumber == "")
	{
		cout << "\nIC Number or Pass. No is required." << endl;
		return 0;
	}
	if (owner.hpNumber == "") {
		cout << "\nHp Number is required." << endl;
		return 0;
	}
	if (owner.gender == "") {
		cout << "\nGender is required." << endl;
		return 0;
	}
	string checkGender =owner.gender;
	transform(checkGender.begin(), checkGender.end(), checkGender.begin(), toupper);
	if (checkGender != "FEMALE" && checkGender != "MALE") {
		cout << "\nOnly female and male is accepted";
		return 0;
	}
	DbConn dbCon;

	string stat;
	stat = "INSERT INTO OwnerOfAdoptedAnimal VALUES (\'" + owner.owner_Id + "\',\'" + owner.firstName + "\',\'" + owner.lastName + "\',\'" +
		owner.IcNumber + "\',\'" + owner.gender + "\',\'" + owner.hpNumber + "\',\'" + owner.staff_Id + "\')";


	PreparedStatement* pstat = dbCon.prepareStatement(stat);
		int status = pstat->executeUpdate();
	delete pstat;


	system("pause");
	return status;
}

string AdoptionManager::generateOwnerId() {
	int varcharId = 4;//the number in id
	//OwnerOfAdoptedAnimal* owner = NULL;
	DbConn dbCon;
	PreparedStatement* pstat = dbCon.prepareStatement("SELECT * FROM OwnerOfAdoptedAnimal");

	ResultSet* rs = pstat->executeQuery();
	//owner = new OwnerOfAdoptedAnimal();

	string last;
	while (rs->next())
	{
		last = rs->getString(1);
	}
	delete rs;
	delete pstat;

	stringstream lastInt(last);
	int lastNumber = 0;
	lastInt.get();
	lastInt.get();//get characters (remove the OW)
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

	last.insert(last.end(), 'W');//maybe can change to .append
	for (int i = 0;i < (varcharId - noOfDigits);i++)
		last.append(zero);
	last.append(to_string(lastNumber));
	
	return last;
}
void AdoptionManager::displayAllOwner() {

	DbConn dbCon;
	PreparedStatement* pstat = dbCon.prepareStatement("SELECT * FROM OwnerOfAdoptedAnimal");

	ResultSet* rs = pstat->executeQuery();

	vector <string> columnName(3);

	columnName = { "Owner Id  ","First Name     ","Last Name     ","Ic Number     ","Gender","Hp Number   "," Staff Id" };

	int a = 1;
	for (int i = 0;i < columnName.size();i++) //find the column size
	{
		a += columnName[i].size();
		a += 1;
	}
	cout << setw(a) << setfill('-') << "-" << " \n" << "|";//need | with a space behind --changed to no space behind

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

int AdoptionManager::registerAdoption() {
	system("cls");
	
	Utils utils;
	Adoption adoption;
	bool wantCont;
	cout << "\n\tAdoption Management: Submenu 1";
	cout << "\n\tRegister New Adoption Page";

	cout << "\nPlease enter the adoption details" << endl;
	
	utils.dateFormat("date");
	bool valid;
	for (int i = 0;i < 4;i++)
	{
	cout << "Adoption Date  : ";
	getline(cin, adoption.adoptDate);
	valid = utils.validDate(adoption.adoptDate);
	if (valid)
		break;
	else
		cout << "\nPlease enter again. You have " << 3 - i << " times of try left. \n";
	}
	if (!valid) {
		cout << "\nWrong date format. If you want to enter, you can start again the Adoption\n";
		return 0;
	}


	system("cls");
	displayAllOwner();
	

	OwnerOfAdoptedAnimal* owner;
	for (int i = 0;i < 4;i++)//three time of peluang to enter
	{
		cout << "\n\n\tPlease enter owner Id : ";
		getline(cin, adoption.owner_Id);
		 owner = selectOwner(adoption.owner_Id);
		if (owner != NULL)
			break;
		else 
			cout << "\nPlease enter again. You have " << 3 - i << " times of try left. \n";
	}
	if (owner == NULL)
	{
		cout << "\nWrong owner Id entered. If you want to enter again, you can start again the Register Adoption.\n";
		//wantCont = utils.wantContinue("Adoption");
		
		return 0;
		/*if (wantCont)
			return registerAdoption();
		else
			return 0;*/
	}
	system("cls");
	AnimalManager animalManager(currentUser);
	animalManager.displayAllAnimal();

	
	int update;
	for (int i = 0;i < 4;i++)//three time of peluang to enter
	{
		cout << "\n\n\tPlease enter animal Id that will be adopted :";
		getline(cin, adoption.animal_Id);

		 update = animalManager.updateStatus(adoption.animal_Id);
		 if (update == 0)
		 {
			 cout << "\nPlease enter again. You have " << 3 - i << " times of try left. \n";
		 
		 }
		 else
			break;
	}
	if (update == 0)
	{
		cout << "\nWrong Animal Id. If you want to enter, you can start again the Register Adoption\n";
		return 0;
		/*cout << "If you want to enter again, you can continue with Adoption\n";
		wantCont = utils.wantContinue("Adoption");
		if (wantCont)
			return registerAdoption();
		else
			return 0;//unsuccessful*/
	}

	DbConn dbConn;
	PreparedStatement* beforeBorn = dbConn.prepareStatement("SELECT timestampdiff(day,dateCameIn,?) FROM animal WHERE animal_Id=?");
	beforeBorn->setString(1, adoption.adoptDate);
	beforeBorn->setString(2, adoption.animal_Id);

	ResultSet* rs = beforeBorn->executeQuery();
	rs->next();
	int day = rs->getInt(1);
	delete rs;
	delete beforeBorn;
	if (day < 0) {
		cout << "\nIt is impossible for the animal to be adopted before came into the animal shelter." << endl;
		cout << "\nWrong date. If you want to edit, you can start again the Register Adoption\n";
		system("pause");
		return 0;
	}


	wantCont = utils.wantContinue("Adoption");
		if (wantCont)
		{
			adoption.adoption_Id = generateAdoptionId();

			adoption.staff_Id = currentUser;

			cout << "\nAdoption Id for this adoption is " << adoption.adoption_Id << endl;
			DbConn dbCon;
			string stat = "INSERT INTO Adoption VALUES(\'" + adoption.adoption_Id + "\',\'" + adoption.owner_Id + "\',\'" +
				adoption.adoptDate + "\',\'" + adoption.staff_Id + "\',\'" + adoption.animal_Id + "\')";

			PreparedStatement* pstat = dbCon.prepareStatement(stat);

			int status = pstat->executeUpdate();

			stat = "UPDATE Animal SET adoption_Id=\'" + adoption.adoption_Id + "\' WHERE animal_Id=\'"+adoption.animal_Id + "\'";
			pstat = dbCon.prepareStatement(stat);
			pstat->executeUpdate();

			delete pstat;


			system("pause");
			return status;
		}
		else
			return 0;
}
string AdoptionManager::generateAdoptionId() {
	int varcharId = 4;//the number in id

	DbConn dbCon;
	PreparedStatement* pstat = dbCon.prepareStatement("SELECT * FROM Adoption");

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
	lastInt.get();
	lastInt.get();//get characters (remove the AD)
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
	last.insert(last.end(), 'D');
	for (int i = 0;i < (varcharId - noOfDigits);i++)
		last.append(zero);
	last.append(to_string(lastNumber));

	return last;
}
OwnerOfAdoptedAnimal* AdoptionManager::selectOwner(string owner_Id) {
	OwnerOfAdoptedAnimal* owner=new OwnerOfAdoptedAnimal();
	
	transform(owner_Id.begin(), owner_Id.end(), owner_Id.begin(), toupper);
	DbConn dbCon;
	PreparedStatement* pstat = dbCon.prepareStatement("SELECT * FROM OwnerOfAdoptedAnimal WHERE UPPER(owner_Id)=?");
	pstat->setString(1, owner_Id);

	ResultSet* rs = pstat->executeQuery();
	if (rs->next())
	{
		owner->owner_Id= rs->getString(1);
		owner->firstName = rs->getString(2);
		owner->lastName = rs->getString(3);
		owner->IcNumber = rs->getString(4);
		owner->gender = rs->getString(5);
		owner->hpNumber= rs->getString(6);
		owner->staff_Id = rs->getString(7);
		delete pstat;
		delete rs;

	
	}
	else
	{
		delete pstat;
		delete rs;
		return NULL;
}
	return owner;
}

int AdoptionManager::editOwner(string owner_Id) {
	Utils utils;
	OwnerOfAdoptedAnimal owner;

	vector<string> editItems = owner.items();
	vector<string> attribute = owner.attributesInTable();
	cout << "\nWhich details you wish to edit?\n";

	for (int i = 1;i < (editItems.size() - 1);i++)
	{
		cout << "\n\t" << i << ". " << editItems[i] ;
	}
	cout << "\n\n\t" << (editItems.size() - 1) << ". " << "Go Back Previous Page" << endl;
	int choice = 0, status;
	choice = utils.inputChoice(1, (editItems.size() - 1), true);

	if (choice == (editItems.size() - 1))
		return 0;
	string updateDetail;
	cout << "Now we will update " << editItems[choice] << " for owner with owner Id of " << owner_Id << endl;


	cout << "Enter the new detail to update : ";
	getline(cin, updateDetail);
	
	if (editItems[choice] == "Gender") {
		string checkGender = updateDetail;
		transform(checkGender.begin(), checkGender.end(), checkGender.begin(), toupper);
		if (checkGender != "FEMALE" && checkGender != "MALE") {
			cout << "\nOnly female and male is accepted";
			return 0;
		}
	}
	if (editItems[choice] == "Ic Number/Pass. No") {
		cout << "\nIC Number or Pass. No is required." << endl;
		return 0;
	}
	if (editItems[choice] == "Hp Number") {
		cout << "\nHp Number is required." << endl;
		return 0;
	}
	DbConn dbCon;

	string stat = "UPDATE OwnerOfAdoptedAnimal SET " + attribute[choice] + " =\'" + updateDetail + "\' WHERE owner_Id=\'" + owner_Id + "\'";

	PreparedStatement* pstat = dbCon.prepareStatement(stat);


	status = pstat->executeUpdate();
	delete pstat;
	return status;
}
int AdoptionManager::viewOwner() {
	OwnerOfAdoptedAnimal* owner;
	Utils utils;
	int choice = 0, noItem;
	string owner_Id;
	//while (choice != (owner.searchItems + 1))
	{
		system("cls");
		cout << "\n\tAdoption Management: Submenu 2";
		cout << "\n\tView Owner or Edit Owner's Details\n";
		displayAllOwner();
		cout << "\nWhich owner you wish to view or edit?\n";
		for (int i = 0;i < 4;i++)//three time of peluang to enter
		{
			cout << "\n\n\tPlease enter owner Id : ";
			getline(cin, owner_Id);
			owner = selectOwner(owner_Id);
			if (owner != NULL)
				break;
			else
				cout << "\nPlease enter again. You have " << 3 - i << " times of try left. \n";
		}
		if (owner == NULL)
		{
			cout << "\nWrong owner Id entered. If you want to enter again, you can start again the Register Adoption.\n";
			//wantCont = utils.wantContinue("Adoption");

			return 0;
		
		}
		else
		{
			system("cls");
			cout << "Owner Id   : " << owner->owner_Id << endl;
			cout << "First Name : " << owner->firstName << endl;
			cout << "Last Name  : " << owner->lastName << endl;
			cout << "Ic Number/Pass. No : " << owner->IcNumber << endl;
			cout << "Gender     : " << owner->gender << endl;
			cout << "Hp Number  : " << owner->hpNumber << endl << endl;
			
			cout << "\nDo you want to edit owner's detail?" << endl;
			cout << "\t1. Yes" << endl;
			cout << "\t2. No" << endl;
			int choiceYesNo = utils.inputChoice(1, 2, true);
			if (choiceYesNo == 1) {
				system("cls");
				int status = editOwner(owner_Id);
				if (status != 0)
					cout << "\nSuccessfully edited the owner's detail" << endl;
				else
					cout << "\nFailed to edit the owner's detail" << endl;
				system("pause");
				return 0;
			}
			else if (choiceYesNo == 2) {
				return 0;
			}
		}

	}
}

/*void AdoptionManager::viewOwner() {
	OwnerOfAdoptedAnimal owner;
	Utils utils;
	vector<string> items = owner.items();
	vector<string> attribute = owner.attributesInTable();

	int choice = 0, noItem;
	while (choice != (owner.searchItems + 1))
	{
		system("cls");
		cout << "\n\tAdoption Management: Submenu 2";
		cout << "\n\tView Owner or Edit Owner's Details\n";
		displayAllOwner();
		cout << "\nWhich owner you wish to view or edit?\n";
		cout << "Please enter the choice number to select the owner with one of the following specific detail.\n";
		for (int i = 0;i < owner.searchItems;i++) {
			cout << "\t" << i + 1 << ". " << items[i] << endl;
		}
		cout << "\n\n\t" << owner.searchItems+1 << ". " << "Go Back Previous Page" << endl;
		choice = utils.inputChoice(1, (owner.searchItems + 1), true);
		if (choice == (owner.searchItems + 1)) 
			continue;
		
		system("cls");
		cout << "\nPlease enter the " << items[choice - 1] << " : ";
		string findItem;
		getline(cin, findItem);

		OwnerOfAdoptedAnimal* foundOwner = NULL;
		
		int status =.editOwner();
		if (status != 0)
			cout << "\nSuccessfully edited the owner's detail" << endl;
		else
			cout << "\nFailed to edit the owner's detail" << endl;

		system("pause");
	}

}*/