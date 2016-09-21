
#ifndef _STD_DEF_EX_H
#define _STD_DEF_EX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <math.h>
#include <assert.h>

#if defined(WIN32) || defined(OS_WINDOWS) // || defined(SDK_WIN32)
	//#if defined(_USRDLL) || defined(_AFXDLL) || defined(_LIB)
	//	#include <WTYPES.H>
    //#else
	//#endif
#elif defined(OS_LINUX)// || defined(SDK_LINUX) || defined(CHIP_ARM)
	#include <fcntl.h>
	#include <pthread.h>
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/ioctl.h>
	#include <sys/poll.h>
	#include <sys/mman.h> 
	#include <sys/time.h>
#elif defined(SDK_DAVINCI)
//	#include <xdc/std.h>
//	#include <ti/xdais/dm/ividenc.h>
//	#include <ti/sdo/ce/trace/gt.h>
#endif

#ifdef __cplusplus
extern "C" {  // 本声明一定要放置于所有 #include 之后，否则编译器将所有 #include 头文件都进行 c 编译链接，导致编译错误。比如 math.h 在VC环境下是禁止 C 编译链接的，会报编译错误。
#endif

//// 格式定义
enum _IMAGE_FORMAT
{
	IMAGE_GRAY,
	IMAGE_RGB,
	IMAGE_BGR,
	IMAGE_YUV420,
	IMAGE_YUV422,
};

typedef int IMAGE_FORMAT;

//// 全局常量定义
#ifndef QCIF_WIDTH
	#define QCIF_WIDTH		176
#endif

#ifndef QCIF_HEIGHT
	#define QCIF_HEIGHT		144
#endif

#ifndef CIF_WIDTH
	#define CIF_WIDTH		352
#endif

#ifndef CIF_HEIGHT
	#define CIF_HEIGHT		288
#endif 

#ifndef D1_WIDTH
	#define D1_WIDTH		704
#endif 

#ifndef D1_HEIGHT
	#define D1_HEIGHT		576
#endif

#ifndef SIF_WIDTH
	#define SIF_WIDTH		320
#endif

#ifndef SIF_HEIGHT
	#define SIF_HEIGHT		240
#endif

#ifndef VGA_WIDTH
	#define VGA_WIDTH		640
#endif

#ifndef VGA_HEIGHT
	#define VGA_HEIGHT		480
#endif

#define FI_QCIF_WIDTH		176
#define FI_QCIF_HEIGHT		144

#define FI_CIF_WIDTH		352
#define FI_CIF_HEIGHT		288

#define FI_D1_WIDTH			704
#define FI_D1_HEIGHT		576

#define FI_SIF_WIDTH		320
#define FI_SIF_HEIGHT		240

#define FI_VGA_WIDTH		640
#define FI_VGA_HEIGHT		480

#if !(defined(OS_WINDOWS) || defined(SDK_WIN32) || defined(WIN32))
	#define S_OK 			0x00000000 	// 成功 
	#define S_FALSE 		0x00000001 	// 函数成功执行完成，但返回时出现错误 
	#define E_INVALIDARG 	0x80070057 	// 参数有错误 
	#define E_OUTOFMEMORY 	0x8007000E 	// 内存申请错误 
	#define E_UNEXPECTED 	0x8000FFFF 	// 未知的异常 
	#define E_NOTIMPL 		0x80004001 	// 未实现功能 
	#define E_FAIL 			0x80004005 	// 没有详细说明的错误。一般需要取得 Rich Error 错误信息(注1) 
	#define E_POINTER 		0x80004003 	// 无效的指针 
	#define E_HANDLE 		0x80070006 	// 无效的句柄 
	#define E_ABORT 		0x80004004 	// 终止操作 
	#define E_ACCESSDENIED 	0x80070005 	// 访问被拒绝 
	#define E_NOINTERFACE 	0x80004002 	// 不支持接口 
#endif

#define MAX_ROI_NUM 		80000	/* 最大 ROI个数 */
#define MAX_OBJ_NUM			100		/* 最大对象个数 */
#define MAX_TRK_NUM			256		/* 最大轨迹长度 */
#define MAX_VTX_NUM			50		/* 最大顶点个数 */
#define MAX_GATE			50		/* 最大绊线个数 */

#define MVTX			  		10		/* 凸多边形最大顶点个数 */
#define MRULE						10		/* 最大规则个数 */


// 穿越行为
enum ACT_ACROSS_ENUM
{
	ACT_ACROSS_NULL	= 0,			// 空动作
	ACT_ACROSS_LEFT	= 1,			// 从左边穿越绊线（到右边）
	ACT_ACROSS_RIGHT	= 2,			// 从右边穿越绊线（到左边）
};

// 出入行为
enum ACT_INOUT_ENUM
{
	ACT_INOUT_NULL	= 0,			// 空动作
	ACT_INOUT_IN		= 1,			// 走进防区
	ACT_INOUT_OUT		= 2,			// 走出防区
};

// 方向行为
enum ACT_DIRECTION_ENUM
{
	ACT_DIRECTION_NULL	= 0,			// 空动作
	ACT_DIRECTION_FORWARD	= 1,		// 正向运动
	ACT_DIRECTION_REVERSE	= 2,		// 逆向运动
};

// 异常行为
enum ACT_DYSTROPY_ENUM
{
	ACT_DYSTROPY_NULL	= 0,			// 空动作
	ACT_DYSTROPY_LINGER	= 1,		// 徘徊运动
};

// 存在行为
enum ACT_EXIST_ENUM
{
	ACT_EXIST_NULL	= 0,			// 空动作
	ACT_EXIST_LEAVE	= 1,			// 遗留物体
	ACT_EXIST_LOSE	= 2,			// 遗失物体
};

///////////////////////////////////////////////////////////////////////////////////////
// 基本类型定义
///////////////////////////////////////////////////////////////////////////////////////
#if defined(WIN32) || defined(OS_WINDOWS) || defined(SDK_WIN32)

	typedef char			*LPCHAR;

#else
	
	#ifndef BOOL
		typedef int				BOOL;
	#endif
	
	#ifndef BYTE
		typedef unsigned char	BYTE;
	#endif
	
	#ifndef SHORT
		typedef unsigned short	SHORT;
	#endif
	
	#ifndef UINT
		typedef unsigned int	UINT;
	#endif 
	
	#ifndef WORD
		typedef unsigned short	WORD;
	#endif
	
	#ifndef DWORD
		typedef unsigned int	DWORD;
	#endif
	
	#ifndef LONG
		typedef long			LONG;
	#endif
	
	#ifndef LPCHAR
		typedef char			*LPCHAR;
	#endif
	
	#ifndef LPBYTE
		typedef unsigned char   *LPBYTE;
	#endif	
		
	#ifndef HANDLE
		typedef void			*HANDLE;
	#endif

	#ifndef FALSE
		#define FALSE	0
	#endif
	
	#ifndef TRUE
		#define TRUE	1
	#endif
	
	#ifndef NULL
		#define NULL	0
	#endif
	
	#ifndef IN
		#define IN
	#endif
	
	#ifndef OUT
		#define OUT
	#endif
#endif

typedef void *XPOS;

typedef void *FI_HANDLE;

typedef XPOS XPOSITION;
typedef XPOS FI_XPOS;

typedef void  *PTR;
typedef PTR		XPTR;
typedef PTR		FI_PTR;

#define HANDLE_INT int

typedef char			_INT8;
typedef unsigned char	_UINT8;
typedef short			_INT16;
typedef int				_INT32;

#if defined(WIN32)
	typedef __int64		_INT64;
	typedef __int64		INT64; 
#else
	typedef long long	_INT64;
	typedef long long	INT64;
#endif

#if defined(WIN32)
	#define INLINE		__inline
#elif defined(SDK_DAVINCI)
	#define INLINE      inline
#else
	#define INLINE		inline
#endif

typedef int 			FI_BOOL;
typedef unsigned char 	FI_BYTE;
typedef unsigned short 	FI_SHORT;
typedef int 						FI_INT;
typedef unsigned int 	FI_UINT;
typedef unsigned short 	FI_WORD;
typedef unsigned int 	FI_DWORD;
//typedef long 			FI_LONG;
//typedef unsigned long FI_ULONG;

#define FI_FALSE 0
#define FI_TRUE  1
#define FI_NULL  0

#if defined(WIN32)
	typedef __int64		FI_INT64; 
#else
	typedef long long	FI_INT64;
#endif

///////////////////////////////////////////////////////////////////////////////////////
// 常用类型定义
///////////////////////////////////////////////////////////////////////////////////////
// 非 WIN32 操作系统
#if defined(WIN32) || defined(OS_WINDOWS) || defined(SDK_WIN32)

	typedef POINT SDE_POINT;
	typedef SIZE  SDE_SIZE;
	typedef RECT  SDE_RECT;

#else
	typedef struct _SDE_POINT
	{
		short x;
		short y;
	}SDE_POINT;

	typedef struct _SDE_SIZE
	{
		short cx;
		short cy;
	}SDE_SIZE;

	typedef struct _SDE_RECT
	{
		short left;
		short top;
		short right;
		short bottom;
	}SDE_RECT;

	typedef SDE_POINT   POINT;
	typedef SDE_SIZE	SIZE;
	typedef SDE_RECT	RECT;

	typedef SDE_POINT   *LPPOINT;
	typedef SDE_SIZE    *LPSIZE;
	typedef SDE_RECT    *LPRECT;

#endif


// 外部定义类型
typedef SDE_POINT   FI_POINT;
typedef SDE_SIZE	FI_SIZE;
typedef SDE_RECT	FI_RECT;

typedef SDE_POINT   *LPFI_POINT;
typedef SDE_SIZE    *LPFI_SIZE;
typedef SDE_RECT    *LPFI_RECT;


///////////////////////////////////////////////////////////////////////////////////////
// 特定类型定义
///////////////////////////////////////////////////////////////////////////////////////
typedef struct _C4
{
	char c1, c2, c3, c4;
}C4;

typedef struct _UC4
{
	unsigned char c1, c2, c3, c4;
}UC4;

typedef struct _C8
{
	char c1, c2, c3, c4, c5, c6, c7, c8;
}C8;

typedef struct _UC8
{
	unsigned char c1, c2, c3, c4, c5, c6, c7, c8;
}UC8;

typedef struct _SHORT2
{
	short s1, s2;
}SHORT2;

typedef struct _SHORT4
{
	short s1, s2, s3, s4;
}SHORT4;

///////////////////////////////////////////////////////////////////////////////////////
// 几何类型定义
///////////////////////////////////////////////////////////////////////////////////////

// 方向线
typedef struct _SDE_LINE
{
	SDE_POINT	start;			// 线段起始点
	SDE_POINT	end;			// 线段终止点
}SDE_LINE;

// 双方向线
typedef struct _SDE_PAIRLINE
{
	SDE_LINE	line1;			// 矢量1
	SDE_LINE	line2;			// 矢量2
}SDE_PAIRLINE;

// 任意矩形
typedef struct _SDE_RECTX
{
	SDE_POINT lt;		/* 左上角点 */
	SDE_POINT rt;		/* 右上角点 */
	SDE_POINT rb;		/* 右下角点 */
	SDE_POINT lb;		/* 左下角点 */
}SDE_RECTX;

// 圆形
typedef struct _SDE_CIRCLE
{
	SDE_POINT  center;			// 圆形中心
	int	       radius;			// 圆形半径
}SDE_CIRCLE;

// 折线
typedef struct _SDE_CURVELINE
{
	int        count;									// 折线节点个数
	SDE_POINT  nodes[MAX_VTX_NUM];		// 折线节点数组
}SDE_CURVELINE;

// 凸多边形
typedef struct _SDE_POLYGON
{
	int        count;									// 凸多边形顶点个数
	SDE_POINT  vertex[MAX_VTX_NUM];		// 凸多边形顶点数组
}SDE_POLYGON;

// 防区
typedef struct _SDE_ZONE
{
	int		  count;									// 防区防线条数
	SDE_LINE  line[MAX_VTX_NUM];		// 防区防线数组
}SDE_ZONE;

typedef SDE_LINE     LINE;
typedef SDE_LINE     VECTOR;
typedef SDE_CIRCLE   CIRCLE;
typedef SDE_POLYGON  POLYGON;
typedef SDE_ZONE     ZONE;
typedef SDE_RECTX		 RECTX;

typedef SDE_LINE     FI_LINE;
typedef SDE_LINE     FI_VECTOR;
typedef SDE_CIRCLE   FI_CIRCLE;
typedef SDE_POLYGON  FI_POLYGON;
typedef SDE_ZONE     FI_ZONE;
typedef SDE_RECTX		 FI_RECTX;

typedef SDE_LINE     *LPLINE;
typedef SDE_LINE     *LPVECTOR;
typedef SDE_CIRCLE   *LPCIRCLE;
typedef SDE_POLYGON  *LPPOLYGON;
typedef SDE_ZONE     *LPZONE;
typedef SDE_RECTX		 *LPRECTX;

typedef SDE_LINE     *LPFI_LINE;
typedef SDE_LINE     *LPFI_VECTOR;
typedef SDE_CIRCLE   *LPFI_CIRCLE;
typedef SDE_POLYGON  *LPFI_POLYGON;
typedef SDE_ZONE     *LPFI_ZONE;
typedef SDE_RECTX		 *LPFI_RECTX;

///////////////////////////////////////////////////////////////////////////////////////
// 规则类型定义
///////////////////////////////////////////////////////////////////////////////////////

/*
// 左右方位
typedef enum _SDE_POS_LR
{
	SDE_POS_LEFT  = 0x0,		// 左方位
	SDE_POS_RIGHT = 0x1,	  // 右方位
}SDE_POS_LR;

// 上下方位
typedef enum _SDE_POS_UD
{
	SDE_POS_DOWN = 0x0,	    // 下方位
	SDE_POS_UP   = 0x1,		  // 上方位
}SDE_POS_UD;

// 里外方位
typedef enum _SDE_POS_IO
{
	SDE_POS_OUT = 0x0,	    // 外方位
	SDE_POS_IN  = 0x1,		  // 里方位
}SDE_POS_IO;
*/

// 方向标左手方位
typedef enum _SDE_HANDPOS
{
	SDE_LEFT_IS_NULL  = 0x0, 	// 无效指定

	//// 水平走，左右穿越为右行，即从左到右穿越为正方向
	//// 正方向＝左 -> 右，即右行为正方向
	SDE_LEFT_IS_LEFT  = 0x1, 	// 左手即左方
	SDE_LEFT_IS_RIGHT = 0x2,	// 左手即右方
	
	/// 上下车，上车为上行，即上车为正方向
	//// 正方向＝下 -> 上
	SDE_LEFT_IS_DOWN  = 0x3,	// 左手即下方
	SDE_LEFT_IS_UP    = 0x4, 	// 左手即上方
	
	/// 出入屋，入屋为内行，即入屋为正方向
	//// 正方向＝外 -> 内
	SDE_LEFT_IS_OUT   = 0x5,	// 左手即外面
	SDE_LEFT_IS_IN    = 0x6,	// 左手即里面
}SDE_HANDPOS;

// 穿越方向
typedef enum _SDE_ACROSS_DIRECT
{
	SDE_ACROSS_NULL        = 0x0,  /* 没有相交 */
	SDE_ACROSS_ONE		   = 0x1,  /* 两线重叠 */
	SDE_ACROSS_LEFT2RIGHT  = 0x2,  /* 从左到右 */
	SDE_ACROSS_RIGHT2LEFT  = 0x3,  /* 从右到左 */
}SDE_ACROSS_DIRECT;

static INLINE FI_BOOL SDECheckAcross(SDE_ACROSS_DIRECT ret)
{
	return SDE_ACROSS_LEFT2RIGHT==ret || SDE_ACROSS_RIGHT2LEFT==ret;
}

// 运动方向
typedef enum _SDE_MOBILE_DIRECT
{
	SDE_MOBILE_NULL     = 0x0,	// 没有方向，无效
	SDE_MOBILE_POSITIVE = 0x1,  // 正方向＝右行（从左到右）、上行（从下到上）、内行（从外到内）
	SDE_MOBILE_NEGATIVE = 0x2,  // 反方向＝左行（从右到左）、下行（从上到下）、外行（从内到外）
}SDE_MOBILE_DIRECT;

// 防区类型
typedef enum _ZONE_TYPE_ENUM 
{
	ZONE_TYPE_NULL    = 0x0,	// 无效类型
	ZONE_TYPE_RECT	  = 0x1,	// 矩形防区
	ZONE_TYPE_CIRCLE  = 0x2,	// 圆形防区
	ZONE_TYPE_POLYGON = 0x3,    // 凸多边形防区
}ZONE_TYPE_ENUM;

// 关卡类型
typedef enum _SDE_GATE_TYPES
{
	SDE_GATE__NULL      = 0x0, // 无效闸门
	SDE_GATE__LINE      = 0x1, // 直线闸门
	SDE_GATE__CURVELINE = 0x2, // 折线闸门
	SDE_GATE__TRIANGLE  = 0x3, // 三角形闸门
	SDE_GATE__RECT      = 0x4, // 矩形闸门
	SDE_GATE__POLYGON   = 0x5, // 凸多边形闸门
}SDE_GATE_TYPES;

//// 计算负值（即）

//// 判断跨越方向（通过判断跨越起始端的手方位），即从起始端跨越到终止端的运动方向
//// handpos						跨越起始端的手方位
//// is_across_from_left_to_right	是否从左到右跨越
static INLINE SDE_MOBILE_DIRECT SDECheckAcrossDirect(SDE_HANDPOS handpos, FI_BOOL is_across_from_left_to_right)
{
	//// 从方向标的左边跨越到右边
	if (is_across_from_left_to_right)
	{
		if (SDE_LEFT_IS_LEFT==handpos || SDE_LEFT_IS_DOWN==handpos || SDE_LEFT_IS_OUT==handpos)
			  return SDE_MOBILE_POSITIVE; // 正方向
		else if (SDE_LEFT_IS_RIGHT==handpos || SDE_LEFT_IS_UP==handpos || SDE_LEFT_IS_IN==handpos)
			  return SDE_MOBILE_NEGATIVE; // 负方向
		else
				return SDE_MOBILE_NULL; // 无方向
	}
	//// 从方向标的右边跨越到左边
	else
	{
		if (SDE_LEFT_IS_LEFT==handpos || SDE_LEFT_IS_DOWN==handpos || SDE_LEFT_IS_OUT==handpos)
			  return SDE_MOBILE_NEGATIVE; // 负方向
		else if (SDE_LEFT_IS_RIGHT==handpos || SDE_LEFT_IS_UP==handpos || SDE_LEFT_IS_IN==handpos)
			  return SDE_MOBILE_POSITIVE; // 正方向
		else
				return SDE_MOBILE_NULL; // 无方向
	}
	return SDE_MOBILE_NULL; // 无方向
}

// 普通向标
typedef struct _SDE_IDLINE
{
	int		    id;			// 线段ID
	SDE_POINT	start;	// 线段起始点
	SDE_POINT	end;		// 线段终止点
}SDE_IDLINE, SDE_IDVECTOR;

// 方位向标
typedef struct _SDE_PLINE
{
	int		    id;			// 线段ID
	SDE_POINT	start;	// 线段起始点
	SDE_POINT	end;		// 线段终止点
	int				width;	// 线段宽度
	SDE_HANDPOS hp;		// 手方位
}SDE_PLINE, SDE_PVECTOR;

// 功能线类型
typedef enum _SDE_XLINE_TYPES
{
	SDE_XLINE_NULL    = 0x0,	/* 空线 */
	SDE_XLINE_ENTER   = 0x1,	/* 进入线*/
	SDE_XLINE_EXIT    = 0x2,	/* 出去线*/
	SDE_XLINE_PASS    = 0x3,	/* 过道线*/
	SDE_XLINE_EXCLUDE = 0x4,	/* 排除线*/
}SDE_XLINE_TYPES;

//// 功能线
typedef struct _SDE_XLINE
{
	SDE_XLINE_TYPES type;			/* 线类型*/
	SDE_PLINE line;						/* 矢量线*/
} SDE_XLINE, *LPSDE_XLINE;

// 普通防区
typedef struct _SDE_IDZONE
{
	int		    		id;					// 防区ID
	int		    		type;				// 防区类型，ZONE_TYPE_ENUM
	int		    		count;			// 防区防线条数
	SDE_IDLINE  	line[256];	// 防区防线数组
}SDE_IDZONE;

// 关卡
typedef struct _SDE_GATE
{
	int		    		  id;						// 关卡ID
	SDE_GATE_TYPES 	type;					// 关卡类型，SDE_GATE_TYPES
	int		    			count;				// 关卡防线条数
	SDE_PLINE  			line[MVTX]; 	// 关卡防线数组（凸多边形的线段必须是按顺时针方向连续存储，即凸多边形的任何一个线段方向标的左边都是凸多边形的外部）
}SDE_GATE;

// 关卡计数器
typedef struct _SDE_GATE_COUNTER
{
	int nGateID;		/* 关卡ID */
	int nUp;			/* 正方向跨越计数（右行/从左边到右边、上行/从车下到车上、内行/从屋外到屋内） */
	int nDown;			/* 逆方向跨越计数（左行/从右边到左边、下行/从车上到车下、外行/从屋内到屋外） */
}SDE_GATE_COUNTER;

// 全域关卡
typedef struct _SDE_GLOBALGATE
{
	int		    		count;			// 关卡防线条数
	SDE_PLINE  			line[MAX_GATE]; // 关卡防线数组（凸多边形的线段必须是按顺时针方向连续存储，即凸多边形的任何一个线段方向标的左边都是凸多边形的外部）
	FI_BOOL				bCountOnce;		/* ==TRUE  计算一次; ==FALSE  计算多次*/
}SDE_GLOBALGATE;

// 全域关卡计数器
typedef struct _SDE_GLOBALGATE_COUNTER
{
	int nGlobalTotalUp;			/* 全局关卡计数器正方向跨越计数（右行/从左边到右边、上行/从车下到车上、内行/从屋外到屋内） */
	int nGlobalTotalDown;		/* 全局关卡计数器逆方向跨越计数（左行/从右边到左边、下行/从车上到车下、外行/从屋内到屋外） */
	SDE_GATE_COUNTER SubCounters[MAX_GATE]; /* 子计数器组 */
}SDE_GLOBALGATE_COUNTER;

////
typedef SDE_IDLINE IDLINE;
typedef SDE_IDZONE IDZONE;

typedef SDE_IDLINE FI_IDLINE;
typedef SDE_IDZONE FI_IDZONE;

typedef SDE_IDLINE *LPIDLINE;
typedef SDE_IDZONE *LPIDZONE;

typedef SDE_IDLINE *LPFI_IDLINE;
typedef SDE_IDZONE *LPFI_IDZONE;

///////////////////////////////////////////////////////////////////////////////////////
// 应用类型定义
///////////////////////////////////////////////////////////////////////////////////////
typedef struct _SDE_OPCMD
{
	int op; 			// Operate code
}SDE_OPCMD;

typedef struct _SDE_IPADDR
{
	unsigned char field1,field2,field3,field4;
}SDE_IPADDR;

typedef struct _SDE_HWADDR
{
	unsigned char field1,field2,field3,field4,field5,field6;
}SDE_HWADDR;

typedef struct _SDE_AUTH_KEYS
{
	unsigned char keys[32];
}SDE_AUTH_KEYS;

typedef struct _SDE_AUTH_KEYS8
{
	unsigned char keys[8];
}SDE_AUTH_KEYS8;

typedef struct _SDE_RESULT
{
	int ret;
}SDE_RESULT;


typedef SDE_OPCMD       FI_OPCMD;
typedef SDE_OPCMD       *LPFI_OPCMD;

typedef SDE_IPADDR      FI_IPADDR;
typedef SDE_IPADDR      *LPFI_IPADDR;

typedef SDE_HWADDR      FI_HWADDR;
typedef SDE_HWADDR      *LPFI_HWADDR;

typedef SDE_AUTH_KEYS   FI_AUTH_KEYS;
typedef SDE_AUTH_KEYS   *LPFI_AUTH_KEYS;

typedef SDE_AUTH_KEYS8  FI_AUTH_KEYS8;
typedef SDE_AUTH_KEYS8  *LPFI_AUTH_KEYS8;

typedef SDE_RESULT      FI_RESULT;
typedef SDE_RESULT      *LPFI_RESULT;


///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
//// 通用对象定义
typedef struct _SDE_OBJ
{
	int 			obj_id;												/* 对象ID */
	SDE_POINT       obj_pos;											/* 对象位置*/
	SDE_RECT        obj_rect;											/* 对象区域*/
	int 			track_length;									/* 运动轨迹点个数*/
	SDE_POINT       track_path[MAX_TRK_NUM];			/* 运动轨迹点位置*/
	INT64			track_timestamp[MAX_TRK_NUM];	/* 运动轨迹点时间戳*/
	int 			track_frame_count;						/* 对象跟踪帧数*/
}SDE_OBJ;

typedef SDE_OBJ      FI_SDE_OBJ;
typedef SDE_OBJ      *LPFI_SDE_OBJ;

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
enum DO_ENUM
{
	DO_NULL		= 0,	// 空操作
	DO_NEW		= 1,	// 新建监控对象
	DO_MODIFY	= 2,	// 修改监控对象
	DO_DELETE	= 3,	// 删除监控对象
	DO_RESET	= 4,	// 复位监控对象
};

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

#define max(a,b)        (((a) > (b)) ? (a) : (b))
#define min(a,b)        (((a) < (b)) ? (a) : (b))
#define RANGE(x,a,b)	(min(max((x),(a)),(b)))

static INLINE float HB_SQRT(float val)
{
#if !defined(WIN32)
	#if 0
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////            异常提醒
	////   此处求double  的开方，用快速算法，可能会出现溢出导致结果值异常的问题，
	////   在DM6446发生过导致人脸检测、人眼检测、提取特征和比对异常失败的情况，在2440没有发生过
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		// 结果非常逼近sqrt函数
		float x = val * 0.5F;
		float y = val;
		int i;						//	* ( int * ) &y;
		memcpy(&i, &y, sizeof(i));
		i = 0x5f3759df - (i>>1);	//	y   = * ( float * ) &i;
		memcpy(&y, &i, sizeof(y));
		y = y * (1.5F - (x*y*y));

		return (float)1.0/y;
   
	#elif 0
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////            异常提醒
	////   此处求double  的开方，用快速算法，可能会出现溢出导致结果值异常的问题，
	////   在DM6446发生过导致人脸检测、人眼检测、提取特征和比对异常失败的情况，在2440没有发生过
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// 结果偏离sqrt函数
		union
		{
			int intPart;
			float floatPart;
		} convertor;
		union
		{
			int intPart;
			float floatPart;
		} convertor2;
		convertor.floatPart = val;
		convertor2.floatPart = val;
		convertor.intPart = 0x1FBCF800 + (convertor.intPart >> 1);
		convertor2.intPart = 0x5f3759df - (convertor2.intPart >> 1);
		return 0.5f*(convertor.floatPart + (val * convertor2.floatPart));
	#else
		return (float)sqrt(val);
	#endif

#else
	return (float)sqrt(val);
#endif   
}

static INLINE double HB_SQRT_DOUBLE(double val)
{
#if !defined(WIN32)
	#if 0
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////            异常提醒
	////   此处求double  的开方，用快速算法，可能会出现溢出导致结果值异常的问题，
	////   在DM6446发生过导致人脸检测、人眼检测、提取特征和比对异常失败的情况，在2440没有发生过
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 结果非常逼近sqrt函数
		double x = val * 0.5F;
		double y = val;
		INT64 i;						//	* ( int * ) &y;
		memcpy(&i, &y, sizeof(i));
		i = 0x5fe6ec85e7de30da - (i>>1);	//	y   = * ( float * ) &i;
		memcpy(&y, &i, sizeof(y));
		y = y * (1.5F - (x*y*y));

		return (double)1.0/y;

 	#elif 0
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////            异常提醒
	////   此处求double  的开方，用快速算法，可能会出现溢出导致结果值异常的问题，
	////   在DM6446发生过导致人脸检测、人眼检测、提取特征和比对异常失败的情况，在2440没有发生过
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
     INT64 i;
	   double x2, y;
	   const double threehalfs = 1.5F;
	   x2 = val * 0.5F;
	   y = val;
	   i = * ( INT64 * ) &y;
	   i = 0x5fe6ec85e7de30da - ( i >> 1 );
	   y = * ( double * ) &i;
	   y = y * ( threehalfs - ( x2 * y * y ) );
	   // 1st iteration       
	   y = y * ( threehalfs - ( x2 * y * y ) );
	   // 2nd iteration, this can be removed       
	   return y;
	#else
		return (double)sqrt(val);
	#endif

#else
	return (double)sqrt(val);
#endif   
}

static INLINE float HB_SQRT_RECIP(float number)
{
#if 0
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////            异常提醒
	////   此处求double  的开方，用快速算法，可能会出现溢出导致结果值异常的问题，
	////   在DM6446发生过导致人脸检测、人眼检测、提取特征和比对异常失败的情况，在2440没有发生过
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
   float x = number * 0.5F;
   float y = number;
   int i   = * ( int * ) &y;
   i   = 0x5f3759df - ( i >> 1 );
   y   = * ( float * ) &i;
   y   = y * ( 1.5F - ( x * y * y ) );
   
   return y;
   
#elif 0
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////            异常提醒
	////   此处求double  的开方，用快速算法，可能会出现溢出导致结果值异常的问题，
	////   在DM6446发生过导致人脸检测、人眼检测、提取特征和比对异常失败的情况，在2440没有发生过
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	float x = number * 0.5F;
	float y = number;
	int 	i;						//	* ( int * ) &y;
	memcpy(&i, &y, sizeof(i));
	i   = 0x5f3759df - ( i >> 1 );	//	y   = * ( float * ) &i;
	memcpy(&y, &i, sizeof(y));
	y   = y * ( 1.5F - ( x * y * y ) );

	return y;
   
#else
	return (float) (1.0 / sqrt(number));
#endif   
}


#ifdef __cplusplus
}
#endif	

#endif // _STD_DEF_EX_H
