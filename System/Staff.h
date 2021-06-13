#include <string>
#include <vector>
using namespace std;
#ifndef STAFF_H
#define STAFF_H

class Staff
{
public:
	string staff_Id, firstName,lastName,position,username,
		password,hpNumber,gender,email,dateStartWorking,status;
	Staff* next;
	vector<string> attributeInTable() {//name exactly same as database
		vector<string> attStaff;
		attStaff.push_back("Staff_Id");
		attStaff.push_back("firstName");
		attStaff.push_back("lastName");
		attStaff.push_back("position");
		attStaff.push_back("username");
		attStaff.push_back("password");
		attStaff.push_back("hpNumber");
		attStaff.push_back("gender");
		attStaff.push_back("email");
		attStaff.push_back("dateStartWorking");
		attStaff.push_back("status");
		return attStaff;
	}
};


#endif