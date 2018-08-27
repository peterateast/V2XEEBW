#pragma once

#include <string> 

using std::string;


struct Point2D
{
	double x;
	double y;
};


struct VehData
{
	long timestamp;
	long vehicle_id;
	long vehicle_type;
	string model_file_name;
	long color;
	double position_x;
	double position_y;
	double position_z;
	double orient_heading;
	double orient_pitch;
	double orient_roll;
	double speed;
	long leading_vehicle_id;
	long trailing_vehicle_id;
	long link_id;
	string link_name;
	double link_coordinate;
	long lane_index;
	long turning_indicator;
};

string createBSMcoreData(VehData veh);

VehData readVehDatafromString(string message);

Point2D RotatePoint(Point2D point, double angle);

float sign(Point2D p1, Point2D p2, Point2D p3);

bool PointInTriangle(Point2D pt, Point2D v1, Point2D v2, Point2D v3);

float doNotPassWarning(double h_x, double h_y, double h_h, double t_x, double t_y, double t_h);

float emergencyBrakeWarning(double h_x, double h_y, double h_s, double h_h, double t_x, double t_y, double t_h, double t_s);




//	timestamp	topic	veh_color	lane_index	leading_veh_id	link_coordinate	link_id	link_name	veh_model_file_name		orient_heading	orient_pitch	orient_roll		position_x	position_y	position_z	speed	trailing_veh_id	turning_indicator	veh_id	veh_type	
//79399	Mri_TrafficVeh	0			0			0				0				0											4.74931			1.57081			1.56929			- 51.6797	2717.24		0.219692	0		0				0					1		0		
//79401	Mri_TrafficVeh	0			0			0				0				0											4.70041			1.57081			1.56929			- 51.5797	2682.95		0.219691	0		0				0					2		0
