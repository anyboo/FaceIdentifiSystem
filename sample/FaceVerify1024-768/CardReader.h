#pragma once

class IDInfo
{
public:
	CString strName;
	CString strSex;
	CString strNation;
	CString strBirthday;
	CString strAddress;
	CString strID;
	CString strStartDate;
	CString strEndDate;
	CString strDepartment;
};
class CCardReader
{
public:
	CCardReader(void);
	~CCardReader(void);
public:
	BOOL InitModal();
	void UnInitModal();
public:
	BOOL OpenDevice(int nPort);
	BOOL OpenDeviceAuto(int &nPort);
	void CloseDevice();
public:
	BOOL ValidateCard();
	int	ReadCard(IDInfo& idi,BYTE* pPhotoBuf,int&  nPhotoSize);
private:
	HMODULE m_hDllMod;
};
