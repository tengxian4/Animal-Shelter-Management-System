#include "Utils.h"
#include <iostream>
#include <iomanip>
using namespace std;

#include "Login.h"

#include <mysql/jdbc.h>
using namespace sql;
#include "DbConn.h"

int Utils::inputChoice(int min, int max,bool firstTime) {
	int choice;
	//string decision;
	while (firstTime)
	{
		cout << "\n\t" << max + 1 << ". Log Out" << endl;
		cout << "\t" << max + 2 << ". Exit Program" << endl;
		firstTime = false;
	}
	cout << "\nChoice : ";
	cin >> choice;
	cin.clear();
	cin.ignore(INT_MAX, '\n');
	if (choice<min || choice>(max + 2) || (!choice))
	{
		cout << "Invalid choice. \n";
		cout << "Please enter again. \n";
		return inputChoice(min, max,firstTime);
		/*cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Enter again? [Yes or No] ";
		getline(cin, decision);
		transform(decision.begin(), decision.end(), decision.begin(), toupper);
		if (decision == "YES")
		{
			return inputChoice(min, max);
		}
		else if (decision =="NO")
		{
			cout << "Direct you back "
		}*/
	}
	else
	{
		if (choice == (max + 1))
		{
			system("cls");
			
			Login login;
			login.displayLogin();
			exit(0);
			
		}
		else if (choice == (max + 2))
		{
			system("cls");
			exitProgram();
			exit(0);
		}
		return choice;
	}
	
}

void Utils::exitProgram() {

	system("cls");
	cout << "\n\n";
	cout << setw(150) << setfill('*') << "\n";
	cout << "\t\t\t\t\t\t\tThank you for using Animal Shelter Management System\n";
	cout << setw(150) << setfill('*') << "\n";
	

	exit(0);
}
bool Utils::dateFormat(string test)
{
	//follow the naming convention, used Camel Case so will have two possibilities only
	size_t foundDate = test.find("date");
	if (foundDate != string::npos)
	{
		cout << "\nDate need to key in with this format : YYYY-MM-DD" << endl;
		return true;
	}
	else
	{
		foundDate = test.find("Date");
		if (foundDate != string::npos)
		{
			cout << "\nDate need to key in with this format : YYYY-MM-DD" << endl;
			return true;
		}
	}
	return false;
}
bool Utils::validDate(string date) {//will return day name but i didnt use it
	string tempDate = date;
	if (tempDate.size() != 10)
		return false;
	for (int i = 0;i < 10;i++)
	{
		char temp = tempDate.back();
		tempDate.pop_back();
		if (i == 2 or i == 5) {
			if (temp != '-')
				return false;
		}

	}
	DbConn dbCon;
	string stat = "SELECT IFNULL(DAYNAME(\'" + date + "\'),\'\')";
	PreparedStatement* pstat = dbCon.prepareStatement(stat);

	ResultSet* rs = pstat->executeQuery();

	if (rs->next()) {

		if (rs->getString(1) != "")
		{

			delete rs;
			delete pstat;

			PreparedStatement* before = dbCon.prepareStatement("SELECT timestampdiff(day,'2014-01-01',?)");
			before->setString(1, date);

			ResultSet* rsBefore = before->executeQuery();
			rsBefore->next();
			int day = rsBefore->getInt(1);

			delete before;
			delete rsBefore;
			if (day < 0) {
				cout << "\nInvalid date. This animal shelter open on 2014-01-01. Therefore, any date before this date will be invalid to use." << endl;
				return false;
			}

			PreparedStatement* after = dbCon.prepareStatement("SELECT timestampdiff(day,?,CURRENT_DATE)");
			after->setString(1, date);
		
			ResultSet* rsAfter = after->executeQuery();
			rsAfter->next();
			day = rsAfter->getInt(1);

			delete after;
			delete rsAfter;

			if (day < 0) {
				cout << "\nInvalid date. Future date is not allowed." << endl;
				return false;
			}
			return true;
		}
		else
		{
			cout << "\nInvalid date format. Date format : YYYY-MM-DD" << endl;
			return false;
		}
	}
	else
	{
		cout << "Invalid Date" << endl;//usually wont come in this
		delete rs;
		delete pstat;
		return false;
	}
	

	 
}
bool Utils::wantContinue(string process) {
	cout << "\n\tAre you sure you want to continue "<< process << " ?";
	cout << "\n\t1. Yes\n";
	cout << "\t2. No\n";
	cout << "Choice : ";
	int input;
	cin >> input;


	if (input == 1) {
		return true;
	}
	else if (input == 2)
		return false;
	else
	{
		cout << "\nInvalid choice. Please enter again." << endl;
		return wantContinue(process);
	}
}
int Utils::inputChoice(int min, int max) {
	int choice;
	cout << "\nChoice : ";
	cin >> choice;
	cin.clear();
	cin.ignore(INT_MAX, '\n');
	if (choice<min || choice>(max) || (!choice))
	{
		cout << "Invalid choice. \n";
		cout << "Please enter again. \n";
		return inputChoice(min, max);
		
	}
	return choice;
}

bool Utils::validDate(string date,bool boolBefore) {//will return day name but i didnt use it
	string tempDate = date;
	if (tempDate.size() != 10)
		return false;
	for (int i = 0;i < 10;i++)
	{
		char temp = tempDate.back();
		tempDate.pop_back();
		if (i == 2 or i==5 ) {
			if (temp != '-')
				return false;
		}
	
	}
	DbConn dbCon;
	string stat = "SELECT IFNULL(DAYNAME(\'" + date + "\'),\'\')";
	PreparedStatement* pstat = dbCon.prepareStatement(stat);

	ResultSet* rs = pstat->executeQuery();

	if (rs->next()) {

		if (rs->getString(1) != "")
		{

			delete rs;
			delete pstat;

			PreparedStatement* before = dbCon.prepareStatement("SELECT timestampdiff(day,'1990-01-01',?)");
			before->setString(1, date);

			ResultSet* rsBefore = before->executeQuery();
			rsBefore->next();
			int day = rsBefore->getInt(1);

			delete before;
			delete rsBefore;
			if (day < 0) {
				cout << "\nInvalid date. Cats and dogs usually will not live more than 30 years old." << endl;
				return false;
			}

			PreparedStatement* after = dbCon.prepareStatement("SELECT timestampdiff(day,?,CURRENT_DATE)");
			after->setString(1, date);

			ResultSet* rsAfter = after->executeQuery();
			rsAfter->next();
			day = rsAfter->getInt(1);

			delete after;
			delete rsAfter;

			if (day < 0) {
				cout << "\nInvalid date. Future date is not allowed." << endl;
				return false;
			}
			return true;
		}
		else
		{
			cout << "\nInvalid date format. Date format : YYYY-MM-DD" << endl;
			return false;
		}
	}
	else
	{
		cout << "Invalid Date" << endl;//usually wont come in this
		delete rs;
		delete pstat;
		return false;
	}



}
bool Utils::validIcNumber(string icNumber) {
	string testIc = icNumber;
	if (icNumber.size() != 12 && icNumber.size() != 14)
	{
		cout << "Invalid Ic Number" << endl;
		return false;
	}
	if (icNumber.size() == 12)
	{
		for (int i = 0;i < 12;i++)
		{
			int test=testIc.back();

			if (test >= 48 && test <= 57)
			{
				if (i == 7 || i == 6) {

			}
				testIc.pop_back();
			
			}
			else
				return false;
		}
	}
}