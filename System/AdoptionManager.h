#include <string>
#include "OwnerOfAdoptedAnimal.h"
#ifndef ADOPTIONMANAGER_H
#define ADOPTIONMANAGER_H

class AdoptionManager {
private:
	string currentUser;
public:
	
	AdoptionManager(string currentUser);
	void displayAllOwner();
	int insertOwner();
	string generateOwnerId();
	int registerAdoption();
	string generateAdoptionId();
	OwnerOfAdoptedAnimal* selectOwner(string owner_Id);
	int editOwner(string owner_Id);
	//int insertAdoption();
	int viewOwner();

	
	
};
#endif