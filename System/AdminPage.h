#ifndef ADMINPAGE_H
#define ADMINPAGE_H
class AdminPage
{
private:
	string currentUser;
public:
	AdminPage(string currentUser);
	void displayPage();
	
void administrationStaff();
void manageAnimal();
void manageAdoption();
	//int inputChoice(int min, int max);

};
#endif // !ADMINPAGE_H
