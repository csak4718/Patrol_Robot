#include "stdafx.h"
#include "TargetQueue.hpp"
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <sstream>
#include <cstring>
#include <vector>

using namespace std;

TargetQueue::TargetQueue(){}
void TargetQueue::pushBack(Point2D* p){ _dq.push_back(p); }
void TargetQueue::pushFront(Point2D* p){ _dq.push_front(p); }
void TargetQueue::popBack(){ _dq.pop_back(); }
void TargetQueue::popFront(){ _dq.pop_front(); }
bool TargetQueue::isEmpty(){ return _dq.empty(); }
Point2D TargetQueue::getTarget(unsigned i){ return *(_dq[i]); }
void TargetQueue::readFile(string file_name)// = string("target.txt"))
{

	ifstream iFile;
	iFile.clear();
	iFile.open(file_name.c_str(), ios::in);
	if(!iFile){
		cout << "TargetQueue: readFile: file open error!" << endl;
		system("pause");
		exit(1);
	}
	string line_str;
	cout << "TargetQueue Load File..." << endl;
	while(iFile && !iFile.eof()){
		getline(iFile, line_str);
		vector<string> vStr;
		char* next_token;
		char* pch = strtok_s(const_cast<char*> (line_str.c_str()), " ", &next_token);
		while(pch != NULL){
			vStr.push_back(string(pch));
			//cout << vStr[vStr.size()-1] << endl;
			pch = strtok_s(NULL, " ", &next_token);
		}
		if(vStr.size() != 0){
			assert(vStr.size() == 3);
			pushBack(new Point2D(atoi(vStr[1].c_str()),atoi(vStr[2].c_str())));
			cout	<< atoi(vStr[0].c_str()) // sequence
					<< " (" << atoi(vStr[1].c_str()) << ", " // x
					<< atoi(vStr[2].c_str()) << ")" << endl; // y
		}
	}
	iFile.clear();
	iFile.close();
	cout << endl;
}


void ReadObstacle(string filename,std::vector<std::pair<int,int>> &vec) {
	ifstream file;
	file.open(filename.c_str(),ios::in);
	if(!file){
		exit(1);
	}
	string line,word;

	while(getline(file,line))
	{
		 int x,y;
		 std::istringstream stream(line);//±N¦r¦ê¸j¨ì¦r¦ê¬y
		 bool first=true;
		 while(stream>>word)
	     {//do per-word processing	
			 if(first){
				 x=atoi(word.c_str());
				 first=false;
			 }
			 else{
				 y=atoi(word.c_str());
			 }
		 }
		 vec.push_back(make_pair(x,y));
	}
	
	
	file.clear();
	file.close();
}

