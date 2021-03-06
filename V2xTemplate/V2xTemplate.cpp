// V2xTemplate.cpp : Defines the entry point for the console application.
//

#include <iostream> //cout
#include "V2xApps.h"


int main()
{
	//float doNotPassWarning(double h_x, double h_y, double h_h, double t_x, double t_y, double t_h)
	// h_  human controlled (subjectCar) veh
	// t_  tested (otherCar) veh
	
	float distance;
	float distance2;

	//test data

	VehData subjectCar;
	VehData otherCar;


	subjectCar.position_x = -51.6797;
	subjectCar.position_y = 2717.24;
	subjectCar.orient_heading = 4.74931;
	subjectCar.vehicle_id = 1;




	//option 1: other car at the opposite lane, opposite direction(heading)
	/*
	otherCar.position_x = -48.7644;
	otherCar.position_y = 2690.96;
	otherCar.orient_heading = 1.48015;
	otherCar.vehicle_id = 2;
	otherCar.speed = 0;
	*/
	
	//option 2: other car on the same lane
	otherCar.position_x = -51.5797;	
	otherCar.position_y = 2682.95;
	otherCar.orient_heading = 4.70041;
	otherCar.vehicle_id = 2;
	otherCar.speed = 20;
	/*
	
	otherCar.position_x = -52;
	otherCar.position_y = 2700;
	otherCar.orient_heading = 4.7;
	otherCar.vehicle_id = 2;
	otherCar.speed = 0;
	*/








	distance = doNotPassWarning(subjectCar.position_x, subjectCar.position_y, subjectCar.orient_heading, otherCar.position_x, otherCar.position_y, otherCar.orient_heading);
	distance2 = emergencyBrakeWarning(subjectCar.position_x, subjectCar.position_y, subjectCar.speed, subjectCar.orient_heading, otherCar.position_x, otherCar.position_y, otherCar.orient_heading, otherCar.speed);
	//std::cout << "DNPW app, distance=" <<distance<<  std::endl;
	std::cout << "EBW app,  distance=" << distance2 << std::endl;

	//press any key and enter
	char x;
	std::cin >> x;
	
	return 0;
}

