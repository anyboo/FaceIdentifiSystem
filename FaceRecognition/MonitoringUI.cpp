#include "stdafx.h"
#include "MonitoringUI.h"

#include "Poco/Delegate.h"
#include "Picture.h"
#include "BitMapCompare.h"
#include "Util.h"

#include "WaittingUI.h"

CMonitoringUI::CMonitoringUI()
	:m_nBmp(0), r(new Camera)
{
	m_testID = 100001;
	//m_pCompare = new BitMapCompare(this);
	//Poco::ThreadPool::defaultPool().start(*m_pCompare);
}


CMonitoringUI::~CMonitoringUI()
{
	/*m_theEvent += Poco::delegate(m_pCompare, &BitMapCompare::onEvent);
	fireEvent(true);
	m_theEvent -= Poco::delegate(m_pCompare, &BitMapCompare::onEvent);*/
	//Poco::ThreadPool::defaultPool().joinAll();
	//delete m_pCompare;
}

DUI_BEGIN_MESSAGE_MAP(CMonitoringUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSE_MonWnd, OnCloseWnd)
DUI_ON_CLICK_CTRNAME(BT_REMOVE_ALARM, OnRemoveAlarm)
DUI_END_MESSAGE_MAP()

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
	return _T("xml\\MonitoringUI.xml");
}

void CMonitoringUI::OnFinalMessage(HWND hWnd)
{
	removeObserver(*this);
	r.stop();
	example.stop();

	WindowImplBase::OnFinalMessage(hWnd);
}

void CMonitoringUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK && msg.pSender->GetName() == _T("test"))
	{
	//	ShowMonitInfoList();
	}
	WindowImplBase::Notify(msg);
}

void CMonitoringUI::OnCloseWnd(TNotifyUI& msg)
{
	Close();
}

void CMonitoringUI::OnRemoveAlarm(TNotifyUI& msg)
{
	std::auto_ptr<CWaittingUI> pDlg(new CWaittingUI);
	assert(pDlg.get());
	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_FRAME, 0L, 1024, 768, 0, 0);
	pDlg->CenterWindow();
	pDlg->ShowModal();
}

void CMonitoringUI::InitWindow()
{
	addObserver(*this);
	r.start();
	example.start();
}

void CMonitoringUI::ShowMonitInfoList()
{
	CDialogBuilder builder;
	CListContainerElementUI* SubList = (CListContainerElementUI*)(builder.Create(_T("xml//RegInfoList.xml"), (UINT)0, NULL, &m_PaintManager));
	CListUI* pList = dynamic_cast<CListUI*>(m_PaintManager.FindControl(_T("Refer_InfoList")));
	pList->Add(SubList);
	CControlUI* photoUI = dynamic_cast<CControlUI*>(m_PaintManager.FindSubControlByClass(SubList, DUI_CTR_CONTROL));
	CLabelUI* lab_ID = dynamic_cast<CLabelUI*>(m_PaintManager.FindSubControlByClass(SubList, DUI_CTR_LABEL, 1));
	CLabelUI* lab_similarity = dynamic_cast<CLabelUI*>(m_PaintManager.FindSubControlByClass(SubList, DUI_CTR_LABEL, 3));
	CLabelUI* lab_time = dynamic_cast<CLabelUI*>(m_PaintManager.FindSubControlByClass(SubList, DUI_CTR_LABEL, 5));

	std::string strName = std::string(_T("file = 'bmp/test")) + std::to_string(m_nBmp) + std::string(".bmp'");
	photoUI->SetBkImage(strName.c_str());
	lab_ID->SetText(std::to_string(m_testID).c_str());
	lab_similarity->SetText("76%");
	lab_time->SetText("2016-7-29 17:12:55");

	if (m_testID % 3 == 0)
	{
		SubList->SetBkColor(0xFFFFDDDD);
	}
	m_testID++;
}

std::queue<CapBitmapData>& CMonitoringUI::getCapDataQueue()
{
	return m_capdata;
}

#include "CaptureNotification.h"
void CMonitoringUI::handle1(Poco::Notification* pNf)
{
	poco_check_ptr(pNf);
	Notification::Ptr pf(pNf);
	poco_check_ptr(pf.get());
	example.enqueueNotification(pf);

	CaptureNotification::Ptr nf = pf.cast<CaptureNotification>();
	poco_check_ptr(nf.get());
	Picture::Ptr pic(nf->data());
	poco_check_ptr(pic.get());

	CControlUI* Image = m_PaintManager.FindControl(_T("photo_video"));
	Util::DrawSomething(pic, Image, GetHWND());
}

LRESULT CMonitoringUI::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	switch (uMsg)
	{
	case WM_TIMER: lRes = OnTimer(uMsg, wParam, lParam, bHandled); break;
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CMonitoringUI::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CVerticalLayoutUI* vLyt = dynamic_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("vLyt_")));
	DWORD bkcolor = vLyt->GetBkColor();
	if (bkcolor == 0xFFFF9999)
	{
		vLyt->SetBkColor(0x00000000);
	}
	else
	{
		vLyt->SetBkColor(0xFFFF9999);
	}
	return 0;
}