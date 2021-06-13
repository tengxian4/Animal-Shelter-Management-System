#include <iostream>
#include <sstream>
#include <algorithm>
using namespace std;

#include <mysql/jdbc.h>
using namespace sql;

#include "DbConn.h"
#include "Staff.h"
#include "StaffManager.h"
#include "Utils.h"
#include "AdminPage.h"
int varcharId = 4;//wihtout the 'S', In database is VARCHAR(10),but here minus one because exclude 'S'
#define mySql_uniqueConstranit 1062

int StaffManager::insertStaff()
{
	DbConn dbCon;
	cout << "\n\tAdministration Staff: Submenu 2";
	cout << "\n\tRegister New Staff Page\n";

	Staff* staff = new Staff();
	cout << "\nPlease enter the staff details " << endl;
	cout << "\'*\' means the detail is required" << endl;

	cout << "first name : ";
	getline(cin, staff->firstName);

	cout << "last name  : ";
	getline(cin, staff->lastName);

	cout << "position*  : ";
	getline(cin, staff->position);

	cout << "username*  : ";
	getline(cin, staff->username);
	

	cout << "password*  : ";
	getline(cin, staff->password);

	cout << "hpNumber*  : ";
	getline(cin, staff->hpNumber);

	cout << "gender     : ";
	getline(cin, staff->gender);

	cout << "email      : ";
	getline(cin, staff->email);
	cout << "Format of date, YYYY-MM-DD" << endl;
	bool valid;
	Utils utils;

	for (int i = 0;i < 4;i++){
		cout << "date start working       : ";
		getline(cin, staff->dateStartWorking);
		valid = utils.validDate(staff->dateStartWorking);
		if (valid)
			break;
		else
			cout << "\nPlease enter again. You have " << 3 - i << " times of try left. \n";
	}
	if (!valid) {
		cout << "\nWrong date format. If you want to enter, you can start again the Registration\n";
		system("pause");
		return 0;
	}
	

	cout << "Status (Default: Active) : ";//maybe the admin want to put in training
	getline(cin, staff->status);
	if(staff->status=="")
		staff->status = "Active";

	

	PreparedStatement* checkUsername = dbCon.prepareStatement("SELECT username From staff WHERE username=?");
	checkUsername->setString(1, staff->username);
	ResultSet* rs = checkUsername->executeQuery();

	if (rs->next())
	{
		cout << "\nUsername already exists. Please choose another username." << endl;
		delete rs;
		delete checkUsername;
		return 0;
	}
	delete rs;
	delete checkUsername;

	/*PreparedStatement* checkHp = dbCon.prepareStatement("SELECT hpNumber FROM Staff WHERE hpNumber=?");
	checkHp->setString(1, staff->hpNumber);
	ResultSet* rsHp=checkHp->executeQuery();
	if (rsHp->next())
	{
		cout << "\nHp Number already exists. Please enter another hp Number" << endl;
		delete rsHp;
		delete checkHp;
		return 0;
	}*/
	
	
	if (staff->hpNumber == "") {
		cout << "\nHp Number is required." << endl;
		return 0;
	}
	if (staff->position == "") {
		cout << "\nPosition is required." << endl;
		return 0;
	}
	if (staff->username == "") {
		cout << "\nUsername is required." << endl;
		return 0;
	}
	if (staff->password == "") {
		cout << "\nPassword is required." << endl;
		return 0;
	}
	
	string checkGender = staff->gender;
	transform(checkGender.begin(), checkGender.end(), checkGender.begin(), toupper);
	if (checkGender != "FEMALE" && checkGender != "MALE") {
		cout << "\nOnly female and male is accepted"<<endl;
		return 0;
	}



	PreparedStatement* pstat = dbCon.prepareStatement("INSERT INTO Staff VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?,?)");
	string last = selectLastStaffId();
	stringstream lastInt(last);
	int lastNumber = 0;
	lastInt.get();
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
	last.insert(last.end(), 'S');//maybe can change to .append
	for (int i = 0;i < (varcharId - noOfDigits);i++)
		last.append(zero);
	last.append(to_string(lastNumber));
	
	staff->staff_Id = last;
	pstat->setString(1, staff->staff_Id);
	pstat->setString(2, staff->firstName);
	pstat->setString(3, staff->lastName);
	pstat->setString(4, staff->position);
	pstat->setString(5, staff->username);
	pstat->setString(6, staff->password);
	pstat->setString(7, staff->hpNumber);
	pstat->setString(8, staff->gender);
	pstat->setString(9, staff->email);
	pstat->setString(10, staff->dateStartWorking);
	pstat->setString(11, staff->status);

	int status=pstat->executeUpdate();
	
	cout << "\nStaff Id for the new staff is " << last << endl;

	delete pstat;

	
	system("pause");
	return status;
}

string StaffManager::selectLastStaffId() {
	Staff* staff = NULL;
	DbConn dbCon;
	PreparedStatement* pstat = dbCon.prepareStatement("SELECT * FROM Staff");

	ResultSet* rs = pstat->executeQuery();

	//	if (rs->next())
	staff = new Staff();
	//rs->next();
	//if ()

	string last;
	while (rs->next())
	{
		last = rs->getString(1);
	}

	/*	staff->staff_Id= rs->getString(1);
	cout << "staff ID is " << staff->staff_Id;
	staff->firstName = rs->getString(2);
	staff->lastName = rs->getString(3);
	staff->position = rs->getString(4);
	staff->username = rs->getString(5);
	staff->password = rs->getString(6);
	staff->IcNumber = rs->getString(7);
	staff->hpNumber = rs->getString(8);
	staff->gender = rs->getString(9);
	staff->email = rs->getString(10);
	staff->dateStartWorking = rs->getString(11);
	*/

	delete rs;
	delete pstat;
	return last;
}

void StaffManager::displayAllStaff() {

	DbConn dbCon;
	PreparedStatement* pstat = dbCon.prepareStatement("SELECT * FROM Staff");
	
	ResultSet* rs = pstat->executeQuery();

	vector <string> columnName(11);

	columnName = { "Staff ID " ,"First Name    " , "Last Name     " ,
		"Position         ","Username  ","Password  " ,"Hp Number   ","Gender",
	" Email                    ","Date Start Working" ,"status"};

	int a = 1;
	for (int i = 0;i < columnName.size();i++)
	{
		a += columnName[i].size();
		a += 1;
	}
	
	cout << setw(a) << setfill('-') << "-" << " \n" << "|";//need | with a space behind

	for (int i = 0;i < columnName.size();i++)
	{
		//cout <<i+1<<" : "<< columnName[i].size() << endl;

		cout << setfill(' ') << setw(columnName[i].size()) << left << columnName[i] << "|";
	}


	cout << setw(a) << setfill('-') << "\n" << "\n"; //this setw and setfill is from right side then one \n is for line before 

	while (rs->next())
	{
		int i = 0;
		while (i < columnName.size())
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
	/*staff = new Staff();
	cout<< rs->getString(1);
	cout << staff->staff_Id;
	staff->firstName = rs->getString(2);
	cout <<  staff->firstName << endl;
	staff->lastName = rs->getString(3);
	cout << staff->lastName << endl;
	staff->position = rs->getString(4);
	cout <<  staff->position << endl;
	staff->username = rs->getString(5);
	cout <<staff->username << endl;
	staff->password = rs->getString(6);
	cout << "******" << endl;
	staff->IcNumber = rs->getString(7);
	cout << staff->IcNumber << endl;
	staff->hpNumber = rs->getString(8);
	cout <<  staff->hpNumber << endl;
	staff->gender = rs->getString(9);
	cout << staff->gender << endl;
	staff->email = rs->getString(10);
	cout << staff->email << endl;
	staff->dateStartWorking = rs->getString(11);
	cout << staff->dateStartWorking << endl << endl;
*/

	delete rs;
	delete pstat;


}


void StaffManager::viewStaff() {

	string findItemList[3] = { "Staff Id", "First Name", "Last Name" };

	int choice = 0;
	vector<string> staffId_edit;
	while (choice != 4)
	{
		system("cls");
		Staff* staff = NULL;
		Staff* edit = NULL;
		cout << "\n\tAdministration Staff: Submenu 1\n";
		cout << "\tView and Edit Staff Page\n";
		cout << "\nWhich staff you wish to view or edit? \n";
		cout << "Please enter the choice number to select the staff with one of the following specific detail.\n";
		cout << "\t1. Staff id\n" << "\t2. First Name\n" << "\t3. Last Name\n";
		cout << "\t4. Go Back Previous Page\n";
		Utils utils;
		choice = utils.inputChoice(1, 4,true);
		if (choice == 4)
			continue;
		

		string findItem;
		cout << "\nPlease key in the "<<findItemList[choice-1]<<" : ";
		getline(cin, findItem);
		edit=staff = selectStaff(findItem, choice);
		
		if (staff == NULL)
		{
			
			cout << "Invalid staff's detail \n" << endl;
			system("pause");
			system("cls");
			cout << "Please choose the action you wish to take\n";
			
		}
		else
		{
			staffId_edit.clear();
			system("cls");
			while (staff != NULL)
			{
				cout << "staff ID   : " << staff->staff_Id << endl;
				staffId_edit.push_back(staff->staff_Id);
				cout << "first name : " << staff->firstName << endl;
				cout << "last name  : " << staff->lastName << endl;
				cout << "position   : " << staff->position << endl;
				cout << "username   : " << staff->username << endl;
				cout << "hp number  : " << staff->hpNumber << endl;
				cout << "gender     : " << staff->gender << endl;
				cout << "email      : " << staff->email << endl;
				cout << "Date started working : " << staff->dateStartWorking << endl << endl;
				staff = staff->next;
			}
		
			if (staffId_edit.size() == 1)
			{
				cout << "Do you want to edit the staff's details?\n\t1. Yes\n\t2. No\n";
				choice = utils.inputChoice(1, 2, true);
				system("cls");
				if (choice == 1)
				{

					int status = editStaff(staffId_edit);
					if (status != 0)
						cout << "Successfully edited." << endl;
					else
						cout << "Fail to edit details" << endl;
					system("pause");
				}
				else if (choice == 2)
					continue;//back to menu View and edit staff page
			}
			else {
				cout << "\nIf you wish to edit, please search using staff Id." << endl;
				system("pause");
			}
		}
		
	}
	
	system("cls");
	
}

void StaffManager::choiceManageStaff() {

	string decision;
	cin.clear();
	cin.ignore(INT_MAX, '\n');
	cout << "Do you want to search and view staff's details? [yes/no] \n";
	getline(cin, decision);
	transform(decision.begin(), decision.end(), decision.begin(), toupper);
	if (decision == "YES")
	{


	}
	else if (decision == "NO")
	{
		string choice;
		//cin.ignore(INT_MAX, '\n');
		cout << "Do you want to register new staff ? [yes/no] \n";
		getline(cin, choice);
		transform(choice.begin(), choice.end(), choice.begin(), toupper);
		if (choice == "YES")
			insertStaff();
		//temporary blank	
	}
	else
		cout << "invalid answer " << endl;



}

Staff* StaffManager::selectStaff(string findItem, int choice)
{

	Staff* staff = NULL;
	DbConn dbCon;

	Staff* temp = NULL;
	Staff* sHead = NULL;


	transform(findItem.begin(), findItem.end(), findItem.begin(), toupper);
	/*staff->staff_Id = rs->getString(1);
	cout << "staff ID is " << staff->staff_Id << endl;
	staff->firstName = rs->getString(2);
	cout << "first name : " << staff->firstName << endl;
	staff->lastName = rs->getString(3);
	cout << "last name : " << staff->lastName << endl;
	*/
	
	if (choice == 1)//seacrh by staff id
	{
		PreparedStatement* pstat = dbCon.prepareStatement("SELECT * FROM Staff WHERE UPPER(staff_id)=?");
		pstat->setString(1, findItem);

		ResultSet* rs = pstat->executeQuery();
		while (rs->next())
		{

			if (staff == NULL)
			{
				staff = new Staff();
				sHead = staff;
			}
			staff->staff_Id = rs->getString(1);
			staff->firstName = rs->getString(2);
			staff->lastName = rs->getString(3);
			staff->position = rs->getString(4);
			staff->username = rs->getString(5);
			staff->password = rs->getString(6);	
			staff->hpNumber = rs->getString(7);
			staff->gender = rs->getString(8);
			staff->email = rs->getString(9);
			staff->dateStartWorking = rs->getString(10);

			temp = staff;
			staff = new Staff();
			temp->next = staff;
		}
		delete rs;
		delete pstat;
	}
	else if (choice == 2)
	{
		PreparedStatement* pstat = dbCon.prepareStatement("SELECT * FROM Staff WHERE UPPER(firstName)=?");
		pstat->setString(1, findItem);

		ResultSet* rs = pstat->executeQuery();
		while (rs->next())
		{

			if (staff == NULL)
			{
				staff = new Staff();
				sHead = staff;
			}
			staff->staff_Id = rs->getString(1);
			staff->firstName = rs->getString(2);
			staff->lastName = rs->getString(3);
			staff->position = rs->getString(4);
			staff->username = rs->getString(5);
			staff->password = rs->getString(6);
			staff->hpNumber = rs->getString(7);
			staff->gender = rs->getString(8);
			staff->email = rs->getString(9);
			staff->dateStartWorking = rs->getString(10);

			temp = staff;
			staff = new Staff();
			temp->next = staff;
		}
		delete rs;
		delete pstat;
	}
	else if (choice == 3)
	{

		PreparedStatement* pstat = dbCon.prepareStatement("SELECT * FROM Staff WHERE UPPER(lastName)=?");
		pstat->setString(1, findItem);

		ResultSet* rs = pstat->executeQuery();
		while (rs->next())
		{

			if (staff == NULL)
			{
				staff = new Staff();
				sHead = staff;
			}
			staff->staff_Id = rs->getString(1);
			staff->firstName = rs->getString(2);
			staff->lastName = rs->getString(3);
			staff->position = rs->getString(4);
			staff->username = rs->getString(5);
			staff->password = rs->getString(6);
			staff->hpNumber = rs->getString(7);
			staff->gender = rs->getString(8);
			staff->email = rs->getString(9);
			staff->dateStartWorking = rs->getString(10);

			temp = staff;
			staff = new Staff();
			temp->next = staff;

		}
		delete rs;
		delete pstat;
	}

	if(sHead!=NULL)
		temp->next = NULL;


	return sHead;

	/*if (rs->getString(1) == findItem || rs->getString(2) == findItem ||rs->getString(3) == findItem)
		{

			if(staff==NULL)
			{
				staff = new Staff();
				 sHead = staff;
			}
			staff->staff_Id = rs->getString(1);
			staff->firstName = rs->getString(2);
			staff->lastName = rs->getString(3);
			staff->position = rs->getString(4);
			staff->username = rs->getString(5);
			staff->password = rs->getString(6);
			staff->IcNumber = rs->getString(7);
			staff->hpNumber = rs->getString(8);
			staff->gender = rs->getString(9);
			staff->email = rs->getString(10);
			staff->dateStartWorking = rs->getString(11);

			temp = staff;
			staff = new Staff();
			temp->next = staff;
		}*/
}

int StaffManager::editStaff(vector<string> staffId_edit) {

	

	Utils utils;
	vector<string> editDetail = { "First Name","Last Name","Position","username","password",
	"Hp Number","Gender","Email","Date Start Working" };//"Staff Id" is not included because it will not wrong, it generated by this system
	cout << "Which details you wish to edit?\n";
	for (int i = 0;i < editDetail.size();i++)
	{
		cout << "\n\t" << i + 1 << "." << editDetail[i];
	}

	int choice=0,status;
	choice=utils.inputChoice(1, editDetail.size(),true);
	
	
	Staff attribute;
	//vector<string> att=attribute.attributeStaff();
	string updateDetail;
	choice = choice - 1;
	bool date=utils.dateFormat(editDetail[choice]);
	
	

	for(int i=0;i< staffId_edit.size();i++)
	{
		
		cout << "Now we will update " << editDetail[choice] << " for staff with staff Id of " << staffId_edit[i] << endl;
		
		if (!date)
		{
			cout << "Enter the new detail to update : ";
			getline(cin, updateDetail);
			if (editDetail[choice] == "username")
			{
				if (updateDetail == "") {
					cout << "\nUsername is required." << endl;
					//system("pause");
					return 0;
				}
				DbConn dbCon;
				PreparedStatement* checkUsername = dbCon.prepareStatement("SELECT username From staff WHERE username=?");
				checkUsername->setString(1, updateDetail);
				ResultSet* rs = checkUsername->executeQuery();

				if (rs->next())
				{
					cout << "\nUsername already exists. Please choose another username." << endl;
					delete rs;
					delete checkUsername;
					//system("pause");
					return 0;
				}
				delete rs;
				delete checkUsername;
			}
			else if (editDetail[choice] == "Gender") {

				string checkGender =updateDetail;
				transform(checkGender.begin(), checkGender.end(), checkGender.begin(), toupper);
				if (checkGender != "FEMALE" && checkGender != "MALE") {
					cout << "\nOnly female and male is accepted" << endl;
					//system("pause");
					return 0;
				}
			}
			else if (editDetail[choice] == "Position") {
				if (updateDetail == "") {
					cout << "\nPosition is required." << endl;
					//system("pause");
					return 0;
				}

			}
			else if (editDetail[choice]=="password") {
				if (updateDetail == "") {
					cout << "\nPassword is required" << endl;
					//system("pause");
					return 0;
						
				}
			}
			else if (editDetail[choice] == "Hp Number") {
				if (updateDetail == "") {
					cout << "\nHp Number is required" << endl;
					//system("pause");
					return 0;
				}
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
				cout << "\nWrong date format. If you want to edit, you can start again the View and Edit Staff\n";
				//system("pause");
				return 0;
			}
		}
		/*cout << "attribute.attributeStaff()[choice] is " << attribute.attributeInTable()[choice + 1] << endl;
		cout << "update details is " << updateDetail << endl;
		cout << "staffId_edit[i] is " << staffId_edit[i] << endl;*/
		DbConn dbCon;

		string stat = "UPDATE Staff SET " + attribute.attributeInTable()[choice+1] + " =\'" + updateDetail + "\' WHERE staff_id=\'" + staffId_edit[i] +"\'";

		PreparedStatement* pstat = dbCon.prepareStatement(stat);
	
	
		status = pstat->executeUpdate();

		delete pstat;
		if(status==0)
			return status;
	}
	return status;


	
}
