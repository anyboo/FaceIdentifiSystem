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

//#include "log.h"
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

	std::string GetSimilarity() const
	{	
		return strSiml;
	}

	std::string GetFaceSize() const
	{
		return strFaceSzie;
	}

	std::string GetTime_interval() const
	{
		return strTimeD;
	}

	void SaveConfig(const std::string strTimeD, const std::string strSiml, const std::string strFaceSize)
	{
		std::string configFile;
		TCHAR PATH[MAX_PATH] = { 0 };
		std::string AppPath = std::string(PATH, ::GetModuleFileNameA(NULL, PATH, MAX_PATH));
		configFile = AppPath.substr(0, AppPath.find_last_of("\\") + 1) + std::string(_T("Setting.json"));

		ofstream ofs(configFile);
		locale utf8;
		ofs.imbue(utf8);
		OStreamWrapper osw(ofs);
		Document d;

		Document::AllocatorType& alloc = d.GetAllocator();
		Value root(kObjectType);
		Value t(kObjectType), s(kObjectType), f(kObjectType);
		t.SetString(strTimeD.c_str(), strTimeD.length(), alloc);
		s.SetString(strSiml.c_str(), strSiml.length(), alloc);
		f.SetString(strFaceSize.c_str(), strFaceSize.length(), alloc);

		root.AddMember(_T("time_interval"), t, alloc);
		root.AddMember(_T("similarity"), s, alloc);
		root.AddMember(_T("MinFaceSize"), f, alloc);

		Writer<OStreamWrapper> writer(osw);
		root.Accept(writer);
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

		strSiml = d[_T("similarity")].GetString();
		strTimeD = d[_T("time_interval")].GetString();
		strFaceSzie = d[_T("MinFaceSize")].GetString();
	}

private:
	std::string strSiml;
	std::string strTimeD;
	std::string	strFaceSzie;
};




