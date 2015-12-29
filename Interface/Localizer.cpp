#include "stdafx.h"
#include "Localizer.hpp"
#include <fstream>
#include <iostream>
#include <cmath>
#include <conio.h>
using namespace std;

Localizer::Localizer(UrgControl* urgC): _urgC(urgC){}
void Localizer::loadGridMap(string file_name,int _width,int _height)
{
	WIDTH=_width;
	HEIGHT=_height;
	_map_arr=std::vector< std::vector<int> >(WIDTH,std::vector<int>(HEIGHT,0));
	
	ifstream iFile(file_name.c_str(), ios::in);
	for(int y=HEIGHT-1; y>=0; y--){
		for(int x=0; x<WIDTH; x++){
			iFile >> _map_arr[x][y];//和原grid map.txt up bottom transform coordinate inversely
			//cout << _map_arr[x][y];
		}
		//cout << endl;
	}
	
	
}

int Localizer::findPose(double gived_x, double gived_y, double gived_th_degree, int search_range, int measurement_limit)
{
	int global_x, global_y;
	int match_points_num;
	int current_match_max = 0;
	_urgC->getScan682();
	Point2DSet pSet = _urgC->getValidPoints(20);//delete noise p2p which depart about 20cm,save valid points into point2Dset class
	while((int)pSet._vP.size() > measurement_limit){
		for(int i=0; i<(int)pSet._vP.size(); i+=2){ // eliminate to 2/3 elements
			pSet._vP.erase(pSet._vP.begin()+i);
		}
	}
	// set initial search grid
	int start_x = (gived_x - search_range/2 > 0)? (int)gived_x - search_range/2 : 0;
	int end_x = (gived_x + search_range/2 < WIDTH-1)? (int)gived_x + search_range/2 : WIDTH-1;
	int start_y = (gived_y - search_range/2 > 0)? (int)gived_y - search_range/2 : 0;
	int end_y = (gived_y + search_range/2 < HEIGHT-1)? (int)gived_y + search_range/2 : HEIGHT-1;
	
	for(int x = start_x; x<=end_x; x=x+1){//x為given_x的相對關係
		for(int y = start_y; y<=end_y; y=y+1){
			if(_map_arr[x][y] == 0){
				for(int theta = (int)gived_th_degree - 30; theta < gived_th_degree + 30; theta++){
					match_points_num = 0;
					for(int i=0; i<(int)pSet._vP.size(); i++){
						global_x = x + (int)(pSet._vP[i]->_r*cos(deg2rad(theta) + pSet._vP[i]->_th - deg2rad(90)));
						global_y = y + (int)(pSet._vP[i]->_r*sin(deg2rad(theta) + pSet._vP[i]->_th - deg2rad(90)));//substrate 90 degree because intial point is 90 degree
						if(	((global_x < WIDTH-1)&&(global_x >0)
							&&(global_y < HEIGHT-1)&&(global_y > 0))
							&&((_map_arr[global_x-1][global_y-1]==1)
							||(_map_arr[global_x-1][global_y]==1)
							||(_map_arr[global_x-1][global_y+1]==1)
							||(_map_arr[global_x][global_y-1]==1)
							||(_map_arr[global_x][global_y]==1)
							||(_map_arr[global_x][global_y+1]==1)
							||(_map_arr[global_x+1][global_y-1]==1)
							||(_map_arr[global_x+1][global_y]==1)
							||(_map_arr[global_x+1][global_y+1]==1)))
						{
							match_points_num++;	
						}
					} // end for
					if(match_points_num > current_match_max){
						current_match_max = match_points_num;
						_pose_x = x;
						_pose_y = y;
						_pose_th = theta;
					}
				}
			}else{ // not possible location
				;// do nothing
			}
		}
	}

	return current_match_max;
}
int Localizer::findPose2(double gived_x, double gived_y, double gived_th_degree, int search_range, int measurement_limit)
{
	int global_x, global_y;
	int match_points_num;
	int current_match_max = 0;
	_urgC->getScan682();
	Point2DSet pSet = _urgC->getValidPoints(10);//delete noise p2p which depart about 20cm,save valid points into point2Dset class
	while((int)pSet._vP.size() > measurement_limit){
		for(int i=0; i<(int)pSet._vP.size(); i+=2){ // eliminate to 2/3 elements
			pSet._vP.erase(pSet._vP.begin()+i);
		}
	}
	// set initial search grid
	int start_x = (gived_x - search_range/2 > 0)? (int)gived_x - search_range/2 : 0;
	int end_x = (gived_x + search_range/2 < WIDTH-1)? (int)gived_x + search_range/2 : WIDTH-1;
	int start_y = (gived_y - search_range/2 > 0)? (int)gived_y - search_range/2 : 0;
	int end_y = (gived_y + search_range/2 < HEIGHT-1)? (int)gived_y + search_range/2 : HEIGHT-1;
	
	for(int x = start_x; x<=end_x; x=x+2){//x為given_x的相對關係
		for(int y = start_y; y<=end_y; y=y+2){
			if(_map_arr[x][y] == 0){
				for(int theta = (int)gived_th_degree - 30; theta < gived_th_degree + 30; theta++){
					match_points_num = 0;
					for(int i=0; i<(int)pSet._vP.size(); i++){
						global_x = x + (int)(pSet._vP[i]->_r*cos(deg2rad(theta) + pSet._vP[i]->_th - deg2rad(90)));
						global_y = y + (int)(pSet._vP[i]->_r*sin(deg2rad(theta) + pSet._vP[i]->_th - deg2rad(90)));//substrate 90 degree because intial point is 90 degree
						//if(	((global_x < WIDTH-1)&&(global_x >0)
						//	&&(global_y < HEIGHT-1)&&(global_y > 0))
						//	&&((_map_arr[global_x-1][global_y-1]==1)
						//	||(_map_arr[global_x-1][global_y]==1)
						//	||(_map_arr[global_x-1][global_y+1]==1)
						//	||(_map_arr[global_x][global_y-1]==1)
						//	||(_map_arr[global_x][global_y]==1)
						//	||(_map_arr[global_x][global_y+1]==1)
						//	||(_map_arr[global_x+1][global_y-1]==1)
						//	||(_map_arr[global_x+1][global_y]==1)
						//	||(_map_arr[global_x+1][global_y+1]==1)))
						//{
						//	match_points_num++;	
						//}
						if(	((global_x < WIDTH-1)&&(global_x >0)
							&&(global_y < HEIGHT-1)&&(global_y > 0)))
						match_points_num+=_map_arr[global_x][global_y];
					} // end for
					if(match_points_num > current_match_max){
						current_match_max = match_points_num;
						_pose_x = x;
						_pose_y = y;
						_pose_th = theta;
					}
				}
			}else{ // not possible location
				;// do nothing
			}
		}
	}

	return current_match_max;
}
int Localizer::findPose2(double gived_x, double gived_y, double gived_th_degree, int search_range,int search_th_range, int measurement_limit)
{                                                                                   // 40, 30, 100
	int global_x, global_y;
	int match_points_num;
	int current_match_max = 0;
	_urgC->getScan682();
	Point2DSet pSet = _urgC->getValidPoints(10);//delete noise p2p which depart about 20cm,save valid points into point2Dset class
	while((int)pSet._vP.size() > measurement_limit){
		for(int i=0; i<(int)pSet._vP.size(); i+=2){ // eliminate to 2/3 elements
			pSet._vP.erase(pSet._vP.begin()+i);
		}
	}
	// set initial search grid
	int start_x = (gived_x - search_range/2 > 0)? (int)gived_x - search_range/2 : 0;
	int end_x = (gived_x + search_range/2 < WIDTH-1)? (int)gived_x + search_range/2 : WIDTH-1;
	int start_y = (gived_y - search_range/2 > 0)? (int)gived_y - search_range/2 : 0;
	int end_y = (gived_y + search_range/2 < HEIGHT-1)? (int)gived_y + search_range/2 : HEIGHT-1;
	
	for(int x = start_x; x<=end_x; x=x+2){//x為given_x的相對關係
		for(int y = start_y; y<=end_y; y=y+2){
			if(_map_arr[x][y] == 0){
				for(int theta = (int)gived_th_degree -search_th_range/2 ; theta < gived_th_degree + search_th_range/2; theta++){
					match_points_num = 0;
					for(int i=0; i<(int)pSet._vP.size(); i++){
						global_x = x + (int)(pSet._vP[i]->_r*cos(deg2rad(theta) + pSet._vP[i]->_th - deg2rad(90)));
						global_y = y + (int)(pSet._vP[i]->_r*sin(deg2rad(theta) + pSet._vP[i]->_th - deg2rad(90)));//substrate 90 degree because intial point is 90 degree
						/*if(	((global_x < WIDTH-1)&&(global_x >0)
							&&(global_y < HEIGHT-1)&&(global_y > 0))
							&&((_map_arr[global_x-1][global_y-1]==1)
							||(_map_arr[global_x-1][global_y]==1)
							||(_map_arr[global_x-1][global_y+1]==1)
							||(_map_arr[global_x][global_y-1]==1)
							||(_map_arr[global_x][global_y]==1)
							||(_map_arr[global_x][global_y+1]==1)
							||(_map_arr[global_x+1][global_y-1]==1)
							||(_map_arr[global_x+1][global_y]==1)
							||(_map_arr[global_x+1][global_y+1]==1)))
						{
							match_points_num++;	
						}*/
						if(	((global_x < WIDTH-1)&&(global_x >0)
							&&(global_y < HEIGHT-1)&&(global_y > 0)))
						match_points_num+=_map_arr[global_x][global_y];
					} // end for
					if(match_points_num > current_match_max){
						current_match_max = match_points_num;
						_pose_x = x;
						_pose_y = y;
						_pose_th = theta;
					}
				}
			}else{ // not possible location
				;// do nothing
			}
		}
	}
	_cprintf("max hit points = %d\n",current_match_max);
	return current_match_max;
}
void Localizer::mapping(double gived_x, double gived_y, double gived_th_degree)
{
	int global_x, global_y;
	int tmp_x,tmp_y;
	_urgC->getScan682();
	Point2DSet pSet = _urgC->getValidPoints(10);//delete noise p2p which depart about 10cm,save valid points into point2Dset class
	for(int i=0; i<(int)pSet._vP.size(); i++)
	{
		global_x = gived_x + (int)(pSet._vP[i]->_r*cos(deg2rad(gived_th_degree) + pSet._vP[i]->_th - deg2rad(90)));
		global_y = gived_y + (int)(pSet._vP[i]->_r*sin(deg2rad(gived_th_degree) + pSet._vP[i]->_th - deg2rad(90)));//substrate 90 degree because intial point is 90 degree ??
		if(global_x>0&&global_x<600&&global_y>0&&global_y<600)
			_map_arr[global_x][global_y]+=3;					
		for(int j=5;j<pSet._vP[i]->_r-5;j++)
		{
			tmp_x=gived_x + (int)((pSet._vP[i]->_r-j)*cos(deg2rad(gived_th_degree) + pSet._vP[i]->_th - deg2rad(90)));
			tmp_y=gived_y + (int)((pSet._vP[i]->_r-j)*sin(deg2rad(gived_th_degree) + pSet._vP[i]->_th - deg2rad(90)));
			if(tmp_x>0&&tmp_x<600&&tmp_y>0&&tmp_y<600)
				_map_arr[tmp_x][tmp_y]-=3;
		}
		
	}
}