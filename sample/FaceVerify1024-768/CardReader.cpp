#include "stdafx.h"
#include "CardReader.h"


typedef int (__stdcall *FInitComm ) (int iPort);
typedef int (__stdcall *FCloseComm ) ();
typedef int (__stdcall *FAuthenticate ) ();
typedef int (__stdcall *FRead_Content ) (int iActive);

typedef int (__stdcall *FGetPeopleName ) (char* pData, UINT iDataLen);
typedef int (__stdcall *FGetPeopleSex ) (char* pData, UINT iDataLen);
typedef int (__stdcall *FGetPeopleNation ) (char* pData, UINT iDataLen);
typedef int (__stdcall *FGetPeopleBirthday ) (char* pData, UINT iDataLen);
typedef int (__stdcall *FGetPeopleAddress ) (char* pData, UINT iDataLen);
typedef int (__stdcall *FGetPeopleIDCode ) (char* pData, UINT iDataLen);
typedef int (__stdcall *FGetStartDate ) (char* pData, UINT iDataLen);
typedef int (__stdcall *FGetEndDate ) (char* pData, UINT iDataLen);
typedef int (__stdcall *FGetDepartment ) (char* pData, UINT iDataLen);
//typedef int (__stdcall *FGetEndDate ) (char* pData, UINT iDataLen);
typedef int (__stdcall *FGetPhotoBMP ) (char* pData, UINT iDataLen);
//typedef int (__stdcall *FGetAppAddress ) (UINT index, char* pData, UINT iDataLen);


FInitComm			InitComm;
FCloseComm			CloseComm;
FAuthenticate		Authenticate;
FRead_Content		Read_Content;
FGetPeopleName      GetPeopleName;   
FGetPeopleSex       GetPeopleSex;     
FGetPeopleNation    GetPeopleNation;  
FGetPeopleBirthday  GetPeopleBirthday;
FGetPeopleAddress   GetPeopleAddress; 
FGetPeopleIDCode    GetPeopleIDCode;  
FGetStartDate       GetStartDate;     
FGetEndDate         GetEndDate;   
FGetDepartment		GetDepartment;
FGetPhotoBMP		GetPhotoBMP;

CCardReader::CCardReader(void)
{
	m_hDllMod=NULL;
}

CCardReader::~CCardReader(void)
{
	m_hDllMod=NULL;
}
BOOL CCardReader::InitModal()
{
		
	m_hDllMod = LoadLibrary(g_strAppPath+"\\termb.dll");
	if (m_hDllMod == NULL) 
	{
		::MessageBox(NULL, "LoadLibrary(termb.dll) failed!", NULL, MB_OK);
		return  FALSE;	
	}	

	HMODULE hMod=m_hDllMod;

	InitComm			= ( FInitComm )			   GetProcAddress(hMod, "CVR_InitComm");
	CloseComm			= ( FCloseComm )		   GetProcAddress(hMod, "CVR_CloseComm");
	Authenticate		= ( FAuthenticate )		   GetProcAddress(hMod, "CVR_Authenticate");
	Read_Content		= ( FRead_Content )		   GetProcAddress(hMod, "CVR_Read_Content");


	GetPeopleName    	= ( FGetPeopleName    )    GetProcAddress(hMod, "GetPeopleName");  
	GetPeopleSex     	= ( FGetPeopleSex     )    GetProcAddress(hMod, "GetPeopleSex"); 
	GetPeopleNation  	= ( FGetPeopleNation  )    GetProcAddress(hMod, "GetPeopleNation"); 
	GetPeopleBirthday	= ( FGetPeopleBirthday)    GetProcAddress(hMod, "GetPeopleBirthday"); 
	GetPeopleAddress 	= ( FGetPeopleAddress )    GetProcAddress(hMod, "GetPeopleAddress"); 
	GetPeopleIDCode  	= ( FGetPeopleIDCode  )    GetProcAddress(hMod, "GetPeopleIDCode"); 
	GetStartDate     	= ( FGetStartDate     )    GetProcAddress(hMod, "GetStartDate"); 
	GetEndDate       	= ( FGetEndDate       )    GetProcAddress(hMod, "GetEndDate"); 
	GetDepartment       = ( FGetDepartment    )    GetProcAddress(hMod, "GetDepartment"); 
	GetPhotoBMP       	= ( FGetPhotoBMP       )    GetProcAddress(hMod, "GetPhotoBMP"); 

	if (InitComm == NULL || CloseComm == NULL ||
		Authenticate == NULL || Read_Content == NULL || 
		GetPeopleName == NULL || GetPeopleSex == NULL || 
		GetPeopleNation == NULL || GetPeopleBirthday == NULL || 
		GetPeopleAddress == NULL || GetPeopleIDCode == NULL || 
		GetStartDate == NULL || GetEndDate == NULL || GetDepartment==NULL ||GetPhotoBMP==NULL
		)
	{
		return FALSE;
	}

	return TRUE;
}
void CCardReader::UnInitModal()
{
	FreeLibrary(m_hDllMod);
}

BOOL CCardReader::OpenDevice(int nPort)
{
	if (InitComm(nPort) != 1) 
	{
		int nPortAuto=-1;
		BOOL bOpen=OpenDeviceAuto(nPortAuto);
		if(bOpen)
		{
			char szSettingFile[256];
			strcpy(szSettingFile,g_strAppPath);
			strcat(szSettingFile,"\\config.ini");

			char t[16]="";
			sprintf(t,"%d",nPortAuto);
			WritePrivateProfileString("cardreader","port",t,szSettingFile);

		}
	}
	else
		return TRUE;
}
BOOL CCardReader::OpenDeviceAuto(int& nPort)
{
	int nComID=1;
	int nUsbID=1001;
	int i;

	for(i=nComID;i<nComID+16;i++)
	{
		if(InitComm(i)==1)
		{
			nPort=i;
			return TRUE;
		}
	}

	for(i=nUsbID;i<nUsbID+16;i++)
	{
		if(InitComm(i)==1)
		{
			nPort=i;
			return TRUE;
		}
	}


	return FALSE;
}
void CCardReader::CloseDevice()
{
	CloseComm();
}
BOOL CCardReader::ValidateCard()
{
	if(Authenticate()==1) return TRUE;
	else return FALSE;
}
int	CCardReader::ReadCard(IDInfo& idi,BYTE* pPhotoBuf,int&  nPhotoSize)
{
	if (1 != Read_Content(2)) 
	{
		return -1;
	}	

	char szData[70]={0};

	memset(szData, 0, sizeof(szData));
	GetPeopleName(szData, sizeof(szData));
	idi.strName=szData;
	
	memset(szData, 0, sizeof(szData));
	GetPeopleSex(szData, sizeof(szData));
	idi.strSex=szData;


	memset(szData, 0, sizeof(szData));
	GetPeopleNation(szData, sizeof(szData));
	idi.strNation=szData;
	
	memset(szData, 0, sizeof(szData));
	GetPeopleBirthday(szData, sizeof(szData));
	idi.strBirthday=szData;
	
	memset(szData, 0, sizeof(szData));
	GetPeopleAddress(szData, sizeof(szData));
	idi.strAddress=szData;

	memset(szData, 0, sizeof(szData));
	GetPeopleIDCode(szData, sizeof(szData));
	idi.strID=szData;

	memset(szData, 0, sizeof(szData));
	GetStartDate(szData, sizeof(szData));
	idi.strStartDate=szData;


	memset(szData, 0, sizeof(szData));
	GetEndDate(szData, sizeof(szData));
	idi.strEndDate=szData;

	memset(szData, 0, sizeof(szData));
	GetDepartment(szData, sizeof(szData));
	idi.strDepartment=szData;

	if(pPhotoBuf)
	{
		int nLen=GetPhotoBMP((char*)pPhotoBuf,nPhotoSize);
		nPhotoSize=nLen;
	}

	return 0;
}

