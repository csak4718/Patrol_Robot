#ifndef DETECTTARGET_H
#define DETECTTARGET_H
#pragma comment(lib, "cxcore210.lib")
#pragma comment(lib, "cv210.lib")
#pragma comment(lib, "ml210.lib")
#pragma comment(lib, "cvaux210.lib")
#pragma comment(lib, "highgui210.lib")
#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <windows.h>
#include<cstdlib>
#include <process.h>
#include <time.h>


#include <map>
#include<fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <utility>
#include <algorithm>


class Singleton{
private:
	static Singleton* uniqueInstance;
	Singleton();
	std::string _filename;
	typedef std::vector<std::string>::size_type line_no;
public:
	static Singleton* getInstance(std::string filename);
	static void modify(std::string parameter,int value);

	void write_file(std::string filename,std::map<std::string,int> map);
	
	std::map<std::string,int> parameters;
	
	void build_map();
	std::vector<std::string> lines_of_text;
	void store_file(std::string filename);
	
};

class Interpolation{
private:
	static Interpolation* unique;
	Interpolation();
	enum word_order{First_word=1,Second_word};
	std::vector<std::pair<long,double> > vec;
	std::pair<long,double> X;
    double Dist( std::vector< std::pair<long,double> > &vec ) ;
public:	
	static double GetInterpolation(long Area);
	static double GetInterpolation(int Area);
};


class DetectTarget
{

public:
	time_t _time;
	void activate();
	void deactivate();
	static void thrEntry(void* ptr);
	bool active;
	int AreaThreshold;
	DetectTarget();
	
	typedef enum Direction{IDLE,LEFT,CENTER,RIGHT,TOO_CLOSE}Direction;
    Direction _Direction;
	
	int ObjectMaxArea;
	void Detect(int version);
	void calibration(int version);
	int Area2Dist(int area);
	CvRect MaxRect;
	int ObjectCenter;
	int width;
	int height;
	int centerY;
	int centerX;
	int _TotalPixel;
	~DetectTarget(){}

private:
	IplImage* frame;
	IplImage* frame_copy;
	IplImage *binary;
	CvCapture* capture;
	
	void ColorFilterYCrCb(IplImage *, IplImage* );
	void Erosion(IplImage*,int r1,int c1);
	void Dilation(IplImage*,int r1,int c1);
	void ConnectComponent(IplImage*);
	bool isCLOSE(const IplImage*);
	CvMemStorage* storage1;
	
};

#endif