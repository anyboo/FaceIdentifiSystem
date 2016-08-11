#ifndef THCLASSSEX_I_H
#define THCLASSSEX_I_H

#ifdef THCLASSSEX_EXPORTS
#define THCLASSSEX_API __declspec(dllexport)
#else
#define THCLASSSEX_API __declspec(dllimport)
#endif

THCLASSSEX_API int THClassSexInit(int nPara,char* pPara2);
THCLASSSEX_API int THClassSexExecute(BYTE* pFeature,void* pResult);
THCLASSSEX_API int THClassSexRelease();


#endif