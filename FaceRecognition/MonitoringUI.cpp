#include "stdafx.h"
#include "MonitoringUI.h"

#include "Poco/Delegate.h"


CMonitoringUI::CMonitoringUI()
:m_nBmp(0)
{
	m_testID = 100001;
	m_pCompare.reset( new BitMapCompare(this));
	Poco::ThreadPool::defaultPool().start(*m_pCompare.get());
}


CMonitoringUI::~CMonitoringUI()
{
	m_theEvent += Poco::delegate(m_pCompare.get(), &BitMapCompare::onEvent);
	fireEvent(true);
	m_theEvent -= Poco::delegate(m_pCompare.get(), &BitMapCompare::onEvent);
	Poco::ThreadPool::defaultPool().joinAll();
}

DUI_BEGIN_MESSAGE_MAP(CMonitoringUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSE_MonWnd, OnCloseWnd)
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
	::KillTimer(GetHWND(), 1);
//	::KillTimer(GetHWND(), 2);
	Close();
}

void CMonitoringUI::InitWindow()
{
	::SetTimer(GetHWND(), 1, 50, nullptr);
//	::SetTimer(GetHWND(), 2, 2000, nullptr);
	
}

LRESULT CMonitoringUI::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == 1)
	{
		m_nBmp = m_nBmp + 1;
		CVerticalLayoutUI* photo_Lyt = dynamic_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("photo_video")));

		std::string strName = std::string(_T("file = 'bmp/test")) + std::to_string(m_nBmp) + std::string(".bmp'");
		photo_Lyt->SetBkImage(strName.c_str());
		if (m_nBmp == 150)
		{
			m_nBmp = 1;
		}
	}
	if (wParam == 2)
	{
		ShowMonitInfoList();
	}
	return 0;
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

std::queue<readCompareInfo>& CMonitoringUI::getCompareQueue()
{
	return m_compare;
}