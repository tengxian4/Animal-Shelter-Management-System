#include "Animal.h"
#include <string>
#ifndef ANIMALMANAGER_H
#define ANIMALMANAGER_H
class AnimalManager {
private:
	string currentUser;
public:
	AnimalManager(string currentUser);
	void displayAllAnimal();
	void viewAnimal();
	Animal* selectAnimal(string attribute, string findItem, int& noItem);
	int editAnimal(string editAniId);
	int editAnimal(string editAniId,string selectAttribute, string updateDetail);//method overloading for update all the selected animal
	int insertAnimal();
	string generateLastId();
	int deleteAnimal();
	int updateStatus(string animal_Id);
	void viewColumnData(int choice);
	

};
#endif // !ANIMALMANAGER_H
