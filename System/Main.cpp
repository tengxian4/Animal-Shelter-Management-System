#include <Windows.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;



#include "Staff.h"
#include "Login.h"
#include "StaffManager.h"


int main()

{

	Login* login = new Login();

	login->displayLogin();

	return 0;
}
