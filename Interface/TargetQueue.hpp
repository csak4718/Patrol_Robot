#ifndef _TARGETQUEUE_HPP
#define _TARGETQUEUE_HPP
#include "stdafx.h"
#include "Point2D.hpp"
#include <deque>

using namespace std;

class TargetQueue
{
public:
	TargetQueue();//{}
	void pushBack(Point2D* p);//{ _dq.push_back(p); }
	void pushFront(Point2D* p);//{ _dq.push_front(p); }
	void popBack();//{ _dq.pop_back(); }
	void popFront();//{ _dq.pop_front(); }
	bool isEmpty();//{ return _dq.empty(); }
	Point2D getTarget(unsigned i);//{ return *(_dq[i]); }
	void readFile(string filename = string("target.txt"));
//protected:
	deque<Point2D*> _dq;
};
void ReadObstacle(string filename,std::vector<std::pair<int,int>> &vec);
#endif
