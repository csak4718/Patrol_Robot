#ifndef _ROBOT_HPP
#define _ROBOT_HPP
#include "stdafx.h"
#include "Platform.hpp"

class Robot 
{
public:
	Robot(string motor_port_name, int baud_rate);
	void resetPose();
	void setPose(double x_cm, double y_cm, double th_degree);
	void setSpeed(int speed);
	void moveForward(double dist_cm);
	void moveForward(void);
	void moveBack(void);
	void turnLeft(double angle_degree);
	void turnLeft(void);
	void turnRight(void);
	void biasForward(int left_motor_vel, int right_motor_vel);
	void biasBackward(int left_motor_vel, int right_motor_vel);
	void stop(void);
	void coutRobotPose(); // for debug
	void getpose(double *x, double *y, double *th){
		*x = _x ;
		*y = _y ;
		*th = _th ;
	}
		//new fx
// protected:
	double _x, _y;
	double _th; // in degree
	Platform* _platform;
};
double TurnLeftAngle(double target_angle,double robot_angle);
#endif