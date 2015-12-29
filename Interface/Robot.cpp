#include "stdafx.h"
#include "Robot.hpp"
#include "Point2D.hpp"
#include <cmath>
#include <iostream>

using namespace std;

Robot::Robot(string motor_port_name, int baud_rate = 9600)
{
	_platform = new Platform(motor_port_name, baud_rate);
	resetPose();
}
void Robot::resetPose()
{
	setPose(30, 270, 0);   //機器人初始位置設定
}

void Robot::setPose(double x_cm, double y_cm, double th_degree)
{
	_x = x_cm;
	_y = y_cm;
	_th = th_degree;
}

void Robot::setSpeed(int speed)
{
	_platform->setSpeed(speed);
}

void Robot::moveForward(double dist_cm)
{
	_platform->moveForward(dist_cm);
	while(!_platform->isIdle()){
		Sleep(10);
	}
	// update pose
	_x += dist_cm*cos(deg2rad(_th));
	_y += dist_cm*sin(deg2rad(_th));
}
void Robot::moveBack(void)
{
	_platform->moveBack();
}
void Robot::moveForward(void) 
{
	// this function do not change pose it self, additional care about position and speed is needed.
	_platform->moveForward();
}

void Robot::turnLeft(double angle_degree)
{
	
	_platform->turnLeft(angle_degree);
	while(!_platform->isIdle()){
		Sleep(10);
	}
	// update pose
	_th += angle_degree;
	while(_th > 360){
		_th -= 360;
	}
	while(_th < -360){
		_th += 360;
	}
	if(_th > 180){
		_th = _th - 360;
	}
	if(_th < -180){
		_th = _th + 360;
	}
}
double TurnLeftAngle(double target_angle,double robot_angle){
    
    double T=fmod(target_angle,360);
    double R=fmod(robot_angle,360);
    if( fabs(T-R) > 180){
        if(T-R>180)
        return (T-R)-360;
        else 
        return ((T-R)+360);
    }    
    return (T-R);    
}





void Robot::turnLeft(void)
{
	// this function do not change pose it self, additional care about position and speed is needed.
	_platform->turnLeft();
}

void Robot::turnRight(void)
{
	// this function do not change pose it self, additional care about position and speed is needed.
	_platform->turnRight();	
}
void Robot::biasForward(int left_motor_vel, int right_motor_vel)
{
	// this function do not change pose it self, additional care about position and speed is needed.
	_platform->biasForward(left_motor_vel, right_motor_vel);
}

void Robot::biasBackward(int left_motor_vel, int right_motor_vel)
{
	// this function do not change pose it self, additional care about position and speed is needed.
	_platform->biasBackward(left_motor_vel, right_motor_vel);
}

void Robot::stop()
{
	_platform->stop();
}
void Robot::coutRobotPose()
{
	cout << "Robot Pose: x y th = " << _x << " " << _y << " " << _th << endl; 
}