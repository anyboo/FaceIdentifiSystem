#include "Stdafx.h"
#include "public.h"

CString g_strAppPath;
CBrush g_BackBrush;

SysConfig g_SysConfig;
CPtrList g_PersonInfoList;
vector<BOOL> g_PersonTemplateEnable;
long g_nFeatureSize;
long g_nFeatureNum=Faceprint_Feature_Num;
//HWND g_hMainWnd;
BYTE* g_pEnrollFeatureBuf=NULL;
EnrollBmpBufInfo* g_pIrEnrollBmpBufInfo=NULL;
EnrollBmpBufInfo* g_pPopEnrollBmpBufInfo=NULL;

BOOL g_bDevelopMode=FALSE;
HWND g_hMainWnd;
int g_nVideoMode=1;

MCapture* m_pCap[1]={NULL};
SCapParam m_capParam[1];

CDWordArray g_SelGrayBufInfoArray;
HMODULE g_hModule;
bool g_bGoodCrop=false;
BOOL g_bHorizontalImage=FALSE;

RGBQUAD GetPixelColor(BYTE* pRgbBuf,long lWidth,long lHeight,long x,long y)
{
	RGBQUAD rgb={0,0,0,0};

	if ((x<0)||(y<0)||
		(x>=lWidth)||(y>=lHeight)) return rgb;

	BYTE* iDst = pRgbBuf + y*WIDTHBYTES(lWidth *24) + x*3;

	rgb.rgbBlue = *iDst++;
	rgb.rgbGreen= *iDst++;
	rgb.rgbRed =*iDst;

	return rgb;
}
void SetPixelColor(BYTE* pRgbBuf,long lWidth,long lHeight,long x,long y,RGBQUAD c)
{
	if ((x<0)||(y<0)||
		(x>=lWidth)||(y>=lHeight)) return;

	BYTE* iDst = pRgbBuf + y*WIDTHBYTES(lWidth *24) + x*3;
	*iDst++ = c.rgbBlue;
	*iDst++ = c.rgbGreen;
	*iDst   = c.rgbRed;

}
BYTE* Rotate(BYTE* pRgbBuf,long& nWidth,long& nHeight,float angle)
{
	BYTE* pRotateBuf=NULL;
	
	double ang = -angle*acos(0.0f)/90.0f;
	int newWidth, newHeight;
	double cos_angle = cos(ang);
	double sin_angle = sin(ang);

	// 计算旋转后位图的大小
	POINT p1={0,0};
	POINT p2={nWidth,0};
	POINT p3={0,nHeight};
	POINT p4={nWidth-1,nHeight};
	POINT newP1,newP2,newP3,newP4, leftTop, rightTop, leftBottom, rightBottom;

	newP1.x = p1.x;
	newP1.y = p1.y;
	newP2.x = (long)(p2.x*cos_angle - p2.y*sin_angle);
	newP2.y = (long)(p2.x*sin_angle + p2.y*cos_angle);
	newP3.x = (long)(p3.x*cos_angle - p3.y*sin_angle);
	newP3.y = (long)(p3.x*sin_angle + p3.y*cos_angle);
	newP4.x = (long)(p4.x*cos_angle - p4.y*sin_angle);
	newP4.y = (long)(p4.x*sin_angle + p4.y*cos_angle);

	leftTop.x = min(min(newP1.x,newP2.x),min(newP3.x,newP4.x));
	leftTop.y = min(min(newP1.y,newP2.y),min(newP3.y,newP4.y));
	rightBottom.x = max(max(newP1.x,newP2.x),max(newP3.x,newP4.x));
	rightBottom.y = max(max(newP1.y,newP2.y),max(newP3.y,newP4.y));
	leftBottom.x = leftTop.x;
	leftBottom.y = rightBottom.y;
	rightTop.x = rightBottom.x;
	rightTop.y = leftTop.y;

	newWidth = rightTop.x - leftTop.x;
	newHeight= leftBottom.y - leftTop.y;

	//将旋转后的宽度DWORD对齐
	newWidth=(newWidth/4)*4;
	rightTop.x=leftTop.x+newWidth;

	pRotateBuf=new BYTE[newWidth*newHeight*3];

	int x,y,newX,newY,oldX,oldY;

	for (y = leftTop.y, newY = 0; y<=leftBottom.y; y++,newY++)
	for (x = leftTop.x, newX = 0; x<=rightTop.x; x++,newX++)
	{
		oldX = (long)(x*cos_angle + y*sin_angle - 0.5);
		oldY = (long)(y*cos_angle - x*sin_angle - 0.5);
		RGBQUAD cr=GetPixelColor(pRgbBuf,nWidth,nHeight,oldX,oldY);
		SetPixelColor(pRotateBuf,newWidth,newHeight,newX,newY,cr);
	}

	nWidth=newWidth;
	nHeight=newHeight;

	return pRotateBuf;
}
BOOL MirrorDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BOOL bDirection,int nImageBits)
{	 
	 // 指向源图像的指针
	 LPSTR	lpSrc; 
	 // 指向要复制区域的指针
	 LPSTR	lpDst;	 
	 // 指向复制图像的指针
	 LPSTR	lpBits;
	 HLOCAL	hBits;	 
	 // 循环变量
	 LONG	i;
	 LONG	j;
	 int nBits;//每像素占的位数
	 // 图像每行的字节数
	 LONG lLineBytes;
	 // 计算图像每行的字节数
	 lLineBytes = WIDTHBYTES(lWidth *nImageBits);
	 // 暂时分配内存，以保存一行图像
	 hBits = LocalAlloc(LHND, lLineBytes);
	 if (hBits == NULL)
	 {
		 // 分配内存失败
		 return FALSE;
	 }	 
	 // 锁定内存
	 lpBits = (char * )LocalLock(hBits);
	 int nStep=nImageBits/8;
	 long lCenter=lWidth/2*nStep;
	 // 判断镜像方式
	 if (bDirection)
	 {
		 // 水平镜像
		 // 针对图像每行进行操作
		 for(i = 0; i < lHeight; i++)
		 {
			 // 针对每行图像左半部分进行操作
			 for(j = 0; j < lCenter; j+=nStep)
			 {
				 for(nBits=0;nBits<nStep;nBits++)
				 {
					 lpSrc = (char *)lpDIBBits + lLineBytes * i +lCenter- j+nBits;
					 lpDst = (char *)lpDIBBits + lLineBytes * i +lCenter+ j+nBits;
					 *lpBits = *lpDst;
					 *lpDst = *lpSrc;
					 *lpSrc = *lpBits;
				 }
				 
			 }
			 
		 }
	 }
	 else
	 {
		 // 垂直镜像
		 // 针对上半图像进行操作
		 for(i = 0; i < lHeight / 2; i++)
		 {		 
			 // 指向倒数第i行象素起点的指针
			 lpSrc = (char *)lpDIBBits + lLineBytes * i;	 
			 // 指向第i行象素起点的指针
			 lpDst = (char *)lpDIBBits + lLineBytes * (lHeight - i - 1);		 
			 // 备份一行，宽度为lWidth
			 memcpy(lpBits, lpDst, lLineBytes);
			 // 将倒数第i行象素复制到第i行
			 memcpy(lpDst, lpSrc, lLineBytes);
			 // 将第i行象素复制到倒数第i行
			 memcpy(lpSrc, lpBits, lLineBytes);
			 
		 }
	 }	 
	 // 释放内存
	 LocalUnlock(hBits);
	 LocalFree(hBits);
	 // 返回
	 return TRUE;
}

//BMP数据的RGB互换
BOOL BGRFromRGB(BYTE *buf, UINT widthPix, UINT height)
{
	if (buf==NULL)
		return FALSE;

	UINT col, row;
	for (row=0;row<height;row++) {
		for (col=0;col<widthPix;col++) {
			LPBYTE pRed, pGrn, pBlu;
			pRed = buf + row * widthPix * 3 + col * 3;
			pGrn = buf + row * widthPix * 3 + col * 3 + 1;
			pBlu = buf + row * widthPix * 3 + col * 3 + 2;

			// 交换红色和蓝色
			BYTE tmp;
			tmp = *pRed;
			*pRed = *pBlu;
			*pBlu = tmp;
		}
	}
	return TRUE;
}
void DrawBmpBuf(BITMAPINFOHEADER& bih,BYTE* pDataBuf,HWND hShowWnd,BOOL bFitWnd)
{
	RECT rc;
	::GetWindowRect( hShowWnd, &rc );
	int nWidth = rc.right - rc.left-2;
	int nHeight = rc.bottom - rc.top-2;

	BITMAPINFO bi;
	memset(&bi,0,sizeof(bi));
	memcpy( &(bi.bmiHeader), &bih, sizeof(BITMAPINFOHEADER) );
	int iWidth=bih.biWidth;
	int iHeight=bih.biHeight;

	// display bitmap
	HDC hdcStill = ::GetDC( hShowWnd );
	PAINTSTRUCT ps;
	::BeginPaint( hShowWnd, &ps );


	::SetStretchBltMode( hdcStill, COLORONCOLOR );

	if(bFitWnd)
		::StretchDIBits( hdcStill, 0, 0, nWidth, nHeight,
			0, 0, iWidth, iHeight, pDataBuf, &bi,
			DIB_RGB_COLORS,	SRCCOPY );
	else
		::StretchDIBits( hdcStill, 0, 0, iWidth, iHeight,
			0, 0, iWidth, iHeight, pDataBuf, &bi,
			DIB_RGB_COLORS,	SRCCOPY );


	::EndPaint( hShowWnd, &ps );
	::ReleaseDC( hShowWnd, hdcStill );
}
void DrawBmpBuf1(BITMAPINFOHEADER& bih,BYTE* pDataBuf,HWND hShowWnd,BOOL bFitWnd,const char* pTopText,COLORREF clTopText)
{
	RECT rc;
	::GetWindowRect( hShowWnd, &rc );
	int nWidth = rc.right - rc.left-2;
	int nHeight = rc.bottom - rc.top-2;

	BITMAPINFO bi;
	memset(&bi,0,sizeof(bi));
	memcpy( &(bi.bmiHeader), &bih, sizeof(BITMAPINFOHEADER) );
	int iWidth=bih.biWidth;
	int iHeight=bih.biHeight;

	// display bitmap
	HDC hdcStill = ::GetDC( hShowWnd );
	PAINTSTRUCT ps;
	::BeginPaint( hShowWnd, &ps );


	::SetStretchBltMode( hdcStill, COLORONCOLOR );

	if(bFitWnd)
		::StretchDIBits( hdcStill, 0, 0, nWidth, nHeight,
			0, 0, iWidth, iHeight, pDataBuf, &bi,
			DIB_RGB_COLORS,	SRCCOPY );
	else
		::StretchDIBits( hdcStill, 0, 0, iWidth, iHeight,
			0, 0, iWidth, iHeight, pDataBuf, &bi,
			DIB_RGB_COLORS,	SRCCOPY );
	if(1)
	{
		::SetBkMode( hdcStill, TRANSPARENT );

		HBRUSH hBrush=(HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH hOldBrush=(HBRUSH)::SelectObject(hdcStill,hBrush);
		HPEN hPen=CreatePen(PS_DOT,1,RGB(0,255,255));
		HPEN hOldPen=(HPEN)::SelectObject(hdcStill,hPen);

		int w=240;
		int h=320;

		::MoveToEx(hdcStill,0,nHeight/2,NULL);
		::LineTo(hdcStill,nWidth-1,nHeight/2);

		::MoveToEx(hdcStill,nWidth/2,0,NULL);
		::LineTo(hdcStill,nWidth/2,nHeight-1);

		::Rectangle(hdcStill,nWidth/2-w/2,nHeight/2-h/2,nWidth/2+w/2,nHeight/2+h/2);

		::SelectObject(hdcStill,hOldPen);
		::SelectObject(hdcStill,hOldBrush);
		::DeleteObject(hPen);
		::DeleteObject(hBrush);
	}

	if(pTopText&&strlen(pTopText)>0)//top tip text
	{
		HFONT hFont, hOldFont;

		hFont = CreateFont( 36, 0, 0, 0, 
		FW_THIN, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_MODERN, TEXT("Arial") );

		HBRUSH hBrush=(HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH hOldBrush=(HBRUSH)::SelectObject(hdcStill,hBrush);
		hOldFont = (HFONT)::SelectObject( hdcStill, hFont );
		::SetTextColor( hdcStill, clTopText);
		::SetBkMode( hdcStill, TRANSPARENT );
		::TextOut( hdcStill, 0, 6, pTopText, strlen(pTopText));

		::SelectObject(hdcStill,hOldBrush);
		::SelectObject( hdcStill, hOldFont );
		::DeleteObject(hBrush);
		::DeleteObject( hFont );
	}
	::EndPaint( hShowWnd, &ps );
	::ReleaseDC( hShowWnd, hdcStill );
}
void ContructBih(int nWidth,int nHeight,BITMAPINFOHEADER& bih)
{
	bih.biSize=40; 						// header size
	bih.biWidth=nWidth;
	bih.biHeight=nHeight;
	bih.biPlanes=1;
	bih.biBitCount=24;					// RGB encoded, 24 bit
	bih.biCompression=BI_RGB;			// no compression
	bih.biSizeImage=nWidth*nHeight*3;
	bih.biXPelsPerMeter=0;
	bih.biYPelsPerMeter=0;
	bih.biClrUsed=0;
	bih.biClrImportant=0;
}

HBITMAP DrawCrossEyes(BYTE* image, POINT LEye,POINT REye,int nWidth,int nHeight,COLORREF color,int nPenWidth,int nCrossSize)
{
//构造RGB24的BMP头信息
	BITMAPINFO bi;
	BITMAPINFOHEADER bih;
	memset(&bih,0,sizeof(bih));
	memset( &bi, 0, sizeof(bi) );
	bih.biSize=40; 				
	bih.biWidth=nWidth;
	bih.biHeight=nHeight;
	bih.biPlanes=1;
	bih.biBitCount=24;	
	bih.biCompression=BI_RGB;
	bih.biSizeImage=nWidth*nHeight*3;
	bih.biXPelsPerMeter=0;
	bih.biYPelsPerMeter=0;
	bih.biClrUsed=0;
	bih.biClrImportant=0;
	memcpy( &(bi.bmiHeader), &bih, sizeof(BITMAPINFOHEADER) );
//取设备DC
	HDC hDC=::GetDC(NULL);
//创建兼容的内存DC
	HDC hMemDC=CreateCompatibleDC(NULL);
//创建兼容位图并将其选入内存DC
	HBITMAP hBmp=CreateCompatibleBitmap(hDC,nWidth,nHeight);
	HBITMAP obmp = (HBITMAP) SelectObject(hMemDC, hBmp); 
//创建GDI对象并选入内存DC
	HPEN hPen=CreatePen(PS_SOLID,nPenWidth,color);
	HBRUSH hOldBrush=(HBRUSH)::SelectObject(hMemDC,GetStockObject(NULL_BRUSH));
	HPEN hOldPen=(HPEN)::SelectObject(hMemDC,hPen);
//将位图数据复制到内存DC
	SetStretchBltMode( hMemDC, COLORONCOLOR );
	StretchDIBits(hMemDC, 0, 0, nWidth, nHeight,
		0, 0, nWidth, nHeight, image, &bi,
		DIB_RGB_COLORS,	SRCCOPY );	
//画左眼
	int nTemp=LEye.y-nCrossSize;
	if(nTemp<0) nTemp=0;
	::MoveToEx(hMemDC,LEye.x,nTemp,NULL);
	::LineTo(hMemDC,LEye.x,LEye.y+nCrossSize);
	nTemp=LEye.x-nCrossSize;
	if(nTemp<0) nTemp=0;
	::MoveToEx(hMemDC,nTemp,LEye.y,NULL);
	::LineTo(hMemDC,LEye.x+nCrossSize,LEye.y);
//画右眼
	nTemp=REye.y-nCrossSize;
	if(nTemp<0) nTemp=0;
	::MoveToEx(hMemDC,REye.x,REye.y-nCrossSize,NULL);
	::LineTo(hMemDC,REye.x,REye.y+nCrossSize);
	nTemp=REye.x-nCrossSize;
	if(nTemp<0) nTemp=0;
	::MoveToEx(hMemDC,REye.x-nCrossSize,REye.y,NULL);
	::LineTo(hMemDC,REye.x+nCrossSize,REye.y);
//取出新的位图句柄
	memset(image,0,bih.biSizeImage);
	hBmp=(HBITMAP) SelectObject(hMemDC, obmp);
//得到新的位图数据
	int hr=GetDIBits(hMemDC,hBmp,0,nHeight,image,&bi,DIB_RGB_COLORS);

	::SelectObject(hMemDC,hOldPen);
	::SelectObject(hMemDC,hOldBrush);

//释放DC和GDI资源
	::DeleteDC(hMemDC);
	::DeleteObject(hPen);
	::ReleaseDC(NULL,hDC);

//	::DeleteObject(hBmp);
	
	return hBmp;//调用方需调用::DeleteObject(hBmp)来释放资源
}

void DrawFaceRects(BYTE* pRgbBuf,int nBufWidth,int nBufHeight,
				   RECT* pFaceRects,int nRectNum,
				   COLORREF color,int nPenWidth)
{
	if(pRgbBuf==NULL||pFaceRects==NULL||nRectNum==0||nPenWidth==0) return;
	RECT rc;
	int rcWidth,rcHeight;
	int widthBytes=nBufWidth*3;
	BYTE* image=pRgbBuf;

	int i,j,k;
	for(i=0;i<nRectNum;i++)
	{
		rc=pFaceRects[i];
		rcWidth=rc.right-rc.left;
		rcHeight=rc.bottom-rc.top;

		for(j=rc.left;j<rc.right;j++)
		{
			for(int a=0;a<nPenWidth;a++)
			{
				image[j*3+(rc.top+a)*widthBytes+0]=GetBValue(color);
				image[j*3+(rc.top+a)*widthBytes+1]=GetGValue(color);
				image[j*3+(rc.top+a)*widthBytes+2]=GetRValue(color);
			}
		}
		for(j=rc.left;j<rc.right;j++)
		{
			for(int a=0;a<nPenWidth;a++)
			{
				image[j*3+(rc.bottom-a)*widthBytes+0]=GetBValue(color);
				image[j*3+(rc.bottom-a)*widthBytes+1]=GetGValue(color);
				image[j*3+(rc.bottom-a)*widthBytes+2]=GetRValue(color);
			}
		}
		for(k=rc.top;k<rc.bottom;k++)
		{
			for(int a=0;a<nPenWidth;a++)
			{
				image[(rc.left+a)*3+k*widthBytes+0]=GetBValue(color);
				image[(rc.left+a)*3+k*widthBytes+1]=GetGValue(color);
				image[(rc.left+a)*3+k*widthBytes+2]=GetRValue(color);
			}
		}
		for(k=rc.top;k<rc.bottom;k++)
		{
			for(int a=0;a<nPenWidth;a++)
			{
				image[(rc.right-a)*3+k*widthBytes+0]=GetBValue(color);
				image[(rc.right-a)*3+k*widthBytes+1]=GetGValue(color);
				image[(rc.right-a)*3+k*widthBytes+2]=GetRValue(color);
			}
		}
	}
}

//good,image=rgb24加载出来的缓冲，不用变换
void GetPartImage( BYTE* image, int imageWidth, int imageHeight, BYTE* part,  int left,int top, int right ,int bottom )
{
	int w = right - left;
	int h = bottom - top;

//	UINT nSize=((imageWidth*(24/8)+3)&~3);
	UINT nSize=imageWidth*3;
	for( int i=0; i<h; i++ )
	{
		for( int j=0; j<w; j++ )
		{
			part[j*3+i*w*3+0] = image[(left+j)*3+(top+i)*nSize+0];
			part[j*3+i*w*3+1] = image[(left+j)*3+(top+i)*nSize+1];
			part[j*3+i*w*3+2] = image[(left+j)*3+(top+i)*nSize+2];
		}
	}
}
//对位图重新采样(利用最近象素点RGB算法),暂时只适用于24位真彩位图
BOOL ResampleBmp(BITMAPINFOHEADER& bih, BYTE* inBuf,int newx,int newy,BYTE* outBuf)
{
	if (newx==0 || newy==0) return FALSE;

	if (bih.biWidth==newx && bih.biHeight==newy){
		if(outBuf)
		{
			memcpy(outBuf,inBuf,bih.biSizeImage);
		}
		return TRUE;
	}

	float xScale, yScale, fX, fY;
	xScale = (float)bih.biWidth  / (float)newx;
	yScale = (float)bih.biHeight / (float)newy;

	//DWORD对齐的扫描行的字节宽度
	int dwEffWidth1 = ((((bih.biBitCount * bih.biWidth) + 31) / 32) * 4);

	bih.biWidth=newx;
	bih.biHeight=newy;
	

	int dwEffWidth2 = ((((bih.biBitCount * bih.biWidth) + 31) / 32) * 4);

	for(long y=0; y<newy; y++)
	{
		fY = y * yScale;
		for(long x=0; x<newx; x++)
		{
			fX = x * xScale;
			//得到源位图的每个象素点的RGB 
			BYTE* iSrc  = inBuf + (int)fY*dwEffWidth1 + (int)fX*3; 
			RGBQUAD rgb;
			rgb.rgbBlue = *iSrc++;
			rgb.rgbGreen= *iSrc++;
			rgb.rgbRed  = *iSrc;
			//设置目标位图的每个象素点的RGB
			BYTE* iDst = outBuf + y*dwEffWidth2 + x*3;
			*iDst++ = rgb.rgbBlue;
			*iDst++ = rgb.rgbGreen;
			*iDst   = rgb.rgbRed;
		}
	}
	bih.biSizeImage=dwEffWidth2*bih.biHeight;
	return TRUE;
}
BOOL GetCBitmap(BITMAPINFOHEADER* pBih, BYTE* pDataBuf,CBitmap &bmp)
{
	BITMAPINFO bi;
	memset( &bi, 0, sizeof(bi) );
	memcpy( &(bi.bmiHeader), pBih, sizeof(BITMAPINFOHEADER) );

//	HDC hDC=::GetDC(g_hBspWnd);
	CClientDC dc(NULL);
	HBITMAP hBmp =::CreateDIBitmap(dc.m_hDC,		
				pBih,	
				CBM_INIT,	
				pDataBuf,	
				&bi,	
				DIB_RGB_COLORS);	
	bmp.Attach( hBmp );
	return TRUE;
}

BOOL SaveBufToBmpFile( BYTE* buf, LPBITMAPINFOHEADER lpBih, LPCSTR strFileName )
{

	DWORD BeWrite=0;
	HANDLE hFile;
	hFile=CreateFile(strFileName,GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_ARCHIVE,NULL);
	if(hFile==INVALID_HANDLE_VALUE)	
	{

		return FALSE;
	}	
	
	BITMAPFILEHEADER bfh;
	memset( &bfh, 0, sizeof( bfh ) );
	bfh.bfSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+lpBih->biSizeImage;
	bfh.bfType = 0x4d42;
	bfh.bfOffBits = sizeof( BITMAPINFOHEADER ) + sizeof( BITMAPFILEHEADER );

	WriteFile(hFile,&bfh,sizeof(bfh),&BeWrite,NULL);
	WriteFile(hFile,lpBih,sizeof(BITMAPINFOHEADER),&BeWrite,NULL);
	WriteFile(hFile,buf,lpBih->biSizeImage,&BeWrite,NULL);

	CloseHandle(hFile);

	return TRUE;
}

BYTE* LoadBmpFromFile(LPCTSTR strFileName,BITMAPINFOHEADER& bih)
{

	HANDLE bmpFile;
	bmpFile = CreateFile( strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( bmpFile==INVALID_HANDLE_VALUE )
		return NULL;

	DWORD dwRead;
	BITMAPFILEHEADER bfh;
	ReadFile( bmpFile, &bfh, sizeof(BITMAPFILEHEADER), &dwRead, NULL );
	if( bfh.bfType != 0x4d42 )
	{
		CloseHandle(bmpFile);
		return NULL;
	}

	ReadFile( bmpFile, &bih, sizeof(BITMAPINFOHEADER), &dwRead, NULL );
	if( bih.biBitCount != 24 )
	{
		CloseHandle( bmpFile );
		return NULL;
	}

	UINT bmpSize = ((bih.biWidth*(bih.biBitCount/8)+3)&~3)*bih.biHeight;
	BYTE* imageBuf = new BYTE[bmpSize];
	ReadFile( bmpFile, imageBuf, bmpSize, &dwRead, NULL );

	bih.biSizeImage=bmpSize;

	CloseHandle(bmpFile);
    return imageBuf;
}

void DeleteDirFile(CString sPath)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(sPath + "\\*.*",&fd); 
	if (hFind != INVALID_HANDLE_VALUE)
	{            
		  while (::FindNextFile(hFind,&fd))
		  {
				 //判断是否为目录
				 if (fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
				 {
						CString name;
						name = fd.cFileName;
						//判断是否为.和..
						if ((name != ".") && (name != ".."))
						{
							  //如果是真正的目录，进行递归
							   DeleteDirFile(sPath + "\\"+fd.cFileName);
						}
				 }
				 else
						DeleteFile(sPath + "\\"+fd.cFileName);
		  }
		  ::FindClose(hFind);
	}
	RemoveDirectory(sPath);
}


void SaveBmp8File(CString szFileName, BYTE * srcImage, int Width, int Height)
{
	int i;
	BITMAPFILEHEADER bmfHdr;

	int nImgHeight=(Width+3)/4*4;
	int imageSize=nImgHeight*Height;
	
	BYTE *saveImage=new BYTE[imageSize];
	memset(saveImage,0,imageSize);
	for(i=0;i<Height;i++)
		memcpy(saveImage+i*nImgHeight,srcImage+i*Width,Width);

	bmfHdr.bfType=0x4d42; 
    bmfHdr.bfSize=imageSize+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD); 
    bmfHdr.bfReserved1=0; 
    bmfHdr.bfReserved2=0; 
    bmfHdr.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD); 

	BITMAPINFO *pInfo=(BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD));
	pInfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER); 
    pInfo->bmiHeader.biWidth=Width; 
    pInfo->bmiHeader.biHeight=Height; 
    pInfo->bmiHeader.biPlanes=1; 
    pInfo->bmiHeader.biBitCount=8; 
    pInfo->bmiHeader.biCompression=BI_RGB; 
    pInfo->bmiHeader.biSizeImage=imageSize; 
    pInfo->bmiHeader.biXPelsPerMeter=0; 
    pInfo->bmiHeader.biYPelsPerMeter=0; 
    pInfo->bmiHeader.biClrUsed=0; 
    pInfo->bmiHeader.biClrImportant=0; 

  	for(i=0;i<256;i++)
	{
		pInfo->bmiColors[i].rgbBlue=i;
		pInfo->bmiColors[i].rgbGreen=i;
		pInfo->bmiColors[i].rgbRed=i;
		pInfo->bmiColors[i].rgbReserved=0;
	}

	FILE* mFile =fopen(szFileName,"wb");
	fwrite((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER),1,mFile);
    fwrite(pInfo,sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD),1, mFile);
	fwrite(saveImage,sizeof(BYTE),imageSize,mFile); 
	fclose(mFile);
	
	free(pInfo);
	delete[]saveImage;
}


void _DebugInfo(LPCSTR szLogFile,LPCSTR szInfo, ...)
{
	FILE*	file = fopen(szLogFile, "a+");
	if (!file)
	{
		return;
	}

	if(0)
	{
	TCHAR		szTime[MAX_PATH] = { NULL };
	SYSTEMTIME	stTime = { NULL };
	GetLocalTime(&stTime);
	_stprintf(szTime, "%04d-%02d-%02d %02d:%02d:%02d -> ",
		stTime.wYear, stTime.wMonth, stTime.wDay,
		stTime.wHour, stTime.wMinute, stTime.wSecond);
	fwrite(szTime, _tcslen(szTime), 1, file);
	}
	
	TCHAR	szOut[MAX_PATH] = { NULL };
	
	va_list	marker;
	va_start(marker, szInfo);
	vsprintf(szOut, szInfo, marker);
	va_end(marker);
	
	_tcscat(szOut, "\r\n");
	fwrite(szOut, 1, _tcslen(szOut), file);
	fclose(file);
}

LPVOID LoadDataResource(LPCTSTR lpResName,const char *name)
{
	HMODULE ghmodule=GetModuleHandle(name);
	if(ghmodule==NULL)
	{
		AfxMessageBox("GetModuleHandle fail");
		return NULL;
	}
	else
	{
	//	 AfxMessageBox(name);
	}
	HRSRC hr=FindResource(ghmodule,lpResName,"MODEL");
	if(hr==NULL)
	{
		int error=GetLastError();
		CString str;
		str.Format("error=%d",error);
		AfxMessageBox(str);
		return FALSE;
	}
	HGLOBAL hg=LoadResource(ghmodule,hr);
	if(hg==NULL)
	{
		FreeResource(hr);
		return FALSE;
	}
	LPVOID pv=(PSZ)LockResource(hg);
	return pv;

}
void RotateRgb(unsigned char* pRgbBuf,int nWidth,int nHeight,short nRgbChannel,int angle)
{
	if(nRgbChannel!=1 && nRgbChannel!=3 ) return;
	if(angle!=90 && angle !=270) return;
	if(nWidth<1 || nHeight <1 ) return ;
		
	int nImageSize=nWidth*nHeight*nRgbChannel;
	unsigned char* pTmpBuf=new BYTE[nWidth*nHeight*nRgbChannel];
	memcpy(pTmpBuf,pRgbBuf,nImageSize);

	if(angle==90)
	{
		for(int i=0;i<nWidth;++i)
		{
			for(int j=0;j<nHeight;++j)
			{	
				for(int k=0;k<nRgbChannel;++k)
				{
					pRgbBuf[ ( (nWidth-1-i)*nHeight+j ) * nRgbChannel + k ]=pTmpBuf[ (nWidth*j+i) * nRgbChannel + k ];//90
				}
			}
		}
	}
	else if(angle==270)
	{
		for(int i=0;i<nWidth;++i)
		{
			for(int j=0;j<nHeight;++j)
			{
				for(int k=0;k<nRgbChannel;++k)
				{
					pRgbBuf[ (i*nHeight+nHeight-1-j) * nRgbChannel + k ]=pTmpBuf[ (nWidth*j+i) * nRgbChannel +k ];//270
				}
			}
		}
		
	}
	else
	{
	}

	delete []pTmpBuf;
}

BOOL WriteIDPhoto(Mat* pMatID,const char* sID)
{
	char sPhotoFile[256]="";
	//write id photo
	if(pMatID && strlen(sID) )
	{
		CreateDirectory(g_strAppPath+"\\id_logs",NULL);
		CreateDirectory(g_strAppPath+"\\id_logs\\ID_Photos",NULL);

		sprintf(sPhotoFile,"%s\\id_logs\\ID_Photos\\%s.jpg",
			(LPCTSTR)g_strAppPath,sID);

		FILE* f=fopen(sPhotoFile,"rb");
		if(f==NULL)//file not exist,write it
		{
			imwrite(sPhotoFile,*pMatID);
		}
		else//file exist
		{
		}
		return TRUE;
	}
	else
		return FALSE;
}

BOOL WriteLog(Log_Info li,Mat* pMatLog)
{
	CreateDirectory(g_strAppPath+"\\id_logs",NULL);
	
	CString strLogDir;
	strLogDir.Format("%s\\id_logs\\%04d-%02d-%02d",g_strAppPath,li.logTime.wYear, li.logTime.wMonth, li.logTime.wDay);
	CreateDirectory(strLogDir,NULL);

	CString strLogFile;
	strLogFile.Format("%s\\id_logs\\%04d-%02d-%02d\\%04d-%02d-%02d.dat",
		g_strAppPath,
		li.logTime.wYear, li.logTime.wMonth, li.logTime.wDay,
		li.logTime.wYear, li.logTime.wMonth, li.logTime.wDay);

	//write log file
	CFile file;
	if(file.Open(strLogFile,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite,NULL))
	{
		file.SeekToEnd();
		file.Write(&li,sizeof(li));
		file.Close();
	}
	else
	{
		return FALSE;
	}
	char sPhotoFile[256]="";

	//write log photo
	if(pMatLog)
	{
		sprintf(sPhotoFile,"%s\\id_logs\\%04d-%02d-%02d\\%04d%02d%02d%02d%02d%02d%04d.jpg",
			(LPCTSTR)g_strAppPath,
			li.logTime.wYear, li.logTime.wMonth, li.logTime.wDay,
			li.logTime.wYear, li.logTime.wMonth, li.logTime.wDay,
			li.logTime.wHour, li.logTime.wMinute, li.logTime.wSecond,li.logTime.wMilliseconds);
		
		imwrite(sPhotoFile,*pMatLog);
	}

	return TRUE;
}

bool IsExpired()
{
//	return false;
	
	CTime t=CTime::GetCurrentTime();

	if(t.GetYear()==2015 && 
		t.GetMonth()<=6)
	{
		return false;
	}
	else
	{
		return true;
	}
}


