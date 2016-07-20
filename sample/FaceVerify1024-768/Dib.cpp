/////////////////////////////////////////////////////////////////////////////////
//
// Dib.cpp: implementation of the CDib class.
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "dib.h"

IMPLEMENT_DYNAMIC(CDib,  CObject)

CDib::CDib()
{
	Init();
}

CDib::CDib(const char* pszDibFileName)
{
	Init();
    LoadFile(pszDibFileName);
}


CDib::~CDib()
{
	ClearMemory();
}

void CDib::ClearMemory()
{
	if(m_hDIB != NULL)
		::GlobalFree(m_hDIB);
	m_hDIB = 0;
    m_pDibData = NULL;		
	m_pszFileName = NULL;
	m_pBitmapInfoHeader = NULL;
    m_pBitmapInfo = NULL;

	m_pRGBQuad = NULL;
	m_pBitmapFileHeader = NULL;
}

#ifdef _DEBUG
void CDib::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}

void CDib::AssertValid() const
{
	CObject::AssertValid();
	ASSERT(m_pszFileName != NULL);	
	ASSERT(m_hDIB != 0);	
}
#endif


void CDib::Init()
{
	m_pRGBQuad = NULL;		//RGBQUAD 表项
    m_pDibData = NULL;		//位图像素数据
	m_nWidth = 0;			//宽度
	m_nHeight = 0;			//高度
	m_nBitCount = 0;		//每个像素占有的位数
    m_nTotalColors = 0;		//颜色总数
    m_pBitmapFileHeader = NULL;
    m_pBitmapInfoHeader = NULL;
    m_pBitmapInfo = NULL;
	m_hDIB = 0;
	m_pszFileName = NULL;
}

void CDib::LoadFile(const char* pszDibFileName)
{
	ASSERT(pszDibFileName);
	//如果存在就释放内存
	if(m_pBitmapInfo) ::GlobalFree(m_hDIB);
	//再次初始化, 用于多次动态重用
	Init();
	m_pszFileName = (char*)pszDibFileName;

	BITMAPFILEHEADER bitmapFileHeader;

	CFile dibFile;
	BOOL bOpen=dibFile.Open(pszDibFileName,  CFile::modeRead);
	if(!bOpen) return;
	try
	{
		//读入位图文件头信息
		UINT nBFHsize = dibFile.Read((void*)&bitmapFileHeader, sizeof(BITMAPFILEHEADER));
		if(nBFHsize != sizeof(BITMAPFILEHEADER))
		{
			m_bValid = FALSE;
			return;
		}
	}
	catch(CFileException* e)
	{
		e->Delete();
	}

	//如果为位图就进行下一步的处理
    if (bitmapFileHeader.bfType == 0x4d42)
    {
		AfxGetApp()->BeginWaitCursor();
		
		//文件长度
        DWORD dwFileLength = dibFile.GetLength();
		
		//位图信息及位图数据的大小(字节)
        DWORD dwSize = dwFileLength - sizeof(BITMAPFILEHEADER);
 

		//为Dib分配全局内存
		m_hDIB = (HGLOBAL) ::GlobalAlloc(GMEM_MOVEABLE,  dwSize);
		if (m_hDIB == 0)	m_bValid = FALSE;

		//指向Dib实际像素数据的指针, 
		BYTE* pbyDib = (BYTE*)::GlobalLock(m_hDIB);
		if (pbyDib == NULL)	m_bValid = FALSE;
	 
		try
		{
			//读入内存
			DWORD dwReadSize = dibFile.Read((void*)pbyDib,  dwSize);
			dibFile.Close();
			if(dwReadSize != dwSize)
			{
				m_bValid = FALSE;
				::GlobalUnlock(m_hDIB);
				::GlobalFree(m_hDIB);
				Init();
				return;
			}
		}
		catch(CFileException* e)
		{
			e->Delete();
		}

		//获取位图文件头指针和位图文件数据信息指针
        m_pBitmapInfo = (BITMAPINFO*) pbyDib;
		m_pBitmapInfoHeader = (BITMAPINFOHEADER*) pbyDib;
		
		//宽度和高度
		m_nWidth = m_pBitmapInfoHeader->biWidth;
		m_nHeight = m_pBitmapInfoHeader->biHeight;
		//每像素的颜色位数
		m_nBitCount = (BYTE)m_pBitmapInfoHeader->biBitCount;
      
		//颜色数
		m_nTotalColors = GetColorNumber();
        if (m_pBitmapInfoHeader->biClrUsed == 0)
            m_pBitmapInfoHeader->biClrUsed = m_nTotalColors;

		//指向位图颜色索引表项的指针, 如果没有颜色表项, 则该值为NULL
        m_pRGBQuad = (RGBQUAD*)(pbyDib + m_pBitmapInfoHeader->biSize);
  
		//颜色索引表的大小(字节)
        DWORD dwColorTableSize = m_nTotalColors * sizeof(RGBQUAD);
        
		//指向位图数据的指针
		m_pDibData = pbyDib + m_pBitmapInfoHeader->biSize + dwColorTableSize;
		
		//如果没有颜色索引表, 则该值为NULL
		if (m_pRGBQuad == (RGBQUAD*)m_pDibData) m_pRGBQuad = NULL;
       
		//位图的大小(面积像素单位)
		m_pBitmapInfoHeader->biSizeImage = GetSize();
		::GlobalUnlock(m_hDIB);
		AfxGetApp()->EndWaitCursor();
		m_bValid = TRUE;
    }    
    else
    {
    //    AfxMessageBox("This is not a bitmap file!");
		m_bValid = FALSE;
    }
}

LONG CDib::GetWidth() const 
{
    return m_nWidth;
}
        
LONG CDib::GetHeight() const
{
    return m_nHeight;
}

//获取数据缓冲区的大小       
DWORD CDib::GetSize()
{
    if (m_pBitmapInfoHeader->biSizeImage != 0)
        return (m_pBitmapInfoHeader->biSizeImage);
	else 
		return (DWORD)GetDibWidthBytes(m_nWidth, 24) * (DWORD)m_nHeight;
}

BYTE CDib::GetBitCount() const
{
	return m_nBitCount;
}

UINT CDib::GetColorNumber() const
{
	UINT nColors = 0;

    if ((m_pBitmapInfoHeader->biClrUsed == 0) &&
          (m_pBitmapInfoHeader->biBitCount < 9))
	{
		switch (m_pBitmapInfoHeader->biBitCount)
		{
		    case 1: nColors = 2; break;
		    case 4: nColors = 16; break;
		    case 8: nColors = 256;break;
		}
	}
    else
		nColors = (UINT) m_pBitmapInfoHeader->biClrUsed;

    return nColors;
}

char* CDib::GetFileName() const
{
	return m_pszFileName;
}
    
BYTE* CDib::GetDibData() const
{
    return m_pDibData;
}

RGBQUAD* CDib::GetRGBQuad() const
{
    return m_pRGBQuad;
}

BITMAPINFO* CDib::GetBmpInfo() const
{
    return m_pBitmapInfo;
}

BOOL CDib::IsValid() const
{
	return m_bValid;
}

//每行的字节宽度
DWORD CDib::GetDibWidthBytes(int nWidth,  BYTE byBitCount)
{
	DWORD dwWidthBytes = (DWORD)nWidth;	//8-bits
	if(byBitCount == 1) dwWidthBytes = (nWidth + 7) / 8;
	else if(byBitCount == 4) dwWidthBytes = (nWidth + 1) / 2;
	else if(byBitCount == 24) dwWidthBytes = 3 * nWidth ;
	
	while((dwWidthBytes & 3) != 0)dwWidthBytes++;

	return dwWidthBytes;

}


//将所有格式的数据转换成DDB--24位数据, 存放于lpbyDdb24
//返回获取数据的总数.
DWORD  CDib::GetDdbData24(LPBYTE lpbyDdb24)
{
	ASSERT(lpbyDdb24);

	//计算DIB位图中一行的字节量
	DWORD dwDibWidthBytes = GetDibWidthBytes(m_nWidth,  m_nBitCount);

	//转换成24位DDB后每行的大小, 注意每行的字节数只需要是偶数即可
	DWORD dwDdbWidthBytes =  ((m_nWidth * 24 + 15) / 16) * 2;

	if(m_nBitCount == 1)
	{
		//每行实际字节数, 8个像素对应一个字节.
		LONG nLineBytes = (m_nWidth + 7) / 8;
		BYTE abyBitMask[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
		
		//DIB位图数据的行头
		DWORD dwDibBaseIndex = (m_nHeight - 1) * dwDibWidthBytes;
		
		//DDB 24位位图数据的行头
		DWORD dwDdbBaseIndex = 0;

		for(LONG i = 0;i < m_nHeight;i++)
		{
			//指向DIB行数据的指针
			BYTE* pbyDibRaw = m_pDibData + dwDibBaseIndex;
	
			//指向 DDB 24位位图的数据的指针
			BYTE* pbyDdbRaw = lpbyDdb24 + dwDdbBaseIndex;
			for(LONG j = 0;j < nLineBytes;j++)
			{
				//8个像素数据的索引值
				BYTE byBit8 =  *(pbyDibRaw++);
				for(int n = 0; n < 8;n++)
				{
					BYTE byBitSingle = (byBit8 & abyBitMask[n]) >> (7 - n);
					*(pbyDdbRaw++) = m_pRGBQuad[byBitSingle].rgbBlue;	//蓝色
					*(pbyDdbRaw++) = m_pRGBQuad[byBitSingle].rgbGreen;	//绿色
					*(pbyDdbRaw++) = m_pRGBQuad[byBitSingle].rgbRed;	//红色
				}
			}//end j
			dwDibBaseIndex -= dwDibWidthBytes;
			dwDdbBaseIndex += dwDdbWidthBytes;
		}//end i
		return (dwDdbWidthBytes * m_nHeight);
	}// end if(m_nBitCount == 1)
	else if(m_nBitCount == 4)
	{
		//每行实际字节数
		LONG nLineBytes = (m_nWidth + 1) / 2;

		//DIB位图数据的行头
		DWORD dwDibBaseIndex = (m_nHeight - 1) * dwDibWidthBytes;
		
		//DDB 24位位图数据的行头
		DWORD dwDdbBaseIndex = 0;

		for(LONG i = 0;i < m_nHeight;i++)
		{
			//指向DIB行数据的指针
			BYTE* pbyDibRaw = m_pDibData + dwDibBaseIndex;
			
			//指向 DDB 24位位图的数据的指针
			BYTE* pbyDdbRaw = lpbyDdb24 + dwDdbBaseIndex;
			for(LONG j = 0;j < nLineBytes;j++)
			{
				//两个像素数据的索引值, 通过高四位和低四位
				BYTE byBit8 =  *(pbyDibRaw++);
				BYTE byBitHigh = (byBit8 & 0xF0) >> 4;
				BYTE byBitLow = (byBit8 & 0x0F);

				*(pbyDdbRaw++) = m_pRGBQuad[byBitHigh].rgbBlue;		//蓝色
				*(pbyDdbRaw++) = m_pRGBQuad[byBitHigh].rgbGreen;	//绿色
				*(pbyDdbRaw++) = m_pRGBQuad[byBitHigh].rgbRed;		//红色

				*(pbyDdbRaw++) = m_pRGBQuad[byBitLow].rgbBlue;		//蓝色
				*(pbyDdbRaw++) = m_pRGBQuad[byBitLow].rgbGreen;		//绿色
				*(pbyDdbRaw++) = m_pRGBQuad[byBitLow].rgbRed;		//红色

			}//end j
			dwDibBaseIndex -= dwDibWidthBytes;
			dwDdbBaseIndex += dwDdbWidthBytes;
		}//end i
		return (dwDdbWidthBytes * m_nHeight);
	}// end if(m_nBitCount == 4)
	else if(m_nBitCount == 8)
	{
		//DIB位图数据的行头
		DWORD dwDibBaseIndex = (m_nHeight - 1) * dwDibWidthBytes;
		
		//DDB 24位位图数据的行头
		DWORD dwDdbBaseIndex = 0;
		for(LONG i = 0;i < m_nHeight;i++)
		{
			//指向DIB行数据的指针
			BYTE* pbyDibRaw = m_pDibData + dwDibBaseIndex;
		
			//指向 DDB 24位位图的数据的指针
			BYTE* pbyDdbRaw = lpbyDdb24 + dwDdbBaseIndex;
			for(LONG j = 0;j < m_nWidth;j++)
			{
				//一个像素数据的索引值
				BYTE byIndex = *(pbyDibRaw++);
	
				*(pbyDdbRaw++) = m_pRGBQuad[byIndex].rgbBlue;	//蓝色
				*(pbyDdbRaw++) = m_pRGBQuad[byIndex].rgbGreen;	//绿色
				*(pbyDdbRaw++) = m_pRGBQuad[byIndex].rgbRed;	//红色
			}//end j
			dwDibBaseIndex -= dwDibWidthBytes;
			dwDdbBaseIndex += dwDdbWidthBytes;
		}//end i
		return (dwDdbWidthBytes * m_nHeight);
	}// end if(m_nBitCount == 8)
	else if(m_nBitCount == 24)
	{
	
		DWORD dwLength = m_nWidth * 3;
		//指向DIB行数据的指针
		BYTE* pbyDibRaw = m_pDibData + (m_nHeight - 1) * dwDibWidthBytes;;
		//指向 DDB 24位位图的数据的指针
		BYTE* pbyDdbRaw = lpbyDdb24;

		//蓝色--绿色--红色
		for(LONG i = 0;i < m_nHeight;i++)
		{
			::CopyMemory(pbyDdbRaw, pbyDibRaw, dwLength);
			pbyDibRaw -= dwDibWidthBytes;		//from 
			pbyDdbRaw += dwDdbWidthBytes;		//to
		}//end i
		return (dwDdbWidthBytes * m_nHeight);
	}// end if(m_nBitCount == 24)

	return (m_nWidth * m_nHeight);
}

//将所有格式的数据转换成DDB--32位数据, 存放于lpbyDdb32
//返回获取数据的总数.
//32位DDB数据按字节顺序:蓝色--绿色--红色--保留字节--
DWORD  CDib::GetDdbData32(LPBYTE lpbyDdb32)
{
	ASSERT(lpbyDdb32);

	//计算DIB位图中一行的字节量
	DWORD dwDibWidthBytes = GetDibWidthBytes(m_nWidth,  m_nBitCount);

	//转换成32位DDB后每行的大小, 注意每位4字节
	DWORD dwDdbWidthBytes =  m_nWidth * 4;

	if(m_nBitCount == 1)
	{
		//每行实际字节数, 8个像素对应一个字节.
		LONG nLineBytes = (m_nWidth + 7) / 8;
		BYTE abyBitMask[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
		
		//DIB位图数据的行头
		DWORD dwDibBaseIndex = (m_nHeight - 1) * dwDibWidthBytes;
		
		//DDB 32位位图数据的行头
		DWORD dwDdbBaseIndex = 0;

		for(LONG i = 0;i < m_nHeight;i++)
		{
			//指向DIB行数据的指针
			BYTE* pbyDibRaw = m_pDibData + dwDibBaseIndex;
	
			//指向 DDB 32位位图的数据的指针
			BYTE* pbyDdbRaw = lpbyDdb32 + dwDdbBaseIndex;
			for(LONG j = 0;j < nLineBytes;j++)
			{
				//8个像素数据的索引值
				BYTE byBit8 =  *(pbyDibRaw++);
				for(int n = 0; n < 8;n++)
				{
					BYTE byBitSingle = (byBit8 & abyBitMask[n]) >> (7 - n);
					
					*(pbyDdbRaw++) = m_pRGBQuad[byBitSingle].rgbBlue;	//蓝色
					*(pbyDdbRaw++) = m_pRGBQuad[byBitSingle].rgbGreen;	//绿色
					*(pbyDdbRaw++) = m_pRGBQuad[byBitSingle].rgbRed;	//红色
					pbyDdbRaw++;										//保留**
				}
			}//end j
			dwDibBaseIndex -= dwDibWidthBytes;
			dwDdbBaseIndex += dwDdbWidthBytes;
		}//end i
		return (dwDdbWidthBytes * m_nHeight);
	}// end if(m_nBitCount == 1)
	else if(m_nBitCount == 4)
	{
		//每行实际字节数
		LONG nLineBytes = (m_nWidth + 1) / 2;

		//DIB位图数据的行头
		DWORD dwDibBaseIndex = (m_nHeight - 1) * dwDibWidthBytes;
		
		//DDB 32位位图数据的行头
		DWORD dwDdbBaseIndex = 0;

		for(LONG i = 0;i < m_nHeight;i++)
		{
			//指向DIB行数据的指针
			BYTE* pbyDibRaw = m_pDibData + dwDibBaseIndex;
			
			//指向 DDB 32位位图的数据的指针
			BYTE* pbyDdbRaw = lpbyDdb32 + dwDdbBaseIndex;
			for(LONG j = 0;j < nLineBytes;j++)
			{
				//两个像素数据的索引值, 通过高四位和低四位
				BYTE byBit8 =  *(pbyDibRaw++);
				BYTE byBitHigh = (byBit8 & 0xF0) >> 4;
				BYTE byBitLow = (byBit8 & 0x0F);
				
				*(pbyDdbRaw++) = m_pRGBQuad[byBitHigh].rgbBlue;		//蓝色
				*(pbyDdbRaw++) = m_pRGBQuad[byBitHigh].rgbGreen;	//绿色
				*(pbyDdbRaw++) = m_pRGBQuad[byBitHigh].rgbRed;		//红色
				pbyDdbRaw++;

				*(pbyDdbRaw++) = m_pRGBQuad[byBitLow].rgbBlue;		//蓝色
				*(pbyDdbRaw++) = m_pRGBQuad[byBitLow].rgbGreen;		//绿色
				*(pbyDdbRaw++) = m_pRGBQuad[byBitLow].rgbRed;		//红色
				pbyDdbRaw++;

			}//end j
			dwDibBaseIndex -= dwDibWidthBytes;
			dwDdbBaseIndex += dwDdbWidthBytes;
		}//end i
		return (dwDdbWidthBytes * m_nHeight);
	}// end if(m_nBitCount == 4)
	else if(m_nBitCount == 8)
	{
		//DIB位图数据的行头
		DWORD dwDibBaseIndex = (m_nHeight - 1) * dwDibWidthBytes;
		
		//DDB 32位位图数据的行头
		DWORD dwDdbBaseIndex = 0;
		for(LONG i = 0;i < m_nHeight;i++)
		{
			//指向DIB行数据的指针
			BYTE* pbyDibRaw = m_pDibData + dwDibBaseIndex;
		
			//指向 DDB 32位位图的数据的指针
			BYTE* pbyDdbRaw = lpbyDdb32 + dwDdbBaseIndex;
			for(LONG j = 0;j < m_nWidth;j++)
			{
				//一个像素数据的索引值
				BYTE byIndex = *(pbyDibRaw++);
	
				*(pbyDdbRaw++) = m_pRGBQuad[byIndex].rgbBlue;	//蓝色
				*(pbyDdbRaw++) = m_pRGBQuad[byIndex].rgbGreen;	//绿色
				*(pbyDdbRaw++) = m_pRGBQuad[byIndex].rgbRed;	//红色
				pbyDdbRaw++;
			}//end j
			dwDibBaseIndex -= dwDibWidthBytes;
			dwDdbBaseIndex += dwDdbWidthBytes;
		}//end i
		return (dwDdbWidthBytes * m_nHeight);
	}// end if(m_nBitCount == 8)
	else if(m_nBitCount == 24)
	{
		//DIB位图数据的行头
		DWORD dwDibBaseIndex = (m_nHeight - 1) * dwDibWidthBytes;
		
		//DDB 32位位图数据的行头
		DWORD dwDdbBaseIndex = 0;
		for(LONG i = 0;i < m_nHeight;i++)
		{
						//指向DIB行数据的指针
			BYTE* pbyDibRaw = m_pDibData + dwDibBaseIndex;
		
			//指向 DDB 32位位图的数据的指针
			BYTE* pbyDdbRaw = lpbyDdb32 + dwDdbBaseIndex;
			for(LONG j = 0;j < m_nWidth;j++)
			{
				*(pbyDdbRaw++) = *(pbyDibRaw++);	//蓝色
				*(pbyDdbRaw++) = *(pbyDibRaw++);	//绿色
				*(pbyDdbRaw++) = *(pbyDibRaw++);	//红色
				pbyDdbRaw++;						
			}//end j			
			dwDibBaseIndex -= dwDibWidthBytes;
			dwDdbBaseIndex += dwDdbWidthBytes;
		}//end i
		return (dwDdbWidthBytes * m_nHeight);
	}// end if(m_nBitCount == 24)


	return (m_nWidth * m_nHeight);
}

//the return value is the number of scan lines copied.
int CDib::Draw(HDC hdc,  int XDest,  int YDest,  int nDestWidth,  int nDestHeight,  int XSrc,  int YSrc,  int nSrcWidth,  int nSrcHeight,  UINT iUsage,  DWORD dwRop)
{
	
	if (m_pRGBQuad) // Has a color table
	{
        HPALETTE hPalette = CreateBitmapPalette();
        HPALETTE hOldPalette = ::SelectPalette(hdc,  hPalette,  FALSE);
        ::RealizePalette(hdc);

		int nScanLines = StretchDIBits(hdc, 
										XDest, YDest, nDestWidth, nDestHeight, 
										XSrc, YSrc, nSrcWidth, nSrcHeight, 
										m_pDibData, m_pBitmapInfo, iUsage, dwRop);

        ::SelectPalette(hdc,  hOldPalette,  FALSE);
        ::DeleteObject(hPalette);
		return nScanLines;
	}
	else
        return StretchDIBits(hdc, 
							XDest, YDest, nDestWidth, nDestHeight, 
							XSrc, YSrc, nSrcWidth, nSrcHeight, 
							m_pDibData, m_pBitmapInfo, iUsage, dwRop);

}

HPALETTE CDib::CreateBitmapPalette()
{
	if((m_pRGBQuad == NULL) || !IsValid())return NULL;
    struct
    {
        WORD palVersion;
        WORD palNumEntries;
        PALETTEENTRY palPalEntry[256];
    } palette = { 0x300,  256 };
    
    LPRGBQUAD pRGBTable = m_pRGBQuad;

    for(UINT i = 0; i < m_nTotalColors; ++i)
    {
        palette.palPalEntry[i].peRed =   pRGBTable[i].rgbRed;
        palette.palPalEntry[i].peGreen = pRGBTable[i].rgbGreen;
        palette.palPalEntry[i].peBlue =  pRGBTable[i].rgbBlue;
        palette.palPalEntry[i].peFlags = 0;
    }

    HPALETTE hPalette = ::CreatePalette((LPLOGPALETTE)&palette);

    return hPalette;

}







