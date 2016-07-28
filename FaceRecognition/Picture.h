#ifndef __PICTURE_H__
#define __PICTURE_H__

class Picture
{
public:
	Picture();
	~Picture();

	BITMAPINFOHEADER m_bih;
	BYTE* m_pCamBuf;;
};

#endif 

