#pragma once

typedef DWORD (WINAPIV *PFNOPENUSB)			(BYTE) ;
typedef DWORD (WINAPIV *PFNCLOSEDEVICE)		(BYTE) ;

typedef DWORD (WINAPIV *PFNWRITEIODATA)		(BYTE,PBYTE,PDWORD) ;
typedef DWORD (WINAPIV *PFNWRITEIODATA2)	(BYTE,BYTE,BYTE,PDWORD) ;

typedef DWORD (WINAPIV *PFNREADIODATA)		(BYTE,PBYTE,DWORD*) ;
typedef DWORD (WINAPIV *PFNREADIODATA2)		(BYTE,PBYTE,PBYTE,PDWORD) ;

typedef DWORD (WINAPIV *PFNGETSERIALNUM)	(BYTE,PBYTE,DWORD,PDWORD);
typedef DWORD (WINAPIV *PFNMODVERSION)		(CHAR *) ;

// Device Number
#define DEVNUM		0

class CIO51Dlg;
class USBControl
{
public:
	USBControl(void);
public:
	~USBControl(void);

	void InitialObject();
	void ReleaseObject();
	// USB Function 
	DWORD OpenUSBPort		(BYTE bDevNum);
	void  CloseUSBPort		(BYTE bDevNum);

	DWORD WriteIOValue		(BYTE bDevNum,LPBYTE lpSrc,DWORD *dwRetSize);
	DWORD WriteIOValue		(BYTE bDevNum,BYTE Port1,BYTE Port2,DWORD *dwRetSize);

	DWORD ReadIOValue		(BYTE bDevNum,LPBYTE lpDst,DWORD *dwRetSize);
	DWORD ReadIOValue		(BYTE bDevNum,PBYTE Port1,PBYTE Port2,DWORD *dwRetSize);

	DWORD GetFirmwareVer	(BYTE bDevNum,LPBYTE lpDst,DWORD Size,DWORD *dwRetSize);
	DWORD GetDllVersion		(CHAR *pVersion);

private:
	BOOL LoadUSBModule();

// property
public:

protected:

private:
	PFNOPENUSB		pfnOpenUSB;
	PFNWRITEIODATA	pfnWriteIO;
	PFNWRITEIODATA2	pfnWriteIOValue2;

	PFNREADIODATA	pfnReadIO;
	PFNREADIODATA2	pfnReadIOValue2;

	PFNGETSERIALNUM	pfnReadSN;
	PFNCLOSEDEVICE	pfnCloseUSB;
	PFNMODVERSION	pfnGetDllVersion;

	HINSTANCE		hDll;
	DWORD			dwErrCode;
	DWORD			dwRetSize;
};
