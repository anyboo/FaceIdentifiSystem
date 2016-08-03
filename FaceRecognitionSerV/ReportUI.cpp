#include "stdafx.h"
#include "ReportUI.h"


CReportUI::CReportUI()
:m_optionSerial(1)
{
}


CReportUI::~CReportUI()
{
}

DUI_BEGIN_MESSAGE_MAP(CReportUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSE_REPORT, OnCloseWnd)
DUI_ON_CLICK_CTRNAME(BT_SELECT_ALL, OnSelectAll)
DUI_END_MESSAGE_MAP()

LPCTSTR CReportUI::GetWindowClassName() const
{
	return _T("CReportUI");
}

CDuiString CReportUI::GetSkinFolder()
{
	return _T("skin");
}

CDuiString CReportUI::GetSkinFile()
{
	return _T("xml\\ReportUI.xml");
}

void CReportUI::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
}

void CReportUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK && msg.pSender->GetName() == _T("test"))
	{
		AddSubList();
	}
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		std::string strName = msg.pSender->GetName();
		SaveSelectOptionTag(strName);
	}
	WindowImplBase::Notify(msg);
}


void CReportUI::OnCloseWnd(TNotifyUI& msg)
{
	Close();
}

void CReportUI::AddSubList()
{
	CDialogBuilder builder;
	CListContainerElementUI* SubList = (CListContainerElementUI*)(builder.Create(_T("xml//ReportListUI.xml"), (UINT)0, NULL, &m_PaintManager));
	CListUI* pList = dynamic_cast<CListUI*>(m_PaintManager.FindControl(_T("RepotList")));
	pList->AddAt(SubList, 0);

	COptionUI* option = dynamic_cast<COptionUI*>(m_PaintManager.FindSubControlByClass(SubList, DUI_CTR_OPTION));
	std::string	optionName = std::string(_T("option")) + std::to_string(m_optionSerial);
	option->SetName(optionName.c_str());
	m_optionSerial += 1;
}

void CReportUI::OnSelectAll(TNotifyUI& msg)
{
	std::string strName = msg.pSender->GetName();
	CListUI* pList = dynamic_cast<CListUI*>(m_PaintManager.FindControl(_T("RepotList")));
	COptionUI* all_option = dynamic_cast<COptionUI*>(m_PaintManager.FindSubControlByName(pList, strName.c_str()));
	int size = pList->GetCount();
	if (!all_option->IsSelected())
	{
		for (int i = 1; i <= size; i++)
		{
			COptionUI* option = dynamic_cast<COptionUI*>(m_PaintManager.FindSubControlByClass(pList, DUI_CTR_OPTION, i));
			option->Selected(true);
			m_SelectOptions.push_back(i);
		}
	}
	else
	{
		for (int i = 1; i <= size; i++)
		{
			COptionUI* option = dynamic_cast<COptionUI*>(m_PaintManager.FindSubControlByClass(pList, DUI_CTR_OPTION, i));
			option->Selected(false);
		}
		m_SelectOptions.clear();
	}
}

void CReportUI::SaveSelectOptionTag(std::string& strName)
{
	if (!strName.compare(0, 6, _T("option")))
	{
		std::string strTag = strName.substr(6);
		int tag = stoi(strTag);
		CListUI* pList = dynamic_cast<CListUI*>(m_PaintManager.FindControl(_T("RepotList")));
		COptionUI* subOption = dynamic_cast<COptionUI*>(m_PaintManager.FindSubControlByName(pList, strName.c_str()));
		if (!subOption->IsSelected())
		{
			m_SelectOptions.push_back(tag);
		}
		else
		{
			for (size_t i = 0; i < m_SelectOptions.size(); i++)
			{
				if (m_SelectOptions[i] = tag)
				{
					m_SelectOptions.erase(m_SelectOptions.begin() + i);
				}
			}
		}
	}
}
