#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;

#include <mysql/jdbc.h>
using namespace sql;

#include "DbConn.h"
#include "Report.h"
#include "Utils.h"
void Report::reportMenu() {
	Utils utils;
	int choice = 0;
	while (choice != 5)
	{
		system("cls");
		cout << "\n\tAnimal Management Page: Submenu 4" << endl;
		cout << "\tReport Menu\n";
		cout << "\n\t1. Number of Died Animal" << endl;
		cout << "\t2. Medical Costs" << endl;
		cout << "\t3. Age of Available Animal" << endl;
		cout << "\t4. Available Animal" << endl;
		cout << "\t5. Go Back Previous Page" << endl;
		choice = utils.inputChoice(1, 5, true);
		if (choice == 1) {
			system("cls");
			viewDiedAnimal();
			system("pause");
		}
		else if (choice == 2) {
			system("cls");
			sumMedicalCosts();
			system("pause");
		}
		else if (choice == 3) {
			system("cls");
			viewAnimalAge();
			system("pause");
		}
		else if (choice == 4) {
			system("cls");
			totalAvailable();
			system("pause");
		}
	}
}
void Report::viewDiedAnimal() {
	cout << endl;
	DbConn dbCon;
	PreparedStatement* pstat = dbCon.prepareStatement("SELECT * FROM DiedAnimal");

	ResultSet* rs = pstat->executeQuery();
	rs->next();
	cout << "-------------------------" << endl;
	cout << "| Number of Died Animal |" << endl;
	cout << "-------------------------" << endl;
	cout << "|   " << rs->getInt(1) << "                   |"<< endl;
	cout << "-------------------------" << endl;
	delete pstat;
	delete rs;
}
int Report::sumMedicalCosts() {
	cout << "\tMedical Costs Report Page" << endl;
	cout << "\nYou can choose to view the sum of medical costs in the specific date range." << endl;
	Utils utils;
	bool valid;

	string startDate;
	utils.dateFormat("date");
	for (int i = 0;i < 4;i++)
	{
		cout << "Please enter the start date  : ";
		getline(cin, startDate);
		valid = utils.validDate(startDate);
		if (valid)
			break;
		else
			cout << "\nPlease enter again. You have " << 3 - i << " times of try left. \n";
	}
	if (!valid) {
		cout << "\nWrong date format. If you want to enter, you can start again by choosing the option medical costs\n";
		cout << "Directing you back to Report Menu" << endl;
		return 0;
	}
	utils.dateFormat("date");
	string endDate;
	for (int i = 0;i < 4;i++)
	{
		cout << "Please enter the end date  : ";
		getline(cin, endDate);
		valid = utils.validDate(endDate);
		if (valid)
			break;
		else
			cout << "\nPlease enter again. You have " << 3 - i << " times of try left. \n";
	}
	if (!valid) {
		cout << "\nWrong date format. If you want to enter, you can start again by choosing the option medical costs\n";
		cout << "Directing you back to Report Menu" << endl;
		return 0;
	}

	DbConn dbCon;
	PreparedStatement* checkRange = dbCon.prepareStatement("SELECT timestampdiff(day,?,?)");
	checkRange->setString(1, startDate);
	checkRange->setString(2, endDate);

	ResultSet* rsCheck = checkRange->executeQuery();
	rsCheck->next();
	int checkDay=rsCheck->getInt(1);
	if (checkDay < 0) {
		cout << "\nStart date must earlier than end date.\n";
		cout << "\nFail to check medical costs. If you want to check, please choose the option Medical Costs again" << endl;
		
		return 0;
	}
	delete rsCheck;
	delete checkRange;

	PreparedStatement* pstat = dbCon.prepareStatement("SELECT SUM(costs) FROM medical WHERE date BETWEEN ? AND ?");
	pstat->setString(1, startDate);
	pstat->setString(2, endDate);
	ResultSet* rs = pstat->executeQuery();
	rs->next();
	cout << "\n\nSum of Medical Costs (RM) between " <<startDate <<" and " << endDate<<" : " << rs->getDouble(1) << endl;

	delete pstat;
	delete rs;
}
void Report::viewAnimalAge() {
	cout << endl;
	DbConn dbCon;
	PreparedStatement* pstat = dbCon.prepareStatement("SELECT animal_Id, timestampdiff(year,animal.dateBorn,CURRENT_DATE) FROM animal WHERE UPPER(status)=\'AVAILABLE\'");
	ResultSet* rs = pstat->executeQuery();
	cout << "-------------------------" << endl;
	cout << "| Animal Id |   Age    |" << endl;
	cout << "-------------------------" << endl;

	while (rs->next()) {
		
		
		cout <<"|"<< setfill(' ') << setw(11) <<left ;
		cout << rs->getString(1)<<"|";
		cout << setfill(' ') << setw(10) << left ;
		cout << rs->getString(2)<< "|";
		
		cout << endl;
	}
	cout << "-------------------------" << endl;
	delete pstat;
	delete rs;
}
void Report::totalAvailable() {
	cout << endl;
	cout << "\tAnimal Available Report Page" << endl<<endl;
	DbConn dbCon;
	PreparedStatement* pstat = dbCon.prepareStatement("SELECT COUNT(animal_Id) FROM animal WHERE UPPER(status) = 'AVAILABLE'");
	ResultSet* rs = pstat->executeQuery();
	rs->next();
	int total = rs->getInt(1);
	cout << "Total Animal Available : " << total << endl;

	delete pstat;
	delete rs;

	PreparedStatement* pCat = dbCon.prepareStatement("SELECT COUNT(animal_Id) FROM animal WHERE UPPER(status)='AVAILABLE' AND UPPER(type)='CAT'");
	ResultSet* rsCat = pCat->executeQuery();
	rsCat->next();
	int totalCat = rsCat->getInt(1);

	cout << "Number of cats available : " << totalCat << endl;
	delete pCat;
	delete rsCat;

	PreparedStatement* pDog = dbCon.prepareStatement("SELECT COUNT(animal_Id) FROM animal WHERE UPPER(status)='AVAILABLE' AND UPPER(type)='DOG'");
	ResultSet* rsDog = pDog->executeQuery();
	rsDog->next();
	int totalDog = rsDog->getInt(1);

	cout << "Number of dogs available : " << totalDog << endl;
	delete pDog;
	delete rsDog;

	double percentageCat = (double)totalCat / (double)total * 100;
	double percentageDog = (double)totalDog / (double)total * 100;
	cout << "The percentage of cats available over the total available animals is " <<  fixed<<setprecision(2)<<percentageCat<< " %" << endl;
	cout << "The percentage of dogs available over the total available animals is " << fixed << setprecision(2) <<  percentageDog << " %" << endl;
	
}