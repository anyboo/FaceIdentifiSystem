#ifndef THCLASSAGE_I_H
#define THCLASSAGE_I_H

#ifdef THCLASSAGE_EXPORTS
#define THCLASSAGE_API __declspec(dllexport)
#else
#define THCLASSAGE_API __declspec(dllimport)
#endif

THCLASSAGE_API int THClassAgeInit(int nPara,char* pPara2);
THCLASSAGE_API int THClassAgeExecute(BYTE* pFeature,void* pResult);
THCLASSAGE_API int THClassAgeRelease();


#endif