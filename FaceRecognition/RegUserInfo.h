#pragma once
#include "QMFileSqlite.h"
class RegUserInfo
{
public:
	RegUserInfo();
	~RegUserInfo();

	static vector<readUserInfo> _userinfo;

	static vector<readUserInfo> getUserInfo();

	static void addUserInfo(writeUserInfo userinfo);

private:
	static void init();

};

