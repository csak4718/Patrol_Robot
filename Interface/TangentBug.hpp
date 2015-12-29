#ifndef TANGENTBUG_HPP
#define TANGENTBUG_HPP
#include "stdafx.h"
#include "Point2D.hpp"
#include "Robot.hpp"
#include "Localizer.hpp"
#include "GoTarget.h"
typedef vector<Point2D> PointSet; 
class TangentBug
{
public:
	TangentBug();
	TangentBug(const int robot_radius);
	void getRangeData(int* range769);
	void eliminateNoise();
	void getEnvPoints();
	void segmentRangeData();
	void setLocalGoal(Point2D pGoal_L);
	void setRobotRadius(double R);
	Point2D getLocalTarget(Point2D pGoal_L, int* range769);

// protected:
	double _range[682];
	PointSet _envPoins;
	vector<PointSet> _segments;
	vector<PointSet> _needed_segments;

	Point2D _pGoal;
	double _R;			// robot radius;
	Robot* _robot2;
	Localizer* _localizer2;
	
	//bool tem;

};

#endif