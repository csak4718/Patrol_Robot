
#include "Robot.hpp"
#include "Localizer.hpp"
#include "FindBall.h"
#include <process.h>
#include <conio.h>
Robot* robot;

int main(){
	robot=new Robot("COM1",9600);
	Localizer* localizer = new Localizer(new UrgControl(string("COM2"), 115200));
	localizer->loadGridMap(string("GridMap.txt"),600,600);
	FindBall findball(robot,localizer,Singleton::getInstance("parameters.txt")->parameters["StopDist"]);//total pixel threshold	
	robot->stop();
	const int rotate_speed=1000;
	const int move_speed=5000;
	char input_c = ' ';
	bool prog_exit = false;
	
	do{
		input_c = _getch();
		switch(input_c){
			case 'w':
				robot->stop();
				robot->setSpeed(move_speed);
				robot->moveForward();
				std::cout<<"w"<<std::endl;
				break;
			case 'a':
				robot->stop();
				robot->setSpeed(rotate_speed);
				robot->turnLeft();
				break;
			case 'd':
				robot->stop();
				robot->setSpeed(rotate_speed);
				robot->turnRight();
				break;
			case 's':
				robot->stop();
				robot->setSpeed(move_speed);
				robot->moveBack();
				break;
			case ' ':
				robot->stop();
				break;
			case 'q':
				robot->stop();
				prog_exit = true;
				Sleep(100);
				break;
			case 'g':
				findball.operation1();//turn right
				break;
			case 'h':
				findball.operation0();//turn left
				break;
			default:
				break;
		}
	}while(!prog_exit);
	
	return 0;
}