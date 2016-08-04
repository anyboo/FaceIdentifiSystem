#pragma once
#include <map>

typedef struct
{
	std::string strName;
	std::string strAge;
	std::string strSex;
	std::string strBirth;
	std::string strIDcard;
	std::string strPhone;
	std::string strCertID;
	std::string strPhotoInfo;
}IdentityInfo, *pIdentityInfo;


typedef std::map<int, IdentityInfo*>  PersonInfo;

class CRegisterInfo
{
public:
	CRegisterInfo();
	~CRegisterInfo();

	void Add(int RegID, IdentityInfo* saveInfo);
	void Remove();
	int GetSize();
	IdentityInfo* Find(int RegID);

public:
	PersonInfo  InfoItem;
};
