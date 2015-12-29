#include "stdafx.h"
#include "DetectTarget.h"

Singleton* Singleton::uniqueInstance=0;
Singleton::Singleton(){}
Singleton* Singleton::getInstance(std::string filename){
	if(uniqueInstance==0){
	uniqueInstance=new Singleton();
	uniqueInstance->_filename=filename;
	uniqueInstance->store_file(uniqueInstance->_filename);
	uniqueInstance->build_map();
	}
	/*for(std::map<std::string,int>::ifterator iter=uniqueInstance->parameters.begin();iter!=uniqueInstance->parameters.end();iter++)
	{	
		std::cout<<iter->first<<" "<<iter->second<<std::endl;
	}*/
	//std::cout<<uniqueInstance->parameters["Crmin"]<<std::endl;
	//std::cout<<uniqueInstance->parameters["Crmax"]<<std::endl;
	//std::cout<<uniqueInstance->parameters["Cbmin"]<<std::endl;
	//std::cout<<uniqueInstance->parameters["Cbmax"]<<std::endl;
	return uniqueInstance;
}

void Singleton::store_file(std::string filename){
	std::ifstream is;
	is.open(filename.c_str(),std::ios::in);
	std::string textline;
	while (std::getline(is, textline))
       lines_of_text.push_back(textline);

	is.clear();
	is.close();
}
void Singleton::write_file(std::string filename,std::map<std::string,int> map){
	std::ofstream of;
	of.open(filename.c_str(),std::ios::out);

	for(std::map<std::string,int>::iterator iter=map.begin();iter!=map.end();iter++)
	{	
		of<<iter->first<<" "<<iter->second<<std::endl;
	}
	of.clear();
	of.close();
}
void Singleton::build_map(){
	 for (line_no line_num = 0;line_num != lines_of_text.size();++line_num)
     {
		std::istringstream line(lines_of_text[line_num]);
		std::string firstword;
		std::string secondword;
        line >> firstword;
		line >> secondword;
		parameters[firstword]= atoi(secondword.c_str()) ;
     }
}

void Singleton::modify(std::string parameter,int value){
	
	if(uniqueInstance!=0){
		uniqueInstance->parameters[parameter]=value;
		uniqueInstance->write_file(uniqueInstance->_filename,uniqueInstance->parameters);

	}

}
bool myfunction(std::pair<long,double>i,std::pair<long,long>j){return(i.first<j.first);}
Interpolation* Interpolation::unique=0;
Interpolation::Interpolation(){
		std::ifstream file;
		 file.clear();
		 file.open("interpolation.txt",std::ios::in);
		 std::string line;
		bool firstline=true;
	    while( std::getline(file,line) )
		{		
			 if(firstline){
				 firstline=!firstline;
				 continue;
			 }
			 std::istringstream iss(line);
			 std::string word;
			 int word_count=First_word;
			 long Area;
			 int dist;
					
			 while(iss>>word){
				 if(word_count==First_word){
					 Area=atol(word.c_str());	
				 }
				 if(word_count==Second_word){
					 dist=atoi(word.c_str());
				 }
				 word_count++;
			 }//end stringstream while
			 vec.push_back(std::make_pair(Area,dist));
		 }//end while
		 file.clear();
		 file.close();
		 std::sort (vec.begin(), vec.end(),myfunction); 
}


double Interpolation::GetInterpolation(long Area){
			
		if(unique==0){
			unique=new Interpolation();
		}
		unique->X.first=Area;

	    return unique->Dist(unique->vec);
}
double Interpolation::GetInterpolation(int Area){
		if(unique==0){
			unique=new Interpolation();
		}
		unique->X.first=(long)Area;

		return unique->Dist(unique->vec);
}
double Interpolation::Dist( std::vector< std::pair<long,double> > &vec ) 
{	
	    
	 long min=99999999;
	 int number;
	 for(int i=0;i!=vec.size();i++)
	 {  
		long temp=abs(vec[i].first- X.first); 
		if(temp<min)
		{ 
			 min=temp; 
			 number=i;  
		}   
	 } 
	
	 if(number+1<vec.size())
	 if(X.first>vec[number].first&& vec[number+1].first>X.first  )
	 {
		 double slope=(vec[number+1].second-vec[number].second)/(vec[number+1].first-vec[number].first);
		 X.second=slope*(X.first-vec[number].first)+vec[number].second;
		 return X.second;
	 }
	 if(number-1>=0)
	 if( X.first<vec[number].first && vec[number-1].first<X.first)
	 {    
		 double slope=(vec[number].second-vec[number-1].second)/(vec[number].first-vec[number-1].first);
		 X.second=slope*(X.first-vec[number-1].first)+vec[number-1].second;
		 return X.second;
	 }

	  if( X.first>vec[vec.size()-1].first)
	 {
		   X.second=vec[vec.size()-1].second;
		   return X.second;
	 }
	 if( X.first<vec[0].first)
	 {
		 X.second=vec[0].second;
		 return X.second;
	 }
	 return X.second;
}
//====================================VISION=========================================
int Ymin=Singleton::getInstance("parameters.txt")->parameters["Ymin"];
int Ymax=Singleton::getInstance("parameters.txt")->parameters["Ymax"];
int Crmin=Singleton::getInstance("parameters.txt")->parameters["Crmin"];
int Crmax=Singleton::getInstance("parameters.txt")->parameters["Crmax"];
int Cbmin=Singleton::getInstance("parameters.txt")->parameters["Cbmin"];
int Cbmax=Singleton::getInstance("parameters.txt")->parameters["Cbmax"];

int connect(unsigned char* pic,int w,int h,int* label);
void onYmin(int position)
{   
    Ymin=position;
	Singleton::modify("Ymin",Ymin);	
}
void onYmax(int position)
{
   Ymax=position;
	Singleton::modify("Ymax",Ymax);
	
}
void onCrmin(int position)
{
	
    Crmin=position;
	Singleton::modify("Crmin",Crmin);
	
}

void onCrmax(int position)
{
     Crmax=position;
	Singleton::modify("Crmax",Crmax);

}

void onCbmin(int position)
{
    Cbmin=position;
	Singleton::modify("Cbmin",Cbmin);
}

void onCbmax(int position)
{
   
    Cbmax=position;
	Singleton::modify("Cbmax",Cbmax);
	
}



struct Label{
	CvRect rect;
	int count;
};

CvRect marklabel(int* label,int w,int h,int total);
DetectTarget::DetectTarget(){
		frame=0;
		frame_copy=0;
		binary=0;
		MaxRect.x=0;
		MaxRect.y=0;
		MaxRect.width=0;
		MaxRect.height=0;
		_Direction=IDLE;
		storage1= cvCreateMemStorage(0);
		active=false;
		AreaThreshold=Singleton::getInstance("parameters.txt")->parameters["AreaThreshold"];
		std::cout<<AreaThreshold<<std::endl;
		ObjectMaxArea=0;
}
void DetectTarget::activate()
{
	active = true;
	_beginthread( DetectTarget::thrEntry, 0, this);
}

void DetectTarget::deactivate()
{
	active = false;
	
}

void DetectTarget::thrEntry(void *ptr)
{
	 DetectTarget* p = ( DetectTarget*)ptr;
	 p->Detect(1);
	 
}
void DetectTarget::Detect(int version)
{	
	capture=cvCaptureFromCAM(0);
	
	cvNamedWindow( "result", 1 );
	
	if( DetectTarget::capture ){
		while(active)
		{
			if( !cvGrabFrame( DetectTarget::capture )){
					std::cout<<"No webcam";
			}
			
			IplImage* cframe=cvRetrieveFrame(capture);
			
			if( !frame ){
			frame=cvCreateImage(cvSize(640,480),8,3);
			height=frame->height;
			width=frame->width;
			}
			if( !frame_copy )frame_copy = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, frame->nChannels );
			
			if(!binary)
			binary=cvCreateImage( cvSize(640,480),IPL_DEPTH_8U,1);
			
			cvResize(cframe,frame);
			
			if( frame->origin == IPL_ORIGIN_TL )
					   cvCopy( frame, frame_copy, 0 );
			 else
					   cvFlip( frame, frame_copy, 0 );	
		

			
			ColorFilterYCrCb(frame_copy,binary);
			
			
			Erosion(binary,3,5);//3x5
			Erosion(binary,5,3);//3x5
			
			Dilation(binary,5,5);
			Dilation(binary,15,15);
			
			ConnectComponent(binary);			
			ObjectMaxArea= MaxRect.height* MaxRect.width;
				if(ObjectMaxArea>AreaThreshold){
						if((MaxRect.x+MaxRect.width/2)>((width/2)+15)){
						_Direction=RIGHT;
						}
						else if((MaxRect.x+MaxRect.width/2)<((width/2)-15)){
						_Direction=LEFT;
						}
						else
						{
						_Direction=CENTER;
						}
					
				}
				else{
					_Direction=IDLE;
				}
				
				//檢查是否太近?
				if(isCLOSE(binary)){
				    			
					_Direction=TOO_CLOSE;
				}
			_time=time (NULL);
			switch(version)
			{
			case 1:
			cvRectangle(binary,cvPoint(MaxRect.x,MaxRect.y),cvPoint(MaxRect.x+MaxRect.width,MaxRect.y+MaxRect.height),CV_RGB(255,255,255),2,CV_AA,0);
			cvShowImage( "result", binary);
			break;
			case 2:
			cvRectangle(frame,cvPoint(MaxRect.x,MaxRect.y),cvPoint(MaxRect.x+MaxRect.width,MaxRect.y+MaxRect.height),CV_RGB(255,255,255),2,CV_AA,0);
			cvShowImage( "result", frame);
			break;
			}
			
			MaxRect.width=0;
			MaxRect.height=0;
			if(cvWaitKey(20)>0)     break;

			//cvReleaseImage(&cframe);
		}//end wihle
		cvDestroyWindow("result");
		//cvReleaseImage( &frame );
		//cvReleaseImage( &frame_copy );
		///cvReleaseImage(&binary);
		if(capture)
		cvReleaseCapture(&capture);
		_endthread();	
	}
}

void DetectTarget::ColorFilterYCrCb(IplImage *_frame_copy, IplImage* binary)
{
	
	cvCvtColor(_frame_copy,_frame_copy,CV_BGR2YCrCb);

	uchar* data=(uchar*)_frame_copy->imageData;
	uchar* data2=(uchar*)binary->imageData;
	
	
	//HUV
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
				
			    float Y,Cr,Cb;
				Y=(data[i*3*width+3*j]);
				Cr=(data[i*3*width+3*j+1]);
				Cb=(data[i*3*width+3*j+2]);
				
				
				
			if(Y>Ymin &&  Y<Ymax )
			{
				 if(Cr>Crmin && Cr<Crmax)
				 {
						if(Cb>Cbmin && Cb<Cbmax)
						{
							data2[i*width+j]=255;
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

void DetectTarget::Erosion(IplImage *binary, int r1, int c1)
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

void DetectTarget::Dilation(IplImage *binary, int r1, int c1)
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

void DetectTarget::ConnectComponent(IplImage *binary)
{
	uchar* data=(uchar*)binary->imageData;
	int* label=(int*)malloc(height*width*sizeof(int));
	int label_max=connect(data,width,height,label);
	if(label_max!=0)
	MaxRect=marklabel(label,width,height,label_max);
	
	
	//std::cout<<label_max;
	free(label);
}

CvRect marklabel(int* label,int w,int h,int label_max)
{
	int i,j;

	std::vector<CvRect> number;
	for(int labelnumber=0;labelnumber<=label_max;labelnumber++){
		
		int xmin=w;
		int xmax=0;
		int ymin=h;
		int ymax=0;
		for(i=0;i<h;i++)
		{
			for(j=0;j<w;j++)
			{		
				if(label[i*w+j]==labelnumber)
				{
					if(j<(xmin))
						 xmin=j;
					if(j>(xmax))
						 xmax=j;
					if(i<(ymin))
						 ymin=i;
					if(i>(ymax))
						 ymax=i;	
				}
			}
		}
		if(xmin>0&&ymin>0&&(xmax-xmin)>0&&(ymax-ymin)>0){
		number.push_back(cvRect(xmin,ymin,xmax-xmin,ymax-ymin));
		}
	}
	int maxindex=0;
	int maxarea=0;
	if(!number.empty()){
	maxarea=number[0].height*number[0].width;
	}
	
	
	for(i=0;i!=number.size();i++)
	{
		int area=number[i].height*number[i].width;
		if(area>maxarea)
		{
			maxindex=i;
		}
	}
	if(!number.empty())
	return number[maxindex];
	else
	{
		CvRect temp;temp.x=0;temp.y=0;temp.height=0;temp.width=0;
		return temp;
	}
}

void DetectTarget::calibration(int version){//version 2 fullcolor
	capture = cvCaptureFromCAM(0);
	cvNamedWindow( "result", 1 );
	int StopDist=Singleton::getInstance("parameters.txt")->parameters["StopDist"];
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
		
			/*cvCreateTrackbar("Y min","result",&Ymin,255,onYmin);
			cvCreateTrackbar("Y max","result",&Ymax,255,onYmax);*/
			cvCreateTrackbar("Cr min","result",&Crmin,255,onCrmin);
			cvCreateTrackbar("Cr max","result",&Crmax,255,onCrmax);
			cvCreateTrackbar("Cb min","result",&Cbmin,255,onCbmin);
			cvCreateTrackbar("Cb max","result",&Cbmax,255,onCbmax);
			ColorFilterYCrCb(frame_copy,binary);
			
			
			Erosion(binary,3,5);//3x5
			Erosion(binary,5,3);//3x5
			
			Dilation(binary,5,5);
			Dilation(binary,15,15);
			
			ConnectComponent(binary);			
			ObjectMaxArea= MaxRect.height* MaxRect.width;
				if(ObjectMaxArea>AreaThreshold){
					ObjectCenter=MaxRect.x+MaxRect.width/2;
						if(ObjectCenter>((width/2)+30)){
						_Direction=RIGHT;
						}
						else if(ObjectCenter<((width/2)-30)){
						_Direction=LEFT;
						}
						else
						{
						_Direction=CENTER;
						}
				}
				else{
					_Direction=IDLE;
				}
				
				//檢查是否太近?
				if(isCLOSE(binary)){
					_Direction=TOO_CLOSE;
				}
			_time=time (NULL);
			switch(version)
			{
			case 1:
				cvRectangle(binary,cvPoint(MaxRect.x,MaxRect.y),cvPoint(MaxRect.x+MaxRect.width,MaxRect.y+MaxRect.height),CV_RGB(255,255,255),2,CV_AA,0);
				cvShowImage( "result", binary);
			break;
			case 2:
				int dist=0;
				if(ObjectMaxArea>AreaThreshold){
				dist=Area2Dist(ObjectMaxArea);
				}
				cvRectangle(frame,cvPoint(MaxRect.x,MaxRect.y),cvPoint(MaxRect.x+MaxRect.width,MaxRect.y+MaxRect.height),CV_RGB(255,255,255),2,CV_AA,0);
				if(dist!=0){
					if(abs(dist-StopDist)<2){//2cm error range
						char s0[30];
						sprintf(s0,"Object Area:%d dist: %d",ObjectMaxArea,dist);
						cvPutText(frame,s0,cvPoint(20,15),&cvFont(1,1), CV_RGB(55,200,200) );	
						char* s="reach";
						cvPutText(frame,s,cvPoint(20,25),&cvFont(1,1), CV_RGB(55,200,200) );
					}
					else{
						char s[30];
						sprintf(s,"Object Area:%d dist: %d",ObjectMaxArea,dist);
						cvPutText(frame,s,cvPoint(20,15),&cvFont(1,1), CV_RGB(55,200,200) );
						sprintf(s,"Stop Dist:%d",Singleton::getInstance("parameters.txt")->parameters["StopDist"]);
						cvPutText(frame,s,cvPoint(20,35),&cvFont(1,1), CV_RGB(55,200,200) );
					}
				}
				cvShowImage( "result", frame);
			break;
			}
			
			MaxRect.width=0;
			MaxRect.height=0;
			cvWaitKey( 10 ); 
		}//end wihle
		cvReleaseImage(&frame);
		cvReleaseImage(&binary);
		cvDestroyWindow("result");
		cvReleaseImage(&frame_copy);
		if(capture)
		cvReleaseCapture(&capture);
	}
}
int DetectTarget::Area2Dist(int area){
	/*const double scale=430000;
	if(area>0){
		return (int)(scale/area);
	}*/
	return (int)Interpolation::GetInterpolation(area);

}
int connect(unsigned char* pic,int w,int h,int* label)//標connected component
{	
	
	int* lable=(int*)malloc(w*h*sizeof(int)); 
	int* flag=(int*)malloc(w*h*sizeof(int)); 
	int next_lable=1;
	int judge[25000]={0};
//one pass
	for(int j=0;j<h;j++)
	{
		for(int i=0;i<w;i++)
		{
			lable[i+j*w]=0;
			if(pic[i+j*w]>0)
			{
				flag[i+j*w]=1;
				if(i==0)
				{
					if(j==0 || flag[i-w+j*w]==0)
					{
						lable[i+j*w]=next_lable;
						judge[lable[i+j*w]]=next_lable;
						next_lable++;
					}
					else
					{
						lable[i+j*w]=lable[i-w+j*w];
					}
				}
				else if(j==0)
				{
					if(flag[i-1+j*w]==0)
					{
						lable[i+j*w]=next_lable;
						judge[lable[i+j*w]]=next_lable;
						next_lable++;
					}
					else
					{
						lable[i+j*w]=lable[i-1+j*w];
					}
				}
				else
				{
					if(flag[i-1+j*w]==0 && flag[i-w+j*w]==0)
					{
						lable[i+j*w]=next_lable;
						judge[lable[i+j*w]]=next_lable;
						next_lable++;
					}
					else if(flag[i-1+j*w]==1 && flag[i-w+j*w]==0)
					{
						lable[i+j*w]=lable[i-1+j*w];
					}
					else if(flag[i-1+j*w]==0 && flag[i-w+j*w]==1)
					{
						lable[i+j*w]=lable[i-w+j*w];
					}
					else if(judge[lable[i-1+j*w]]>judge[lable[i-w+j*w]])
					{
						lable[i+j*w]=judge[lable[i-w+j*w]];
						judge[lable[i-1+j*w]]=judge[lable[i-w+j*w]];
					}
					else if(judge[lable[i-1+j*w]]<=judge[lable[i-w+j*w]])
					{
						lable[i+j*w]=judge[lable[i-1+j*w]];
						judge[lable[i-w+j*w]]=judge[lable[i-1+j*w]];
					}
				}
			}
			else
			{
				flag[i+j*w]=0;
			}
		}
	}

	//計算重複的lable
	int lable_max=0;
	for(int i=1;i<=next_lable;i++)
	{
		while(judge[judge[i]]!=judge[i])
			judge[i]=judge[judge[i]];
		if(judge[i]>lable_max)
			lable_max=judge[i];
	}
	//標回pic上
	for(int j=0;j<h;j++)
	{
		for(int i=0;i<w;i++)
		{
			label[i+j*w]=judge[lable[i+j*w]];
		}
	}
	
	free(flag);
	free(lable);
	return lable_max;
}
bool DetectTarget::isCLOSE(const IplImage* binary)
{
	uchar* data=(uchar*)binary->imageData;
	int count=0;
	int totalpixel=0;
	int xpixel=0,ypixel=0;
	_TotalPixel=0;
	for(int i=0;i<binary->height;i++)
	{
		for(int j=0;j<binary->width;j++)
		{
			if(i<binary->height-binary->height/3){
				if(data[i*(binary->width)+j]>=200)
				{
					count++;
					ypixel+=i;
					xpixel+=j;
					totalpixel++;
				}
				else
					continue;
			}
			else{
				if(data[i*(binary->width)+j]>=200)
				{
					
					ypixel+=i;
					xpixel+=j;
					totalpixel++;
					continue;
				}
				else
					count++;			
			}
		}
	}
	
	if(totalpixel!=0){
	centerY=ypixel/totalpixel;
	centerX=xpixel/totalpixel;
	}
	_TotalPixel=totalpixel;
	std::cout<<"center:"<<centerX<<","<<centerY<<"total"<<_TotalPixel<<std::endl;;
	if(count>(binary->width)*(binary->height)/3 &&centerY>binary->height-(binary->height)/4)//binary->height-(binary->height)/3
	{
		return true;
	}
	else 
		return false;
}

