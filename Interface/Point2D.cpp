#include "stdafx.h"
#include "Point2D.hpp"
#include <cmath>
#include <iostream>
#include <vector>

#define PI 3.1415926


using namespace std;

Point2D::Point2D(double a, double b, bool isPolar)
{
	if(!isPolar){
		_x = a;
		_y = b;
		_r = sqrt(pow(_x,2)+pow(_y,2));
		_th = atan2(_y,_x);
	}else{ // is Polar Coordinate
		_r = a;
		_th = b;
		_x = _r*cos(_th);
		_y = _r*sin(_th);
	}
	_degree = _th*180/PI;
}

double Point2D::p2pDist(Point2D p1, Point2D p2)
{
	return sqrt(pow(fabs(p2._x-p1._x),2)+pow(fabs(p2._y-p1._y),2));
}

void Point2DSet::addPoint(Point2D* p)
{
	_vP.push_back(p);
}
void Point2D::findLineParam(Point2D p1, Point2D p2, double *aa, double *bb, double *cc)
{
	double a, b, c;	// ax+by+c=0;
	a = p1._y - p2._y;
	b = p2._x - p1._x;
	c = (p1._x * p2._y)-(p2._x * p1._y);
	
	*aa = a;
	*bb = b;
	*cc = c;
}


Point2D Point2D::Global2Local(double pos_x, double pos_y, double pos_deg, Point2D pGlobal)
{
	double x, x_p, x_pp, y, y_p, y_pp;
	x = pGlobal._x;
	y = pGlobal._y;
	x_p = x - pos_x;
	y_p = y - pos_y;
	double th = pos_deg*3.1415926/180;
	x_pp = x_p * sin(th) - y_p * cos(th);
	y_pp = x_p * cos(th) + y_p * sin(th);
	
	return Point2D(x_pp, y_pp);
}

Point2D Point2D::Local2Global(double pos_x, double pos_y, double pos_deg, Point2D pLocal)
{
	double x, x_p, x_pp, y, y_p, y_pp;
	x_pp = pLocal._x;
	y_pp = pLocal._y;
	double th = pos_deg*3.1415926/180;
	x_p = x_pp * sin(th) + y_pp * cos(th); 
	y_p = (-1)*x_pp * cos(th) + y_pp * sin(th);
	x = x_p + pos_x;
	y = y_p + pos_y;

	return Point2D(x, y);
}
CurveSet Point2DSet::getCurveSet(double max_p2p_dist, int min_cont_point)
{
	CurveSet Cset;
	Cset.push_back(new Point2DSet());
	for(int cur=0; cur<(int)_vP.size()-1; cur++){
		Cset[Cset.size()-1]->addPoint(_vP[cur]);
		if(Point2D::p2pDist(*_vP[cur],*_vP[cur+1]) > max_p2p_dist){
			Cset.push_back(new Point2DSet());
		}
	}

	// eliminate too few set
	for(int i=0; i<(int)Cset.size(); i++){
		if((int)Cset[i]->_vP.size() < min_cont_point){
			Cset.erase(Cset.begin()+i);
			i--;
		}
	}

	return Cset;
}
LineSet Point2DSet::getIepfLineSet()
{
	vector<Point2DSet*> vPSet;
	vPSet.push_back(new Point2DSet(*this));
	double a, b, c; // ax+by+c=0
	double max_dist;
	double dist;
	int max_dist_index;
	
	for(int i=0; i<(int)vPSet.size(); i++){
		a = vPSet[i]->_vP[0]->_y - vPSet[i]->_vP[vPSet[i]->_vP.size()-1]->_y;
		b = vPSet[i]->_vP[vPSet[i]->_vP.size()-1]->_x - vPSet[i]->_vP[0]->_x;
		c = ((vPSet[i]->_vP[0]->_x)*(vPSet[i]->_vP[vPSet[i]->_vP.size()-1]->_y))
			-((vPSet[i]->_vP[vPSet[i]->_vP.size()-1]->_x)*(vPSet[i]->_vP[0]->_y));
		// find max_dist_index;
		max_dist_index = 0;
		max_dist = 0;
		for(int j=0; j<(int)vPSet[i]->_vP.size(); j++){
			dist = fabs((a*vPSet[i]->_vP[j]->_x + b*vPSet[i]->_vP[j]->_y + c)/(sqrt(pow(a,2)+pow(b,2))));
			if(fabs((a*vPSet[i]->_vP[j]->_x + b*vPSet[i]->_vP[j]->_y + c)/(sqrt(pow(a,2)+pow(b,2)))) > max_dist){
				max_dist_index = j;
				max_dist = dist;
			}
		}
		if(max_dist > 6){ // seperate into 2 point set
			Point2DSet* subSet1 = new Point2DSet();
			Point2DSet* subSet2 = new Point2DSet();
			for(int j=0; j<max_dist_index; j++){
				subSet1->addPoint(vPSet[i]->_vP[j]);
			}
			for(int j=max_dist_index+1; j<(int)vPSet[i]->_vP.size(); j++){
				subSet2->addPoint(vPSet[i]->_vP[j]);
			}
			vPSet.push_back(subSet1);
			vPSet.push_back(subSet2);
			vPSet.erase(vPSet.begin()+i);
			i--;
		}// end if
	}// end for
	for(int i=0; i<(int)vPSet.size(); i++){
		if(vPSet[i]->_vP.size() < 15){
			vPSet.erase(vPSet.begin()+i);
			i--;
		}
	}
	return vPSet;
}

double p2pDist(Point2D p, Point2D q)
{
	return sqrt(pow((p._x - q._x), 2) + pow((p._y - q._y), 2));
}

double p2pAngle(Point2D start, Point2D end)
{
	return rad2deg(atan2(end._y - start._y , end._x - start._x));
}

double rad2deg(double rad)
{
	return rad*180/PI;
}

double deg2rad(double deg)
{
	return deg*PI/180;
}


void findLineParam(Point2D p1, Point2D p2, double* a, double* b, double* c)
{
	Point2D::findLineParam(p1, p2, a, b, c);
}

Point2D Global2Local(double pos_x, double pos_y, double pos_deg, Point2D pGlobal)
{
	return Point2D::Global2Local(pos_x, pos_y, pos_deg, pGlobal);
}
Point2D Local2Global(double pos_x, double pos_y, double pos_deg, Point2D pLocal)
{
	return Point2D::Local2Global(pos_x, pos_y, pos_deg, pLocal);
}