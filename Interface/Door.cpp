#include "stdafx.h"
#include "Door.h"
int Door_Ymin=Singleton::getInstance("parameters.txt")->parameters["Door_Ymin"];	
int Door_Ymax=Singleton::getInstance("parameters.txt")->parameters["Door_Ymax"];
int Door_Crmin=Singleton::getInstance("parameters.txt")->parameters["Door_Crmin"];
int Door_Crmax=Singleton::getInstance("parameters.txt")->parameters["Door_Crmax"];
int Door_Cbmin=Singleton::getInstance("parameters.txt")->parameters["Door_Cbmin"];
int Door_Cbmax=Singleton::getInstance("parameters.txt")->parameters["Door_Cbmax"];
typedef enum DoorState{CLOSE,OPEN}DoorState;

void Door_onYmin(int position)
{   
    Door_Ymin=position;
	Singleton::modify("Door_Ymin",Door_Ymin);	
}
void Door_onYmax(int position)
{
   Door_Ymax=position;
	Singleton::modify("Door_Ymax",Door_Ymax);
	
}
void Door_onCrmin(int position)
{
	
    Door_Crmin=position;
	Singleton::modify("Door_Crmin",Door_Crmin);
	
}

void Door_onCrmax(int position)
{
     Door_Crmax=position;
	Singleton::modify("Door_Crmax",Door_Crmax);
}

void Door_onCbmin(int position)
{
    Door_Cbmin=position;
	Singleton::modify("Door_Cbmin",Door_Cbmin);
}

void Door_onCbmax(int position)
{
   
    Door_Cbmax=position;
	Singleton::modify("Door_Cbmax",Door_Cbmax);
	
}
Door::Door(){
	Total=0;
	Totalthreshold=Singleton::getInstance("parameters.txt")->parameters["Door_AreaThreshold"];
}

bool Door::CanGo(){  
	IplImage* frame=0;
	IplImage* frame_copy=0;
	IplImage *binary=0;
	DoorState door=OPEN;
	capture = cvCaptureFromCAM(0);
	cvNamedWindow( "result", 1 );
	clock_t t1,t2;
	t1=clock();
	if( capture ){
		while(1)
		{
			t2=clock();
			if((double)(t2-t1)/CLK_TCK > 22){//over 20sec ,break
				break;
			}
			if( !cvGrabFrame( capture )){
					std::cout<<"No webcam";
			}
			IplImage* cframe=0;
			frame=cvCreateImage(cvSize(640,480),8,3);
			cframe=cvRetrieveFrame(capture);
			cvResize(cframe,frame);
			if( !frame_copy ){
			frame_copy = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, frame->nChannels );
			
			height=frame->height;
			width=frame->width;
			}
			if(!binary)
			binary=cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U,1);
			if( frame->origin == IPL_ORIGIN_TL )
					   cvCopy( frame, frame_copy, 0 );
			 else
					   cvFlip( frame, frame_copy, 0 );	
		
			ColorFilterYCrCb(frame_copy,binary);
			Erosion(binary,3,5);//3x5
			Erosion(binary,5,3);//3x5
			Dilation(binary,3,3);
			Dilation(binary,5,5);
			
			if(Total>Totalthreshold){
				char temp[40];
				sprintf(temp,"can't go, than threshhold number %d",Total);
				cvPutText(binary,temp,cvPoint(20,400),&cvFont(1,1), CV_RGB(255,255,255) );
				door=CLOSE;
			}
			else{
				if(door==CLOSE){
					door=OPEN;
					cvPutText(binary,"go",cvPoint(20,400),&cvFont(1,1), CV_RGB(255,255,255) );
					break;
				}				
				else{
					cvPutText(binary,"can't go,wait....",cvPoint(20,400),&cvFont(1,1), CV_RGB(255,255,255) );
					door=OPEN;
				}
			}
			cvShowImage( "result", binary);
			
			cvWaitKey( 10 ); 
		}//end wihle
		cvReleaseImage(&frame);
		cvReleaseImage(&binary);
		cvDestroyWindow("result");
		cvReleaseImage(&frame_copy);
		cvReleaseCapture(&capture);
	}

	return true;
}

void Door::ColorFilterYCrCb(IplImage *_frame_copy, IplImage* binary)
{
	
	cvCvtColor(_frame_copy,_frame_copy,CV_BGR2YCrCb);
	uchar* data=(uchar*)_frame_copy->imageData;
	uchar* data2=(uchar*)binary->imageData;
	Total=0;
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
				
			    float Y,Cr,Cb;
				Y=(data[i*3*width+3*j]);
				Cr=(data[i*3*width+3*j+1]);
				Cb=(data[i*3*width+3*j+2]);
				
				
				
			if(Y>Door_Ymin &&  Y<Door_Ymax )
			{
				 if(Cr>Door_Crmin && Cr<Door_Crmax)
				 {
						if(Cb>Door_Cbmin && Cb<Door_Cbmax)
						{
							data2[i*width+j]=255;
							Total++;
						}
						else
						{
							data2[i*width+j]=0;
						}
				 }
				 else
				 {
					data2[i*width+j]=0;		
				 }
			}
				
			else
			{
				data2[i*width+j]=0;
			}

		
		}
	}
	
}

void Door::calibration(){
	IplImage* frame=0;
	IplImage* frame_copy=0;
	IplImage *binary=0;
	
	capture = cvCaptureFromCAM(0);
	cvNamedWindow( "result", 1 );
	
	if( capture ){
		for(int c=0;c<15000;c++)
		{
			if( !cvGrabFrame( capture )){
					std::cout<<"No webcam";
			}
			IplImage* cframe=0;
			frame=cvCreateImage(cvSize(640,480),8,3);
			cframe=cvRetrieveFrame(capture);
			cvResize(cframe,frame);
			if( !frame_copy ){
			frame_copy = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, frame->nChannels );
			
			height=frame->height;
			width=frame->width;
			}
			if(!binary)
			binary=cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U,1);
			if( frame->origin == IPL_ORIGIN_TL )
					   cvCopy( frame, frame_copy, 0 );
			 else
					   cvFlip( frame, frame_copy, 0 );	
		
			/*cvCreateTrackbar("Y min","result",&Door_Ymin,255,Door_onYmin);
			cvCreateTrackbar("Y max","result",&Door_Ymax,255,Door_onYmax);*/
			cvCreateTrackbar("Cr min","result",&Door_Crmin,255,Door_onCrmin);
			cvCreateTrackbar("Cr max","result",&Door_Crmax,255,Door_onCrmax);
			cvCreateTrackbar("Cb min","result",&Door_Cbmin,255,Door_onCbmin);
			cvCreateTrackbar("Cb max","result",&Door_Cbmax,255,Door_onCbmax);
			ColorFilterYCrCb(frame_copy,binary);
			Erosion(binary,3,5);//3x5
			Erosion(binary,5,3);//3x5
			Dilation(binary,5,5);
			if(Total>Totalthreshold){
				char temp[40];
				sprintf(temp,"can't go, than threshhold number %d",Total);
				cvPutText(binary,temp,cvPoint(20,400),&cvFont(1,1), CV_RGB(255,255,255) );
				
			}
			cvShowImage( "result", binary);
			
			cvWaitKey( 10 ); 
		}//end wihle
		cvReleaseImage(&frame);
		cvReleaseImage(&binary);
		cvDestroyWindow("result");
		cvReleaseImage(&frame_copy);
		cvReleaseCapture(&capture);
	}
}


void Door::Erosion(IplImage *binary, int r1, int c1)
{
	uchar* data=(uchar*)binary->imageData;
	unsigned char* data2=(unsigned char*)malloc(width*height*sizeof(unsigned char));
	r1=r1/2;
	c1=c1/2;
	int i,j,k1,k2;
	for(i=0;i<height;i++)
	{	for(j=0;j<width;j++)
		{	bool ok=true;
			if(data[i*width+j]==255)
			{
				for(k1=-r1;k1<=r1;k1++)
				{	
					for(k2=-c1;k2<=c1;k2++)
					{
						int r=i+k1;
						int c=j+k2;
						if(r>=height)
							r=height-1;
						if(r<0)
							r=0;
						if(c>=width)
							c=width-1;
						if(c<0)
							c=0;

						if(data[r*width+c]!=255)
						ok=false;
					}
				}
			}
			else 
				ok=false;

			if(ok==true)
			data2[i*width+j]=255;
			else
			data2[i*width+j]=0;
		}
	}//end for

	for(i=0;i<height;i++)
	{	for( j=0;j<width;j++)
		{
			data[i*width+j]=data2[i*width+j];
		}
	}
	free(data2);
}

void Door::Dilation(IplImage *binary, int r1, int c1)
{
	uchar* data=(uchar*)binary->imageData;
	unsigned char* data2=(unsigned char*)malloc(width*height*sizeof(unsigned char));
	r1=r1/2;
	c1=c1/2;
	int i,j;
	for(i=0;i<height;i++)
	{	for(j=0;j<width;j++)
		{	
			if(data[i*width+j]==255)
			{	data2[i*width+j]=255;
				for(int k1=-r1;k1<=r1;k1++)
				{	
					for(int k2=-c1;k2<=c1;k2++)
					{
						int r=i+k1;
						int c=j+k2;
						if(r>=height)
							r=height-1;
						if(r<0)
							r=0;
						if(c>=width)
							c=width-1;
						if(c<0)
							c=0;

						data2[r*width+c]=255;
						
					}
				}
			}
			else if(data2[i*width+j]==255){
				data2[i*width+j]=255;
			}
			else
			data2[i*width+j]=0;
		}
	}//end for

	for(i=0;i<height;i++)
	{	for(j=0;j<width;j++)
		{
			data[i*width+j]=data2[i*width+j];
		}
	}
	free(data2);
}