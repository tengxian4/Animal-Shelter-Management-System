#include "Staff.h"
#include <vector>
#ifndef  STAFFMANAGER_H
#define STAFFMANAGER_H

class StaffManager
{
public: 
	void displayAllStaff();
	void choiceManageStaff();
	void viewStaff();
	int insertStaff();
	int editStaff(vector<string> staffId_edit);

	string selectLastStaffId();
	Staff* selectStaff(string findItem,int choice);
};
#endif // ! STAFFMANAGER_H
