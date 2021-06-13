#include <Windows.h>
#include <conio.h>
#include <algorithm>
#include <iostream>

using namespace std;

#include <mysql/jdbc.h>
using namespace sql;

#include "DbConn.h"
#include "Staff.h"
#include "Login.h"
#include "AdminPage.h"
#include "StaffPage.h"
#include "Utils.h"



#define enter '\n'
#define asciiEnter 13
#define backspace 8




Staff* Login::selectStaff(string selectUsername, string selectPassword)
{
	Staff* staff = NULL;
	DbConn dbCon;
	PreparedStatement* pstat = dbCon.prepareStatement("SELECT * FROM Staff WHERE username= ? AND password= ?");

	pstat->setString(1, selectUsername);
	pstat->setString(2, selectPassword);

	ResultSet* rs = pstat->executeQuery();

	if (rs->next())
	{
		staff = new Staff();
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
		staff->status = rs->getString(11);
	}
	else
		return false;

	delete rs;
	delete pstat;
	return staff;
}

int Login::displayLogin() {


	

	int choice = 0;
	while (choice !=2)
	{
		system("cls");
		char pss = '0';
		string password = "";
		string username = "";
		cout << "\n";
		cout << setw(50) << setfill('*') << "\n";
		cout << "\t\tAnimal Management Sytem\n";
		cout << setw(50) << setfill('*') << "\n";
	
		cout << "\t\t\tLogin Page\n";
	
		
		cout << "Username     : ";
		getline(cin, username);
		
		cout << "Password     : ";
		
		pss = _getch();
		while (pss!=asciiEnter) {
			if (pss == backspace) {
				if (password.size() > 0) {
					cout << "\b \b";
					password.pop_back();
				}
			}
			else
			{
				password.push_back(pss);
				cout << "*";
			}
			pss = _getch();
		
			
	}	
		Staff* staff = NULL;
		staff = selectStaff(username, password);


		if (staff)
		{
			string status = staff->status;

			transform(status.begin(), status.end(), status.begin(), toupper);
			if (status != "LEFT")
			{
				userNow = staff->staff_Id;
				string checkUser = staff->position;
				transform(checkUser.begin(), checkUser.end(), checkUser.begin(), toupper);
				if (checkUser == "ADMIN")
				{

					system("cls");
					cout << "\nWelcome, " << staff->firstName << endl << endl;
					AdminPage adminPage(staff->staff_Id);
					//adminPage->setCurrentUser(staff->staff_Id);
					adminPage.displayPage();
				}
				else
				{
					system("cls");
					cout << "\nWelcome, " << staff->firstName << endl << endl;
					StaffPage staffPage(staff->staff_Id);
					staffPage.displayPage();
				}
			}
			else
			{
				cout << "\nwrong username or password \n";

				cout << "\t1.Enter again?\n";
				cout << "\t2.Exit Program\n";
				cout << "Choice : ";
				cin >> choice;

				cin.clear();
				cin.ignore(INT_MAX, '\n');
				while (choice < 1 || choice>2) {
					cout << "\nInvalid choice. Please enter again.\n";
					cout << "Choice : ";
					cin >> choice;
					cin.clear();
					cin.ignore(INT_MAX, '\n');
				}


			}
		}
		else
		{
			cout << "\nwrong username or password \n";

			cout << "\t1.Enter again?\n";
			cout << "\t2.Exit Program\n";
			cout << "Choice : ";
			cin >> choice;

			cin.clear();
			cin.ignore(INT_MAX, '\n');
			while (choice < 1 || choice>2) {
				cout << "\nInvalid choice. Please enter again.\n";
				cout << "Choice : ";
				cin >> choice;
				cin.clear();
				cin.ignore(INT_MAX, '\n');
			}
		}
	}
	Utils utils;
	utils.exitProgram();
}



string Login::getUserNow()
{
	return userNow;
}