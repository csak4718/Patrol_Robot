#ifndef _POINT2D_HPP
#define _POINT2D_HPP
#include "stdafx.h"
#include <vector>

using namespace std;

class Point2D
{
public:
	Point2D(double a = 0, double b = 0, bool isPolar = false);
	
	static double p2pDist(Point2D p1, Point2D p2);
	static double p2pAngle(Point2D start, Point2D end);
	static void findLineParam(Point2D p1, Point2D p2, double* aa, double* bb, double* cc);
	static Point2D Local2Global(double pos_x, double pos_y, double pos_deg, Point2D pLocal);
	static Point2D Global2Local(double pos_x, double pos_y, double pos_deg, Point2D pGlobal);
//protected:
	double _x, _y;
	double _r, _th;
	double _degree;
};

class Point2DSet
{
public:
	void addPoint(Point2D* p);
	vector<Point2DSet*> getCurveSet(double max_p2p_dist, int min_cont_point);
	//vector<Point2DSet*> getHoughLineSet();
	vector<Point2DSet*> getIepfLineSet();
//protected:
	vector<Point2D*> _vP;
};

typedef vector<Point2DSet*> LineSet;
typedef vector<Point2DSet*> CurveSet;

double p2pDist(Point2D p, Point2D q);
double p2pAngle(Point2D start, Point2D end);
double deg2rad(double degree);
double rad2deg(double rad);

void findLineParam(Point2D p1, Point2D p2, double* aa, double* bb, double* cc);
Point2D Local2Global(double pos_x, double pos_y, double pos_deg, Point2D pLocal);
Point2D Global2Local(double pos_x, double pos_y, double pos_deg, Point2D pGlobal);
#endif