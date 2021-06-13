#include <iostream>
using namespace std;


#include <mysql/jdbc.h>
using namespace sql;
#include "DbConn.h"
#include "StaffPage.h"
#include "Utils.h"
#include "Login.h"
#include "AnimalManager.h"
#include "AdoptionManager.h"
StaffPage::StaffPage(string currentUser) {
	this->currentUser = currentUser;
}
void StaffPage::displayPage() {
	Utils utils;
	int choice = 0;
	
	while (choice != 3)
	{
		cout << "\tMain Menu for Staff Page\n" << endl;
		cout << "Please enter your choice you wish to do " << endl;
		cout << "\n\t1. Animal Management\n\t2. Adoption Animal Management";
		cout << "\n\t3. Go Back Previous Page\n";
		choice = utils.inputChoice(1, 3,true);

		if (choice == 1)
		{
			system("cls");
			manageAnimal();
		}
		else if (choice == 2)
		{
			system("cls");
			manageAdoption();

		}
		/*else if (choice == 3)
		{
			system("cls");
			Login login;
			login.displayLogin();
		}*/
	}
}


void StaffPage::manageAnimal() {
	Utils utils;
	int choice = 0;


	while (choice != 4)
	{
		AnimalManager animalManager(currentUser);
		animalManager.displayAllAnimal();


		cout << "\n\n\tAnimal Management Page :\n\t";
		cout << "\n\tPlease enter your choice number you wish to do \n";
		cout << "\t1. View Animal or Edit Animal's Details\n"
			<< "\t2. Register New Animal\n"
			<< "\t3. Delete Animal\n"
			<< "\t4. Go Back Previous Page\n";
		choice = utils.inputChoice(1, 4,true);

		if (choice == 1)
		{
			system("cls");
			animalManager.viewAnimal();
		}
		else if (choice == 2)
		{
			int status = animalManager.insertAnimal();
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
		/*else if (choice == 4) {
			system("cls");
			cout << "\n\tAnimal Management Page: Submenu 4" << endl;
			cout << "\tReport" << endl;
			Report report;
			report.viewDiedAnimal();
			report.sumMedicalCosts();
			report.viewAnimalAge();
			report.totalAvailable();
			system("pause");
		}want report for staff to access?*/
		system("cls");

	}
}
void StaffPage::manageAdoption() {
	Utils utils;
	int choice = 0;
	while (choice != 4)
	{
		system("cls");
		AdoptionManager adoptionManager(currentUser);


		cout << "\n\n\tAdoption Management Page:\n\t";
		cout << "Please enter your choice number you wish to do \n";
		cout << "\t1. Register an Adoption\n";
		cout << "\t2. View Owner or Edit Owner's details\n";
		cout << "\t3. Register New Owner of Adopted Animal\n";
		cout << "\t4. Go Back Previous Page\n";
		choice = utils.inputChoice(1, 4,true);

		if (choice == 1)
		{
			int status = adoptionManager.registerAdoption();
			if (status != 0)
				cout << "\nSuccessfully register an adoption" << endl;
			else
				cout << "\nFailed to register an adoption" << endl;
			system("pause");
		}
		else if (choice == 2)
		{
			system("cls");
			adoptionManager.viewOwner();
			/*cout << "\n\tAdoption Management: Submenu 2";
			cout << "\n\tView Owner or Edit Owner's Details\n";
			adoptionManager.displayAllOwner();
			system("pause");*/
		}
		else if (choice == 3)
		{
			int status = adoptionManager.insertOwner();
			if (status != 0)
				cout << "\nSuccessfully added an owner" << endl;
			else
				cout << "\nFailed to add an owner" << endl;
			system("pause");
		}
	
	}
}