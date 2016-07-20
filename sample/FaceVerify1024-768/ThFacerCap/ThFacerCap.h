

#ifndef __TICATPURE_H__
#define __TICATPURE_H__


#ifndef TIEXPORT
//	#define TIEXPORT		__declspec(dllexport)
//#else
	#define TIEXPORT		__declspec(dllimport)
#endif

enum ECapType
{
	CAP_INVALID	= 0,			//	无效卡
	CAP_WDM		= 1,			//	WDM 设备
	CAP_IP		= 3,			//	Sony IP Camera
};


struct SCapParam
{
	ECapType	eType;			//	设备类型
	SIZE		szCap;			//	采集尺寸
	union
	{
		LONG	lIndex;			//	WDM  设备中的序号
		char	szIP[128];		//	Sony IP Camera 的 IP 地址
	};
	DWORD		dwSingle;		//	制式
	BYTE		bReserve[16];	//	保留
};

//	采集品质
#define		CAP_QUALITY_MINVALUE	-100
#define		CAP_QUALITY_MAXVALUE	100
#define		CAP_QUALITY_RANGE		(CAP_QUALITY_MAXVALUE - CAP_QUALITY_MINVALUE)

#define		CAP_QUALITY_BRIGHT		0x0001
#define		CAP_QUALITY_CONST		0x0002
#define		CAP_QUALITY_HUE			0x0004
#define		CAP_QUALITY_SATURA		0x0008

#define		CAP_QUALITY_SHARP   	0x0010
#define		CAP_QUALITY_GAMMA		0x0020
#define		CAP_QUALITY_COLOR		0x0040
#define		CAP_QUALITY_WBALANCE	0x0080
#define		CAP_QUALITY_BACKLIGHTC	0x0100

#define		CAP_QUALITY_COMPRESS	0x1000
#define		CAP_QUALITY_FRAMERATE	0x2000

struct SCapQuality
{
	WORD		wFlag;
	//	取值范围: -100 ~ +100, Flag: 见上 CAP_QUALITY_XXXXX
	SHORT		sBrightness;//亮度
	SHORT		sConstant;//对比度
	SHORT		sHue;//色调
	SHORT		sSaturation;//饱和度

	SHORT       sSharpness;//清晰度
	SHORT       sGamma;//伽马
	SHORT       sColorEnable;//启用颜色
	SHORT       sWhiteBalance;//白光平衡
	SHORT       sBacklightCompensation;//逆光对比
	//
	SHORT		sCompress;
	SHORT		sFrameRate;
};

#define MsgTiCapture	WM_USER + 0x100


//////////////////////////////////////////////////////////////////////////
//	Base Capture

class TIEXPORT MCapture 
{
public:
	static LONG GetCaptureNum(ECapType eType);		//	取某类型设备数量
	static MCapture* Create(SCapParam* pParam);		//	建立设备实例

public:
	MCapture(SCapParam* pParam);
	virtual ~MCapture(void);

	virtual BOOL InitCapture(void) = 0;				//	初始化设备
	virtual BOOL Play(BOOL bVideo = TRUE) = 0;		//	启动设备
	virtual void Stop(void) = 0;					//	停止设备
	virtual BOOL SetQuality(SCapQuality* pQuality) = 0;			//	设置图像质量
	virtual BOOL GetFrame(BYTE* pFrame, LONG& lFrameSize) = 0;	//	取一帧图像
	virtual BOOL DeviceConfig(int nType)=0;

protected:
	SCapParam	m_Param;

	
private:
	MCapture(MCapture& cap);	//	保留

};


#endif	//	__TICATPURE_H__
