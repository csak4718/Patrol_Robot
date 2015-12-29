#include "StdAfx.h"
#include "USBControl.h"

USBControl::USBControl(void)
{
}

USBControl::~USBControl(void)
{
}

void USBControl::InitialObject()
{
	pfnOpenUSB		=NULL;
	pfnWriteIO		=NULL;
	pfnReadIO		=NULL;
	pfnReadSN		=NULL;
	pfnCloseUSB		=NULL;
	hDll			=NULL;
	pfnGetDllVersion=NULL;

	LoadUSBModule();

}

void USBControl::ReleaseObject()
{
	if(hDll != NULL)
	{
		FreeLibrary(hDll);
		hDll=NULL;
	}
}

BOOL USBControl::LoadUSBModule()
{
	hDll = LoadLibrary(_T("BinSource\\USBDIOV2A.dll"));
	if(hDll ==NULL)
	{
		AfxMessageBox(_T("LOAD USBDIOV2.dll FAIL"),MB_ICONHAND);
		return FALSE;
	}

	pfnOpenUSB = (PFNOPENUSB)GetProcAddress(hDll,"MCDIOCreateTask");
	if(pfnOpenUSB ==NULL)
		goto return_fail;

	pfnCloseUSB = (PFNCLOSEDEVICE)GetProcAddress(hDll,"MCDIOClearTask");
	if(pfnCloseUSB ==NULL)
		goto return_fail;

	pfnWriteIO = (PFNWRITEIODATA)GetProcAddress(hDll,"MCDIOWriteValue");
	if(pfnWriteIO ==NULL)
		goto return_fail;
	pfnWriteIOValue2 = (PFNWRITEIODATA2)GetProcAddress(hDll,"MCDIOWriteValue2");
	if(pfnWriteIOValue2 ==NULL)
		goto return_fail;

	pfnReadIO = (PFNREADIODATA)GetProcAddress(hDll,"MCDIOReadValue");
	if(pfnReadIO ==NULL)
		goto return_fail;
	pfnReadIOValue2 = (PFNREADIODATA2)GetProcAddress(hDll,"MCDIOReadValue2");
	if(pfnReadIOValue2 ==NULL)
		goto return_fail;

	pfnReadSN = (PFNGETSERIALNUM)GetProcAddress(hDll,"MCDIOGetFWVersion");
	if(pfnReadSN ==NULL)
		goto return_fail;


	pfnGetDllVersion = (PFNMODVERSION)GetProcAddress(hDll,"MCDIOGetDLLVersion");
	if(pfnGetDllVersion ==NULL)
		goto return_fail;

	return TRUE;

return_fail:
	AfxMessageBox(_T("Get USBIODLL.dll ProcAddress error"),MB_ICONHAND);
	FreeLibrary(hDll);
	return FALSE;
}

DWORD USBControl::OpenUSBPort(BYTE bDevNum)
{
	if((dwErrCode = pfnOpenUSB(bDevNum)) != ERROR_SUCCESS)
	{
		return dwErrCode;
	}
	return dwErrCode;
}

DWORD USBControl::WriteIOValue(BYTE bDevNum,LPBYTE lpSrc,DWORD *dwRetSize)
{
	if((dwErrCode = pfnWriteIO(bDevNum,lpSrc,dwRetSize)) != ERROR_SUCCESS)
	{
		return dwErrCode;
	}
	return dwErrCode;
}
DWORD USBControl::WriteIOValue(BYTE bDevNum,BYTE Port1,BYTE Port2,DWORD *dwRetSize)
{
	if((dwErrCode = pfnWriteIOValue2(bDevNum,Port1,Port2,dwRetSize)) != ERROR_SUCCESS)
	{
		return dwErrCode;
	}
	return dwErrCode;
}

DWORD USBControl::ReadIOValue(BYTE bDevNum,LPBYTE lpDst,DWORD *dwRetSize)
{
	if((dwErrCode = pfnReadIO(bDevNum,lpDst,dwRetSize)) != ERROR_SUCCESS)
	{
		return dwErrCode;
	}
	return dwErrCode;
}
DWORD USBControl::ReadIOValue(BYTE bDevNum,PBYTE Port1,PBYTE Port2,DWORD *dwRetSize)
{
	if((dwErrCode = pfnReadIOValue2(bDevNum,Port1,Port2,dwRetSize)) != ERROR_SUCCESS)
	{
		return dwErrCode;
	}
	return dwErrCode;
}

DWORD USBControl::GetFirmwareVer(BYTE bDevNum,LPBYTE lpDst,DWORD Size,DWORD *dwRetSize)
{
	if((dwErrCode = pfnReadSN(bDevNum,lpDst,Size,dwRetSize)) != ERROR_SUCCESS)
	{
		return dwErrCode;
	}
	return dwErrCode;
}

void  USBControl::CloseUSBPort(BYTE bDevNum)
{
	pfnCloseUSB(bDevNum);
}

DWORD  USBControl::GetDllVersion(CHAR *pVersion)
{
	if((dwErrCode = pfnGetDllVersion(pVersion)) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	return TRUE;
}
