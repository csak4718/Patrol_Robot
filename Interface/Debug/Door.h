#ifndef DOOR_H
#define DOOR_H
#include "stdafx.h"
#include "DetectTarget.h"
class Door{
public:
	Door();
	bool CanGo();
	void calibration();
private:
	
	void Erosion(IplImage*,int r1,int c1);
	void Dilation(IplImage*,int r1,int c1);
	void ColorFilterYCrCb(IplImage *, IplImage* );
	int width;
	int height;
	int Total;
	CvCapture* capture;
	
	int Totalthreshold;
};

#endif