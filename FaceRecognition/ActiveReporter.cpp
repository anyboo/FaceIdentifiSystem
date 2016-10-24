#include "stdafx.h"
#include "ActiveReporter.h"
#include <Poco/StreamCopier.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <sstream>

using Poco::StreamCopier;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;

ActiveReporter::ActiveReporter(Poco::Net::HTTPClientSession& session) :
report(this, &ActiveReporter::reportImp),
_session(session)
{
	//_session.setKeepAlive(true);
}


ActiveReporter::~ActiveReporter()
{
}

std::string ActiveReporter::reportImp(const std::string& json)
{
	HTTPRequest request(HTTPRequest::HTTP_POST, "/api/index.php?s=/Services/index");
	request.setVersion(HTTPRequest::HTTP_1_1);
	request.setContentType("application/x-www-form-urlencoded");
	request.setChunkedTransferEncoding(true);
	request.add("Cache-Control", "no-cache");
	request.add("Connection", "keep-alive");
	request.add("Accept-Charset", "utf8");
	request.add("Accept", "text/json");
	request.add("User-Agent", "monitor/1.0.0/1/window");

	std::string body("body="+json);
	request.setContentLength((int)body.length());
	DUITRACE("http data : %s \n", body.c_str());
	std::ostream& os = _session.sendRequest(request);
	os << body;
	os.flush();

	Poco::Thread::sleep(2000);

	HTTPResponse response;
	std::istream& rs = _session.receiveResponse(response);
	DUITRACE("HTTPResponse status : %d", response.getStatus());
	if (response.getStatus() == HTTPResponse::HTTP_OK)
	{
		std::ostringstream ostr;
		StreamCopier::copyStream(rs, ostr);
		std::string message(ostr.str());
		DUITRACE("HTTPResponse:\n %s", message.c_str());
		return message;
	}

	return "";
}