#pragma once
#include "QMFileSqlite.h"
class RegUserInfo
{
public:
	RegUserInfo();
	~RegUserInfo();	

	static vector<readUserInfo> getUserInfo();

	static void addUserInfo(writeUserInfo userinfo);

	static void init();

private:
	
	static vector<readUserInfo> _userinfo;
};

