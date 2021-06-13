#include <string>
#include <vector>

using namespace std;
#ifndef  ANIMAL_H
#define ANIMAL_H


class Animal
{
private:
	vector<string> attribute{ "animal_Id" ,"type","breed" ,"gender" ,"sterilized","dateCameIn",
		"dateBorn","status","staff_Id","adoption_Id" };
	vector<string> item{ "Animal Id" ,"Type" ,"Breed","Gender" ,"Sterilized" ,"Date Came In" ,"Date born",
	"Status" };//,"Staff Id" ,"Adoption Id"

	//for searching		
	vector<string> searchItem = { "Animal Id","Type","Breed","Gender","Status" };
	vector<int> indexSearchItem = {0,1,2,3,7};
	int totalSearchItem = 5;
public : 


	string animal_Id, type, breed, gender, sterilized, dateCameIn, dateBorn, status, staff_Id,adoption_Id;	
	vector <string>	attributesInTable() {	
		return attribute;
	}
	vector<string> items() { //for viewing purpose
		return item;
	}
	Animal* next;
	vector<string> getSearchItem() {
		return searchItem;
	}
	vector<int> getIndexSItem() {
		return indexSearchItem;
	}
	int getTotalSearchItem() {
		return totalSearchItem;
	}
};
#endif // ! ANIMAL_H
