#pragma once
#include "Picture.h"

class Util
{
public:
	Util();
	~Util();
	static void Util::DrawSomething(Picture::Ptr pic, CControlUI* control, HWND hwnd);
	static void Util::DrawSomething(Picture& pic, CControlUI* control, HWND hwnd);
};

