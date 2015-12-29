#ifndef FINDBALL_H
#define FINDBALL_H
#include "stdafx.h"
#include "DetectTarget.h"
#include "Robot.hpp"
#include "Localizer.hpp"
#include <process.h>
#include <conio.h>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include "GrabBall.h"
#include "TargetQueue.hpp"
#include "GoTarget.h"

#include "Point2D.hpp"
typedef vector<Point2D> PointSet; 

class FindBall{
public:
	FindBall(Robot* robot,Localizer* localizer,double stop_dist):_robot(robot),_localizer(localizer),StopDist(stop_dist){}
	void test(); //debug find 4
	void calibration();//calibration area
	void calibration_Fullcolor();
	void operation0();  
	void operation1();  
	void operation2();//not yet
	void operation3();//not yet
	double StopDist;
protected:
	Robot* _robot;
	Localizer* _localizer;
	
}; 

	
#endif