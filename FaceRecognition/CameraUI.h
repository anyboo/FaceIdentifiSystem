#pragma once

#include <DuiLib/UIlib.h>

#define  DUI_CTR_CAMERAUI                         (_T("CameraUI"))
#define  EVENT_CAMER_TIEM_ID  1010
#define  CUSTOM_EVENT_TAKE_PHOTO    "take_a_photo"

class CCapture;
class CameraUI : public CControlUI
{
public:
	CameraUI();
	~CameraUI();

	LPCTSTR	GetClass() const;
	LPVOID	GetInterface(LPCTSTR pstrName);
	virtual void SetVisible(bool bVisible = true);
	virtual void Init();
	virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
	virtual void DoEvent(TEventUI& event);
	BYTE* beginSnapshot();
	void endSnapshot();
	bool ScreenSnapshot(const std::string& jpegPath);
	void ScreenSnapshot(BYTE* framebuffer, long len);
	long GetBitmapSize();
	int GetFrameWidth() const;
	int  GetFrameHeight() const;
private:
	void OnTimer(UINT_PTR idEvent);
	void DrawFrame(HDC hDC);	
	void StartCapture();
	void StopCapture();
	void Prepare();
	void SaveBmp(char* data, int width, int height, const std::string& path);

	CCapture* _camera;
	bool _captured;
	int _width;
	int _height;
	long _bitmapSize;
	BYTE* _buffer;
};


inline int CameraUI::GetFrameWidth() const
{
	return _width;
}

inline int CameraUI::GetFrameHeight() const
{
	return _height;
}