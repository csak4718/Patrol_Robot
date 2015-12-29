#ifndef GRABBALL_H
#define GRABBALL_H
#include "stdafx.h"
#include <cstring>
#include <string>
#include <iostream>
#include <windows.h>
#include <ctime>

using namespace std;
class GrabBall{

public:
	GrabBall(string port_name, int baud_rate);
	void Grab();
	bool IsEnd();
	void LiftUP();
	void PutDown(double lowerbound);
	void smallmove();
	void stop();
	~GrabBall(){
			CloseHandle(_hComm);
	}

//private:
	bool writePort(string str);
	string readPort();
	
private:
	bool openPort(string port_name, int baud_rate);
    HANDLE  _hComm;
	clock_t t1,t2;
};


#endif