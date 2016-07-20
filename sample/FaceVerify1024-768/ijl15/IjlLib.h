#ifndef USING_IJL_LIB
#define USING_IJL_LIB


BOOL DecodeFromJPEGBuffer(
	BYTE* lpJpgBuffer,
	DWORD dwJpgBufferSize,
	BYTE** lppRgbBuffer,
	DWORD* lpdwWidth,
	DWORD* lpdwHeight,
	DWORD* lpdwNumberOfChannels);

BOOL EncodeToJPEGBuffer(
	BYTE* lpRgbBuffer,
	DWORD dwWidth,
	DWORD dwHeight,
	BYTE** lppJpgBuffer,
	DWORD* lpdwJpgBufferSize,
	int nQuality);

#endif