#ifndef JUDGE_H
#define JUDGE_H
#pragma comment(lib,"cxcore210.lib")

#pragma comment(lib,"ml210.lib")

#pragma comment(lib,"cv210.lib")

#pragma comment(lib,"cvaux210.lib")

#pragma comment(lib,"highgui210.lib")
#include "UrgControl.hpp"
#include <cv.h>
#include <highgui.h>
#include <ctime>
#include "stdafx.h"
class Judge{
public:
	Judge(UrgControl* urg,int scan_range);
	bool HaveDoor();

	bool CanGo();
	void ShowImg();
	
	double _degree;
	double _theta;
	double _distance;
private:
	int laser_range;
	UrgControl* _urg;
	int start_n,end_n;
};

#endif