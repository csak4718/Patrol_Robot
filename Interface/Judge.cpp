#include "stdafx.h"
#include "Judge.h"
#include <iostream>

Judge::Judge(UrgControl* urg,int scan_range=90):_urg(urg){
	 laser_range=scan_range;
}
bool Judge::HaveDoor(){
	   const int maxdoorsize=100;
	   const int mindoorsize=20;
		_urg->getScan682();
		start_n=-1;
		start_n=-1;
		Point2DSet pSet = _urg->getValidPoints(10);
		for(int i=0;i!=pSet._vP.size();i++){
			if(pSet._vP[i]->_r>=2 && pSet._vP[i]->_r<400 && rad2deg(pSet._vP[i]->_th)<90+laser_range && rad2deg(pSet._vP[i]->_th)>90-laser_range){
				if(i>0 && i+1!=pSet._vP.size()){
					if(pSet._vP[i]->_r-pSet._vP[i-1]->_r>30 && pSet._vP[i+1]->_r-pSet._vP[i-1]->_r>30){
						start_n=i-1;	
						double x, y;
						x=pSet._vP[start_n]->_x;
						y=pSet._vP[start_n]->_y;
					}
				if(pSet._vP[i]->_r-pSet._vP[i-1]->_r<-30 && pSet._vP[i+1]->_r-pSet._vP[i-1]->_r<-30){
					if(i+1>end_n){
					end_n=i+1;	
					}
				}		
			  }//endif	
			}				
		}//end for
		if(start_n>0 &&end_n>0){
		double x, y;
		x=pSet._vP[end_n]->_x;
    	y=pSet._vP[end_n]->_y;
		int dispart=sqrt(pow(fabs(pSet._vP[end_n]->_x-pSet._vP[start_n]->_x),2)+pow(fabs(pSet._vP[end_n]->_y-pSet._vP[start_n]->_y),2));
			if(dispart<maxdoorsize && dispart>mindoorsize){
				x=(pSet._vP[start_n]->_x+pSet._vP[end_n]->_x)/2;
				y=(pSet._vP[start_n]->_y+pSet._vP[end_n]->_y)/2;
				double angle=-1*(90-57.3*atan2(y ,x));
				this->_degree=angle;
				this->_theta=angle/57.3;
				this->_distance=sqrt(pow(fabs(x),2)+pow(fabs(y),2));
				//cout<<"degree:"<<_degree<<"\t"<<"dist:"<<_distance<<endl;
				return false;//don't have door
			}
		}
	return true;  //HAVEDOOR close=true
}
bool Judge::CanGo(){  
	bool HAVEDOOR; 
    clock_t t1,t2,t3;
	t1=clock();
	bool state=true;
	while(state){
		t2=clock();
		if((double)(t2-t1)/CLK_TCK > 12){//over 12 sec brek
			break;
		}
		bool num[5]={0};
		for(size_t i=0;i<5;i++){
			num[i]=HaveDoor();
		}

		for(int i=0;i<5;i++){
			if(num[i]==false){
				break;
			}
			state=false;
		}
		
	}
	
	std::cout<<"wait for opening door\n";
	int count=0;
	while(count<3){
		t3=clock();
		if((double)(t3-t2)/CLK_TCK > 12){//over 12 sec brek;
			break;
		}
		HAVEDOOR=HaveDoor();
		std::cout<<HAVEDOOR<<"\n";
		if(HAVEDOOR==false){count++;
		}
	}	
	//if it speed too much time here, robot go ahead
	this->_degree=0;
	this->_distance=100;
	return true;
}

void Judge::ShowImg(){
	IplImage* img = cvCreateImage(cvSize(800, 800), IPL_DEPTH_32F, 3);
	cvNamedWindow("win", CV_WINDOW_AUTOSIZE);
	for(int j=0; j<10000; j++){
		cvZero(img);
		_urg->getScan682();
		Point2DSet pSet = _urg->getValidPoints(10);
		for(int i=0;i!=pSet._vP.size();i++){
			if(pSet._vP[i]->_r>=2 && pSet._vP[i]->_r<400 && rad2deg(pSet._vP[i]->_th)<180 && rad2deg(pSet._vP[i]->_th)>0){
				double x, y;
				x=pSet._vP[i]->_x;
				y=pSet._vP[i]->_y;
				cvCircle(img, cvPoint(400+x, 400-y), 1, cvScalar(255,255,255));
				cvLine(img, cvPoint(400,400), cvPoint(400+x, 400-y), cvScalar(255,0,0));
			
		    if(i>0 && i+1!=pSet._vP.size()){
				if(pSet._vP[i]->_r-pSet._vP[i-1]->_r>30 && pSet._vP[i+1]->_r-pSet._vP[i-1]->_r>30){
					start_n=i-1;	

					double x, y;
				    x=pSet._vP[start_n]->_x;
				    y=pSet._vP[start_n]->_y;
				    cvCircle(img, cvPoint(400+x, 400-y), 5, CV_RGB(255,0,0));
				}
				if(pSet._vP[i]->_r-pSet._vP[i-1]->_r<-30 && pSet._vP[i+1]->_r-pSet._vP[i-1]->_r<-30){
					end_n=i+1;	
					
					double x, y;
					x=pSet._vP[end_n]->_x;
			    	y=pSet._vP[end_n]->_y;
					cvCircle(img, cvPoint(400+x, 400-y), 5, CV_RGB(0,255,0));
					int dispart=sqrt(pow(fabs(pSet._vP[end_n]->_x-pSet._vP[start_n]->_x),2)+pow(fabs(pSet._vP[end_n]->_y-pSet._vP[start_n]->_y),2));
					if(dispart<150 && dispart>40){
						cvLine(img, cvPoint(400+x,400-y), cvPoint(400+pSet._vP[start_n]->_x, 400-pSet._vP[start_n]->_y), CV_RGB(255,0,0),3);
						x=(pSet._vP[start_n]->_x+pSet._vP[end_n]->_x)/2;
						y=(pSet._vP[start_n]->_y+pSet._vP[end_n]->_y)/2;
						double angle=-1*(90-57.3*atan2(y ,x));
						cout<<"1turn left angle:"<<angle<<endl;
						cout<<"depart door cm:"<<sqrt(pow(fabs(x),2)+pow(fabs(y),2))<<endl;
						if(abs(angle) >10){
							if(angle<0){
								cout<<"21eft angle: "<<90+angle<<endl;
							}
							if(angle>0){
								cout<<"21eft angle: "<<90-angle<<endl;
							}
						//	
						}
						cvLine(img, cvPoint(400,400), cvPoint(400+(int)x, 400-(int)y), CV_RGB(0,200,0));
					}
				}
			
						
			}//endif
				
			
			}				
			
		}
		cvShowImage("win", img);
		cvWaitKey(1000);
	}
}