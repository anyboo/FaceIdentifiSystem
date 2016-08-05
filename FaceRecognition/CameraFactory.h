#pragma once

class Camera;
class CameraInfo;

class CameraFactory
{
public:
	CameraFactory();
	virtual ~CameraFactory();

	Camera* createCamera(const CameraInfo& info);
	Camera* CameraFactory::createCamera(const CameraInfo& info, const std::string& ipStr);
	static	CameraFactory& defaultFactory();
private:
	CameraFactory(const CameraFactory&);
	CameraFactory& operator = (const CameraFactory&);
};

