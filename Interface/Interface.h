// Interface.h : PROJECT_NAME ���ε{�����D�n���Y��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"		// �D�n�Ÿ�


// CInterfaceApp:
// �аѾ\��@�����O�� Interface.cpp
//

class CInterfaceApp : public CWinApp
{
public:
	CInterfaceApp();

// �мg
	public:
	virtual BOOL InitInstance();

// �{���X��@

	DECLARE_MESSAGE_MAP()
};

extern CInterfaceApp theApp;