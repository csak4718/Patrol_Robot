#ifndef _LOCALIZER_HPP
#define _LOCALIZER_HPP
#include "stdafx.h"
#include <string>
#include "UrgControl.hpp"
#include <vector>
class Localizer
{
public:
	Localizer(UrgControl* urgC);
	void loadGridMap(string file_name = string("GridMap.txt"),int _width=600,int _height=600);
	int findPose(double x, double y, double th_degree, int search_range, int measurement_limit);
	int findPose2(double gived_x, double gived_y, double gived_th_degree, int search_range, int measurement_limit);
	int findPose2(double gived_x, double gived_y, double gived_th_degree, int search_range,int search_th_range, int measurement_limit);
	void mapping(double gived_x, double gived_y, double gived_th_degree);
	//bool findPose(); // global
//protected:
	UrgControl* _urgC;
	std::vector< std::vector<int> > _map_arr;
	//int _map_arr[WIDTH][HEIGHT];
	double _pose_x, _pose_y, _pose_th;
	int WIDTH;
	int HEIGHT;
};

#endif