#include <vector>
#include <iomanip>
#include <string>
using namespace std;
#ifndef UTILS_H
#define UTILS_H
class Utils {
public :
	int inputChoice(int min, int max,bool firstTime);
	void exitProgram();
	//vector<string> attributeStaff();
	bool dateFormat(string test);
	bool validDate(string date);
	bool validDate(string date, bool boolBefore);
	bool wantContinue(string process);
	int inputChoice(int min, int max);
	bool validIcNumber(string icNumber);

};
#endif // !UTILS_H

