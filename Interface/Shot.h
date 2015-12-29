#ifndef SHOT_H
#define SHOT_H
#include "Resource.h"
#include "USBControl.h"
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "Robot.hpp"
#include "Localizer.hpp"
#include <iostream>
#include <string>
#include<windows.h> 
#pragma comment(lib,"winmm.lib") 
#include <mmsystem.h> 

static CvMemStorage* storage = 0;
static CvHaarClassifierCascade* cascade = 0;

class Shot{
public:
	Shot(Robot* Robotptr,Localizer* Localizerptr,USBControl* mp_USBControl_ptr);
	//Shot(Robot* robot):_robot(robot){};
	//~Shot();
	void detect_and_draw( IplImage* img );
	void take_a_shot();
	void showThief();
	static void playSound(void *p);
	void check(int value);
	void just_shot();
	bool isCaught;
	bool isCAUGHT();
	bool TooLong;
	bool checkLeft1;
	bool checkLeft2;
	bool checkRight1;
	bool checkRight2;
	Robot* robot;
	Localizer* localizer;
	USBControl* mp_USBControl;
	DWORD			dwRet;
private:
	CvCapture* captureThief;
	IplImage *frame;
	IplImage *frame_copy;
//protected:
//	Robot* _robot;

};
#endif