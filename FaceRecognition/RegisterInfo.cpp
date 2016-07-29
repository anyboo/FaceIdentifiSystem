#include "stdafx.h"
#include "RegisterInfo.h"


CRegisterInfo::CRegisterInfo()
{
}


CRegisterInfo::~CRegisterInfo()
{
}

void CRegisterInfo::Add(int RegID, IdentityInfo* saveInfo)
{
	InfoItem.insert(std::pair<int, IdentityInfo*>(RegID, saveInfo));
}

void CRegisterInfo::Remove()
{

}

int CRegisterInfo::GetSize()
{
	return InfoItem.size();
}

IdentityInfo* CRegisterInfo::Find(int RegID)
{
	return InfoItem[RegID];
}
