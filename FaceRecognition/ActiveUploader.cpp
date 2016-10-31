#include "stdafx.h"
#include "ActiveUploader.h"
#include <Poco/Net/FTPClientSession.h>
#include <Poco/StreamCopier.h>
#include <Poco/FileStream.h>
#include <Poco/Path.h>
#include <sstream>
#include <Poco/Logger.h>

using Poco::Net::FTPClientSession;
using Poco::StreamCopier;
using Poco::FileInputStream;
using Poco::Path;

ActiveUploader::ActiveUploader(FTPClientSession& session):
upload(this, &ActiveUploader::uploadImp),
_session(session)
{
}


ActiveUploader::~ActiveUploader()
{
}

bool ActiveUploader::uploadImp(const std::string& file)
{
	_session.setWorkingDirectory("/var/www/sisec_website/photos");
	DUITRACE(_session.getWorkingDirectory().c_str());
	Path p(file);
	DUITRACE("upload file : %s -> Remote WorkingDirectory %s", p.getFileName().c_str(), _session.getWorkingDirectory().c_str());
	poco_information_f2(Poco::Logger::get("FileLogger"), "upload file : %s -> Remote WorkingDirectory %s", 
		p.getFileName().c_str(), _session.getWorkingDirectory().c_str());

	FileInputStream fis(file);
	std::ostream& ostr = _session.beginUpload(p.getFileName());
	StreamCopier::copyStream(fis, ostr);
	_session.endUpload();

	return true;
}