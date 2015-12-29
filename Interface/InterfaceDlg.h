// InterfaceDlg.h : 標頭檔
//

#pragma once
#include "Resource.h"
#include "USBControl.h"

#pragma comment(lib,"cxcore210.lib")
#pragma comment(lib,"ml210.lib")
#pragma comment(lib,"cv210.lib")
#pragma comment(lib,"cvaux210.lib")
#pragma comment(lib,"highgui210.lib")

#include"cv.h"
#include"highgui.h"
#include "ml.h"


#include <vector>
#include <string>
#include <fstream>
#include "afxwin.h"

#include "DetectTarget.h"
#include "Judge.h"
#include "Robot.hpp"
#include "Localizer.hpp"
#include "GoTarget.h"
#include "FindBall.h"
#include "LaserDetectDoor.h"
#include "Shot.h"
#include <process.h>
#include <conio.h>

// CInterfaceDlg 對話方塊
class CInterfaceDlg : public CDialog
{
// 建構
public:
	CInterfaceDlg(CWnd* pParent = NULL);	// 標準建構函式

// 對話方塊資料
	enum { IDD = IDD_INTERFACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援
public:
		DWORD			dwRet;

// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit1;
	afx_msg void OnBnClickedButton1();
	CEdit m_edit2;
	afx_msg void OnBnClickedButton2();
	CListBox m_list1;
	CListBox m_list3;
	CListBox m_list2;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton9();
	afx_msg void OnLbnSelchangeList2();
	CEdit m_edit3;
	afx_msg void OnBnClickedButton10();
	CEdit m_edit4;
	CEdit m_edit5;
	afx_msg void OnEnChangeEdit1();
	CEdit m_edit6;
	afx_msg void OnBnClickedButton11();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnLbnSelchangeList3();
	CEdit m_edit7;
	afx_msg void OnEnChangeEdit7();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();
};
