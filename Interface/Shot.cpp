#include "stdafx.h"
#include "Shot.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include<process.h>
#include<windows.h> 
//#pragma comment(lib,"winmm.lib") 
const char* cascade_name =
"haarcascade_frontalface_alt_tree.xml";
int countFrame = 0;
//static bool isCaught = false;
Shot::Shot(Robot *Robotptr, Localizer *Localizerptr,USBControl	*mp_USBControl_ptr)
{	captureThief = 0;
	frame = 0;
	frame_copy = 0;
	isCaught = false;
	checkLeft1 = false;
	checkLeft2 = false;
	checkRight1 = false;
	checkRight2 = false;
	//TooLong = false;
	localizer=Localizerptr;
	robot=Robotptr;
	mp_USBControl=mp_USBControl_ptr;

}
bool Shot::isCAUGHT()
{
	return isCaught;
}
void Shot::take_a_shot()
{
	captureThief = cvCaptureFromCAM(0);
	_cprintf("test1\n");

	cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );  //?
    if( !cascade )
    { 
		std::cout<<"CASCADE ERROR!!\n";
		system("pause");
    }

	_cprintf("test2\n");
    storage = cvCreateMemStorage(0);
	cvNamedWindow( "camera", 1 );
	_cprintf("test3\n");
	//int j=1;
	TooLong = false;
	
    if( captureThief && !isCaught)
    {	_cprintf("capture1\n");
		
		float total_time = 0;
		countFrame = 0;
        while(!TooLong)
        {	//countFrame = 0;
			_cprintf("capture2\n");
            if( !cvGrabFrame( captureThief ))
                break;
            frame =cvQueryFrame( captureThief );
			//cvWriteFrame(writer,frame);
            if( !frame )
                break;
            if( !frame_copy )
                frame_copy = cvCreateImage( cvSize(frame->width,frame->height),
                                            IPL_DEPTH_8U, frame->nChannels );
            if( frame->origin == IPL_ORIGIN_TL )
                cvCopy( frame, frame_copy, 0 );
            else
                cvFlip( frame, frame_copy, 0 );
 
			clock_t start_time, end_time;
			float detect_time = 0;
			start_time = clock(); /* microsecond */

					
			detect_and_draw( frame_copy );    //主要偵測臉程式
			_cprintf("test4\n");
			
			end_time = clock();
			detect_time = (float)(end_time - start_time)/CLOCKS_PER_SEC;
			_cprintf("elapse time: %lf \n ", detect_time);
				//cvShowImage("result", frame_copy);
			
			total_time += detect_time;
			_cprintf("total time: %lf \n ", total_time);
			
			/*if( cvWaitKey( 10 ) >= 0 )
                break;*/
			cvWaitKey(10);
			if(isCaught)
				break;

			if( total_time > 0.6){ //1.2){
				TooLong = true;
				break;
			}

        }// end while
		//cvReleaseVideoWriter(&writer);
        cvReleaseImage( &frame_copy );
        cvReleaseCapture( &captureThief );
    } // end if
    cvDestroyWindow("camera");


}
void Shot::just_shot()
{
	captureThief = cvCaptureFromCAM(0);
	_cprintf("test1\n");


    //storage = cvCreateMemStorage(0);
	cvNamedWindow( "camera", 1 );
	_cprintf("test3\n");
	//int j=1;
	//TooLong = false;
	
    if( captureThief && !isCaught)
    {	_cprintf("capture1\n");
		
		//float total_time = 0;
		//countFrame = 0;
        while(!isCaught)
        {	
			_cprintf("capture2\n");
            if( !cvGrabFrame( captureThief ))
                break;
            frame =cvQueryFrame( captureThief );
			
            if( !frame )
                break;
            if( !frame_copy )
                frame_copy = cvCreateImage( cvSize(frame->width,frame->height),
                                            IPL_DEPTH_8U, frame->nChannels );
            if( frame->origin == IPL_ORIGIN_TL )
                cvCopy( frame, frame_copy, 0 );
            else
                cvFlip( frame, frame_copy, 0 );
 
			
			/*if( cvWaitKey( 10 ) >= 0 )
                break;*/
			cvSaveImage("C://Users//IRA//Desktop//SKS_Competition//Thief.jpg", frame_copy);
			//cvShowImage("Caught",frame_copy);
			isCaught = true;
			cvWaitKey(10);
			
			if(isCaught)
				break;

        }// end while
		
        cvReleaseImage( &frame_copy );
        cvReleaseCapture( &captureThief );
    } // end if
    cvDestroyWindow("camera");

}
void Shot::detect_and_draw( IplImage* img )
{

	static CvScalar colors[] = 
    {
        {{0,0,255}},
        {{0,128,255}},
        {{0,255,255}},
        {{0,0,255}},
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}}

    };
    double scale = 1.3;
    IplImage* gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );   //灰階影像
    IplImage* small_img = cvCreateImage( cvSize( cvRound (img->width/scale),
                         cvRound (img->height/scale)), 8, 1 );
    int i;
 
    cvCvtColor( img, gray, CV_BGR2GRAY );
    cvResize( gray, small_img, CV_INTER_LINEAR );
    cvEqualizeHist( small_img, small_img );
    cvClearMemStorage( storage );
	
    if( cascade )
    {
        double t = (double)cvGetTickCount();
        CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage,
                                            1.1, 2, 0/*CV_HAAR_DO_CANNY_PRUNING*/,
                                            cvSize(30, 30) );       //偵測臉
        t = (double)cvGetTickCount() - t;
      
		_cprintf("faces->total: %d\n", faces->total);
		
		/*while( faces->total == 0 ){
			_robot->turnLeft(10);
			_robot->stop();
			Sleep(500);
		}*/
        
		for( i = 0; i < (faces ? faces->total : 0); i++ )
        {	
			/*robot->setSpeed(1000);
			robot->turnLeft(5);
			robot->stop();
			Sleep(500);*/
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
			countFrame ++;

			CvRect set;
			set.x=r->x*scale;
			set.y=r->y*scale;
			set.width=r->width*scale;
			set.height=r->height*scale;
			
			cvSetImageROI( gray,set);
			IplImage* resize_img=cvCreateImage(cvSize(100,100),8,1);
			char name[100];
			cvResize(gray,resize_img,1);
			cvEqualizeHist(resize_img ,resize_img );			
			
			sprintf(name,"Thief!!!");
			//cvSaveImage(filename,resize_img);	
			_cprintf("test  draw\n");
			cvRectangle(img, cvPoint(r->x*scale,r->y*scale), cvPoint((r->x+r->width)*scale, (r->y+r->height)*scale), colors[i%8], 1, CV_AA, 0);
		    CvFont Font1 = cvFont(5, 7);	
			
			/*if(   (r->width*scale)<50  ||  (r->height*scale)<50   ){
				_robot->moveForward(10);
				_robot->stop();
			}
			else*/	
				//countFrame++;

			if( countFrame == 4){
				cvPutText(img, name, cvPoint(r->x*scale,r->y*scale), &Font1, colors[i%8]);  
				cvSaveImage("C://Users//IRA//Desktop//SKS_Competition//Thief.jpg", img);
				//cvShowImage("Caught",img);
				isCaught = true;
			}
			
			////***** face recognition 人臉辨識**************************************************************************************************************
			//std::pair<std::string,int> Person=test.recognize(resize_img);                                                                                
			//cvRectangle(img,cvPoint(r->x*scale,r->y*scale),cvPoint((r->x+r->width)*scale,(r->y+r->height)*scale),colors[Person.second%8],2,CV_AA,0);   
			//CvFont Font1=cvFont(2,1);	                                                                                                                
			////cvPutText(img,Person.first.c_str(),cvPoint(r->x*scale+r->width*scale*0.15,r->y*scale+40),&cvFont(1,1)/*&Font1*/,colors[Person.second%8]);  
			//cvPutText(img,Person.first.c_str(),cvPoint(r->x*scale,r->y*scale),&cvFont(1,1)/*&Font1*/,colors[Person.second%8]);  
			////********************************************************************************************************************************************* 
			//		
			cvReleaseImage(&resize_img);
			cvResetImageROI(gray );
        }	
    }
	
    cvShowImage( "camera", img );
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );

}
void Shot::playSound(void *ptr)
{
	 Shot* p = (Shot*)ptr;
	 p->mp_USBControl->WriteIOValue(DEVNUM,128,0,&p->dwRet);
	//for(int i=0;i<2;i++)
	 PlaySound("ambulance.wav",NULL,SND_FILENAME);//不開thread catch_thief
	 PlaySound("kc.wav",NULL,SND_FILENAME);
	// PlaySound("destiny.wav",NULL,SND_FILENAME|SND_ASYNC);//開thread
}
void Shot::showThief()
{
	IplImage *photo = 0;
	photo = cvLoadImage("C://Users//IRA//Desktop//SKS_Competition//Thief.jpg",1);
	cvNamedWindow("Thief",0);
	_beginthread(Shot::playSound,0,this);
	
	cvShowImage("Thief",photo);
	//playSound();
	/*robot->setSpeed(1200);
	robot->turnLeft(30);
	robot->stop();*/
	cvWaitKey(5000);
	cvDestroyWindow("Thief");
	cvReleaseImage(&photo);
}

void Shot::check(int value)
{	
	int rotate1 = 45;
	int rotate2 = -180;
	if(value == 1){
		if( TooLong == true && isCaught == false){
			int rotate_speed=1200;
			robot->setSpeed(rotate_speed);
			if(!checkLeft1){
				robot->turnLeft(rotate1);
				checkLeft1 = true;
			}
			else if(!checkLeft2){
				robot->turnLeft(rotate1);
				checkLeft2 = true;
			}
			else if(!checkRight1){
				robot->turnLeft(rotate2);
				checkRight1 = true;
			}
			else if(!checkRight2){
				robot->turnLeft(rotate1);
				checkRight2 = true;
			}
			else{
				robot->turnLeft(rotate1);
			}
			robot->stop();
			//_cprintf("check: %d, %d\n",checkLeft,checkRight);
		}
	}


}