#include "StdAfx.h"
#include "IjlLib.h"

#include "ijl.h"
//#pragma comment(lib,"ijl15\\ijl15.lib")//Dynamic Link Library
#pragma comment(lib,"ijl15\\ijl15l.lib")//Static Link Library

BOOL DecodeFromJPEGBuffer(
	BYTE* lpJpgBuffer,
	DWORD dwJpgBufferSize,
	BYTE** lppRgbBuffer,
	DWORD* lpdwWidth,
	DWORD* lpdwHeight,
	DWORD* lpdwNumberOfChannels)
{
	BOOL bres;
	IJLERR jerr;
	DWORD dwWholeImageSize;
	BYTE* lpTemp = NULL;

	JPEG_CORE_PROPERTIES jcprops;
	bres = TRUE;
	__try
	{

		jerr = ijlInit(&jcprops);
		if(IJL_OK != jerr)
		{
		bres = FALSE;
		__leave;
		}

		jcprops.JPGFile = NULL;
		jcprops.JPGBytes = lpJpgBuffer;
		jcprops.JPGSizeBytes = dwJpgBufferSize;
		jerr = ijlRead(&jcprops, IJL_JBUFF_READPARAMS);
		if(IJL_OK != jerr)
		{
			bres = FALSE;
			__leave;
		}

		switch(jcprops.JPGChannels)
		{
			case 1:
			{
				jcprops.JPGColor = IJL_G;
				jcprops.DIBColor = IJL_RGB;
				jcprops.DIBChannels = 3;
				break;
			}
			case 3:
			{
				jcprops.JPGColor = IJL_YCBCR;
				jcprops.DIBColor = IJL_RGB;
				jcprops.DIBChannels = 3;
				break;
			}
			default:
			{

				jcprops.JPGColor = IJL_OTHER;
				jcprops.DIBColor = IJL_OTHER;
				jcprops.DIBChannels = jcprops.JPGChannels;
				break;
			}
		}


		dwWholeImageSize = jcprops.JPGWidth * jcprops.JPGHeight *jcprops.DIBChannels;
		lpTemp = new BYTE [dwWholeImageSize];
		if(NULL == lpTemp)
		{
			bres = FALSE;
			__leave;
		}

		jcprops.DIBWidth = jcprops.JPGWidth;
		jcprops.DIBHeight = jcprops.JPGHeight;
		jcprops.DIBPadBytes = 0;
		jcprops.DIBBytes = lpTemp;

		jerr = ijlRead(&jcprops, IJL_JBUFF_READWHOLEIMAGE);
		if(IJL_OK != jerr)
		{
			bres = FALSE;
			__leave;
		}
	} // __try
	__finally
	{
		if(FALSE == bres)
		{
			if(NULL != lpTemp)
			{
				delete [] lpTemp;
				lpTemp = NULL;
			}
		}

		ijlFree(&jcprops);
		*lpdwWidth = jcprops.DIBWidth;
		*lpdwHeight = jcprops.DIBHeight;
		*lpdwNumberOfChannels = jcprops.DIBChannels;
		*lppRgbBuffer = lpTemp;

	} // __finally
	return bres;
}// DecodeFromJPEGBuffer()


BOOL EncodeToJPEGBuffer(
	BYTE* lpRgbBuffer,
	DWORD dwWidth,
	DWORD dwHeight,
	BYTE** lppJpgBuffer,
	DWORD* lpdwJpgBufferSize,
	int nQuality)
{
	BOOL bres;
	IJLERR jerr;
	DWORD dwRgbBufferSize;
	BYTE* lpTemp;

	JPEG_CORE_PROPERTIES jcprops;
	bres = TRUE;
	__try
	{
		jerr = ijlInit(&jcprops);
		if(IJL_OK != jerr)
		{
			bres = FALSE;
			__leave;
		}
		dwRgbBufferSize = dwWidth * dwHeight * 3;
		lpTemp = new BYTE [dwRgbBufferSize];
		if(NULL == lpTemp)
		{
			bres = FALSE;
			__leave;
		}

		jcprops.DIBWidth = dwWidth;
		jcprops.DIBHeight = dwHeight; 
		jcprops.DIBBytes = lpRgbBuffer;
		jcprops.DIBPadBytes = 0;
		jcprops.DIBChannels = 3;
		jcprops.DIBColor = IJL_RGB;
		jcprops.JPGWidth = dwWidth;
		jcprops.JPGHeight = dwHeight;
		jcprops.JPGFile = NULL;
		jcprops.JPGBytes = lpTemp;
		jcprops.JPGSizeBytes = dwRgbBufferSize;
		jcprops.JPGChannels = 3;
		jcprops.JPGColor = IJL_YCBCR;
		jcprops.JPGSubsampling = IJL_411;
		jcprops.jquality = nQuality;//75; 

		jerr = ijlWrite(&jcprops,IJL_JBUFF_WRITEWHOLEIMAGE);
		if(IJL_OK != jerr)
		{
			bres = FALSE;
			__leave;
		}
	} // __try
	__finally
	{
		if(FALSE == bres)
		{
			if(NULL != lpTemp)
			{
				delete[] lpTemp;
				lpTemp = NULL;
			}
		}
		*lppJpgBuffer = lpTemp;
		*lpdwJpgBufferSize = jcprops.JPGSizeBytes;
		ijlFree(&jcprops);
	}

	return bres;
} // EncodeToJPEGBuffer()