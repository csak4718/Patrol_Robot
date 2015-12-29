#ifndef GOTARGET_H
#define GOTARGET_H
#include "stdafx.h"
#include "Robot.hpp"
#include "Localizer.hpp"
#include "TargetQueue.hpp"
#include <math.h>
#include <sstream>
#include <fstream>
#include "LaserDetectDoor.h"
#include "Door.h"

#include<iostream>
#include <process.h>
#include <conio.h>
#include <cstdlib>


class GoTarget{
public:
	GoTarget(Robot* Robotptr,Localizer* Localizerptr);
	void Action_Go_Point(string file_name_str, double init_degree);
	void Action_Go_Point_Calibrate(string file_name_str, double init_degree);
	void Action_Go_Point_Calibrate_Avoid(string file_name_str, double init_degree);
	void Action_Go_Point_Calibrate_JudgeDoor(string file_name_str, double init_degree);
	void Action_Go_Point_avoid(string file_name_str, double init_degree);
	void GoTarget::Push_Buttton(int n_points);
	void Action_Go_Point_Calibrate_aavoid(double a, double b);

	TargetQueue* tq;
	Robot* robot;
	Localizer* localizer;
	
private:
	
};

#endif
