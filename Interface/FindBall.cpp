#include "stdafx.h"
#include "FindBall.h"
#include "cmath"
#include "TangentBug.hpp"
#include <iostream>

#define PI 3.1415926

using namespace std;

void FindBall::test()
{
	const int rotate_speed = 0;
	const int move_speed = 0;		
	bool StartFind=true;
	 _robot->setSpeed(rotate_speed);
	 DetectTarget _detectTarget;
	 _detectTarget.activate();
	 Sleep(1000);
	 int count=0;
		 while(StartFind)
		 {		 
			 if(_detectTarget._time==time(NULL)){
				 switch(_detectTarget._Direction)
				 {
				 
				  case DetectTarget::RIGHT:
					 cout<<"r"<<endl;
					 
					 break;
				  case DetectTarget::LEFT:
					  cout<<"l"<<endl;
					  
					 break;
				  case DetectTarget::CENTER:
					 cout<<"c"<<endl;
					
					 //if( Interpolation::GetInterpolation(_detectTarget.ObjectMaxArea) < StopDist)
					 //{
						// 
						// std::cout<<"find";
						//// StartFind=false;
						// StartFind=true;
					 //}
					 //else
					 //{
						// std::cout<<"move forward"<<std::endl;//move 2 cm
					 //}
					 break; 
				  case DetectTarget::IDLE:
					  cout<<"non"<<endl; 			  
					  break;
				  /*case DetectTarget::TOO_CLOSE:
					  cout<<"too close"<<endl;
					  break;*/
				 }
			 }//end if
			 else{
			 cout<<"wait for image processing....."<<endl;
			 }
	    }//end while
		 _detectTarget.deactivate();
}
void FindBall::calibration(){
	 DetectTarget _detectTarget;
	 _detectTarget.calibration(1);
}
void FindBall::calibration_Fullcolor(){
	 DetectTarget _detectTarget;
	 _detectTarget.calibration(2);
}
void FindBall::operation0()//Find 0 //turn right
{
	cout<<"opeation0\n";
	const int rotate_speed =1200;
	const int rotate_speed2 =250;
	const int move_speed =2500;	
	DetectTarget _detectTarget;
	bool StartFind=true;
	 _robot->setSpeed(rotate_speed);
	 _detectTarget.activate();
	
	_localizer->findPose2(_robot->_x,_robot->_y,_robot->_th, 40,30);
	_robot->setPose(_localizer->_pose_x,_localizer->_pose_y,_localizer->_pose_th);
	int start_x=_robot->_x;
	int start_y=_robot->_y;
	double Total_angle=0;//count total angle robot turn
	
	// if has already into center, next time into IDLE and find nothing, then doesn't need to go straight 100cm(at that time: true)
	bool centerdone=false;
    //double PI=3.1415926;
	// count findangle
	double Total_findangle=0;

	int room;
	bool room_count0=false;
	bool room_count1=false;
	bool room_count2=false;
    bool room_count3=false;
	Point2D _target;
	GoTarget ToTarget(_robot,_localizer);
	bool first_time_getball=false;
	bool IDLEleftdone=false;
	bool IDLErightdone=false;
	//bool IDLEcenter=false;

	double ang_degree=-40;//if robot don't find the ball,and then turn ang degree (右轉)
	srand((unsigned)time(0));
	double rotate_ang=4;
	int Error_range=2;////
	int leftcount=0,rightcount=0,countthreshold=4;
	int trytime=0;
		 while(StartFind)
		 {		 
			
			 double TargetDist;/////
			 if(_detectTarget._time==time(NULL)){
				 switch(_detectTarget._Direction)
				 {									 
				  case DetectTarget::RIGHT:
					 //AllocConsole();
					  cout<<"-------right------------"<<endl;
					 _cprintf("-------right------------\n");
					 _robot->stop();
				 	 _robot->setSpeed(rotate_speed2);
					 if(leftcount<countthreshold){
						//rotate_ang=3+rand()%4;
						rotate_ang=2+rand()%4;
						rotate_ang=-1*rotate_ang;
					 }
					 else{
						//rotate_ang=1+rand()%4;
						rotate_ang=1+rand()%3;
						 rotate_ang=-1*rotate_ang;
					 }
					 leftcount++;
					_robot->turnLeft(rotate_ang);//rotate_ang=-1*rotate_ang;
					_cprintf("%f\n",rotate_ang);
					centerdone=false;
					//_localizer->findPose2(_robot->_x,_robot->_y,_robot->_th, 10,30,100);
					//_robot->setPose(_localizer->_pose_x,_localizer->_pose_y,_localizer->_pose_th);
					 break;
				  case DetectTarget::LEFT:
					  //AllocConsole();
					 cout<<"-------left------------"<<endl;
					 _cprintf("-------left------------\n");
					 _robot->stop();
				  	 _robot->setSpeed(rotate_speed2);
					 if(rightcount<countthreshold){
						//rotate_ang=1+rand()%4;
						 rotate_ang=2+rand()%4;
					 }
					 else{
						 rotate_ang=1+rand()%2;
					 }
					 rightcount++;
					 _robot->turnLeft(rotate_ang);//rotate_ang為正時，代表向右轉
					 _cprintf("%f\n",rotate_ang);
					 centerdone=false;
					 //_localizer->findPose2(_robot->_x,_robot->_y,_robot->_th, 10,30,100);
					 //_robot->setPose(_localizer->_pose_x,_localizer->_pose_y,_localizer->_pose_th);
					 break;
				  case DetectTarget::CENTER:
					 cout<<"-------center------------"<<endl;
					 //AllocConsole();
		//			 do//use do while，使robot立刻先往前，而不必先做判斷
		//			 { 
					 _cprintf("-------center------------\n");
					 //for(int u=0; u<70; u++ ){
					 
					 //_cprintf("_detectTarget.ObjectMaxArea = ");
					//	 _cprintf("%d\n",_detectTarget.ObjectMaxArea);
				
					 //}
					 //system("pause");


					  //------------------------------
					 trytime++;
					 _cprintf("Have tried : ");
					 _cprintf("%d\n",trytime);

					 if(trytime==4){//IF 4的話, then 試3次
						 _cprintf("need to Give up\n");
						 StartFind=false;
						break;
					 }
					 //----------------------------------------------

						TargetDist=Interpolation::GetInterpolation(_detectTarget.ObjectMaxArea)+30; //25;//方向正確後，量測實際距離+error10/////
						
						_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //40   //number is different from POINT (30,25)
						_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);

						/*_cprintf("TargetDist ");
						_cprintf("%f\n",TargetDist);
						_cprintf("LLpose x ");
						_cprintf("%f\n",_localizer->_pose_x);
						_cprintf("pose y ");
						_cprintf("%f\n",_localizer->_pose_y);
						_cprintf("pose_th ");
						_cprintf("%f\n",_localizer->_pose_th);*/

						//Point2D _target;
						//Point2D _target2;

						// now use Total_angle_check, but _localizer->_pose_th is almost the same with it 
						//_target._x= _robot->_x+TargetDist*cos(Total_angle_check*PI/180);
						//_target._y= _robot->_y+TargetDist*sin(Total_angle_check*PI/180);
		/*				if (TargetDist>90)
						{	
							_cprintf("into if\n");
							_target2._x= _robot->_x+70*cos(_localizer->_pose_th*PI/180);
							_target2._y= _robot->_y+70*sin(_localizer->_pose_th*PI/180);
						}
		*/				
						_target._x= _robot->_x+TargetDist*cos(_localizer->_pose_th*PI/180);
						_target._y= _robot->_y+TargetDist*sin(_localizer->_pose_th*PI/180);
						
						
						_cprintf("_target x ");
 						_cprintf("%f\n",_target._x);
						_cprintf("_target y ");
 						_cprintf("%f\n",_target._y);


						//---------------------------------------以下為各種特例-----------------------------------------//
						
		/*				if( first_time_getball==false && _target._x < 295 &&_target._y > 480)//room1
						{
							ToTarget.Action_Go_Point_Calibrate_aavoid(42,340);
							ToTarget.Action_Go_Point_Calibrate_aavoid(42,550);
							Total_angle=0;
							first_time_getball=true;
							break;//跳出case center，但仍會繼續try to 抓球，∵StartFind==true(此情況下，centerdone仍==false)
						}
		*/
						//以下為避免將目的地設為別間的紅球
					/*	if( _robot->_x < 298 && _robot->_y > 305 ){//room1
							if (_target._x >298 || _target._y <305){//看到的球是別間的
								_cprintf("ball robot not in the same room\n");
								_robot->turnLeft(-28);//讓camera的視線離開別間的紅球
								_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								break;//跳出case center，但仍會繼續try to 抓球，∵StartFind==true(此情況下，centerdone仍==false)
							}
						}
						
						if( _robot->_x > 302 && _robot->_y > 305 ){//room2
							if (_target._x < 302 || _target._y <305){//看到的球是別間的
								_cprintf("ball robot not in the same room\n");
								_robot->turnLeft(-28);//讓camera的視線離開別間的紅球
								_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								break;
							}
						}

						if( _robot->_x > 303 && _robot->_y < 202 ){//room3
							if (_target._x < 303 || _target._y > 202){//看到的球是別間的
								_cprintf("ball robot not in the same room\n");
								_robot->turnLeft(-28);//讓camera的視線離開別間的紅球
								_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								break;
							}
						}
						
						if( _robot->_x < 297 && _robot->_y < 208 ){//living room
							if (_target._x > 297 || _target._y > 208){//看到的球是別間的
								_cprintf("ball robot not in the same room\n");
								_robot->turnLeft(-28);//讓camera的視線離開別間的紅球
								_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								break;
							}
						}
						*/
						// wall
						if( _robot->_x < 298 && _robot->_y > 305 ){//room1
							if (_target._x > 295 || _target._x < 7){
								//_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								//_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								_cprintf(" wall: give up\n");
								StartFind=false;
								break;
							}
							else if (_target._y > 591 || _target._y < 309){
								//_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								//_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								_cprintf("wall: give up\n");
								StartFind=false;
								break;
							}
						}
						if( _robot->_x > 302 && _robot->_y > 305 ){//room2
							if (_target._x > 592 || _target._x < 306){
								//_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								//_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								_cprintf("wall: give up\n");
								StartFind=false;
								break;
							}
							else if (_target._y > 591 || _target._y < 308){
								//_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								//_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								_cprintf("wall: give up\n");
								StartFind=false;
								break;
							}
						}
						if( _robot->_x > 303 && _robot->_y < 202 ){//room3
							if (_target._x > 592 || _target._x < 306){
								//_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								//_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								_cprintf("wall: give up\n");
								StartFind=false;
								break;
							}
							else if (_target._y > 199 || _target._y < 7){
								//_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								//_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								_cprintf("wall: give up\n");
								StartFind=false;
								break;
							}
						}
						if( _robot->_x < 297 && _robot->_y < 208 ){//living room
							if (_target._x > 294 || _target._x < 7){
								//_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								//_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								_cprintf("wall: give up\n");
								StartFind=false;
								break;
							}
							else if ( _target._y < 7){
								//_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								//_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								_cprintf("wall: give up\n");
								StartFind=false;
								break;
							}
						}
						

				//---------------------------------------以上為各種特例-----------------------------------------//

						_cprintf("---into aavoid--------------------\n");
						ToTarget.Action_Go_Point_Calibrate_aavoid(_target._x,_target._y);
					  
					  centerdone=true;
					  _cprintf("centerdone= ");
					 _cprintf("%d\n",centerdone);
					 _robot->stop();
					 
					 _localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 40,30,100); //number is different from POINT (30,25)
					 _robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);

		//			 }while (_detectTarget.ObjectMaxArea !=0 );//球仍在camera的window中
					 //_robot->moveForward(21);
					 /*_robot->stop();
					 Sleep(100);*/
					 //-----------------------------------------------------------------------------------//
					 //關門
					 //-----------------------------------------------------------------------------------//
					 std::cout<<"maybe has got the ball"<<std::endl;
					 _cprintf("moveForward(-60)\n");
					 //system("pause");
					 _robot->moveForward(-60);
					 // StartFind=true;
					 //centerdone=true;
					 //_cprintf("%d\n",centerdone);
					 Total_angle=0;
					 IDLErightdone=false;
					 IDLEleftdone=false;
					 //IDLEcenter=false;
					 break; //跳出case center，但仍會繼續try to 抓球(做檢查)，∵StartFind==true
					 
				  case DetectTarget::IDLE:
					  cout<<"non\n";
					  AllocConsole();
						 _cprintf("---------into IDLE--------\n");
					   if(_detectTarget._TotalPixel>50&& _detectTarget.centerY<_detectTarget.height/3){
							
							if(_detectTarget.centerX-_detectTarget.width/2>30){
								_robot->stop();
								_robot->setSpeed(rotate_speed);
								//_robot->turnLeft(-3-rand()%3);
								
								rotate_ang=-3-rand()%3;
								_robot->turnLeft(rotate_ang);
							}
							else if(_detectTarget.width/2-_detectTarget.centerX>30){
								_robot->stop();
								_robot->setSpeed(rotate_speed);
								//_robot->turnLeft(3+rand()%3);							
								rotate_ang=3+rand()%3;
								_robot->turnLeft(rotate_ang);								
							}
							else{
								_robot->stop();
								_robot->setSpeed(move_speed);
								_robot->moveForward(10);
							}
					   }
					   else if(centerdone==true && IDLEleftdone==false && /*IDLEcenter==false &&*/ IDLErightdone==false)
					   {
						   //turn left
						   _cprintf("turn left 45\n");
						   _robot->turnLeft(45);
						   //system("pause");
						   IDLEleftdone=true;
					   }
					   else if(centerdone==true && IDLEleftdone==true && /*IDLEcenter==false &&*/ IDLErightdone==false)
					   {
						   //turn right
						   _cprintf("turn right 90\n");
						   _robot->turnLeft(-90);
						   IDLErightdone=true;
						   //system("pause");
						   //IDLEcenter=true;
					   }
					/*   else if(centerdone==true && IDLEleftdone==true && IDLEcenter==true && IDLErightdone==false)
					   {
						   //turn right
						   _cprintf("turn right 45\n");
						   _robot->turnLeft(-45);
						   IDLErightdone=true;
					   }*/
					   else if(centerdone==true && IDLEleftdone==true && /*IDLEcenter==true &&*/ IDLErightdone==true)
					   {
						   //has already turn to check
						    StartFind=false;//(end while)
						   _cprintf("got the ball\n");
						 //  _localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 50,25); //number is different from POINT (30,25)
						 //  _robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
						 // 
							//_cprintf("local x ");
							//_cprintf("%f\n",_localizer->_pose_x);
							//_cprintf("local y ");
							//_cprintf("%f\n",_localizer->_pose_y);
							break;
					   }
					   else{
						_robot->stop();
					   _robot->setSpeed(rotate_speed);
					   _robot->turnLeft(ang_degree);//can't find ball，右轉30度
					    //_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
					   //_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
					   Total_angle+=ang_degree;
					   _cprintf("Total_angle= ");
					   _cprintf("%f\n",Total_angle);
					   
					   }// end if  first place to check where the ball is(whether we have got the ball or not)
					   
					   //已因找不到而右轉了365度以上
					   if(abs(Total_angle)>365){
						   StartFind=false;
					   }
		/*			   if(abs(Total_angle)>365){
						   						   
						   // has not got the ball, judge which room
						   _localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 50,25); //number is different from POINT (30,25)
						   _robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
						   if (_localizer->_pose_x < 295 && _localizer->_pose_y > 300)//賽前確認threshold精確
						   {
							   room=1;
						   }
						   else if (_localizer->_pose_x > 306 && _localizer->_pose_y > 300)
						   {
							   room=2;
						   }
						   else if (_localizer->_pose_x > 306 && _localizer->_pose_y < 200)
						   {
							   room=3;
						   }
						   else 
						   {
							   // living room
							   room=0;
						   }

						   switch(room)
						   {
							   case 0:
								   if(room_count0==false)
								   {
									   GoTarget _ToTarget(_robot,_localizer);
									   //next position(,)
									   //_ToTarget.Action_Go_Point_Calibrate_aavoid(98,43);//賽前確認
									   
									   room_count0=true;
								   }
								   else 
								   {
									   // give up
									   StartFind=false;
									   _cprintf("give up");
								   }
								   _localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 50,25); //number is different from POINT (30,25)
								   _robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								   break;

							   case 1:
								   if(room_count1==false && _localizer->_pose_y < 480)
								   {
									   GoTarget _ToTarget(_robot,_localizer);
									   //next position(,)
									   _ToTarget.Action_Go_Point_Calibrate_aavoid(42,340);
									   _ToTarget.Action_Go_Point_Calibrate_aavoid(42,550);

									   room_count1=true;

								   }								 
								   else 
								   {
									   // give up
									   StartFind=false;
									   _cprintf("give up");
								   }
								   _localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 50,25); //number is different from POINT (30,25)
								   _robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								   break;

							   case 2:
								   if(room_count2==false)
								   {
									   GoTarget _ToTarget(_robot,_localizer);
									   //next position(,)
									   _ToTarget.Action_Go_Point_Calibrate_aavoid(352,548);

									   room_count2=true;
								   }
								   else 
								   {
									   // give up
									   StartFind=false;
									   _cprintf("give up");
								   }
								   _localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 50,25); //number is different from POINT (30,25)
								   _robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								   break;

							   case 3:
								   if(room_count3==false)
								   {
									   GoTarget _ToTarget(_robot,_localizer);
									   //next position(,)
									   _ToTarget.Action_Go_Point_Calibrate_aavoid(441,50);

									   room_count3=true;
								   }
								   else 
								   {
									   // give up
									   StartFind=false;
									   _cprintf("give up");
								   }
								   _localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 50,25); //number is different from POINT (30,25)
								   _robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								   break;
						   }// end switch

						   // restart to find
						   Total_angle=0;
						   IDLErightdone=false;
						   IDLEleftdone=false;
						   IDLEcenter=false;
						   
						   }// end if */
				      _localizer->findPose2(_robot->_x,_robot->_y,_robot->_th, 30,25);
	        		  _robot->setPose(_localizer->_pose_x,_localizer->_pose_y,_localizer->_pose_th);
					  break;
/*				  case DetectTarget::TOO_CLOSE://///
					  cout<<"too close"<<endl;////
					  _robot->stop();
					  _robot->setSpeed(rotate_speed);
					  _robot->moveForward(-4);
					  _localizer->findPose2(_robot->_x,_robot->_y,_robot->_th, 30,25);
	        		  _robot->setPose(_localizer->_pose_x,_localizer->_pose_y,_localizer->_pose_th);/////
					  break;	////////
*/
				 }//end switch
			 }//end if   i.e.  if(_detectTarget._time==time(NULL))
				else{
				 cout<<"wait for image processing....."<<endl;
			 }
	    }//end while
		 _detectTarget.deactivate();
		_localizer->findPose2(_robot->_x,_robot->_y,_robot->_th, 30, 20, 80);
		_robot->setPose(_localizer->_pose_x,_localizer->_pose_y,_localizer->_pose_th);
}

void FindBall::operation1()//Find 1 //turn left
{
	cout<<"opeation1\n";
	const int rotate_speed =1200;
	const int rotate_speed2 =250;
	const int move_speed =2500;	
	DetectTarget _detectTarget;
	bool StartFind=true;
	 _robot->setSpeed(rotate_speed);
	 _detectTarget.activate();
	
	_localizer->findPose2(_robot->_x,_robot->_y,_robot->_th, 40,30);
	_robot->setPose(_localizer->_pose_x,_localizer->_pose_y,_localizer->_pose_th);
	int start_x=_robot->_x;
	int start_y=_robot->_y;
	double Total_angle=0;//count total angle robot turn
	
	// if has already into center, next time into IDLE and find nothing, then doesn't need to go straight 100cm(at that time: true)
	bool centerdone=false;
    //double PI=3.1415926;
	// count findangle
	double Total_findangle=0;

	int room;
	bool room_count0=false;
	bool room_count1=false;
	bool room_count2=false;
    bool room_count3=false;
	Point2D _target;
	GoTarget ToTarget(_robot,_localizer);
	bool first_time_getball=false;
	bool IDLEleftdone=false;
	bool IDLErightdone=false;
	//bool IDLEcenter=false;

	double ang_degree=40;//if robot don't find the ball,and then turn ang degree (右轉)
	srand((unsigned)time(0));
	double rotate_ang=4;
	int Error_range=2;////
	int leftcount=0,rightcount=0,countthreshold=4;
	int trytime=0;
		 while(StartFind)
		 {		 
			
			 double TargetDist;/////
			 if(_detectTarget._time==time(NULL)){
				 switch(_detectTarget._Direction)
				 {									 
				  case DetectTarget::RIGHT:
					 //AllocConsole();
					  cout<<"-------right------------"<<endl;
					 _cprintf("-------right------------\n");
					 _robot->stop();
				 	 _robot->setSpeed(rotate_speed2);
					 if(leftcount<countthreshold){
						//rotate_ang=3+rand()%4;
						rotate_ang=2+rand()%4;
						rotate_ang=-1*rotate_ang;
					 }
					 else{
						//rotate_ang=1+rand()%4;
						rotate_ang=1+rand()%3;
						 rotate_ang=-1*rotate_ang;
					 }
					 leftcount++;
					_robot->turnLeft(rotate_ang);//rotate_ang=-1*rotate_ang;
					_cprintf("%f\n",rotate_ang);
					centerdone=false;
					//_localizer->findPose2(_robot->_x,_robot->_y,_robot->_th, 10,30,100);
					//_robot->setPose(_localizer->_pose_x,_localizer->_pose_y,_localizer->_pose_th);
					 break;
				  case DetectTarget::LEFT:
					  //AllocConsole();
					 cout<<"-------left------------"<<endl;
					 _cprintf("-------left------------\n");
					 _robot->stop();
				  	 _robot->setSpeed(rotate_speed2);
					 if(rightcount<countthreshold){
						//rotate_ang=1+rand()%4;
						 rotate_ang=2+rand()%4;
					 }
					 else{
						 rotate_ang=1+rand()%2;
					 }
					 rightcount++;
					 _robot->turnLeft(rotate_ang);//rotate_ang為正時，代表向右轉
					 _cprintf("%f\n",rotate_ang);
					 centerdone=false;
					 //_localizer->findPose2(_robot->_x,_robot->_y,_robot->_th, 10,30,100);
					 //_robot->setPose(_localizer->_pose_x,_localizer->_pose_y,_localizer->_pose_th);
					 break;
				  case DetectTarget::CENTER:
					 cout<<"-------center------------"<<endl;
					 //AllocConsole();
		//			 do//use do while，使robot立刻先往前，而不必先做判斷
		//			 { 
					 _cprintf("-------center------------\n");
					 //for(int u=0; u<70; u++ ){
					 
					 //_cprintf("_detectTarget.ObjectMaxArea = ");
					//	 _cprintf("%d\n",_detectTarget.ObjectMaxArea);
				
					 //}
					 //system("pause");


					  //------------------------------
					 trytime++;
					 _cprintf("Have tried : ");
					 _cprintf("%d\n",trytime);

					 if(trytime==4){//IF 4的話, then 試3次
						 _cprintf("need to Give up\n");
						 StartFind=false;
						break;
					 }
					 //----------------------------------------------

						TargetDist=Interpolation::GetInterpolation(_detectTarget.ObjectMaxArea)+30; //25;//方向正確後，量測實際距離+error10/////
						
						_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //40   //number is different from POINT (30,25)
						_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);

						/*_cprintf("TargetDist ");
						_cprintf("%f\n",TargetDist);
						_cprintf("LLpose x ");
						_cprintf("%f\n",_localizer->_pose_x);
						_cprintf("pose y ");
						_cprintf("%f\n",_localizer->_pose_y);
						_cprintf("pose_th ");
						_cprintf("%f\n",_localizer->_pose_th);*/

						//Point2D _target;
						//Point2D _target2;

						// now use Total_angle_check, but _localizer->_pose_th is almost the same with it 
						//_target._x= _robot->_x+TargetDist*cos(Total_angle_check*PI/180);
						//_target._y= _robot->_y+TargetDist*sin(Total_angle_check*PI/180);
		/*				if (TargetDist>90)
						{	
							_cprintf("into if\n");
							_target2._x= _robot->_x+70*cos(_localizer->_pose_th*PI/180);
							_target2._y= _robot->_y+70*sin(_localizer->_pose_th*PI/180);
						}
		*/				
						_target._x= _robot->_x+TargetDist*cos(_localizer->_pose_th*PI/180);
						_target._y= _robot->_y+TargetDist*sin(_localizer->_pose_th*PI/180);
						
						
						_cprintf("_target x ");
 						_cprintf("%f\n",_target._x);
						_cprintf("_target y ");
 						_cprintf("%f\n",_target._y);


						//---------------------------------------以下為各種特例-----------------------------------------//
						
		/*				if( first_time_getball==false && _target._x < 295 &&_target._y > 480)//room1
						{
							ToTarget.Action_Go_Point_Calibrate_aavoid(42,340);
							ToTarget.Action_Go_Point_Calibrate_aavoid(42,550);
							Total_angle=0;
							first_time_getball=true;
							break;//跳出case center，但仍會繼續try to 抓球，∵StartFind==true(此情況下，centerdone仍==false)
						}
		*/
						//以下為避免將目的地設為別間的紅球
					/*	if( _robot->_x < 298 && _robot->_y > 305 ){//room1
							if (_target._x >298 || _target._y <305){//看到的球是別間的
								_cprintf("ball robot not in the same room\n");
								_robot->turnLeft(-28);//讓camera的視線離開別間的紅球
								_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								break;//跳出case center，但仍會繼續try to 抓球，∵StartFind==true(此情況下，centerdone仍==false)
							}
						}
						
						if( _robot->_x > 302 && _robot->_y > 305 ){//room2
							if (_target._x < 302 || _target._y <305){//看到的球是別間的
								_cprintf("ball robot not in the same room\n");
								_robot->turnLeft(-28);//讓camera的視線離開別間的紅球
								_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								break;
							}
						}

						if( _robot->_x > 303 && _robot->_y < 202 ){//room3
							if (_target._x < 303 || _target._y > 202){//看到的球是別間的
								_cprintf("ball robot not in the same room\n");
								_robot->turnLeft(-28);//讓camera的視線離開別間的紅球
								_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								break;
							}
						}
						
						if( _robot->_x < 297 && _robot->_y < 208 ){//living room
							if (_target._x > 297 || _target._y > 208){//看到的球是別間的
								_cprintf("ball robot not in the same room\n");
								_robot->turnLeft(-28);//讓camera的視線離開別間的紅球
								_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								break;
							}
						}

						*/
						// wall
						if( _robot->_x < 298 && _robot->_y > 305 ){//room1
							if (_target._x > 295 || _target._x < 7){
								//_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								//_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								_cprintf(" wall: give up\n");
								StartFind=false;
								break;
							}
							else if (_target._y > 591 || _target._y < 309){
								//_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								//_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								_cprintf("wall: give up\n");
								StartFind=false;
								break;
							}
						}
						if( _robot->_x > 302 && _robot->_y > 305 ){//room2
							if (_target._x > 592 || _target._x < 306){
								//_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								//_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								_cprintf("wall: give up\n");
								StartFind=false;
								break;
							}
							else if (_target._y > 591 || _target._y < 308){
								//_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								//_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								_cprintf("wall: give up\n");
								StartFind=false;
								break;
							}
						}
						if( _robot->_x > 303 && _robot->_y < 202 ){//room3
							if (_target._x > 592 || _target._x < 306){
								//_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								//_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								_cprintf("wall: give up\n");
								StartFind=false;
								break;
							}
							else if (_target._y > 199 || _target._y < 7){
								//_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								//_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								_cprintf("wall: give up\n");
								StartFind=false;
								break;
							}
						}
						if( _robot->_x < 297 && _robot->_y < 208 ){//living room
							if (_target._x > 294 || _target._x < 7){
								//_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								//_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								_cprintf("wall: give up\n");
								StartFind=false;
								break;
							}
							else if ( _target._y < 7){
								//_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
								//_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								_cprintf("wall: give up\n");
								StartFind=false;
								break;
							}
						}
						

				//---------------------------------------以上為各種特例-----------------------------------------//

						_cprintf("---into aavoid--------------------\n");
						ToTarget.Action_Go_Point_Calibrate_aavoid(_target._x,_target._y);
					  
					  centerdone=true;
					  _cprintf("centerdone= ");
					 _cprintf("%d\n",centerdone);
					 _robot->stop();
					 
					 _localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 40,30,100); //number is different from POINT (30,25)
					 _robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);

		//			 }while (_detectTarget.ObjectMaxArea !=0 );//球仍在camera的window中
					 //_robot->moveForward(21);
					 /*_robot->stop();
					 Sleep(100);*/
					 //-----------------------------------------------------------------------------------//
					 //關門
					 //-----------------------------------------------------------------------------------//
					 std::cout<<"maybe has got the ball"<<std::endl;
					 _cprintf("moveForward(-60)\n");
					 //system("pause");
					 _robot->moveForward(-60);
					 // StartFind=true;
					 //centerdone=true;
					 //_cprintf("%d\n",centerdone);
					 Total_angle=0;
					 IDLErightdone=false;
					 IDLEleftdone=false;
					 //IDLEcenter=false;
					 break; //跳出case center，但仍會繼續try to 抓球(做檢查)，∵StartFind==true
					 
				  case DetectTarget::IDLE:
					  cout<<"non\n";
					  AllocConsole();
						 _cprintf("---------into IDLE--------\n");
					   if(_detectTarget._TotalPixel>50&& _detectTarget.centerY<_detectTarget.height/3){
							
							if(_detectTarget.centerX-_detectTarget.width/2>30){
								_robot->stop();
								_robot->setSpeed(rotate_speed);
								//_robot->turnLeft(-3-rand()%3);
								
								rotate_ang=-3-rand()%3;
								_robot->turnLeft(rotate_ang);
							}
							else if(_detectTarget.width/2-_detectTarget.centerX>30){
								_robot->stop();
								_robot->setSpeed(rotate_speed);
								//_robot->turnLeft(3+rand()%3);							
								rotate_ang=3+rand()%3;
								_robot->turnLeft(rotate_ang);								
							}
							else{
								_robot->stop();
								_robot->setSpeed(move_speed);
								_robot->moveForward(10);
							}
					   }
					   else if(centerdone==true && IDLEleftdone==false && /*IDLEcenter==false &&*/ IDLErightdone==false)
					   {
						   //turn left
						   _cprintf("turn left 45\n");
						   _robot->turnLeft(45);
						   //system("pause");
						   IDLEleftdone=true;
					   }
					   else if(centerdone==true && IDLEleftdone==true && /*IDLEcenter==false &&*/ IDLErightdone==false)
					   {
						   //turn right
						   _cprintf("turn right 90\n");
						   _robot->turnLeft(-90);
						   IDLErightdone=true;
						   //system("pause");
						   //IDLEcenter=true;
					   }
					/*   else if(centerdone==true && IDLEleftdone==true && IDLEcenter==true && IDLErightdone==false)
					   {
						   //turn right
						   _cprintf("turn right 45\n");
						   _robot->turnLeft(-45);
						   IDLErightdone=true;
					   }*/
					   else if(centerdone==true && IDLEleftdone==true && /*IDLEcenter==true &&*/ IDLErightdone==true)
					   {
						   //has already turn to check
						    StartFind=false;//(end while)
						   _cprintf("got the ball\n");
						 //  _localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 50,25); //number is different from POINT (30,25)
						 //  _robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
						 // 
							//_cprintf("local x ");
							//_cprintf("%f\n",_localizer->_pose_x);
							//_cprintf("local y ");
							//_cprintf("%f\n",_localizer->_pose_y);
							break;
					   }
					   else{
						_robot->stop();
					   _robot->setSpeed(rotate_speed);
					   _robot->turnLeft(ang_degree);//can't find ball，右轉30度
					    //_localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 10,30,100); //number is different from POINT (30,25)
					   //_robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
					   Total_angle+=ang_degree;
					   _cprintf("Total_angle= ");
					   _cprintf("%f\n",Total_angle);
					   
					   }// end if  first place to check where the ball is(whether we have got the ball or not)
					   
					   //已因找不到而右轉了365度以上
					   if(abs(Total_angle)>365){
						   StartFind=false;
					   }
		/*			   if(abs(Total_angle)>365){
						   						   
						   // has not got the ball, judge which room
						   _localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 50,25); //number is different from POINT (30,25)
						   _robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
						   if (_localizer->_pose_x < 295 && _localizer->_pose_y > 300)//賽前確認threshold精確
						   {
							   room=1;
						   }
						   else if (_localizer->_pose_x > 306 && _localizer->_pose_y > 300)
						   {
							   room=2;
						   }
						   else if (_localizer->_pose_x > 306 && _localizer->_pose_y < 200)
						   {
							   room=3;
						   }
						   else 
						   {
							   // living room
							   room=0;
						   }

						   switch(room)
						   {
							   case 0:
								   if(room_count0==false)
								   {
									   GoTarget _ToTarget(_robot,_localizer);
									   //next position(,)
									   //_ToTarget.Action_Go_Point_Calibrate_aavoid(98,43);//賽前確認
									   
									   room_count0=true;
								   }
								   else 
								   {
									   // give up
									   StartFind=false;
									   _cprintf("give up");
								   }
								   _localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 50,25); //number is different from POINT (30,25)
								   _robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								   break;

							   case 1:
								   if(room_count1==false && _localizer->_pose_y < 480)
								   {
									   GoTarget _ToTarget(_robot,_localizer);
									   //next position(,)
									   _ToTarget.Action_Go_Point_Calibrate_aavoid(42,340);
									   _ToTarget.Action_Go_Point_Calibrate_aavoid(42,550);

									   room_count1=true;

								   }								 
								   else 
								   {
									   // give up
									   StartFind=false;
									   _cprintf("give up");
								   }
								   _localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 50,25); //number is different from POINT (30,25)
								   _robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								   break;

							   case 2:
								   if(room_count2==false)
								   {
									   GoTarget _ToTarget(_robot,_localizer);
									   //next position(,)
									   _ToTarget.Action_Go_Point_Calibrate_aavoid(352,548);

									   room_count2=true;
								   }
								   else 
								   {
									   // give up
									   StartFind=false;
									   _cprintf("give up");
								   }
								   _localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 50,25); //number is different from POINT (30,25)
								   _robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								   break;

							   case 3:
								   if(room_count3==false)
								   {
									   GoTarget _ToTarget(_robot,_localizer);
									   //next position(,)
									   _ToTarget.Action_Go_Point_Calibrate_aavoid(441,50);

									   room_count3=true;
								   }
								   else 
								   {
									   // give up
									   StartFind=false;
									   _cprintf("give up");
								   }
								   _localizer->findPose2(_robot->_x, _robot->_y, _robot->_th, 50,25); //number is different from POINT (30,25)
								   _robot->setPose(_localizer->_pose_x, _localizer->_pose_y, _localizer->_pose_th);
								   break;
						   }// end switch

						   // restart to find
						   Total_angle=0;
						   IDLErightdone=false;
						   IDLEleftdone=false;
						   IDLEcenter=false;
						   
						   }// end if */
				      _localizer->findPose2(_robot->_x,_robot->_y,_robot->_th, 30,25);
	        		  _robot->setPose(_localizer->_pose_x,_localizer->_pose_y,_localizer->_pose_th);
					  break;
/*				  case DetectTarget::TOO_CLOSE://///
					  cout<<"too close"<<endl;////
					  _robot->stop();
					  _robot->setSpeed(rotate_speed);
					  _robot->moveForward(-4);
					  _localizer->findPose2(_robot->_x,_robot->_y,_robot->_th, 30,25);
	        		  _robot->setPose(_localizer->_pose_x,_localizer->_pose_y,_localizer->_pose_th);/////
					  break;	////////
*/
				 }//end switch
			 }//end if   i.e.  if(_detectTarget._time==time(NULL))
				else{
				 cout<<"wait for image processing....."<<endl;
			 }
	    }//end while
		 _detectTarget.deactivate();
		_localizer->findPose2(_robot->_x,_robot->_y,_robot->_th, 30, 20, 80);
		_robot->setPose(_localizer->_pose_x,_localizer->_pose_y,_localizer->_pose_th);
}

void FindBall::operation2(){//find 2
}
void FindBall::operation3(){	 
}