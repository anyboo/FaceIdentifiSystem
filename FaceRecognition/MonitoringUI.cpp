#include "stdafx.h"
#include "MonitoringUI.h"

#include "Poco/Delegate.h"
#include "Picture.h"
#include "BitMapCompare.h"

#include "Util.h"

#include "Mmsystem.h"
#include "CaptureNotification.h"
#include "WaittingUI.h"
#include "SettingConfig.h"

#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"

using Poco::Data::Session;
using Poco::Data::Statement;
using namespace Poco::Data::Keywords;

CMonitoringUI::CMonitoringUI()
:r(new Camera), t(100, 1000), tc(*this, &CMonitoringUI::onTimer)
{
	m_closeApp = true;
	m_bSendMsg = false;
	ValueSetting vSet;
	m_timeInterval = std::stoi(vSet.GetTime_interval()) * 1000 - 15000;

	t.start(tc);
}


CMonitoringUI::~CMonitoringUI()
{
	t.stop();
}

DUI_BEGIN_MESSAGE_MAP(CMonitoringUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSE_MonWnd, OnCloseWnd)
DUI_END_MESSAGE_MAP()

void CMonitoringUI::onTimer(Poco::Timer& timer)
{
	DUITRACE("CMonitoringUI::onTimer");
	if (is_need_report())
	{
		report_to_server();
	}
}

bool CMonitoringUI::is_need_report()
{
	struct notify_main
	{
		int id;
		int opcode;
		std::string comment;
		int status;
	};

	try
	{
		Session session("SQLite", "D:\\workstation\\code\\GitHub\\FaceIdentifiSystem\\bin\\facerecog.db");
		notify_main notify;
		session << "SELECT id, opcode, comment, status FROM notify_main",
			into(notify.id),
			into(notify.opcode),
			into(notify.comment),
			into(notify.status),
			now;

		DUITRACE("%d,%d,%s,%d\n", notify.id, notify.opcode, notify.comment, notify.status);

		if (notify.opcode == 1)
			return true;
	}
	catch (Poco::Exception& e)
	{
		DUITRACE(e.displayText().c_str());
	}
	
	return false;
}

void CMonitoringUI::report_to_server()
{
	struct alert_table
	{
		int id;
		std::string filepath;
		int type;
		std::string timestamp;
		int userid;
		int deviceid;
	};

	try
	{
		Session session("SQLite", "D:\\workstation\\code\\GitHub\\FaceIdentifiSystem\\bin\\facerecog.db");
		Statement select(session);
		alert_table alert;
		select << "SELECT id, filepath, type, timestamp, UserInfoID, DeviceID FROM alert",
			into(alert.id),
			into(alert.filepath),
			into(alert.type),
			into(alert.timestamp),
			into(alert.userid),
			into(alert.deviceid),
			range(0, 1);

		while (!select.done())
		{
			select.execute();
			DUITRACE("id:%d,filepath:%s,type:%d,timestamp:%s,userid:%d,deviceid:%d\n",
				alert.id, alert.filepath.c_str(), alert.type, alert.timestamp.c_str(), alert.userid, alert.deviceid);
		}
	}
	catch (Poco::Exception& e)
	{
		DUITRACE(e.displayText().c_str());
	}

	//http post data
	//ftp upload image
	post_alert_data();
	//upload_image();
}

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/NetException.h>

void CMonitoringUI::post_alert_data()
{
	using Poco::Net::HTTPClientSession;
	using Poco::Net::HTTPRequest;
	using Poco::Net::HTTPResponse;
	using Poco::StreamCopier;

	HTTPClientSession session("202.103.191.73");
	session.setKeepAlive(true);

	try
	{
		HTTPRequest request(HTTPRequest::HTTP_POST, "/api/index.php?s=/Services/index");
		request.setVersion(HTTPRequest::HTTP_1_1);
		request.setContentType("multipart/form-data; boundary=----011000010111000001101001");
		request.add("Cache-Control", "no-cache");
		request.add("Connection", "keep-alive");
		std::string body("{\"api\":\"wn_add\", \"filepath\":\"http://202.103.191.73/api/3870861421055882.jpg\",\"type\":1,\"timestamp\":\"20161019221808\",\"UserInfoID\":1,\"DeviceID\":1,\"summary\":\"test\"}");	
		std::ostream& os = session.sendRequest(request);
		os << "-----011000010111000001101001\r\nContent-Disposition: form-data; name=\"body\"\r\n\r\n{\"api\":\"wn_add\", \"filepath\":\"http://202.103.191.73/api/3870861421055882.jpg\",\"type\":1,\"timestamp\":\"20161019221808\",\"UserInfoID\":1,\"DeviceID\":1,\"summary\":\"test\"}\r\n-----011000010111000001101001--\r\n";

		os.flush();
		Poco::Thread::sleep(200);

		HTTPResponse response;
		std::istream& rs = session.receiveResponse(response);
		DUITRACE("HTTPResponse status : %d", response.getStatus());
		assert(response.getStatus() == HTTPResponse::HTTP_OK);
		std::ostringstream ostr;
		StreamCopier::copyStream(rs, ostr);
		std::string message(ostr.str());
		DUITRACE("HTTPResponse:\n %s", message.c_str());
	}
	catch (Poco::Net::NetException& e)
	{
		session.reset();
		DUITRACE(e.displayText().c_str());
	}
	
}

#include <Poco/Net/FTPClientSession.h>
#include <Poco/StreamCopier.h>
#include <Poco/FileStream.h>
#include <sstream>

void CMonitoringUI::upload_image()
{
	using Poco::Net::FTPClientSession;
	using Poco::StreamCopier;
	using Poco::FileInputStream;

	std::string host("202.103.191.73");
	std::string username("ftpalert");
	std::string passwd("1");
	FTPClientSession  session(host, FTPClientSession::FTP_PORT, username, passwd);
	
	session.setWorkingDirectory("/var/www/sisec_website/photos");
	DUITRACE(session.getWorkingDirectory().c_str());
	FileInputStream fis("D:/workstation/code/GitHub/FaceIdentifiSystem/bin/photo/20161020114211.jpg");
	std::ostream& ostr = session.beginUpload("20161020114211.jpg");
	StreamCopier::copyStream(fis, ostr);
	session.endUpload();
	session.close();
}

LPCTSTR CMonitoringUI::GetWindowClassName() const
{
	return _T("CMonitoringUI");
}

CDuiString CMonitoringUI::GetSkinFolder()
{
	return _T("skin");
}

CDuiString CMonitoringUI::GetSkinFile()
{
	return _T("xml\\\\MonitoringUI.xml");
}

void CMonitoringUI::OnFinalMessage(HWND hWnd)
{
	removeObserver(*this);
	r.stop();
	//example.stop();

	WindowImplBase::OnFinalMessage(hWnd);
}

void CMonitoringUI::Notify(TNotifyUI& msg)
{
	WindowImplBase::Notify(msg);
}

void CMonitoringUI::OnCloseWnd(TNotifyUI& msg)
{
	m_closeApp = false;
	Close();
}

void CMonitoringUI::OnInitCtrl()
{
	m_Main_Lyt = dynamic_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("vLyt_")));
	m_Prompt_lab = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("result")));
	m_photo_Ctrl = dynamic_cast<CControlUI*>(m_PaintManager.FindControl(_T("photo_video")));
}

void CMonitoringUI::OnRemoveAlarm()
{
	//std::auto_ptr<CWaittingUI> pDlg(new CWaittingUI);
	//assert(pDlg.get());
	//pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_FRAME, 0L, 1024, 768, 0, 0);
	//pDlg->CenterWindow();
	//pDlg->ShowModal();
}

void CMonitoringUI::InitWindow()
{
	//SetTimer(GetHWND(), 1, m_timeInterval, nullptr);
	addObserver(*this);
	r.start();
	//example.start();
	m_count = 0;
}


std::queue<CapBitmapData>& CMonitoringUI::getCapDataQueue()
{
	return m_capdata;
}


void CMonitoringUI::handle1(Poco::Notification* pNf)
{
	poco_check_ptr(pNf);
	CaptureNotification* nf = dynamic_cast<CaptureNotification*>(pNf);
	if (nf)
	{
		/*Poco::Data::CLOB saveImage((const char*)pic->data(), pic->len());
		m_userInfo.set<8>(saveImage);*/

		CControlUI* Image = m_PaintManager.FindControl(_T("photo_video"));
		Util::DrawSomething(nf->data(), Image, GetHWND());
	}
	pNf->release();
	/*
	poco_check_ptr(pNf);

	Notification::Ptr pf(pNf);
	poco_check_ptr(pf.get());
	if (m_bSendMsg && m_count % 10 == 0)
	{	
		//example.enqueueNotification(pf);	
		m_count++;
	}
	
	CaptureNotification::Ptr nf = pf.cast<CaptureNotification>();
	poco_check_ptr(nf.get());
	Picture::Ptr pic(nf->data());
	poco_check_ptr(pic.get());

	Util::DrawSomething(pic, m_photo_Ctrl, GetHWND());


	CControlUI* Image = m_PaintManager.FindControl(_T("photo_video"));
	Util::DrawSomething(pic, Image, GetHWND());
	*/
}

LRESULT CMonitoringUI::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	switch (uMsg)
	{
	case WM_TIMER: lRes = OnTimer(uMsg, wParam, lParam, bHandled);
		break;
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CMonitoringUI::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (wParam)
	{
	case 1: OnTimer1();	break;
	case 2: OnTimer2(); break;
	case 3: OnTimer3(); break;
	case 4: OnTimer4(); break;
	default:
		break;
	}
	return 0;
}

void CMonitoringUI::OnTimer1()
{
	m_bSendMsg = true;
	time(&m_lastTime);
	SetTimer(GetHWND(), 2, 250, nullptr);
	KillTimer(GetHWND(), 1);
}

void CMonitoringUI::OnTimer2()
{
	/*time(&m_nowTime);
	if (example.queryResult() && m_nowTime - m_lastTime <= 15)
	{
		m_bSendMsg = false;
		KillTimer(GetHWND(), 2);
		SetTimer(GetHWND(), 1, m_timeInterval, nullptr);
	}
	else if (!example.queryResult() && m_nowTime - m_lastTime > 15)
	{
		KillTimer(GetHWND(), 2);
		SetTimer(GetHWND(), 3, 500, nullptr);
		SetTimer(GetHWND(), 4, 1000, nullptr);
	}*/
}

void CMonitoringUI::OnTimer3()
{
	/*if (example.queryResult())
	{
		KillTimer(GetHWND(), 3);
		KillTimer(GetHWND(), 4);
		SetTimer(GetHWND(), 1, m_timeInterval, nullptr);
		m_bSendMsg = false;
		m_Prompt_lab->SetVisible(true);
		m_Main_Lyt->SetBkColor(0x00000000);
	}
	else
	{
		PlaySoundA(_T("BJ.wav"), NULL, SND_FILENAME | SND_ASYNC);
		m_Prompt_lab->SetVisible(false);
		DWORD bkcolor = m_Main_Lyt->GetBkColor();
		DWORD itembkcolor = bkcolor == 0x00000000 ? 0xFFFF9999 : 0x00000000;
		m_Main_Lyt->SetBkColor(itembkcolor);
	}*/
}

void CMonitoringUI::OnTimer4()
{
	m_bSendMsg = (m_bSendMsg == false ? true : false);
}


