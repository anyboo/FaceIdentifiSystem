#pragma once
class CAppInitialize
{
public:
	CAppInitialize();
	~CAppInitialize();

protected:
	void PrepareCamera();
	void PrepareRegisteredFace();
	void LoadUserConfig();
	void TiCapture2SDK();
	void THFaceImageSDK();
	void AttatchSDK();
	void DetachedSDK();
};

