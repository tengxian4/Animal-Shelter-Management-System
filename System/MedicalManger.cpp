#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

#include <mysql/jdbc.h>
using namespace sql;

#include "DbConn.h"
#include "MedicalManager.h"
#include "Medical.h"
#include "Medicine.h"
#include "Utils.h"

/*MedicalManager::MedicalManager(string animal_Id, string staff_Id) {
	this->animal_Id = animal_Id;
	this->staff_Id = staff_Id;
}
*/
int MedicalManager::insertMedical(string animal_Id, string staff_Id) {
	Utils utils;
	cout << "\n\tInsert Medical Details Page\n";
	Medical medical;
	cout << "\nPlease enter the medical details of the animal" << endl;

	cout << "Medical Condition : ";
	getline(cin, medical.medicalCondition);

	cout << "Veterinary Place  : ";
	getline(cin, medical.veterinaryPlace);
	bool cinCost;
	for (int i = 0;i < 4;i++)//three time of peluang to enter
	{
		cout << "Costs             : ";
		cin >> medical.costs;

		if (!cin)
		{
			cout << "Please key in the valid costs." << endl;
			cout << "\nPlease enter again. You have " << 3 - i << " times of try left. \n";
			cinCost = false;
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}
		else
		{
			cinCost = true;
			break;
		}
	}
	if (!cinCost) {
		cout << "Directing you back." << endl;
		system("pause");
		return 0;
	}
	cin.clear();
	cin.ignore(INT_MAX, '\n');
	bool valid;
	for (int i = 0;i < 4;i++)//three time of peluang to enter
	{
		cout << "Format of date, YYYY-MM-DD" << endl;
		cout << "Date              : ";
		getline(cin, medical.date);
		valid = utils.validDate(medical.date);

		if (valid)
			break;
		else
			cout << "\nPlease enter again. You have " << 3 - i << " times of try left. \n";
	}
	if (!valid) {	
		cout << "Directing you back." << endl;
		system("pause");
		return 0;
	}

	DbConn dbConn;
	PreparedStatement* beforeBorn = dbConn.prepareStatement("SELECT timestampdiff(day,dateBorn,?) FROM animal WHERE animal_Id=?");
	beforeBorn->setString(1, medical.date);
	beforeBorn->setString(2, animal_Id);

	ResultSet* rs = beforeBorn->executeQuery();
	rs->next();
	int day = rs->getInt(1);
	delete rs;
	delete beforeBorn;
	if (day < 0) {
		cout << "\nIt is impossible for the animal to have medical consultation before born." << endl;
		cout << "\nWrong date. If you want to edit, you can start again the View and Edit Animal\n";
		system("pause");
		return 0;
	}
	bool cont=utils.wantContinue("insert the medical information");

	if (cont == false)
		return 0;

	medical.animal_Id = animal_Id;
	medical.staff_Id = staff_Id;

	DbConn dbCon;
	PreparedStatement* pstat = dbCon.prepareStatement("INSERT INTO Medical VALUES(0,?,?,?,?,?,?)");
	pstat->setString(1, medical.medicalCondition);
	pstat->setString(2, medical.veterinaryPlace);
	pstat->setDouble(3, medical.costs);
	pstat->setString(4, medical.date);
	pstat->setString(5, medical.animal_Id);
	pstat->setString(6, medical.staff_Id);

	int status = pstat->executeUpdate();
	delete pstat;
	if (status == 0)
		return 0;


	cout << "\nAny medicine needed for the medical condition?\n ";
	cout << "\t1. Yes" << endl;
	cout << "\t2. No" << endl;
	int choice = utils.inputChoice(1, 2);
	if (choice == 1) {
		DbConn dbCon;
		string stat="SELECT AUTO_INCREMENT FROM information_schema.tables WHERE table_name = 'medical' AND table_schema = 'dbasm'";
		PreparedStatement* pstat = dbCon.prepareStatement(stat);
		ResultSet* rs = pstat->executeQuery();
		
		rs->next();
			int medical_Id =( rs->getInt(1)-1);
		
			delete rs;
		delete pstat;
		int choice1 = 0;
		
			while (choice1 != 2)
			{
				system("cls");
				int status = insertMedicine(medical_Id);
				if (status != 0)
					cout << "\nSuccessfully inserted the medicine" << endl << endl;
				else
					cout << "\nFailed to insert the medicine" << endl;

				cout << "\nNeed to add more medicine or want to add again the medicine?" << endl;
				cout << "\t1. Yes\n";
				cout << "\t2. No\n";
				choice1 = utils.inputChoice(1, 2);
			}
			
	}
	else if(choice==2) {
		return status;
	}


}
/*void MedicalManager::viewAllMedicine() {
	DbConn dbCon;
	string stat = "SELECT * FROM Medicine";
	PreparedStatement* pstat = dbCon.prepareStatement(stat);
	ResultSet* rs = pstat->executeQuery();
	vector<string> columnName(4);

	columnName = { "Medicine Id  ","Medicine Name            ","Ingredients                               " ,"Medical Id" };


	int a = 1;
	for (int i = 0;i < columnName.size();i++) //find the column size
	{
		a += columnName[i].size();
		a += 1;
	}
	cout << setw(a) << setfill('-') << "-" << " \n" << "| ";//need | with a space behind

	for (int i = 0;i < columnName.size();i++)//print column name
	{
		cout << setfill(' ') << setw(columnName[i].size()) << left << columnName[i] << "|";
	}
	cout << setw(a) << setfill('-') << "\n" << "\n";//this setw and setfill is from right side then one \n is for line before 

	while (rs->next()) {
	
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

}*/
int MedicalManager::insertMedicine(int medical_Id) {
	system("cls");
	cout << "\n\tAdd Medicine Page";
	Medicine medicine;
	Utils utils;
	bool valid;
	cout << "\nPlease enter the medicine details" << endl;

	cout << "Medicine Name  : ";
	getline(cin, medicine.medicineName);

	cout << "Ingredients    : ";
	getline(cin, medicine.ingredients);

	
	if (medicine.medicineName == "") {
		cout << "Medicine name is required." << endl;
		return 0;
	}
	if (medicine.ingredients == "") {
		cout << "Medicine ingredients is required." << endl;
		return 0;
	}

	bool cont = utils.wantContinue("insert the medicine information");

	if (cont == false)
		return 0;
	DbConn dbCon;


	PreparedStatement* pstat = dbCon.prepareStatement("INSERT INTO Medicine VALUES (0,?,?,?)");
	pstat->setString(1, medicine.medicineName);
	pstat->setString(2, medicine.ingredients);
	pstat->setInt(3, medical_Id);
	int status = pstat->executeUpdate();
	delete pstat;


	system("pause");
	return status;
}
void MedicalManager::viewMedical(string animal_Id) {

	Medical medical;
	Medicine medicine;
	transform(animal_Id.begin(), animal_Id.end(), animal_Id.begin(), toupper);
	string stat = "SELECT ml.medicalCondition, ml.date, ml.veterinaryPlace, ml.costs, m.medicineName, m.ingredients from  medical ml LEFT join medicine m ON m.medicine_Id = ml.medical_Id WHERE UPPER(animal_Id)=\'"+animal_Id+"\'";
	DbConn dbCon;
	PreparedStatement* pstat = dbCon.prepareStatement(stat);

	ResultSet* rs = pstat->executeQuery();


	vector <string> columnName(6);
	columnName = { "Medical Condition                    ","Date         ","Veterinary Place                    ","Costs (RM)","Medicine Name       " ," Ingredients                  " };
	
	

	int a = 1;
	for (int i = 0;i < columnName.size();i++) //find the column size
	{
		a += columnName[i].size();
		a += 1;
	}
	cout << setw(a) << setfill('-') << "-" << " \n" << "| ";//need | with a space behind
	for (int i = 0;i < columnName.size();i++)//print column name
	{
		cout << setfill(' ') << setw(columnName[i].size()) << left << columnName[i] << "|";
	}

	cout << setw(a) << setfill('-') << "\n" << "\n";

	while (rs->next())
	{
		cout << "| ";
		medical.medicalCondition=rs->getString(1);
		cout << setfill(' ') << setw(columnName[0].size()) << left << medical.medicalCondition << "|";

		medical.date=rs->getString(2);
		cout << setfill(' ') << setw(columnName[1].size()) << left << medical.date << "|";

		medical.veterinaryPlace=rs->getString(3);
		cout << setfill(' ') << setw(columnName[2].size()) << left << medical.veterinaryPlace << "|";

		medical.costs=rs->getDouble(4);
		cout << setfill(' ') << setw(columnName[3].size()) << left << medical.costs << "|";

		medicine.medicineName = rs->getString(5);
		cout << setfill(' ') << setw(columnName[4].size()) << left << medicine.medicineName << "|";

		medicine.ingredients = rs->getString(6);
		cout << setfill(' ') << setw(columnName[5].size()) << left << medicine.ingredients << "|";
		cout << "\n";
		
	}
	delete rs;
	delete pstat;

}
