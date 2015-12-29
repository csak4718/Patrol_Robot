#ifndef _MOTORPORTCONTROL_HPP
#define _MOTORPORTCONTROL_HPP
#include "stdafx.h"
#include <cstring>
#include <string>
#include <iostream>
#include <windows.h>

using namespace std;

/**
 * MotorPortControl:
 *   In the case of RenBot, the two wheel 
 *   motors are controlled by only one serial port 
 */

class MotorPortControl
{
public:
	MotorPortControl(string port_name, int baud_rate)
	{
		openPort(port_name, baud_rate);
	}
	int readLeftEncoder()
	{
		writePort(string("1pos\n"));
		//cout << "Wait: readLeftEncoder " << endl;
		Sleep(5);
		return atoi(readPort().c_str());
	}
	int readRightEncoder()
	{
		writePort(string("2pos\n"));
		//cout << "Wait: readRightEncoder " << endl;
		Sleep(5);
		return atoi(readPort().c_str());
	}

public:
	bool openPort(string port_name, int baud_rate);
	bool writePort(string str);
	string readPort();
protected:
	HANDLE  _hComm;
};

#endif