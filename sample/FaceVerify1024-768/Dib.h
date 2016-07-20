/////////////////////////////////////////////////////////////////////////////////
//
//		Dib.h: interface for the CDib class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _CDIB_H
#define _CDIB_H

class CDib : public CObject
{
	DECLARE_DYNAMIC(CDib)
protected:

	BOOL m_bValid;			//是否有效
    RGBQUAD* m_pRGBQuad;	//RGBQUAD 表项
    BYTE* m_pDibData;		//位图像素数据
	LONG m_nWidth;			//宽度
	LONG m_nHeight;			//高度
	BYTE m_nBitCount;		//每个像素占有的位数
    UINT m_nTotalColors;	//颜色总数
	char* m_pszFileName;
    BITMAPFILEHEADER* m_pBitmapFileHeader;
    BITMAPINFOHEADER* m_pBitmapInfoHeader;
    BITMAPINFO* m_pBitmapInfo;
	HGLOBAL m_hDIB;
	
    
public:

	CDib();
    CDib(const char* pszDibFileName);
    ~CDib();

	int Draw(HDC hdc, int XDest, int YDest, int nDestWidth, int nDestHeight, int XSrc, int YSrc, int nSrcWidth, int nSrcHeight, UINT iUsage, DWORD dwRop);


	DWORD GetDdbData32(LPBYTE lpbyDdb32);
	DWORD GetDdbData24(LPBYTE lpbyDdb24);

	DWORD GetDibWidthBytes(int nWidth,  BYTE byBitCount);
	BYTE GetBitCount() const;
    DWORD GetSize();
    LONG GetWidth() const;
    LONG GetHeight() const;
    UINT GetColorNumber() const;
	char* GetFileName() const;
	BOOL IsValid() const ;
    RGBQUAD* GetRGBQuad() const ;
    BYTE* GetDibData() const ;
    BITMAPINFO* GetBmpInfo() const;

public:
	void ClearMemory();
	HPALETTE CreateBitmapPalette();

#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;
#endif

protected:
	
    
	void LoadFile(const char* pszDibFileName);
private:
	void Init();
};

#endif
