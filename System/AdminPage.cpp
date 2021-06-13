#include <iostream>
using namespace std;
#include "AdminPage.h"
#include "StaffManager.h"
#include "Utils.h"
#include "Login.h"
#include "AnimalManager.h"
#include "AdoptionManager.h"
#include "Report.h"
#include <mysql/jdbc.h>
using namespace sql;
#include "DbConn.h"


AdminPage::AdminPage(string currentUser)
{
	this->currentUser = currentUser;
}
void AdminPage::displayPage() {
	

	Utils utils;

	int choice = 0;
	while (choice!=4)
	{ 
		cout << "\n\tMain Menu for Admin Page\n\n";
		cout << "\tPlease enter your choice number you wish to do " << endl;
		cout << "\t1. Administration Staff"
			<< "\n\t2. Animal Management\n\t3. Adoption Animal Management";
		cout << "\n\t4. Go Back Previous Page\n";
		 choice = utils.inputChoice(1, 4,true);
		if (choice == 1)
		{
			cout << "\n ";
			system("cls");
			administrationStaff();
		}
		else if (choice == 2)
		{
			system("cls");
			manageAnimal();
		}
		else if (choice == 3)
		{
			manageAdoption();
			system("cls");
		}
		/*else if (choice == 4)
		{
			system("cls");
			Login* login = new Login();
			login->displayLogin();
		}*/
	}


}

/*int AdminPage::inputChoice(int min,int max) {
	int choice;
	string decision;

	cout << "\nChoice : ";
	cin >> choice;
	if (choice<min || choice>max)
	{
		cout << "Invalid choice. \n";
		cout << "Please enter again. \n";
		return inputChoice(min, max);
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
		}
	}
	else
	{
		return choice;
	}
}*/

void AdminPage::administrationStaff() { //manage Staff
	int choice1=0;

	

	//	staffManager->choiceManageStaff();
	
	while (choice1 != 3)
	{
		StaffManager* staffManager = NULL;
		staffManager->displayAllStaff();
		cout << "\n\n\tAdministration Staff\n\t--Manage Staff Page--\n";
		cout << "\n\tPlease enter your choice number you wish to do \n";
		cout << "\t1. View Staff or Edit Staff's Details\n"
			<< "\t2. Register New Staff\n"
			<< "\t3. Go Back Previous Page\n";

		Utils utils;
		choice1 = utils.inputChoice(1, 3,true);
		if (choice1 == 1)
		{
			system("cls");
			 staffManager->viewStaff();
			//cout << "value of rtn is " <<rtn << endl;
			/*if (rtn == -10)
				administrationStaff();//means go back*/
		}
		else if (choice1 == 2)
		{
			system("cls");

			int status =staffManager->insertStaff();
			if (status != 0)
				cout << "Successfully inserted a staff" << endl;
			else
				cout << "Failed to insert a staff" << endl;
			system("pause");
		}
		system("cls");
	}
	/*else if (choice1 == 4)
	{
		system("cls");
		displayPage();
	}*/

}
void AdminPage::manageAnimal() {
	Utils utils;
	int choice = 0;
	

	while (choice != 5)
	{
		AnimalManager animalManager(currentUser);
		animalManager.displayAllAnimal();
	

		cout << "\n\n\tAnimal Management Page :\n\t";
		cout << "\n\tPlease enter your choice number you wish to do \n";
		cout << "\t1. View Animal or Edit Animal's Details\n"
			<< "\t2. Register New Animal\n"
			<< "\t3. Delete Animal\n"
			<<"\t4. Report\n"
			<< "\t5. Go Back Previous Page\n";
		choice = utils.inputChoice(1, 5,true);
		
		if (choice == 1)
		{
			system("cls");
			animalManager.viewAnimal();
		}
		else if (choice == 2)
		{
			int status=animalManager.insertAnimal();
			if (status != 0)
				cout << "\nSuccessfully added an animal" << endl;
			else
				cout << "\nFailed to add an animal" << endl;
			system("pause");
		}
		else if (choice == 3)
		{
			system("cls");
			int status = animalManager.deleteAnimal();
			if (status != 0)
				cout << "\nSuccessfully deleted the animal" << endl;
			else
				cout << "\nFailed to delete the animal" << endl;
			system("pause");
		}
		else if (choice == 4) {
		
			Report report;
			report.reportMenu();
			
		}
		system("cls");

	}
}
void AdminPage::manageAdoption() {
	Utils utils;
	int choice = 0;
	while (choice != 4)
	{
		system("cls");
		AdoptionManager adoptionManager(currentUser);
		
	
		cout << "\n\n\tAdoption Management Page:\n\t";
		cout << "Please enter your choice number you wish to do \n";
		cout << "\t1. Register an Adoption\n";
		cout << "\t2. View Owner or Edit Owner's Details\n";
		cout << "\t3. Register New Owner of Adopted Animal\n";
		cout << "\t4. Go Back Previous Page\n";
		choice = utils.inputChoice(1, 4,true);

		if (choice == 1)
		{
			int status=adoptionManager.registerAdoption();
			if (status != 0)
				cout << "\nSuccessfully registered an adoption" << endl;
			else
				cout << "\nFailed to register an adoption" << endl;
			system("pause");
		}
		else if (choice == 2)
		{
			system("cls");
			adoptionManager.viewOwner();

		}
		else if (choice == 3)
		{
			int status=adoptionManager.insertOwner();
			if (status != 0)
				cout << "\nSuccessfully added an owner" << endl;
			else
				cout << "\nFailed to add an owner" << endl;
			system("pause");
		}

	}
	

}	