#include "stdafx.h"
#include "CaptureNotification.h"
#include "CaptureNotify.h"
#include <Poco/AutoPtr.h>
#include <Poco/Observer.h>
#include <Poco/NotificationCenter.h>

using Poco::AutoPtr;
using Poco::Observer;
using Poco::Notification;
using Poco::NotificationCenter;

CaptureNotify::CaptureNotify()
:nc(NotificationCenter::defaultCenter())
{
}


CaptureNotify::~CaptureNotify()
{
}

void CaptureNotify::handle1(Poco::Notification* pNf)
{
	poco_check_ptr(pNf);
	Notification::Ptr pf(pNf);
	poco_check_ptr(pf.get());
	CaptureNotification::Ptr nf = pf.cast<CaptureNotification>();
	poco_check_ptr(nf.get());
	Picture *capdata = nf->data();
	Picture::Ptr pic(nf->data());
	poco_check_ptr(pic.get());
	//pic->out2bmp();
}

void CaptureNotify::handle1(Poco::Notification* pNf, Picture **pImg)
{
	poco_check_ptr(pNf);
	Notification::Ptr pf(pNf);
	poco_check_ptr(pf.get());
	CaptureNotification::Ptr nf = pf.cast<CaptureNotification>();
	poco_check_ptr(nf.get());
	Picture *capdata = nf->data();
	Picture *pic = new Picture(capdata->data(), capdata->width() * capdata->height() * 3);
	pic->SetWidth(capdata->width());
	pic->SetHeight(capdata->height());
	*pImg = pic;
	//Picture::Ptr pic(nf->data());
	//poco_check_ptr(pic.get());
	//pic->out2bmp();
}
