#include "V2xApps.h"

#include <sstream>      // std::stringstream, std::stringbuf
#include <iostream> //cout




using std::cerr;
using std::cout;
using std::endl;
using std::string;

#define PI 3.14159265359
extern long THIS_APP_ID;

//converts data from VehData veh to BSM text
string createBSMcoreData(VehData veh) {


	std::stringstream tekst;
	tekst << veh.orient_heading << ";" << veh.position_x << ";" << veh.position_y << ";" << veh.position_z << ";" << veh.speed << ";" << veh.vehicle_id;
	return tekst.str();


}

//convert BSM text to VehData struct
VehData readVehDatafromString(std::string message) {


		VehData _veh;
		std::stringstream  lineStream(message);
		std::string cell;

		std::getline(lineStream, cell, ';');
		_veh.orient_heading = std::atof(cell.c_str());
	
		std::getline(lineStream, cell, ';');
		_veh.position_x = std::atof(cell.c_str());

		std::getline(lineStream, cell, ';');
		_veh.position_y = std::atof(cell.c_str());

		std::getline(lineStream, cell, ';');
		_veh.position_z = std::atof(cell.c_str());

		std::getline(lineStream, cell, ';');
		_veh.speed = std::atof(cell.c_str());

		std::getline(lineStream, cell, ';');
		_veh.vehicle_id = std::atol(cell.c_str());

		return _veh;



}

//returns position of point after rotation
Point2D RotatePoint(Point2D point, double angle)
{
	Point2D _point;
	_point.x = point.x * cos(angle) - point.y * sin(angle);
	_point.y = point.y * cos(angle) + point.x * sin(angle);

	return _point;
};

// helper for PointInTriangle function
float sign(Point2D p1, Point2D p2, Point2D p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

//checks if point pt is inside a triangle v1,v2,v3
bool PointInTriangle(Point2D pt, Point2D v1, Point2D v2, Point2D v3)
{
	bool b1, b2, b3;

	b1 = sign(pt, v1, v2) < 0.0f;
	b2 = sign(pt, v2, v3) < 0.0f;
	b3 = sign(pt, v3, v1) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}

//checks if tested vehicle needs to brake based on if vehicles in front hard brake
//if it does, returns distance from vehicle hard braking and tested vehicle (to give tested vehicle ample time to brake)
//if it doesn't, returns -1 (no warning)

float emergencyBrakeWarning(double h_x, double h_y, double h_s, double h_h, double t_x, double t_y, double t_h, double t_s) 
{
	//h_ human controlled vehicle
	//t_ tested vehicle
	//s: speed
	//no warning if brakeBoost is unavailable -> distance = -1
	
	float distance = -1;

	
	h_h = fmod((h_h + (2 * PI)), (2 * PI));
	t_h = fmod((t_h + (2 * PI)), (2 * PI));


	double alpha_right = h_h + (0.65 * PI);
	alpha_right = fmod((alpha_right + (2 * PI)), (2 * PI));

	double alpha_left = h_h - (0.65 * PI);
	alpha_left = fmod((alpha_left + (2 * PI)), (2 * PI));

	Point2D a_oryg = { 0,4 };
	Point2D b_oryg = { 200,4};
	Point2D c_oryg = { 200, -2 };
	Point2D d_oryg = { 0, -2};


	Point2D a = RotatePoint(a_oryg, h_h);
	Point2D b = RotatePoint(b_oryg, h_h);
	Point2D c = RotatePoint(c_oryg, h_h);
	Point2D d = RotatePoint(d_oryg, h_h);
	
	Point2D x_veh_point;
	//Check if in same lane and in front:(or somewhat close in case a vehicle is turning into the lane)												
	if (abs(h_x - t_x) < 2.0)	

	//(t_h < alpha_left && t_h > alpha_right) || ((alpha_left<(-PI / 2) && alpha_right>(PI / 2)) && (t_h < alpha_left || t_h > alpha_right)	
	{
		a = { a.x + h_x, a.y + h_y };
		b = { b.x + h_x, b.y + h_y };
		c = { c.x + h_x, c.y + h_y };
		d = { d.x + h_x, d.y + h_y };

		x_veh_point = { t_x, t_y };
		
		if ((PointInTriangle(x_veh_point, a, b, c) || PointInTriangle(x_veh_point, a, c, d)) && (t_s == 0))
		{
			//show warning and distance
			distance = sqrt(pow((t_x - h_x), 2.0) + pow((t_y - h_y), 2.0));

		}


	}


	return distance;
	
}



//checks if tested vehicle (t_) is on the opposite lane on the road
//if it does, return distance from human veh and other('tested") veh
//if it doesn't return -1   which means no warning
float doNotPassWarning(double h_x, double h_y, double h_h, double t_x, double t_y, double t_h)
{
	// h_  human controlled veh
	// t_  tested veh

	//no warning -> distance = -1
	float distance = -1;


	//normalization
	h_h = fmod((h_h + (2 * PI)), (2 * PI));
	t_h = fmod((t_h + (2 * PI)), (2 * PI));


	double alpha_right = h_h + (0.65 * PI);
	alpha_right = fmod((alpha_right + (2 * PI)), (2 * PI));

	double alpha_left = h_h - (0.65 * PI);
	alpha_left = fmod((alpha_left + (2 * PI)), (2 * PI));


	Point2D a_oryg = { 0,11 };
	Point2D b_oryg = { 260,18 };
	Point2D c_oryg = { 260,-3 };
	Point2D d_oryg = { 0,-3 };


	Point2D a = RotatePoint(a_oryg, h_h);
	Point2D b = RotatePoint(b_oryg, h_h);
	Point2D c = RotatePoint(c_oryg, h_h);
	Point2D d = RotatePoint(d_oryg, h_h);

	Point2D x_veh_point;

	if ((t_h < alpha_left && t_h > alpha_right) || ((alpha_left<(-PI / 2) && alpha_right>(PI / 2)) && (t_h < alpha_left || t_h > alpha_right)))
	{
		a = { a.x + h_x, a.y + h_y };
		b = { b.x + h_x, b.y + h_y };
		c = { c.x + h_x, c.y + h_y };
		d = { d.x + h_x, d.y + h_y };

		x_veh_point = { t_x, t_y };

		if (PointInTriangle(x_veh_point, a, b, c) || PointInTriangle(x_veh_point, a, c, d))
		{
			//show warning and distance
			distance = sqrt(pow((t_x - h_x), 2.0) + pow((t_y - h_y), 2.0));

		}


	}


	return distance;

}




