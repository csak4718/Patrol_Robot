// InterfaceDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "Interface.h"
#include "InterfaceDlg.h"
//#include "conio.h"
//#include <string>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


std::vector<CvPoint> Move_Point; 
IplImage* MapImg;
Robot* robot;
Localizer* localizer;
UrgControl* urgC=0;
GoTarget* gt;
GoTarget* ToTarget;
USBControl		*mp_USBControl;
bool isdefault=false;

void CreateGridMap(IplImage* MapImg)
{
	int w=MapImg->width;
	int h=MapImg->height;
	int value;
	char c_value[5];
	uchar* data=(uchar*)MapImg->imageData;
	ofstream file;
	file.open("GridMap.txt",ios::out);
	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
		{
			int B=data[3*i*w+3*j];
			int G=data[3*i*w+3*j+1];
			int R=data[3*i*w+3*j+2];
			value=(B+G+R)/3;
			/*if(B==255&&G==255&&R==255)
				file<<"15"<<" ";
			else if(B==0&&G==0&&R==0)
				file<<"0"<<" ";
			else
				file<<"5"<<" ";*/
			file<<itoa(value,c_value,10)<< " ";
		}
		file<<std::endl;
	}
	file.clear();
	file.close();
}

void MySaveImage(char* filename,IplImage* MapImg){
	int w=MapImg->width;
	int h=MapImg->height;
	uchar* data=(uchar*)MapImg->imageData;
	IplImage* image=cvCreateImage(cvSize(w,h),8,3);
	uchar* temp=(uchar*)image->imageData;

	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
		{
			int B=data[3*i*w+3*j];
			int G=data[3*i*w+3*j+1];
			int R=data[3*i*w+3*j+2];
			if(B==255&&G==255&&R==255)
			{
				temp[3*i*w+3*j]=255;
				temp[3*i*w+3*j+1]=255;
				temp[3*i*w+3*j+2]=255;
			}
			else
			{
				temp[3*i*w+3*j]=0;
				temp[3*i*w+3*j+1]=0;
				temp[3*i*w+3*j+2]=0;
			}
		}
	}
	cvSaveImage(filename,image);
	cvReleaseImage(&image);
}

void readfile(std::string filename,std::vector<std::pair<std::string,int>>& fvec)
{
	std::ifstream file;
	
	file.open(filename.c_str(),std::ios::in);
	//file.close();
	//file.clear();
	//file.open(filename.c_str(),std::ios::in);
	std::string line,word;
	while( std::getline(file,line) )
	{
		std::string loop;
		int orient;
		std::istringstream stream(line);
		int count=0;
		while(stream>>word)
		{
			if(count==0){
				count++;
				loop="./Path/"+word+".txt";
			}
			else{
				orient=atoi(word.c_str());
			}
		}
		fvec.push_back(std::make_pair(loop,orient));		
	}
	file.clear();
	file.close();
}


CEdit* tedit;
void onMouse(int Event,int x,int y,int flags,void* param)
{
	char str[255];
	sprintf(str,"(%d,%d)",x,599-y);
	tedit->SetWindowTextA(str);
	int size=36;
	int Obstacle_width=Singleton::getInstance("parameters.txt")->parameters["ObstacleWidth"];
	switch(Event)
	{
	case CV_EVENT_LBUTTONDOWN:
		if(MapImg->imageData[3*y*MapImg->width+3*x]!=0&&MapImg->imageData[3*y*MapImg->width+3*x+1]!=0&&MapImg->imageData[3*y*MapImg->width+3*x+2]!=0){
			break;
		}
		else{
		cvCircle(MapImg,cvPoint(x,y),2,CV_RGB(255,0,0),size,4,0);//draw circle
		Move_Point.push_back(cvPoint(x,599-y));// add target point
		}
		break;
	case  CV_EVENT_RBUTTONDOWN:
		if(Move_Point.size()!=0){
			cvCircle(MapImg,cvPoint(Move_Point[Move_Point.size()-1].x,599-Move_Point[Move_Point.size()-1].y),2,CV_RGB(0,0,0),size,4,0);//塗黑
			
			if(Move_Point.size()-1!=0){
				cvLine(MapImg,cvPoint(Move_Point[Move_Point.size()-1].x,599-Move_Point[Move_Point.size()-1].y),cvPoint(Move_Point[Move_Point.size()-2].x,599-Move_Point[Move_Point.size()-2].y),CV_RGB(0,0,0),2,1,0);
				cvCircle(MapImg,cvPoint(Move_Point[Move_Point.size()-2].x,599-Move_Point[Move_Point.size()-2].y),2,CV_RGB(255,0,0),size,4,0);//re draw circle
			}
			Move_Point.pop_back(); //delete last target point	
		}
		else{
			std::cout<<"empty"<<std::endl;
		}
		break;
	case CV_EVENT_MBUTTONDOWN:
		
		CvPoint UpperLeft=cvPoint(x-Obstacle_width/2,y-Obstacle_width/2);
		CvPoint BottomRight=cvPoint(x+Obstacle_width/2,y+Obstacle_width/2);
		cvRectangle(MapImg,UpperLeft,BottomRight,CV_RGB(255,255,255),1,0,0);
		uchar* data=(uchar*)MapImg->imageData;
		for(int i=UpperLeft.y;i!=BottomRight.y;i++)
		{
			for(int j=UpperLeft.x;j!=BottomRight.x;j++)
			{
				data[3*i*(MapImg->width)+3*j]=255;
				data[3*i*(MapImg->width)+3*j+1]=255;
				data[3*i*(MapImg->width)+3*j+2]=255;
			}
		}
		break;
	}


	for(size_t i=0;i!=Move_Point.size();i++)//draw line
	{
		if(i+1<Move_Point.size())
		cvLine(MapImg,cvPoint(Move_Point[i].x,599-Move_Point[i].y),cvPoint(Move_Point[i+1].x,599-Move_Point[i+1].y),CV_RGB(0,255,255),2,1,0);
	}
	cvShowImage("image",MapImg);

}
//-------------------------------------------------------------


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 對話方塊資料
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CInterfaceDlg 對話方塊

CInterfaceDlg::CInterfaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInterfaceDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInterfaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_Control(pDX, IDC_LIST3, m_list3);
	DDX_Control(pDX, IDC_LIST2, m_list2);
	DDX_Control(pDX, IDC_EDIT3, m_edit3);
	DDX_Control(pDX, IDC_EDIT4, m_edit4);
	DDX_Control(pDX, IDC_EDIT5, m_edit5);
	DDX_Control(pDX, IDC_EDIT6, m_edit6);
	DDX_Control(pDX, IDC_EDIT7, m_edit7);
}

BEGIN_MESSAGE_MAP(CInterfaceDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CInterfaceDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CInterfaceDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CInterfaceDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CInterfaceDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CInterfaceDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CInterfaceDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CInterfaceDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CInterfaceDlg::OnBnClickedButton8)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON9, &CInterfaceDlg::OnBnClickedButton9)
	ON_LBN_SELCHANGE(IDC_LIST2, &CInterfaceDlg::OnLbnSelchangeList2)
	ON_BN_CLICKED(IDC_BUTTON10, &CInterfaceDlg::OnBnClickedButton10)
	ON_EN_CHANGE(IDC_EDIT1, &CInterfaceDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON11, &CInterfaceDlg::OnBnClickedButton11)
	ON_EN_CHANGE(IDC_EDIT4, &CInterfaceDlg::OnEnChangeEdit4)
	ON_LBN_SELCHANGE(IDC_LIST3, &CInterfaceDlg::OnLbnSelchangeList3)
	ON_EN_CHANGE(IDC_EDIT7, &CInterfaceDlg::OnEnChangeEdit7)
	ON_BN_CLICKED(IDC_BUTTON12, &CInterfaceDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CInterfaceDlg::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CInterfaceDlg::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &CInterfaceDlg::OnBnClickedButton15)
END_MESSAGE_MAP()

// CInterfaceDlg 訊息處理常式
BOOL CInterfaceDlg::OnInitDialog()
{
	AllocConsole();
	
	CDialog::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。
	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業

	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示
	// TODO: 在此加入額外的初始設定
	MapImg=cvLoadImage("MapImg_n3.bmp",1);
	
	cvNamedWindow( "image", 0);
	cvResizeWindow( "image",600,600);
    HWND hWnd = (HWND)cvGetWindowHandle("image");
    HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd,GetDlgItem(IDC_STATIC)->m_hWnd);
    ::ShowWindow(hParent,SW_HIDE);
		
	std::vector<std::pair<int,int>> obstacles;
	ReadObstacle("Obstacle.txt",obstacles);
	for(SIZE_T i=0;i!=obstacles.size();i++){
		int width=Singleton::getInstance("parameters.txt")->parameters["ObstacleWidth"];
		CvPoint UpperLeft=cvPoint(obstacles[i].first-width/2,MapImg->height-obstacles[i].second-width/2);
		CvPoint BottomRight=cvPoint(obstacles[i].first+width/2,MapImg->height-obstacles[i].second+width/2);

		cvRectangle(MapImg,UpperLeft,BottomRight,CV_RGB(255,255,255),1,0,0);
		uchar* data=(uchar*)MapImg->imageData;
		for(int i=UpperLeft.y;i!=BottomRight.y;i++)
		{
			for(int j=UpperLeft.x;j!=BottomRight.x;j++)
			{
				data[3*i*(MapImg->width)+3*j]=255;
				data[3*i*(MapImg->width)+3*j+1]=255;
				data[3*i*(MapImg->width)+3*j+2]=255;
			}
		}
	
	}

	cvShowImage("image",MapImg);
	cvSetMouseCallback("image",onMouse,NULL);
	cvWaitKey(5);
	m_list2.AddString("Find-0");
	m_list2.AddString("Find-1");
	//m_list2.AddString("Find-2");
	/*m_list2.AddString("Judge-1");
	m_list2.AddString("Fixbydoor-1");*/
	m_list2.AddString("Calibration-1");
	m_list2.AddString("Calibration-2");
	m_list2.AddString("Button-1");
	m_list2.AddString("Shot-1");
	//m_list2.AddString("Shot-2");
	//m_list2.AddString("DoorColor-1");

	//--------------------------------------
	m_list1.Dir(0,"Path\\*.txt");
	m_edit1.SetWindowText("MapImg.bmp");
	m_edit2.SetWindowText("0-01.txt");
	int stoparea=Singleton::getInstance("parameters.txt")->parameters["StopDist"];
	char m3string[20];
	sprintf(m3string,"%d",stoparea);
	m_edit3.SetWindowText(m3string);
	//tedit=&m_edit1;

	robot=new Robot("COM6",9600);	//reset pose 250 25 90
	
	if(urgC==0){
	urgC = new UrgControl(string("COM8"), 115200);
	localizer = new Localizer(urgC);
	}
	CreateGridMap(MapImg);//create MapImg to GridMap

	//gt = new GoTarget(robot,localizer);
	ToTarget = new GoTarget(robot,localizer); 
	
	char m4string[30];
	int initial_x=(int)robot->_x;
	int initial_y=(int)robot->_y;
	int initial_th=(int)robot->_th;
	sprintf(m4string,"%d",initial_x);
	m_edit4.SetWindowText(m4string);
	sprintf(m4string,"%d",initial_y);
	m_edit5.SetWindowText(m4string);
	sprintf(m4string,"%d",initial_th);
	m_edit6.SetWindowText(m4string);
	m_edit7.SetWindowText("(x, y)");
	tedit=&m_edit7;

//-----------------usb----------------------------------
	mp_USBControl = new USBControl();
	mp_USBControl->InitialObject();
	mp_USBControl->OpenUSBPort(DEVNUM);
	mp_USBControl->WriteIOValue(DEVNUM,0,0,&dwRet);
//--------------------------------------------------------------------
	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CInterfaceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CInterfaceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

}
// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CInterfaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CInterfaceDlg::OnBnClickedButton1()
{	
	CString filename;
	std::string sstring;
	m_edit1.GetWindowText(filename);
	sstring=filename.GetBuffer(0);

	MapImg=cvLoadImage(sstring.c_str(),1);
	cvShowImage("image",MapImg);
	Move_Point.clear();
	cvSetMouseCallback("image",onMouse,NULL);
}

void CInterfaceDlg::OnBnClickedButton2()
{
	std::fstream out;
	out.clear();
	CString css;
	std::string sstring;
	m_edit2.GetWindowText(css);
	sstring=css.GetBuffer(0);
	sstring="./Path/"+sstring;
	out.open(sstring.c_str(),std::ios::out);
	for(size_t i=0;i!=Move_Point.size();i++)
	{
		out<<i<<" "<<Move_Point[i].x<<" "<<Move_Point[i].y<<std::endl;	
	}
	out.clear();
	out.close();
	m_list1.ResetContent();
	m_list1.Dir(0,"Path\\*.txt");
}

void CInterfaceDlg::OnBnClickedButton3()
{
	CString ss;
	int n=m_list1.GetCurSel();
	if(n!=LB_ERR){
		m_list1.GetText(n,ss);
		//m_list3.AddString(ss);
		m_list3.InsertString(-1,ss);
	}
}

void CInterfaceDlg::OnBnClickedButton4()
{
	CString ss;
	int n=m_list2.GetCurSel();
	if(n!=LB_ERR){
		m_list2.GetText(n,ss);
		m_list3.InsertString(-1,ss);
		
	}
}

void CInterfaceDlg::OnBnClickedButton5()
{
	CString ss;
	int n=m_list3.GetCurSel();
	if(n!=LB_ERR){
	m_list3.DeleteString(n);
	}
}

void start(void* ptr){

	//=============================================================
	localizer->loadGridMap(string("GridMap.txt"),600,600);
	std::vector<std::pair<std::string,int>> fvec;
	if(isdefault==false){
		readfile("Step.txt",fvec);
	}
	else{
		readfile("DefaultStep.txt",fvec);
	}

	 for(int i=0;i!=fvec.size();i++){
		 
		 if(fvec[i].first=="./Path/Find.txt" ){
			 FindBall findball(robot,localizer,Singleton::getInstance("parameters.txt")->parameters["StopDist"]);//total pixel threshold		  			
			switch(fvec[i].second){
				 case 0://Find 0
					 findball.operation0();//turn left
					break;
				 case 1://Find 1
					 findball.operation1();//turn right
					 break;
				 case 2://Find 2 for debug
					 findball.test();
					 break;
				 
			
			 }
		 }
		 /*else if(fvec[i].first=="./Path/Judge.txt"){
			 switch(fvec[i].second){	 
				case 1:
					Judge* pJudge=new Judge(urgC,70);
					if(pJudge->CanGo()){
							int rotate_speed=1200;
							int move_speed=6000;
							int robot_size=40;
							robot->stop();
							robot->setSpeed(rotate_speed);
							robot->turnLeft((pJudge->_degree));
							robot->stop();
							robot->setSpeed(1000);
							robot->moveForward(10);
							robot->stop();
							robot->setSpeed(move_speed);
							robot->moveForward( (int)(pJudge->_distance+robot_size));			
					}
					delete pJudge;
					 break;
			 }
		 }
		 else if(fvec[i].first=="./Path/Fixbydoor.txt"){
				Judge judge(urgC,90);
				judge.HaveDoor();
				int rotate_speed=1000;
				int move_speed=6000;
				int robot_size=40;
				robot->stop();
				robot->setSpeed(rotate_speed);
				robot->turnLeft(judge._degree);
				robot->stop();
				localizer->findPose2(robot->_x,robot->_y,robot->_th, 50,30);
			    robot->setPose(localizer->_pose_x,localizer->_pose_y,localizer->_pose_th);
		 }*/
		 else if(fvec[i].first=="./Path/Calibration.txt"){
			 FindBall findball(robot,localizer,30);//total pixel threshold	
			 switch(fvec[i].second){	 
				case 1:
					findball.calibration();
					 break;
				case 2:
					findball.calibration_Fullcolor();
					break;
			 }
		 }
		 /*else if(fvec[i].first=="./Path/DoorColor.txt"){
			 Door dr;
			 switch(fvec[i].second){	 
				case 1:
					dr.calibration();
					 break;
			 }
		 }
		 else if(fvec[i].first=="./Path/Door_Enter.txt"){
			 GoTarget ToTarget(robot,localizer);
			 ToTarget.Action_Go_Point_Calibrate_JudgeDoor(fvec[i].first,180);
		 }
		  else if(fvec[i].first=="./Path/Door_Leave.txt"){
			 GoTarget ToTarget(robot,localizer);
			 ToTarget.Action_Go_Point_Calibrate_JudgeDoor(fvec[i].first,0);					 
		 }*/
		 else if(fvec[i].first=="./Path/Button.txt"){

			 ToTarget->Push_Buttton(10);

			 /*robot->setSpeed(1000);
			 robot->turnLeft(90);*/
			 /*robot->moveForward(20);
			 robot->stop();
			 robot->moveForward(-30);*/
			 robot->stop();
		 }
		 else if(fvec[i].first=="./Path/Shot.txt"){
			Shot* thief;
			thief=new Shot(robot,localizer,mp_USBControl);
			//thief Shot(robot,localizer);
			int totalCheck = 0;
			while( !thief->isCAUGHT())
			{	_cprintf("take shot \n");
				thief->take_a_shot();
				switch(fvec[i].second)
				{	
					case 1:	 
						thief->check(1);	
						totalCheck++;
						break;
				}
				if(totalCheck == 5 && thief->isCaught == false){
					_cprintf("total check: %d \n",totalCheck);
					thief->just_shot();
					break;
				}
			}
			_cprintf("caught: %d\n",thief->isCAUGHT());
			if( thief->isCAUGHT() )
			{	
				thief->showThief();
			}
		 }
		 else{
			 static bool isFirst=true;
			
			 if(isFirst){
						
				 ToTarget->Action_Go_Point_avoid(fvec[i].first,robot->_th);
				 //gt->Action_Go_Point_avoid(fvec[i].first,fvec[i].second);
				 //delete ToTarget;
				// ToTarget->Action_Go_Point_Calibrate_Avoid(fvec[i].first,fvec[i].second);
				 isFirst=false;  // 保留上次呼叫最後設定的值
			 }
			 else{
				//GoTarget ToTarget(robot,localizer);
				//gt->Action_Go_Point_avoid(fvec[i].first, robot->_th);
				//gt->Action_Go_Point_Calibrate(fvec[i].first,fvec[i].second);
				//ToTarget.Action_Go_Point_Calibrate(fvec[i].first,fvec[i].second);
				//if(ToTarget!=NULL)
				//	delete ToTarget;
				//ToTarget=new GoTarget(robot,localizer);
				//ToTarget->Action_Go_Point_Calibrate_Avoid(fvec[i].first,robot->_th);
				 ToTarget->Action_Go_Point_avoid(fvec[i].first, robot->_th);	
				//delete ToTarget;
			 }
		 } 
	 }
	 
	 _endthread(); 
}

void ShowPose(void* ptr)
{
	
	IplImage *img;
	img = cvCreateImage(cvSize(600, 600), IPL_DEPTH_8U, 3);
	int map_value;
	
	while(true)
	{   
		Sleep(1000);
		
		cvZero(img);
		for(int i=0;i<600;i++)
			for(int j=0;j<600;j++)
			{	
			map_value=localizer->_map_arr[j][599-i];
			img->imageData[i*img->widthStep+j*3]=(unsigned char)map_value;
			img->imageData[i*img->widthStep+j*3+1]=(unsigned char)map_value;
			img->imageData[i*img->widthStep+j*3+2]=(unsigned char)map_value;	
				//if(map_value>=25)
				//{
				//	img->imageData[i*img->widthStep+j*3]=255;
				//	img->imageData[i*img->widthStep+j*3+1]=255;
				//	img->imageData[i*img->widthStep+j*3+2]=255;				
				//}
				//else if(map_value<0)
				//{
				//	img->imageData[i*img->widthStep+j*3]=0;
				//	img->imageData[i*img->widthStep+j*3+1]=0;
				//	img->imageData[i*img->widthStep+j*3+2]=0;				
				//}
				//else
				//{
				//	img->imageData[i*img->widthStep+j*3]=map_value*10;
				//	img->imageData[i*img->widthStep+j*3+1]=map_value*10;
				//	img->imageData[i*img->widthStep+j*3+2]=map_value*10;								
				//}
			}
		
		cvDrawCircle(img, cvPoint((int)robot->_x, 599-(int)robot->_y), 5,  cvScalar(255,255,0), 5);
		//_cprintf("B-robot: (%d, %d) \n",(int)robot->_x, 599-(int)robot->_y);
		//Sleep(200);
		
		if(!ToTarget->tq->isEmpty() ){
			Point2D target=ToTarget->tq->getTarget(0);
			cvDrawCircle(img, cvPoint((int)target._x, 599-(int)target._y), 5,  CV_RGB(255,0,0), 5);
			//cvDrawCircle(MapImg, cvPoint((int)robot->_x, 599-(int)robot->_y), 5,  cvScalar(0,0,0), 5);
		}
		else{
			cvDrawCircle(img, cvPoint((int)robot->_x, 599-(int)robot->_y), 5,  CV_RGB(255,0,0), 5);
			//cvDrawCircle(MapImg, cvPoint((int)robot->_x, 599-(int)robot->_y), 5,  cvScalar(0,0,0), 5);
		}
		

		cvShowImage("image", img);
		if(cvWaitKey(10) >= 0){
			break;
		}

	} //end while

	cvReleaseImage(&img);
	_endthread();
}

void CInterfaceDlg::OnBnClickedButton6()//start 
{
	CString css;
	std::string sstring;
	m_edit4.GetWindowText(css);
	sstring=css.GetBuffer(0);
	robot->_x=atoi(sstring.c_str());
	m_edit5.GetWindowText(css);
	sstring=css.GetBuffer(0);
	robot->_y==atoi(sstring.c_str());
	m_edit6.GetWindowText(css);
	sstring=css.GetBuffer(0);
	robot->_th==atoi(sstring.c_str());
    
	cvSetMouseCallback("image",NULL,NULL);

	//if(urgC==0){
	//urgC = new UrgControl(string("COM2"), 115200);
	//localizer = new Localizer(urgC);
	//}
	//CreateGridMap(MapImg);//create MapImg to GridMap
	//MySaveImage("show.bmp",MapImg);
	
	
	std::ofstream file;
	
//------------------所有步驟存在 Step.txt裡	------------------------------
	file.open("Step.txt",std::ios::out);

	for(int i=0;i!=m_list3.GetCount();i++)
	{
		m_list3.GetText(i,css);
		std::string stemp=css.GetBuffer(0);
		std::string::size_type found = stemp.find_first_of('-',0);
		std::string::size_type founddot = stemp.find_first_of('.',0);
		if(founddot == std::string::npos)
		{
			std::string sfind;
			std::string sfind2;
			sfind=stemp.substr(0,found);
			sfind2=stemp.substr(found+1,1);
			file<<sfind<<" "<<sfind2<<std::endl;
		}
		else{
			std::string spath;
			std::string sdegree;
			if(found == std::string::npos){
				sdegree="90";
				spath=stemp.substr(0,founddot);
			}
			else{
			sdegree=stemp.substr(0,found);
			spath=stemp.substr(0,founddot);
			}
			file<<spath<<" "<<sdegree<<std::endl;
		}		
	}
	file.clear();
	file.close();	 

	_beginthread(start,0,NULL);
	 Sleep(100);
	 _beginthread(ShowPose, 0, NULL);

}

void CInterfaceDlg::OnBnClickedButton7()//stop
{	
		for(int i=0;i<15;i++){
		robot->stop();
		}
}

void CInterfaceDlg::OnBnClickedButton8() //Default start
{
	CString css;
	std::string sstring;
	m_edit4.GetWindowText(css);
	sstring=css.GetBuffer(0);
	robot->_x=atoi(sstring.c_str());
	m_edit5.GetWindowText(css);
	sstring=css.GetBuffer(0);
	robot->_y==atoi(sstring.c_str());
	m_edit6.GetWindowText(css);
	sstring=css.GetBuffer(0);
	robot->_th==atoi(sstring.c_str());
	if(urgC==0){
	urgC = new UrgControl(string("COM8"), 115200);
	localizer = new Localizer(urgC);
	}
	/*m_list3.AddString("90-01.txt");
    m_list3.AddString("Find-0");;
	m_list3.AddString("0-12.txt");
	m_list3.AddString("Find-1");
	m_list3.AddString("270-23.txt");
	m_list3.AddString("Find-0");
	m_list3.AddString("180-34.txt");
	m_list3.AddString("Find-0");
	m_list3.AddString("270-45.txt");
	m_list3.AddString("Find-0");
	m_list3.AddString("90-50.txt");*/
	/*m_list3.AddString("90-01.txt");
    m_list3.AddString("Find-0");;
	m_list3.AddString("0-12.txt");
	m_list3.AddString("Find-1");
	m_list3.AddString("270-23.txt");
	m_list3.AddString("Find-0");
	m_list3.AddString("180-34.txt");
	m_list3.AddString("Door_Enter.txt");
	m_list3.AddString("Find-0");
	m_list3.AddString("Door_Leave.txt");
	m_list3.AddString("270-45.txt");
	m_list3.AddString("Find-0");
	m_list3.AddString("90-50.txt");*/
	
	CreateGridMap(MapImg);
	MySaveImage("show.bmp",MapImg);
	isdefault=true;
	/*std::ofstream file;
	file.open("DefultStep.txt",std::ios::out);
	for(int i=0;i!=m_list3.GetCount();i++)
	{
		m_list3.GetText(i,css);
		std::string stemp=css.GetBuffer(0);
		std::string::size_type found = stemp.find_first_of('-',0);
		std::string::size_type founddot = stemp.find_first_of('.',0);
		if(founddot == std::string::npos)
		{
			std::string sfind;
			std::string sfind2;
			sfind=stemp.substr(0,found);
			sfind2=stemp.substr(found+1,1);
			file<<sfind<<" "<<sfind2<<std::endl;
		}
		else{
			std::string spath;
			std::string sdegree;
			sdegree=stemp.substr(0,found);
			spath=stemp.substr(0,founddot);
			file<<spath<<" "<<sdegree<<std::endl;
		}		
	}
	file.clear();
	file.close();*/	 
	 _beginthread(start,0,NULL);
	 Sleep(1000);
	 _beginthread(ShowPose, 0, NULL);
	 //m_list3.ResetContent();
}

void CInterfaceDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	//char str[255];
	//sprintf(str,"(%d,%d)",global_x,global_y);
	//m_edit1.SetWindowTextA(str);
	CDialog::OnMouseMove(nFlags, point);
}

void CInterfaceDlg::OnBnClickedButton9()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	robot->stop();
	robot->setSpeed(1500);
	robot->moveForward(30);
}

void CInterfaceDlg::OnLbnSelchangeList2()
{
	// TODO: 在此加入控制項告知處理常式程式碼
}

void CInterfaceDlg::OnBnClickedButton10()
{
	CString css;
	std::string sstring;
	m_edit3.GetWindowText(css);
	sstring=css.GetBuffer(0);
	Singleton::modify("StopDist",atoi(sstring.c_str()));
}

void CInterfaceDlg::OnEnChangeEdit1()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 CDialog::OnInitDialog()
	// 函式和呼叫 CRichEditCtrl().SetEventMask()
	// 讓具有 ENM_CHANGE 旗標 ORed 加入遮罩。

	// TODO:  在此加入控制項告知處理常式程式碼
}

void CInterfaceDlg::OnBnClickedButton11()
{
	robot->stop();
	robot->setSpeed(1000);
	robot->turnLeft(90);
	// TODO: 在此加入控制項告知處理常式程式碼
}

void CInterfaceDlg::OnEnChangeEdit4()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 CDialog::OnInitDialog()
	// 函式和呼叫 CRichEditCtrl().SetEventMask()
	// 讓具有 ENM_CHANGE 旗標 ORed 加入遮罩。

	// TODO:  在此加入控制項告知處理常式程式碼
}

void CInterfaceDlg::OnLbnSelchangeList3()
{
	// TODO: 在此加入控制項告知處理常式程式碼
}

void CInterfaceDlg::OnEnChangeEdit7()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 CDialog::OnInitDialog()
	// 函式和呼叫 CRichEditCtrl().SetEventMask()
	// 讓具有 ENM_CHANGE 旗標 ORed 加入遮罩。

	// TODO:  在此加入控制項告知處理常式程式碼
}

void CInterfaceDlg::OnBnClickedButton12()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	robot->stop();
	robot->setSpeed(1500);
	robot->moveForward(-30);
}

void CInterfaceDlg::OnBnClickedButton13()
{
	// TODO: 在此加入控制項告知處理常式程式碼
			CString css1,css2,css3;
	std::string sstring1,sstring2,sstring3;
	
	m_edit4.GetWindowText(css1);
	sstring1=css1.GetBuffer(0);
	//robot->_x=atoi(sstring1.c_str());
	
	m_edit5.GetWindowText(css2);
	sstring2=css2.GetBuffer(0);
	robot->_y==atoi(sstring2.c_str());
	m_edit6.GetWindowText(css3);
	sstring3=css3.GetBuffer(0);
	//robot->_th==atoi(sstring3.c_str());
	robot->setPose(atoi(sstring1.c_str()),atoi(sstring2.c_str()),atoi(sstring3.c_str()));
	_cprintf("x=%lf\n",robot->_x);	
	_cprintf("y=%lf\n",robot->_y);
	_cprintf("th=%lf\n",robot->_th);
	
}

void CInterfaceDlg::OnBnClickedButton14()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	mp_USBControl->WriteIOValue(DEVNUM,128,0,&dwRet);

}

void CInterfaceDlg::OnBnClickedButton15()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	 mp_USBControl->WriteIOValue(DEVNUM,0,0,&dwRet);
}
