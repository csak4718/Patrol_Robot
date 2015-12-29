#include "stdafx.h"
#include "GoTarget.h"
#include "TangentBug.hpp"
#include "conio.h"

#include <iostream>
#include "math.h"
#define PI 3.14159265

GoTarget::GoTarget(Robot *Robotptr, Localizer *Localizerptr)
{
	localizer=Localizerptr;
	robot=Robotptr;
	tq = new TargetQueue();
}
void GoTarget::Action_Go_Point(string file_name_str, double init_degree)
{
	tq->readFile(file_name_str);
	const int rotate_speed=2000;
	const int start_move_speed=1000;
	const int move_speed=3700;
	robot->setPose(tq->getTarget(0)._x, tq->getTarget(0)._y, init_degree);
	tq->popFront();
	while(!tq->isEmpty()){
		localizer->findPose2(robot->_x, robot->_y, robot->_th, 30,25);
		_cprintf("Action_Go_Point\n");
		robot->setPose(localizer->_pose_x, localizer->_pose_y, localizer->_pose_th);
		Point2D robot_pose = Point2D(robot->_x, robot->_y);
		Point2D target = tq->getTarget(0);
		double r = p2pDist(robot_pose, target);
		double th = p2pAngle(robot_pose, target);
		robot->setSpeed(rotate_speed);
		robot->turnLeft(TurnLeftAngle(th,robot->_th));
		/*robot->setSpeed(start_move_speed);
		robot->moveForward(10);*/
		robot->setSpeed(move_speed);
		robot->moveForward(r);//-10);
		
		robot_pose = Point2D(robot->_x, robot->_y);
		if(p2pDist(robot_pose, target) < 5){
			tq->popFront();
		}
	} // end while
}
void GoTarget::Action_Go_Point_Calibrate(string file_name_str, double init_degree)
{
	tq->readFile(file_name_str);
	//in order to overcome the problem:robot get wrong pose
	const int rotate_speed=2000;
	const int start_move_speed=1000;
	const int move_speed=3000;
	while(true){
		localizer->findPose2(robot->_x,robot->_y,robot->_th, 50,25);
		robot->setPose(localizer->_pose_x,localizer->_pose_y,localizer->_pose_th);
		Point2D robot_pose = Point2D(robot->_x,robot->_y);
		Point2D target = Point2D(tq->getTarget(0)._x,tq->getTarget(0)._y);
		double r = p2pDist(robot_pose, target);
		double th = p2pAngle(robot_pose, target);
		robot->setSpeed(rotate_speed);
		robot->turnLeft(TurnLeftAngle(th,robot->_th));
		/*robot->setSpeed(start_move_speed);
		robot->moveForward(10);*/
		robot->setSpeed(move_speed);
		robot->moveForward(r);//-10);
		robot_pose = Point2D(robot->_x,robot->_y);
		if(p2pDist(robot_pose, target) < 5)
		 break;
	}
	//angle modify
	double angle_diff=TurnLeftAngle(init_degree,robot->_th);
	robot->stop();
	robot->setSpeed(rotate_speed);
	robot->turnLeft(angle_diff);
	//
	robot->setPose(tq->getTarget(0)._x, tq->getTarget(0)._y, init_degree);
	robot->setSpeed(move_speed);
	tq->popFront();
	while(!tq->isEmpty()){
		localizer->findPose2(robot->_x, robot->_y, robot->_th, 30,25);
		robot->setPose(localizer->_pose_x, localizer->_pose_y, localizer->_pose_th);
		system("cls");
		/*_cprintf("robot pose = (%lf,%lf)\n",robot->_x,robot->_y);
		_cprintf("th = %lf\n",robot->_th);*/
		cout << "robot pose = (" << robot->_x << "," << robot->_y << ")" << endl;
		cout << "th = " << robot->_th << endl;
		//system("pause");
		Point2D robot_pose = Point2D(robot->_x, robot->_y);
		Point2D target = tq->getTarget(0);
		double r = p2pDist(robot_pose, target);
		double th = p2pAngle(robot_pose, target);
		robot->setSpeed(rotate_speed);
		//robot->turnLeft(fmod(th - robot->_th, 360));
		robot->turnLeft(TurnLeftAngle(th,robot->_th));
		/*robot->setSpeed(start_move_speed);
		robot->moveForward(10);*/
		robot->setSpeed(move_speed);
		robot->moveForward(r);//-10);
		
		robot_pose = Point2D(robot->_x, robot->_y);
		if(p2pDist(robot_pose, target) < 5){
			system("cls");
			cout << "target achieved!" << endl;
			//_cprintf("robot pose = (%lf,%lf)\n",robot->_x,robot->_y);
			cout << "robot pose = (" << robot->_x << "," << robot->_y << ")" << endl;
			tq->popFront();
		}
	} // end while
}
void GoTarget::Action_Go_Point_Calibrate_JudgeDoor(string file_name_str, double init_degree){
	tq->readFile(file_name_str);
	//in order to overcome the problem:robot get wrong pose
	const int rotate_speed=1500;
	const int start_move_speed=700;
	const int move_speed=3300;

	LaserDetectDoor *laser_detect_door = new LaserDetectDoor(localizer->_urgC);

	while(true){
		localizer->findPose2(robot->_x,robot->_y,robot->_th, 50,30);
		robot->setPose(localizer->_pose_x,localizer->_pose_y,localizer->_pose_th);

		Point2D robot_pose = Point2D(robot->_x,robot->_y);
		Point2D target = Point2D(tq->getTarget(0)._x,tq->getTarget(0)._y);
		double r = p2pDist(robot_pose, target);
		double th = p2pAngle(robot_pose, target);
		robot->setSpeed(rotate_speed);
		//robot->turnLeft(fmod(th - robot->_th, 360));
		robot->turnLeft(TurnLeftAngle(th,robot->_th));
		robot->setSpeed(start_move_speed);
		robot->moveForward(10);
		robot->setSpeed(move_speed);
		robot->moveForward(r-10);
		robot_pose = Point2D(robot->_x,robot->_y);
		if(p2pDist(robot_pose, target) < 5)
		 break;
	}
	//angle modify
	double angle_diff=TurnLeftAngle(init_degree,robot->_th);
	robot->stop();
	robot->setSpeed(rotate_speed);
	robot->turnLeft(angle_diff);
	//
	robot->setPose(tq->getTarget(0)._x, tq->getTarget(0)._y, init_degree);
	robot->setSpeed(move_speed);
	tq->popFront();
	while(!tq->isEmpty()){
		localizer->findPose2(robot->_x, robot->_y, robot->_th, 40,30);
		robot->setPose(localizer->_pose_x, localizer->_pose_y, localizer->_pose_th);
		system("cls");
		cout << "robot pose = (" << robot->_x << "," << robot->_y << ")" << endl;
		cout << "th = " << robot->_th << endl;
		//system("pause");
		Point2D robot_pose = Point2D(robot->_x, robot->_y);
		Point2D target = tq->getTarget(0);
		double r = p2pDist(robot_pose, target);
		double th = p2pAngle(robot_pose, target);
		robot->setSpeed(rotate_speed);
		//robot->turnLeft(fmod(th - robot->_th, 360));
		robot->turnLeft(TurnLeftAngle(th,robot->_th));
		/*robot->setSpeed(start_move_speed);
		robot->moveForward(10);*/
		robot->setSpeed(move_speed);
		

		/*Door djuge;
		if(djuge.CanGo()){
			Sleep(	Singleton::getInstance("parameters.txt")->parameters["WaitDoorMileSec"]);
			robot->moveForward(r-10);				
		}*/
		
		if(!laser_detect_door->haveDoor(100))
		{
			while(!laser_detect_door->haveDoor(100))
			{
				robot->stop();
			}	
			while(laser_detect_door->haveDoor(100))
			{
				robot->stop();
			}
			robot->setSpeed(move_speed);
			robot->moveForward(r);//-10);
		}

		else
		{
			while(laser_detect_door->haveDoor(100))
			{
				robot->stop();
			}
			robot->setSpeed(move_speed);
			robot->moveForward(r);//-10);
		}

		//robot->setSpeed(move_speed);
		//robot->moveForward(r-10);


		

		robot_pose = Point2D(robot->_x, robot->_y);
		if(p2pDist(robot_pose, target) < 5){
			system("cls");
			cout << "target achieved!" << endl;
			cout << "robot pose = (" << robot->_x << "," << robot->_y << ")" << endl;
			tq->popFront();
		}
	} // end while
	delete laser_detect_door;


}
void GoTarget::Action_Go_Point_Calibrate_Avoid(string file_name_str, double init_degree)
{
	tq->readFile(file_name_str);
	//in order to overcome the problem:robot get wrong pose
	const int rotate_speed=1000;
	while(true){
		localizer->findPose2(robot->_x,robot->_y,robot->_th, 40,30,100);
		robot->setPose(localizer->_pose_x,localizer->_pose_y,localizer->_pose_th);

		Point2D robot_pose = Point2D(robot->_x,robot->_y);
		Point2D target = Point2D(tq->getTarget(0)._x,tq->getTarget(0)._y);
		double r = p2pDist(robot_pose, target);
		double th = p2pAngle(robot_pose, target);
		robot->setSpeed(rotate_speed);
		//robot->turnLeft(fmod(th - robot->_th, 360));
		robot->turnLeft(TurnLeftAngle(th,robot->_th));
		robot->setSpeed(1000);
		robot->moveForward(10);
		robot->setSpeed(6000);
		//robot->setSpeed(2000);
		robot->moveForward(r-10);
		robot_pose = Point2D(robot->_x,robot->_y);
		if(p2pDist(robot_pose, target) < 5)
		 break;
	}
	//angle modify
	double angle_diff=TurnLeftAngle(init_degree,robot->_th);
	robot->stop();
	robot->setSpeed(rotate_speed);
	robot->turnLeft(angle_diff);
	//
	robot->setPose(tq->getTarget(0)._x, tq->getTarget(0)._y, init_degree);
	robot->setSpeed(6000);
	tq->popFront();
	while(!tq->isEmpty()){
		localizer->findPose2(robot->_x, robot->_y, robot->_th, 40,30,100);
		robot->setPose(localizer->_pose_x, localizer->_pose_y, localizer->_pose_th);
		system("cls");
		cout << "robot pose = (" << robot->_x << "," << robot->_y << ")" << endl;
		cout << "th = " << robot->_th << endl;
		//system("pause");
		Point2D robot_pose = Point2D(robot->_x, robot->_y);
		Point2D target = tq->getTarget(0);
		double r = p2pDist(robot_pose, target);
		double th = p2pAngle(robot_pose, target);
		robot->setSpeed(rotate_speed);
		//robot->turnLeft(fmod(th - robot->_th, 360));
		robot->turnLeft(TurnLeftAngle(th,robot->_th));
		robot->setSpeed(1000);
		robot->moveForward(10);
		robot->setSpeed(6000);
		robot->moveForward(r-10);
		
		robot_pose = Point2D(robot->_x, robot->_y);
		if(p2pDist(robot_pose, target) < 5){
			system("cls");
			cout << "target achieved!" << endl;
			cout << "robot pose = (" << robot->_x << "," << robot->_y << ")" << endl;
			tq->popFront();
		}
	} // end while
}


void GoTarget::Action_Go_Point_avoid(string file_name_str, double init_degree)
{
	tq->readFile(file_name_str);
	//robot->setPose(tq->getTarget(0)._x, tq->getTarget(0)._y, init_degree);
	//tq->popFront();
//-----------------------確認初始位置----------------------------------------------
	localizer->findPose2(robot->_x,robot->_y,robot->_th, 40,4,100);
	robot->setPose(localizer->_pose_x,localizer->_pose_y,localizer->_pose_th);
	Point2D init_target = tq->getTarget(0);
		if(p2pDist(Point2D(robot->_x, robot->_y), init_target) < 15){			
			tq->popFront();
		}
//------------------------------------------------------------------------------------

	TangentBug* tbC;
	tbC = new TangentBug(25);//Construcor sets robot size
	int* range769 = (int*)malloc(769*sizeof(int));
	double delta_deg;
	double delta_dist;
	while(!tq->isEmpty()){

		Point2D pre_robot_pose = Point2D(robot->_x, robot->_y);
		
		localizer->findPose2(robot->_x, robot->_y, robot->_th, 40,30,80);
		robot->setPose(localizer->_pose_x, localizer->_pose_y, localizer->_pose_th);	
		//	localizer->mapping(robot->_x,robot->_y,robot->_th);
		Point2D robot_pose = Point2D(robot->_x, robot->_y);
		Point2D target = tq->getTarget(0);

		int d_error=p2pDist(pre_robot_pose,robot_pose);
		double r = p2pDist(robot_pose, target);
		double th = p2pAngle(robot_pose, target);
		robot->setSpeed(3500);  //(3500)
		robot->turnLeft(TurnLeftAngle(th,robot->_th));

		localizer->findPose2(robot->_x, robot->_y, robot->_th, 2,30,100);
		robot->setPose(localizer->_pose_x, localizer->_pose_y, localizer->_pose_th);	

//-------------------------避障--------------------------------------------------------------------------------
		localizer->_urgC->Scan796(range769);
		Point2D tmpTarget;
		tmpTarget = Local2Global(robot->_x, robot->_y, robot->_th, tbC->getLocalTarget(Global2Local(robot->_x, robot->_y, robot->_th, target),range769));
		delta_deg = p2pAngle(Point2D(robot->_x, robot->_y), tmpTarget) - robot->_th;
		delta_dist = p2pDist(Point2D(robot->_x, robot->_y), tmpTarget);
//-----------------------------------------------------------------------------------------------	
		robot->setSpeed(3500); //(3500);
		robot->turnLeft(delta_deg);
		
		robot->setSpeed(6000);//6000);
		robot->moveForward(delta_dist);

		robot_pose = Point2D(robot->_x, robot->_y);
//-------------------------檢查是否到達 Target-------------------------------------		
		if(p2pDist(robot_pose, target) < 15){			
			tq->popFront();
		}
	} // end while
	delete tbC;

}


void GoTarget::Action_Go_Point_Calibrate_aavoid(double a, double b)
{
	
	//AllocConsole();

	//_cprintf("into aavoid--------------------\n");
	/*			
	tq->readFile(file_name_str);
	localizer->findPose2(robot->_x,robot->_y,robot->_th, 50,25);
	robot->setPose(tq->getTarget(0)._x, tq->getTarget(0)._y, init_degree);
	tq->popFront();*/


	// prepare for TangentBug
	TangentBug* tbC;
	tbC = new TangentBug(20);//25 //Construcor sets robot size
	int* range769 = (int*)malloc(769*sizeof(int));
	double delta_deg;
	double delta_dist;
	//tbC->tem=true;

	int i;
	double s=600.0;

	for(i=0;s>15.0;++i)
	{
        // now position
		//Point2D pre_robot_pose = Point2D(robot->_x, robot->_y);  //  8/26
		/*_cprintf("pre:\n");
		_cprintf("robot x ");
		_cprintf("%f\n",robot->_x);
		_cprintf("robot y ");
		_cprintf("%f\n",robot->_y);*/


		// FindBall has already located, but check
		std::cout<<"Localizing....\n";
		//   8/26修改
		//localizer->findPose2(robot->_x, robot->_y, robot->_th, 50,25); //number is different from POINT (30,25)
		//robot->setPose(localizer->_pose_x, localizer->_pose_y, localizer->_pose_th);
		/*_cprintf("Lrobot x = ");
		_cprintf("%f\n",robot->_x);
		_cprintf("Lrobot y = ");
		_cprintf("%f\n",robot->_y);*/

		// no need to print
		/*std::cout<<"1:\n";
		robot->coutRobotPose();*/

		Point2D robot_pose = Point2D(robot->_x, robot->_y);
		/*_cprintf("new:");
		_cprintf("robot x = ");
		_cprintf("%f\n",robot->_x);
		_cprintf("robot y = ");
		_cprintf("%f\n",robot->_y);*/

		//basically, pre/new are the same
		/*int d_error=p2pDist(pre_robot_pose,robot_pose);
		std::cout<<"error ="<<d_error<<std::endl;*/
		
//		Point2D target = tq->getTarget(0);  //different sequence 
		Point2D target;
		target._x=a;
		target._y=b;

	    /*_cprintf("target x = ");
		_cprintf("%f\n",target._x);
		_cprintf("target y = ");
		_cprintf("%f\n",target._y);*/

		//system("pause");
		// not to print
		/*std::cout<<"2:\n";
		robot->coutRobotPose();*/

		double r = p2pDist(robot_pose, target);
		//double th = p2pAngle(robot_pose, target);//angle define has problem:: atan is no used for here:: findball has been centor so no need to turn
		robot->setSpeed(2000);//2000);
		//robot->turnLeft(TurnLeftAngle(th,robot->_th));
		
		
		// not to print 
		/*std::cout<<"2:\n";
		robot->coutRobotPose();*/

		// TangentBug part
		localizer->_urgC->Scan796(range769);
		Point2D tmpTarget;
		tmpTarget = Local2Global(robot->_x, robot->_y, robot->_th, tbC->getLocalTarget(Global2Local(robot->_x, robot->_y, robot->_th, target),range769));
		delta_deg = p2pAngle(Point2D(robot->_x, robot->_y), tmpTarget) - robot->_th;
		delta_dist = p2pDist(Point2D(robot->_x, robot->_y), tmpTarget);


		_cprintf("tem:");
		_cprintf("robot x = ");
		_cprintf("%f\n",robot->_x);
		_cprintf("robot y = ");
		_cprintf("%f\n",robot->_y);
		robot->setSpeed(2000);;
		_cprintf("delta_deg= ");
		_cprintf("%f\n",delta_deg);

		//if(tbC->tem==true){
		robot->turnLeft(delta_deg);
		_cprintf("has turned LEFT delta_deg \n");
		//}
		std::cout<<"3:\n";
		robot->coutRobotPose();

		/*robot->setSpeed(2800);
		if(robot->moveForward(delta_dist,localizer->_urgC,20)){
			robot->moveForward(-10);
			std::cout<<"4:\n";
			robot->coutRobotPose();
		}*/
		
		robot->setSpeed(2500);//5000);
		_cprintf("delta_dist ");
		_cprintf("%f\n",delta_dist);
		//system("pause");
		
/*		if(delta_dist>70)
		{
			_cprintf("walk 60\n");
			robot->moveForward(60);			
		}
		else
		{
			robot->moveForward(delta_dist);
		}
*/
		//system("pause");
		robot->moveForward(delta_dist);
		//_cprintf("get tem");
		
//		localizer->findPose2(robot->_x, robot->_y, robot->_th, 50,25); //number is different from POINT (30,25)
//		robot->setPose(localizer->_pose_x, localizer->_pose_y, localizer->_pose_th);

		robot_pose = Point2D(robot->_x, robot->_y);


		s=p2pDist(robot_pose, target);
		_cprintf("s = ");
		_cprintf("%f\n",s);
		
		_cprintf("--------------------\n");
		//if(p2pDist(robot_pose, target) < 10){			
		//	tq->popFront();
		//}
		
		std::cout<<"Target achieve:\n";
		//system("pause");
	} // end for
	_cprintf("Target achieve:\n");
	delete tbC;

}
void PrintMatrix(CvMat *Matrix,int Rows,int Cols)
{
    for(int i=0;i<Rows;i++)
    {
        for(int j=0;j<Cols;j++)
        {
            _cprintf("%.2f ",cvGet2D(Matrix,i,j).val[0]); 
        } 
        printf("\n");
    } 
	_cprintf("\n");
}
void GoTarget::Push_Buttton(int n_points){  //n_points 要用幾個點做運算
	bool CANPUSH = false;
	double move_dist;
	while(!CANPUSH){

		localizer->_urgC->getScan682();
		Point2DSet pSet=localizer->_urgC->get682centervalues(n_points);
		//Ax=b
		CvMat *Matrix1=cvCreateMat(n_points,2,CV_32FC1);  
		CvMat *Matrix2=cvCreateMat(n_points,1,CV_32FC1);
		CvMat *SolveSet=cvCreateMat(2,1,CV_32FC1);
		for(int i=0;i<n_points;i++)
		{
			Matrix1->data.fl[i*2]=pSet._vP[i]->_r*cos(pSet._vP[i]->_th);
			Matrix1->data.fl[i*2+1]=1;
			Matrix2->data.fl[i]=pSet._vP[i]->_r*sin(pSet._vP[i]->_th);
		}
	   cvSolve(Matrix1,Matrix2,SolveSet,CV_SVD);
	   PrintMatrix(SolveSet,SolveSet->rows,SolveSet->cols);

	   double arctan = atan(cvGet2D(SolveSet,0,0).val[0]);
	   move_dist = cvGet2D(SolveSet,1,0).val[0];
	   double turnAngle = arctan * 180 / PI;
	   _cprintf(" angle: %lf\n ", turnAngle );
	   if(turnAngle < 15 && turnAngle > -15)
	   {	CANPUSH = true;
			_cprintf("CAN PUSH\n");
			break;
	   }
	   else{
		   _cprintf("DONT PUSH\n");
		   robot->setSpeed(1200);
		   robot->turnLeft(turnAngle);
		   robot->stop();
		   Sleep(800);
	   }
	}
	_cprintf(" move dist: %lf\n ", move_dist);
	robot->setSpeed(1200);
	robot->moveForward(move_dist-16);
	robot->stop();
	robot->moveForward(-30);
	robot->stop();
}
