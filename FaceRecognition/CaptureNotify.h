#pragma once

#include <Poco/Observer.h>
#include <Poco/Notification.h>
#include <Poco/NotificationCenter.h>

using Poco::Observer;
using Poco::Notification;

class CaptureNotify
{
public:
	CaptureNotify();
	virtual ~CaptureNotify();

	virtual void handle1(Poco::Notification* pNf);

	virtual void handle1(Poco::Notification* pNf, Picture **pImg);

protected:
	template <class C>
	void addObserver(C& object)
	{
		Observer<C, Notification> o1(object, &C::handle1);
		nc.addObserver(o1);
	}

	template <class C>
	void removeObserver(C& object)
	{
		nc.removeObserver(Observer<C, Notification>(object, &C::handle1));
	}

private:
	Poco::NotificationCenter& nc;
};

