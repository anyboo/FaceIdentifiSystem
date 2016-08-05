#include "stdafx.h"
#include "RegUserInfo.h"

vector<readUserInfo> RegUserInfo::_userinfo;
RegUserInfo::RegUserInfo()
{
}


RegUserInfo::~RegUserInfo()
{
}

vector<readUserInfo> RegUserInfo::getUserInfo()
{
	/*if (_userinfo.size() == 0)
		init();*/
	return _userinfo;
}

void RegUserInfo::addUserInfo(writeUserInfo userinfo)
{
	QFileSqlite* pDb = QFileSqlite::getInstance();
	pDb->writeData(INSERT_USET_INFO, userinfo);
	init();
}

void RegUserInfo::init()
{
	_userinfo.clear();

	QFileSqlite* pDb = QFileSqlite::getInstance();
	pDb->GetData(SELECT_ALL_USER_INFO, _userinfo);
}
