#include "stdafx.h"
#include "DeviceManage.h"


CDeviceManage::CDeviceManage()
{
}


CDeviceManage::~CDeviceManage()
{
}

DUI_BEGIN_MESSAGE_MAP(CDeviceManage, CNotifyPump)

DUI_END_MESSAGE_MAP()

void CDeviceManage::SetPaintMagager(CPaintManagerUI* pPaintMgr)
{
	assert(pPaintMgr);
	_ppm = pPaintMgr;
}

void CDeviceManage::InitDeviceInfo()
{

}

void CDeviceManage::SaveDeviceInfo()
{
	CEditUI* edit_Dtype = dynamic_cast<CEditUI*>(_ppm->FindControl(_T("Device_Type")));
	CComboUI* comb_Area = dynamic_cast<CComboUI*>(_ppm->FindControl(_T("combo_area")));
	CEditUI* edit_UnitName = dynamic_cast<CEditUI*>(_ppm->FindControl(_T("entity_name")));
	CEditUI* edit_Building = dynamic_cast<CEditUI*>(_ppm->FindControl(_T("Building_name")));
	CEditUI* edit_Address = dynamic_cast<CEditUI*>(_ppm->FindControl(_T("Address")));
	CEditUI* edit_IPAddr = dynamic_cast<CEditUI*>(_ppm->FindControl(_T("Device_IP")));
	CDuiString device_type = edit_Dtype->GetText();
	CDuiString device_area = comb_Area->GetText();
	CDuiString unit_name = edit_UnitName->GetText();
	CDuiString building_name = edit_Building->GetText();
	CDuiString strAddr = edit_Address->GetText();
	CDuiString ipAddr = edit_IPAddr->GetText();
}

void CDeviceManage::ReadDeviceInfo()
{

}