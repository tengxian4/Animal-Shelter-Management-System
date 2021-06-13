#ifndef STAFFPAGE_H
#define STAFFPAGE_H

class StaffPage 
{
private:
	string currentUser;
public:
	StaffPage(string currentUser);
	void displayPage();
	void manageAnimal();
	void manageAdoption();
};
#endif