#include <string>
#include <vector>
using namespace std;

#ifndef OWNEROFADOPTEDANIMAL_H
#define OWNEROFADOPTEDANIMAL_H

class OwnerOfAdoptedAnimal
{
private:
	vector<string> attribute{
		"owner_Id","firstName","lastName","IcNumber","gender","hpNumber","staff_Id" };
	vector<string> item{
		"Owner Id","First Name","Last Name","Ic Number/Pass. No","Gender","Hp Number","Staff Id"
	};
public:
	string owner_Id, firstName, lastName, IcNumber, gender, hpNumber, staff_Id;

	vector<string> attributesInTable() {
		return attribute;
	}
	vector <string>items() {
		return item;
	}
	OwnerOfAdoptedAnimal* next;
	int searchItems = 4;

};
#endif
