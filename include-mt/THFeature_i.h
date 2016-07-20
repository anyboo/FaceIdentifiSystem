#ifndef THFEATURE_I_H
#define THFEATURE_I_H
/*
* ============================================================================
*  Name     : THFeature_i.h
*  Part of  : Face Feature (THFeature) SDK
*  Created  : 10.18.2013 by xxx
*  Description:
*     THFeature_i.h -  Face Feature(THFeature) SDK header file
*  Version  : 1.0.0
*  Copyright: All Rights Reserved by XXX
*  Revision:
* ============================================================================
*/

#define THEATUREE_API

THEATUREE_API short EF_Init(int nChannelNum);
/*
 The EF_Init function will initialize the Face Feature(THFeature) algorithm module

 Parameters:
	nChannelNum,the channel number,support for muti-thread,one channel stand for one thread.max value is 255.
 Return Values:
	If the function succeeds, the return value is valid channel number.
	If the function fails, the return value is 0 or nagative;
 Remarks: 
	This function can be called one time at program initialization.
*/

THEATUREE_API int EF_Size();
/*
 The EF_Size function will return face feature size.

 Parameters:
	No parameter.
 Return Values:
	If the function succeeds, the return value is face feature size.
	If the function fails, the return value is 0;
 Remarks: 
	No remark.
*/

THEATUREE_API int EF_Extract(short nChannelID,BYTE* pBuf,int nWidth,int nHeight,int nChannel,DWORD pFacialData,BYTE* pFeature);
/*
 The EF_Extract function execute face feature extraction.

 Parameters:
	nChannelID[input],channel ID(from 0 to nChannelNum-1)
	pGray[input],point to an image buffer,BGR format.
	nWidth[input],the image width.
	nHeight[input],the image height.
	nChannel[input],image buffer channel,must be 3
	pFacialData[input],the face position rectangle data.
	pFeatures[output],the face feature buffer
 Return Values:
	If the function succeeds, the return value is 1.
	If the function fails, the return value is -1.
 Remarks:
	No remark.
 */

THEATUREE_API float EF_Compare(BYTE* pFeature1,BYTE* pFeature2);
/*
 The EF_Compare function execute two face features compare.

 Parameters:
	pFeature1[input],point to one face feature buffer.
	pFeature2[input],point to another face feature buffer.
 Return Values:
	the return value is the two face features's similarity.
 Remarks:
	No remark.
 */

THEATUREE_API void EF_Release();
/*
 The EF_Release function will release the Face Feature (THFeature) algorithm module

 Parameters:
	No parameter.
 Return Values:
	No return value.
 Remarks:
	This function can be called one time at program Un-Initialization.
*/

#endif