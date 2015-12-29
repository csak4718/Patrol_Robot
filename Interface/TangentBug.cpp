#include "stdafx.h"
#include "TangentBug.hpp"
#include <cmath>
#include "conio.h"
#include "GoTarget.h"

TangentBug::TangentBug()
{
	setRobotRadius(25);
}
TangentBug::TangentBug(const int robot_radius){
	setRobotRadius(robot_radius);
}
void TangentBug::getRangeData(int *range769)
{
	for(size_t i=0; i<682; i++){
		if(range769[i+44] > 179 && range769[i+44] < 4000){
			_range[i] = 0.1*range769[i+44]; // in cm
		}else{
			_range[i] = 0;
		}
	}
}

void TangentBug::eliminateNoise()
{
	const double threshold = 20;
	for(size_t i=0; i<682; i++){
		if(i!=0 && i!=682-1){
			if((fabs(_range[i] - _range[i-1]) > threshold)&&
				(fabs(_range[i] - _range[i+1]) > threshold)){
					_range[i] = _range[i-1];
			}
		}
	}
	// check head and tail
	if(fabs(_range[0] - _range[1]) > threshold){
		_range[0] = _range[1];
	}
	if(fabs(_range[681] - _range[680]) > threshold){
		_range[681] = _range[680];
	}
}

void TangentBug::getEnvPoints()
{
	_envPoins.clear();
	for(size_t i=0; i<682; i++){
		if(_range[i] > 0.5 && _range[i] < _pGoal._r){
			_envPoins.push_back(Point2D(_range[i], (-30+i*0.3515625)*3.1415926/180, true));
		}
	}
}

void TangentBug::segmentRangeData()
{
	_segments.clear();
	const double threshold = 20;
	size_t cur_index = 0;
	size_t start_index = 0;
	for(; cur_index<_envPoins.size(); cur_index++){
		if(cur_index == 0){
			continue;
		}
		if(p2pDist(_envPoins[cur_index], _envPoins[cur_index-1]) > threshold){
			// new segment appear
			PointSet new_Seg;
			for(size_t i=start_index; i<cur_index; i++){
				new_Seg.push_back(_envPoins[i]);
			}
			_segments.push_back(new_Seg);
			start_index = cur_index;
		}
		if(cur_index == _envPoins.size()-1){
			PointSet new_Seg;
			for(size_t i=start_index; i<=cur_index; i++){
				new_Seg.push_back(_envPoins[i]);
			}
			_segments.push_back(new_Seg);
		}
	}
	for(size_t i=0; i<_segments.size(); i++){
		if(_segments[i].size() < 10){
			// too few points
			_segments.erase(_segments.begin()+i);
			i--;
		}
	}
}

void TangentBug::setLocalGoal(Point2D pGoal_L)
{
	_pGoal = pGoal_L;
}
void TangentBug::setRobotRadius(double R)
{
	_R = R;
}


Point2D TangentBug::getLocalTarget(Point2D pGoal_L, int* range769)
{
	AllocConsole();
	// initialization
	setLocalGoal(pGoal_L);
	getRangeData(range769);
	eliminateNoise();
	getEnvPoints();
	segmentRangeData();
	//GoTarget gotarget(_robot2, _localizer2);

	// keep needed segment
	// rectangle:
	// ax+by+d=0, ax+by+e=0, bx-ay+f=0, bx-ay+g=0;
	// ax+by+c=0 is the line pass through robot and target
	Point2D pOrig = Point2D(0,0);
	double a,b,c,d,e,f,g;
	const double l = _R; // robot radius
	findLineParam(_pGoal, pOrig, &a, &b, &c);
	d = c + l*sqrt(a*a + b*b);
	e = c - l*sqrt(a*a + b*b);
	f = a*pOrig._y - b*pOrig._x;
	g = a*_pGoal._y - b*_pGoal._x;

	_needed_segments.clear();
	Point2D tmpP;
	for(size_t i=0; i<_segments.size(); i++){
		for(size_t j=0; j<_segments[i].size(); j++){
			tmpP = _segments[i][j];
			// if one point is inside the rectangle, keep the segment
			if(	(a*tmpP._x + b*tmpP._y + d)*(a*tmpP._x + b*tmpP._y + e) < 0 &&
				(b*tmpP._x - a*tmpP._y + f)*(b*tmpP._x - a*tmpP._y + g) < 0){
					_needed_segments.push_back(_segments[i]);
					// skip the rest points of this segment
					break;
			}
		}
	}
	// if no segment between robot and target, return _pGoal
	if(_needed_segments.empty()){
		//tem= false;
		_cprintf("empty\n");
		//_cprintf("%d\n",tem);
		return _pGoal;
	}
	// there are obstacles between robot and target, find a temporary target
	Point2D pRightEnd = _needed_segments[0][0];
	Point2D pLeftEnd = _needed_segments[_needed_segments.size()-1][_needed_segments[_needed_segments.size()-1].size()-1];
	// choose the end point that has a miner delta angle
	Point2D pEnd = (fabs(pRightEnd._degree - _pGoal._degree)<fabs(pLeftEnd._degree - _pGoal._degree))? pRightEnd : pLeftEnd;
	Point2D pOtherEnd = (fabs(pRightEnd._degree - _pGoal._degree)<fabs(pLeftEnd._degree - _pGoal._degree))? pLeftEnd : pRightEnd;
	// find the line pass through robot and pEnd, ax+by+c=0
	findLineParam(pOrig, pEnd, &a, &b, &c);
	// find normal unit vector (uVa, uVb)
	double uVa = a / sqrt(a*a + b*b);
	double uVb = b / sqrt(a*a + b*b);
	Point2D pTmpTarget1, pTmpTarget2;
	pTmpTarget1 = Point2D(pEnd._x + 1.5*_R*uVa, pEnd._y + 1.5*_R*uVb);
	pTmpTarget2 = Point2D(pEnd._x + 1.5*_R*(-1)*uVa, pEnd._y + 1.5*_R*(-1)*uVb);
	// choose the TmpTarget that at the different side with the other end point
	if((a*pOtherEnd._x + b*pOtherEnd._y + c)*(a*pTmpTarget1._x + b*pTmpTarget1._y + c) < 0){
		//_cprintf("has obstacle\n");
		//_cprintf("gotarget.tem=");
		//_cprintf("%d\n", gotarget.tem);
		
		_cprintf("has obstacle\n");
		//system("pause");
		return pTmpTarget1;
	}else{
		//_cprintf("has obstacle\n");
		//_cprintf("gotarget.tem=");
		//_cprintf("%d\n", gotarget.tem);
		_cprintf("has obstacle\n");
		//system("pause");
		return pTmpTarget2;
	}
}

