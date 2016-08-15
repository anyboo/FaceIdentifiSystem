#pragma once

#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"

#include "ostreamwrapper.h"
#include "istreamwrapper.h"

#include <string>
#include <tchar.h>
#include <windows.h>
#include <fstream>

#include "log.h"
using namespace rapidjson;

class LangueConfig
{
public:
	LangueConfig(){};
	~LangueConfig(){};

	static std::string GetShowText(int Serial)
	{
		std::string configFile;
		TCHAR PATH[MAX_PATH] = { 0 };
		std::string AppPath = std::string(PATH, ::GetModuleFileNameA(NULL, PATH, MAX_PATH));
		configFile = AppPath.substr(0, AppPath.find_last_of("\\") + 1) + std::string(_T("Config.json"));

		ifstream ifs(configFile);
		locale utf8;
		ifs.imbue(utf8);
		IStreamWrapper isw(ifs);
		Document d;
		d.ParseStream(isw);
		size_t file_size = isw.Tell();
		if (isw.Tell() == 0)
			return _T("");

		std::string TypeName = to_string(Serial);
		std::string ShowText = d[TypeName.c_str()].GetString();

		return ShowText;
	}
};


class ValueSetting
{
public:
	ValueSetting()
	{
		readJsonFile();
	};
	~ValueSetting(){};

public:

	float SetSimilarity() const
	{	
		return Simlarity;
	}

	int SetFaceSize() const
	{
		return FaceSize;
	}

	int SetTime_interval() const
	{
		return Time_interval;
	}
private:
	void readJsonFile()
	{
		std::string configFile;
		TCHAR PATH[MAX_PATH] = { 0 };
		std::string AppPath = std::string(PATH, ::GetModuleFileNameA(NULL, PATH, MAX_PATH));
		configFile = AppPath.substr(0, AppPath.find_last_of("\\") + 1) + std::string(_T("Setting.json"));

		ifstream ifs(configFile);
		locale utf8;
		ifs.imbue(utf8);
		IStreamWrapper isw(ifs);
		Document d;
		d.ParseStream(isw);
		size_t file_size = isw.Tell();
		if (isw.Tell() == 0)
			return;

		std::string strSiml = d[_T("similarity")].GetString();
		std::string strTimeD = d[_T("time_interval")].GetString();
		std::string strFaceSzie = d[_T("MinFaceSize")].GetString();
		Simlarity = stof(strSiml) / 100.00000;
		Time_interval = stoi(strTimeD);
		FaceSize = stoi(strFaceSzie);
	}

private:
	float Simlarity;
	int  Time_interval;
	int	FaceSize;
};




