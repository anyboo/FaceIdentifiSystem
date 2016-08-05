#include "stdafx.h"
#include "log.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/AutoPtr.h"
#include "Poco/Path.h"

//using Poco::Logger;
//using Poco::FileChannel;
//using Poco::AutoPtr;
//using Poco::PatternFormatter;
//using Poco::FormattingChannel;


namespace Loggering
{

	void Logger_initiation()
	{
		Poco::AutoPtr<Poco::FileChannel> file_channel(new Poco::FileChannel());
		file_channel->setProperty("rotation", "10M");
		file_channel->setProperty("archive", "timestamp");
		std::string strpath = Poco::Path::current();
		std::wstring dump_path = L"log\\";
		::CreateDirectoryW(dump_path.c_str(), NULL);
		strpath.append("log\\facerecognition.log");
		file_channel->setProperty("path", strpath);		
		Poco::AutoPtr<Poco::PatternFormatter> pattern_formatter(new Poco::PatternFormatter("%Y-%m-%d %L%H:%M:%S %t"));
		Poco::AutoPtr<Poco::FormattingChannel> formatter_channle(new Poco::FormattingChannel(pattern_formatter, file_channel));
		Poco::Logger::root().setChannel(formatter_channle);
		///- finish logger initiation
	}

	void Setup_logger()
	{
		static bool b_setup = false;     // only allow run once time 
		if (!b_setup)
		{
			b_setup = true;
			Logger_initiation();
		}
	}

}