#include "stdafx.h"
#include "Platform.hpp"
#include "process.h"

const float motorpara= 5590; //2110;//2750.5;

Platform::Platform(string motor_port_name, int baud_rate = 9600)
{
	_motorC = new MotorPortControl(motor_port_name, baud_rate);
	_isIdle = false;
	_motorC->writePort(string("1ANSW0\n"));
	_motorC->writePort(string("2ANSW0\n"));
	_motorC->writePort(string("ac200\n"));   //200,80
	_motorC->writePort(string("dec150\n"));  //150,80
	resetMotorEncoder();
	setSpeed(3000);
}

void Platform::moveForward(double dist_cm)
{
	int	LeftMotorPulseVal = (int)((-1)*dist_cm*motorpara);
	int	RightMotorPulseVal = (int)(dist_cm*motorpara);
	char LeftMotorCmdBuffer[4096];
	char RightMotorCmdBuffer[4096];
	int nLStr, nRStr;
	nLStr = sprintf_s(LeftMotorCmdBuffer, "%d", LeftMotorPulseVal);
	nRStr = sprintf_s(RightMotorCmdBuffer, "%d", RightMotorPulseVal); 
	string LeftMotorCmd = string("1lr");
	string RightMotorCmd = string("2lr");
	for(int i=0; i<nLStr; i++){
		LeftMotorCmd += LeftMotorCmdBuffer[i];
	}
	for(int i=0; i<nRStr; i++){
		RightMotorCmd += RightMotorCmdBuffer[i];
	}
	LeftMotorCmd += "\n";
	RightMotorCmd += "\n";
	resetMotorEncoder();
	//cout << "Wait: moveForward " << endl;
	Sleep(50);
	// send command
	_motorC->writePort(LeftMotorCmd);
	_motorC->writePort(RightMotorCmd);
	_isIdle = false;
	_motorC->writePort(string("m\n"));
	Sleep(50);
	watchMotion(); // enable encoder monitor
}


void Platform::moveForward(void)
{
	char LeftMotorCmdBuffer[4096];
	char RightMotorCmdBuffer[4096];
	int nLStr, nRStr;
	nLStr = sprintf_s(LeftMotorCmdBuffer, "%d", (-1)*_speed);
	nRStr = sprintf_s(RightMotorCmdBuffer, "%d", _speed);
	string LeftMotorCmd = string("1v");
	string RightMotorCmd = string("2v");
	for(int i=0; i<nLStr; i++){
		LeftMotorCmd += LeftMotorCmdBuffer[i];
	}
	for(int i=0; i<nRStr; i++){
		RightMotorCmd += RightMotorCmdBuffer[i];
	}
	LeftMotorCmd += "\n";
	RightMotorCmd += "\n";
	_isIdle = false;
	_motorC->writePort(LeftMotorCmd);
	_motorC->writePort(RightMotorCmd);
	watchMotion();
}
void Platform::moveBack(void)
{char LeftMotorCmdBuffer[4096];
	char RightMotorCmdBuffer[4096];
	int nLStr, nRStr;
	nLStr = sprintf_s(LeftMotorCmdBuffer, "%d", _speed);
	nRStr = sprintf_s(RightMotorCmdBuffer, "%d", (-1)*_speed);
	string LeftMotorCmd = string("1v");
	string RightMotorCmd = string("2v");
	for(int i=0; i<nLStr; i++){
		LeftMotorCmd += LeftMotorCmdBuffer[i];
	}
	for(int i=0; i<nRStr; i++){
		RightMotorCmd += RightMotorCmdBuffer[i];
	}
	LeftMotorCmd += "\n";
	RightMotorCmd += "\n";
	_isIdle = false;
	_motorC->writePort(LeftMotorCmd);
	_motorC->writePort(RightMotorCmd);
	watchMotion();
}
void Platform::turnLeft(double angle_degree)
{
	if(angle_degree > 180){
		while(angle_degree > 0){
			angle_degree -= 360;
		}
	}
	if(angle_degree < -180){
		while(angle_degree < 0){
			angle_degree += 360;
		}
	}
	const double r = 13; //16.0;//25.5;
	double theta = angle_degree*3.1415926/180;
	double length = r*theta;
	int PulseVale = (int)((1)*length*(motorpara+40));
	//int PulseVale = (int)((1)*length*(motorpara+115));
	
	char MotorCmdBuffer[4096];
	int nStr;
	nStr = sprintf_s(MotorCmdBuffer, "%d", PulseVale);
	string MotorCmd = string("lr");
	for(int i=0; i<nStr; i++){
		MotorCmd += MotorCmdBuffer[i];
	}
	MotorCmd += "\n";
	// set encoder to 0 first!
	resetMotorEncoder();
	//cout << "Wait: turnLeft " << endl;
	Sleep(50);
	// send command
	_motorC->writePort(MotorCmd);
	_isIdle = false;
	_motorC->writePort(string("m\n"));
	//cout << "Wait: turnLeft" << endl;
	Sleep(50);
	watchMotion(); // enable encoder monitor
}
void Platform::turnLeft(void)
{
	char MotorCmdBuffer[4096];
	int nStr;
	nStr = sprintf_s(MotorCmdBuffer, "%d", (1)*_speed);
	string MotorCmd = string("v");
	for(int i=0; i<nStr; i++){
		MotorCmd += MotorCmdBuffer[i];
	}
	MotorCmd += "\n";
	_isIdle = false;
	_motorC->writePort(MotorCmd);
	watchMotion();
}

void Platform::turnRight(void)
{
	char MotorCmdBuffer[4096];
	int nStr;
	nStr = sprintf_s(MotorCmdBuffer, "%d", (-1)*_speed);
	string MotorCmd = string("v");
	for(int i=0; i<nStr; i++){
		MotorCmd += MotorCmdBuffer[i];
	}
	MotorCmd += "\n";
	_isIdle = false;
	_motorC->writePort(MotorCmd);
	watchMotion();
}

void Platform::biasForward(int left_motor_vel, int right_motor_vel)
{
	int LeftMotorVel = abs(left_motor_vel);
	int RightMototVel = (-1)*abs(right_motor_vel);
	char LeftMotorCmdBuffer[4096];
	char RightMotorCmdBuffer[4096];
	int nLStr, nRStr;
	nLStr = sprintf_s(LeftMotorCmdBuffer, "%d", LeftMotorVel);
	nRStr = sprintf_s(RightMotorCmdBuffer, "%d", RightMototVel);
	string LeftMotorCmd = string("1v");
	string RightMotorCmd = string("2v");
	for(int i=0; i<nLStr; i++){
		LeftMotorCmd += LeftMotorCmdBuffer[i];
	}
	for(int i=0; i<nRStr; i++){
		RightMotorCmd += RightMotorCmdBuffer[i];
	}
	LeftMotorCmd += "\n";
	RightMotorCmd += "\n";
	resetMotorEncoder();
	_motorC->writePort(LeftMotorCmd);
	_motorC->writePort(RightMotorCmd);
	_isIdle = false;
	watchMotion(); // enable encoder monitor
}
void Platform::biasBackward(int left_motor_vel, int right_motor_vel)
{
	int LeftMotorVel = (-1)*abs(left_motor_vel);
	int RightMotorVel = abs(right_motor_vel);
	char LeftMotorCmdBuffer[4096];
	char RightMotorCmdBuffer[4096];
	int nLStr, nRStr;
	nLStr = sprintf_s(LeftMotorCmdBuffer, "%d", LeftMotorVel);
	nRStr = sprintf_s(RightMotorCmdBuffer, "%d", RightMotorVel);
	string LeftMotorCmd = string("1v");
	string RightMotorCmd = string("2v");
	for(int i=0; i<nLStr; i++){
		LeftMotorCmd += LeftMotorCmdBuffer[i];
	}
	for(int i=0; i<nRStr; i++){
		RightMotorCmd += RightMotorCmdBuffer[i];
	}
	LeftMotorCmd += "\n";
	RightMotorCmd += "\n";
	resetMotorEncoder();
	_motorC->writePort(LeftMotorCmd);
	_motorC->writePort(RightMotorCmd);
	_isIdle = false;
	watchMotion(); // enable encoder monitor
}
void Platform::stop()
{
	_motorC->writePort(string("v0\n"));
}
void Platform::watchMotion()
{
	_watching = true;
	_beginthread(Platform::thrWatchMotion, 0, this);
}

void Platform::thrWatchMotion(void* p_platform)
{
	Platform* platform = (Platform*)p_platform;
	int prev_left_encoder_value, prev_right_encoder_value, 
		next_left_encoder_value, next_right_encoder_value;
	prev_left_encoder_value = platform->_motorC->readLeftEncoder();
	prev_right_encoder_value = platform->_motorC->readRightEncoder();
	int stop_count = 0;
	
	while(platform->_watching){
		// 1. get encoder value
		// 2. compare prev_enc_val and next_enc_val
		// 3. find out whether platform is moving
		//cout << "Wait: thrWatchMotion " << endl;
		Sleep(50); // wait for motor rotating

		next_left_encoder_value = platform->_motorC->readLeftEncoder();
		next_right_encoder_value = platform->_motorC->readRightEncoder();
		
		if(	abs(next_left_encoder_value - prev_left_encoder_value) < 300 && 
			abs(next_right_encoder_value - prev_right_encoder_value) < 300){
			
			stop_count++;
		}else{
			stop_count = 0;
		}

		if(stop_count >= 2){
			platform->_isIdle = true;
			platform->_watching = false;
		}

		prev_left_encoder_value = next_left_encoder_value;
		prev_right_encoder_value = next_right_encoder_value;
	}
	_endthread();
}


void Platform::setSpeed(int speed)
{
	char speed_cstr[33];
	int nStr;
	nStr = sprintf_s(speed_cstr, "%d", speed);
	string MotorCommand = string("sp");
	for(int i=0; i<nStr; i++){
		MotorCommand += speed_cstr[i];
	}
	MotorCommand += "\n";
	_speed = speed;
	_motorC->writePort(MotorCommand);
}

void Platform::resetMotorEncoder()
{
	_motorC->writePort("ho\n");
	//cout << "Wait: resetMotorEncoder " << endl;
	Sleep(5);
}
bool Platform::isIdle()
{
	return _isIdle;
}