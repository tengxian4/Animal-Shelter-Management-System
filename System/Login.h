
#include <string>
using namespace std;

#include "Staff.h"
#ifndef LOGIN_H
#define LOGIN_H

class Login 
{
private:
	string userNow;
public: 
	string getUserNow();
	int displayLogin();	
	Staff* selectStaff(string selectUsername, string selectPassword);



};
#endif