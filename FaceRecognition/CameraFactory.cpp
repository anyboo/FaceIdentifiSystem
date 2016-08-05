#include "stdafx.h"
#include "CameraFactory.h"
#include <Poco/SingletonHolder.h>

CameraFactory::CameraFactory()
{
}

CameraFactory::~CameraFactory()
{
}

namespace
{
	static Poco::SingletonHolder<CameraFactory> holder;
}


CameraFactory& CameraFactory::defaultFactory()
{
	return *holder.get();
}

Camera* CameraFactory::createCamera(const CameraInfo& info)
{
	//return new CameraImpl(info);
	return nullptr;
}

Camera* CameraFactory::createCamera(const CameraInfo& info, const std::string& ipStr)
{
	//return new CameraImpl(info, ipStr);
	return nullptr;
}

