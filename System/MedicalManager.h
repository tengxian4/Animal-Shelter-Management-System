#ifndef MEDICALMANAGER_H
#define MEDICALMANAGER_H

class MedicalManager {
public :
	//MedicalManager(string animal_Id, string staff_Id);
	int insertMedical(string animal_Id, string staff_Id);
	int insertMedicine(int medical_Id);
//	void viewAllMedicine();
	void viewMedical(string animal_Id);

};

#endif // !MEDICAL_MANAGER
