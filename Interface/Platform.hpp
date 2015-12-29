#ifndef _PLATFORM_HPP
#define _PLATFORM_HPP
#include "stdafx.h"
#include "MotorPortControl.hpp"

class Platform
{
public:
	Platform(string motor_port_name, int baud_rate);
	void moveForward(double dist_cm);
	void moveForward(void);
	void moveBack(void);
	void turnLeft(double angle_degree);
	void turnLeft(void);
	void turnRight(void);
	void biasForward(int left_motor_vel, int right_motor_vel); // differencial velocity control
	void biasBackward(int left_motor_vel, int right_motor_vel); // differencial velocity control
	void stop(void);
	void watchMotion();
	static void thrWatchMotion(void*);
	void setSpeed(int speed);
	void resetMotorEncoder();
	bool isIdle();
// protected:
	bool _isIdle;
	bool _watching;
	int _speed;
	MotorPortControl* _motorC;
};

#endif