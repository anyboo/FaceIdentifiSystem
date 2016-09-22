
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
extern "C" {  // ������һ��Ҫ���������� #include ֮�󣬷�������������� #include ͷ�ļ������� c �������ӣ����±�����󡣱��� math.h ��VC�������ǽ�ֹ C �������ӵģ��ᱨ�������
#endif

//// ��ʽ����
enum _IMAGE_FORMAT
{
	IMAGE_GRAY,
	IMAGE_RGB,
	IMAGE_BGR,
	IMAGE_YUV420,
	IMAGE_YUV422,
};

typedef int IMAGE_FORMAT;

//// ȫ�ֳ�������
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
	#define S_OK 			0x00000000 	// �ɹ� 
	#define S_FALSE 		0x00000001 	// �����ɹ�ִ����ɣ�������ʱ���ִ��� 
	#define E_INVALIDARG 	0x80070057 	// �����д��� 
	#define E_OUTOFMEMORY 	0x8007000E 	// �ڴ�������� 
	#define E_UNEXPECTED 	0x8000FFFF 	// δ֪���쳣 
	#define E_NOTIMPL 		0x80004001 	// δʵ�ֹ��� 
	#define E_FAIL 			0x80004005 	// û����ϸ˵���Ĵ���һ����Ҫȡ�� Rich Error ������Ϣ(ע1) 
	#define E_POINTER 		0x80004003 	// ��Ч��ָ�� 
	#define E_HANDLE 		0x80070006 	// ��Ч�ľ�� 
	#define E_ABORT 		0x80004004 	// ��ֹ���� 
	#define E_ACCESSDENIED 	0x80070005 	// ���ʱ��ܾ� 
	#define E_NOINTERFACE 	0x80004002 	// ��֧�ֽӿ� 
#endif

#define MAX_ROI_NUM 		80000	/* ��� ROI���� */
#define MAX_OBJ_NUM			100		/* ��������� */
#define MAX_TRK_NUM			256		/* ���켣���� */
#define MAX_VTX_NUM			50		/* ��󶥵���� */
#define MAX_GATE			50		/* �����߸��� */

#define MVTX			  		10		/* ͹�������󶥵���� */
#define MRULE						10		/* ��������� */


// ��Խ��Ϊ
enum ACT_ACROSS_ENUM
{
	ACT_ACROSS_NULL	= 0,			// �ն���
	ACT_ACROSS_LEFT	= 1,			// ����ߴ�Խ���ߣ����ұߣ�
	ACT_ACROSS_RIGHT	= 2,			// ���ұߴ�Խ���ߣ�����ߣ�
};

// ������Ϊ
enum ACT_INOUT_ENUM
{
	ACT_INOUT_NULL	= 0,			// �ն���
	ACT_INOUT_IN		= 1,			// �߽�����
	ACT_INOUT_OUT		= 2,			// �߳�����
};

// ������Ϊ
enum ACT_DIRECTION_ENUM
{
	ACT_DIRECTION_NULL	= 0,			// �ն���
	ACT_DIRECTION_FORWARD	= 1,		// �����˶�
	ACT_DIRECTION_REVERSE	= 2,		// �����˶�
};

// �쳣��Ϊ
enum ACT_DYSTROPY_ENUM
{
	ACT_DYSTROPY_NULL	= 0,			// �ն���
	ACT_DYSTROPY_LINGER	= 1,		// �ǻ��˶�
};

// ������Ϊ
enum ACT_EXIST_ENUM
{
	ACT_EXIST_NULL	= 0,			// �ն���
	ACT_EXIST_LEAVE	= 1,			// ��������
	ACT_EXIST_LOSE	= 2,			// ��ʧ����
};

///////////////////////////////////////////////////////////////////////////////////////
// �������Ͷ���
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
// �������Ͷ���
///////////////////////////////////////////////////////////////////////////////////////
// �� WIN32 ����ϵͳ
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


// �ⲿ��������
typedef SDE_POINT   FI_POINT;
typedef SDE_SIZE	FI_SIZE;
typedef SDE_RECT	FI_RECT;

typedef SDE_POINT   *LPFI_POINT;
typedef SDE_SIZE    *LPFI_SIZE;
typedef SDE_RECT    *LPFI_RECT;


///////////////////////////////////////////////////////////////////////////////////////
// �ض����Ͷ���
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
// �������Ͷ���
///////////////////////////////////////////////////////////////////////////////////////

// ������
typedef struct _SDE_LINE
{
	SDE_POINT	start;			// �߶���ʼ��
	SDE_POINT	end;			// �߶���ֹ��
}SDE_LINE;

// ˫������
typedef struct _SDE_PAIRLINE
{
	SDE_LINE	line1;			// ʸ��1
	SDE_LINE	line2;			// ʸ��2
}SDE_PAIRLINE;

// �������
typedef struct _SDE_RECTX
{
	SDE_POINT lt;		/* ���Ͻǵ� */
	SDE_POINT rt;		/* ���Ͻǵ� */
	SDE_POINT rb;		/* ���½ǵ� */
	SDE_POINT lb;		/* ���½ǵ� */
}SDE_RECTX;

// Բ��
typedef struct _SDE_CIRCLE
{
	SDE_POINT  center;			// Բ������
	int	       radius;			// Բ�ΰ뾶
}SDE_CIRCLE;

// ����
typedef struct _SDE_CURVELINE
{
	int        count;									// ���߽ڵ����
	SDE_POINT  nodes[MAX_VTX_NUM];		// ���߽ڵ�����
}SDE_CURVELINE;

// ͹�����
typedef struct _SDE_POLYGON
{
	int        count;									// ͹����ζ������
	SDE_POINT  vertex[MAX_VTX_NUM];		// ͹����ζ�������
}SDE_POLYGON;

// ����
typedef struct _SDE_ZONE
{
	int		  count;									// ������������
	SDE_LINE  line[MAX_VTX_NUM];		// ������������
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
// �������Ͷ���
///////////////////////////////////////////////////////////////////////////////////////

/*
// ���ҷ�λ
typedef enum _SDE_POS_LR
{
	SDE_POS_LEFT  = 0x0,		// ��λ
	SDE_POS_RIGHT = 0x1,	  // �ҷ�λ
}SDE_POS_LR;

// ���·�λ
typedef enum _SDE_POS_UD
{
	SDE_POS_DOWN = 0x0,	    // �·�λ
	SDE_POS_UP   = 0x1,		  // �Ϸ�λ
}SDE_POS_UD;

// ���ⷽλ
typedef enum _SDE_POS_IO
{
	SDE_POS_OUT = 0x0,	    // �ⷽλ
	SDE_POS_IN  = 0x1,		  // �﷽λ
}SDE_POS_IO;
*/

// ��������ַ�λ
typedef enum _SDE_HANDPOS
{
	SDE_LEFT_IS_NULL  = 0x0, 	// ��Чָ��

	//// ˮƽ�ߣ����Ҵ�ԽΪ���У��������Ҵ�ԽΪ������
	//// �������� -> �ң�������Ϊ������
	SDE_LEFT_IS_LEFT  = 0x1, 	// ���ּ���
	SDE_LEFT_IS_RIGHT = 0x2,	// ���ּ��ҷ�
	
	/// ���³����ϳ�Ϊ���У����ϳ�Ϊ������
	//// �������� -> ��
	SDE_LEFT_IS_DOWN  = 0x3,	// ���ּ��·�
	SDE_LEFT_IS_UP    = 0x4, 	// ���ּ��Ϸ�
	
	/// �����ݣ�����Ϊ���У�������Ϊ������
	//// �������� -> ��
	SDE_LEFT_IS_OUT   = 0x5,	// ���ּ�����
	SDE_LEFT_IS_IN    = 0x6,	// ���ּ�����
}SDE_HANDPOS;

// ��Խ����
typedef enum _SDE_ACROSS_DIRECT
{
	SDE_ACROSS_NULL        = 0x0,  /* û���ཻ */
	SDE_ACROSS_ONE		   = 0x1,  /* �����ص� */
	SDE_ACROSS_LEFT2RIGHT  = 0x2,  /* ������ */
	SDE_ACROSS_RIGHT2LEFT  = 0x3,  /* ���ҵ��� */
}SDE_ACROSS_DIRECT;

static INLINE FI_BOOL SDECheckAcross(SDE_ACROSS_DIRECT ret)
{
	return SDE_ACROSS_LEFT2RIGHT==ret || SDE_ACROSS_RIGHT2LEFT==ret;
}

// �˶�����
typedef enum _SDE_MOBILE_DIRECT
{
	SDE_MOBILE_NULL     = 0x0,	// û�з�����Ч
	SDE_MOBILE_POSITIVE = 0x1,  // ���������У������ң������У����µ��ϣ������У����⵽�ڣ�
	SDE_MOBILE_NEGATIVE = 0x2,  // ���������У����ҵ��󣩡����У����ϵ��£������У����ڵ��⣩
}SDE_MOBILE_DIRECT;

// ��������
typedef enum _ZONE_TYPE_ENUM 
{
	ZONE_TYPE_NULL    = 0x0,	// ��Ч����
	ZONE_TYPE_RECT	  = 0x1,	// ���η���
	ZONE_TYPE_CIRCLE  = 0x2,	// Բ�η���
	ZONE_TYPE_POLYGON = 0x3,    // ͹����η���
}ZONE_TYPE_ENUM;

// �ؿ�����
typedef enum _SDE_GATE_TYPES
{
	SDE_GATE__NULL      = 0x0, // ��Чբ��
	SDE_GATE__LINE      = 0x1, // ֱ��բ��
	SDE_GATE__CURVELINE = 0x2, // ����բ��
	SDE_GATE__TRIANGLE  = 0x3, // ������բ��
	SDE_GATE__RECT      = 0x4, // ����բ��
	SDE_GATE__POLYGON   = 0x5, // ͹�����բ��
}SDE_GATE_TYPES;

//// ���㸺ֵ������

//// �жϿ�Խ����ͨ���жϿ�Խ��ʼ�˵��ַ�λ����������ʼ�˿�Խ����ֹ�˵��˶�����
//// handpos						��Խ��ʼ�˵��ַ�λ
//// is_across_from_left_to_right	�Ƿ�����ҿ�Խ
static INLINE SDE_MOBILE_DIRECT SDECheckAcrossDirect(SDE_HANDPOS handpos, FI_BOOL is_across_from_left_to_right)
{
	//// �ӷ�������߿�Խ���ұ�
	if (is_across_from_left_to_right)
	{
		if (SDE_LEFT_IS_LEFT==handpos || SDE_LEFT_IS_DOWN==handpos || SDE_LEFT_IS_OUT==handpos)
			  return SDE_MOBILE_POSITIVE; // ������
		else if (SDE_LEFT_IS_RIGHT==handpos || SDE_LEFT_IS_UP==handpos || SDE_LEFT_IS_IN==handpos)
			  return SDE_MOBILE_NEGATIVE; // ������
		else
				return SDE_MOBILE_NULL; // �޷���
	}
	//// �ӷ������ұ߿�Խ�����
	else
	{
		if (SDE_LEFT_IS_LEFT==handpos || SDE_LEFT_IS_DOWN==handpos || SDE_LEFT_IS_OUT==handpos)
			  return SDE_MOBILE_NEGATIVE; // ������
		else if (SDE_LEFT_IS_RIGHT==handpos || SDE_LEFT_IS_UP==handpos || SDE_LEFT_IS_IN==handpos)
			  return SDE_MOBILE_POSITIVE; // ������
		else
				return SDE_MOBILE_NULL; // �޷���
	}
	return SDE_MOBILE_NULL; // �޷���
}

// ��ͨ���
typedef struct _SDE_IDLINE
{
	int		    id;			// �߶�ID
	SDE_POINT	start;	// �߶���ʼ��
	SDE_POINT	end;		// �߶���ֹ��
}SDE_IDLINE, SDE_IDVECTOR;

// ��λ���
typedef struct _SDE_PLINE
{
	int		    id;			// �߶�ID
	SDE_POINT	start;	// �߶���ʼ��
	SDE_POINT	end;		// �߶���ֹ��
	int				width;	// �߶ο��
	SDE_HANDPOS hp;		// �ַ�λ
}SDE_PLINE, SDE_PVECTOR;

// ����������
typedef enum _SDE_XLINE_TYPES
{
	SDE_XLINE_NULL    = 0x0,	/* ���� */
	SDE_XLINE_ENTER   = 0x1,	/* ������*/
	SDE_XLINE_EXIT    = 0x2,	/* ��ȥ��*/
	SDE_XLINE_PASS    = 0x3,	/* ������*/
	SDE_XLINE_EXCLUDE = 0x4,	/* �ų���*/
}SDE_XLINE_TYPES;

//// ������
typedef struct _SDE_XLINE
{
	SDE_XLINE_TYPES type;			/* ������*/
	SDE_PLINE line;						/* ʸ����*/
} SDE_XLINE, *LPSDE_XLINE;

// ��ͨ����
typedef struct _SDE_IDZONE
{
	int		    		id;					// ����ID
	int		    		type;				// �������ͣ�ZONE_TYPE_ENUM
	int		    		count;			// ������������
	SDE_IDLINE  	line[256];	// ������������
}SDE_IDZONE;

// �ؿ�
typedef struct _SDE_GATE
{
	int		    		  id;						// �ؿ�ID
	SDE_GATE_TYPES 	type;					// �ؿ����ͣ�SDE_GATE_TYPES
	int		    			count;				// �ؿ���������
	SDE_PLINE  			line[MVTX]; 	// �ؿ��������飨͹����ε��߶α����ǰ�˳ʱ�뷽�������洢����͹����ε��κ�һ���߶η�������߶���͹����ε��ⲿ��
}SDE_GATE;

// �ؿ�������
typedef struct _SDE_GATE_COUNTER
{
	int nGateID;		/* �ؿ�ID */
	int nUp;			/* �������Խ����������/����ߵ��ұߡ�����/�ӳ��µ����ϡ�����/�����⵽���ڣ� */
	int nDown;			/* �淽���Խ����������/���ұߵ���ߡ�����/�ӳ��ϵ����¡�����/�����ڵ����⣩ */
}SDE_GATE_COUNTER;

// ȫ��ؿ�
typedef struct _SDE_GLOBALGATE
{
	int		    		count;			// �ؿ���������
	SDE_PLINE  			line[MAX_GATE]; // �ؿ��������飨͹����ε��߶α����ǰ�˳ʱ�뷽�������洢����͹����ε��κ�һ���߶η�������߶���͹����ε��ⲿ��
	FI_BOOL				bCountOnce;		/* ==TRUE  ����һ��; ==FALSE  ������*/
}SDE_GLOBALGATE;

// ȫ��ؿ�������
typedef struct _SDE_GLOBALGATE_COUNTER
{
	int nGlobalTotalUp;			/* ȫ�ֹؿ��������������Խ����������/����ߵ��ұߡ�����/�ӳ��µ����ϡ�����/�����⵽���ڣ� */
	int nGlobalTotalDown;		/* ȫ�ֹؿ��������淽���Խ����������/���ұߵ���ߡ�����/�ӳ��ϵ����¡�����/�����ڵ����⣩ */
	SDE_GATE_COUNTER SubCounters[MAX_GATE]; /* �Ӽ������� */
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
// Ӧ�����Ͷ���
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
//// ͨ�ö�����
typedef struct _SDE_OBJ
{
	int 			obj_id;												/* ����ID */
	SDE_POINT       obj_pos;											/* ����λ��*/
	SDE_RECT        obj_rect;											/* ��������*/
	int 			track_length;									/* �˶��켣�����*/
	SDE_POINT       track_path[MAX_TRK_NUM];			/* �˶��켣��λ��*/
	INT64			track_timestamp[MAX_TRK_NUM];	/* �˶��켣��ʱ���*/
	int 			track_frame_count;						/* �������֡��*/
}SDE_OBJ;

typedef SDE_OBJ      FI_SDE_OBJ;
typedef SDE_OBJ      *LPFI_SDE_OBJ;

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
enum DO_ENUM
{
	DO_NULL		= 0,	// �ղ���
	DO_NEW		= 1,	// �½���ض���
	DO_MODIFY	= 2,	// �޸ļ�ض���
	DO_DELETE	= 3,	// ɾ����ض���
	DO_RESET	= 4,	// ��λ��ض���
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
	////            �쳣����
	////   �˴���double  �Ŀ������ÿ����㷨�����ܻ����������½��ֵ�쳣�����⣬
	////   ��DM6446����������������⡢���ۼ�⡢��ȡ�����ͱȶ��쳣ʧ�ܵ��������2440û�з�����
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		// ����ǳ��ƽ�sqrt����
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
	////            �쳣����
	////   �˴���double  �Ŀ������ÿ����㷨�����ܻ����������½��ֵ�쳣�����⣬
	////   ��DM6446����������������⡢���ۼ�⡢��ȡ�����ͱȶ��쳣ʧ�ܵ��������2440û�з�����
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// ���ƫ��sqrt����
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
	////            �쳣����
	////   �˴���double  �Ŀ������ÿ����㷨�����ܻ����������½��ֵ�쳣�����⣬
	////   ��DM6446����������������⡢���ۼ�⡢��ȡ�����ͱȶ��쳣ʧ�ܵ��������2440û�з�����
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ����ǳ��ƽ�sqrt����
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
	////            �쳣����
	////   �˴���double  �Ŀ������ÿ����㷨�����ܻ����������½��ֵ�쳣�����⣬
	////   ��DM6446����������������⡢���ۼ�⡢��ȡ�����ͱȶ��쳣ʧ�ܵ��������2440û�з�����
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
	////            �쳣����
	////   �˴���double  �Ŀ������ÿ����㷨�����ܻ����������½��ֵ�쳣�����⣬
	////   ��DM6446����������������⡢���ۼ�⡢��ȡ�����ͱȶ��쳣ʧ�ܵ��������2440û�з�����
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
	////            �쳣����
	////   �˴���double  �Ŀ������ÿ����㷨�����ܻ����������½��ֵ�쳣�����⣬
	////   ��DM6446����������������⡢���ۼ�⡢��ȡ�����ͱȶ��쳣ʧ�ܵ��������2440û�з�����
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
