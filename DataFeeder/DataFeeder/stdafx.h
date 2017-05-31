// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <errno.h>
#include <stddef.h>
#include <ctype.h>
#include <fcntl.h>

#ifdef VOS_WINDOWS
#include <windows.h>
#include <winbase.h>
#include <devguid.h>      //GUID_DEVCLASS_PORTS
#include <SetupAPI.h>     //HDEVINFO

// adding "pragma warning" to avoid C4995 waning: "gets(vsprintf):#pragma deprecated"
#pragma warning(push)
#pragma warning(disable:4995)
#include <Dshow.h>        //ICaptureGraphBuilder2,IBaseFilter
#pragma warning(pop)
#pragma warning(disable:4995)
// add end


#include <process.h>        //_beginthreadex
//#include <atlstr.h>     //cstring
//#include <comdef.h>
//#include <QLibrary>
//#include <QList>
#else


#endif


#ifdef VOS_WINDOWS
#define vos_snprintf    sprintf_s
#define vos_closesocket closesocket
#else
#define vos_snprintf    sprintf
#define vos_closesocket       close
#endif
#define RET_INVALID     -11
#define RET_SUCCESS     0
#define RET_FAILURE     1
#define RET_OPENCOMFAILURE 2
#define RET_RSPCMDERR   -1
#define RET_RSPDATERR   -2
#define RET_FILENOTEXISTS -4
#define READ_CFG_ERR_RET -100

#define DEV_MAX_PATH_LEN        256

typedef int             INT32;
typedef short           INT16;
typedef char            VOS_INT8;
typedef unsigned int    UINT32;
typedef unsigned short  UINT16;
typedef unsigned char   UINT8;
typedef long long       INT64;
extern INT32 g_nLogLevel;
extern FILE *g_pLogFile;
extern VOS_INT8    g_acCurAppRunDir[DEV_MAX_PATH_LEN];
extern int g_nMainScreenX; 
extern int g_nMainScreenY; 

#define CAP_MAX_NUMBER_H 400
#define CAP_MAX_NUMBER_L 400
extern int g_nCapLineNumH;  
extern int g_nCapLineNumL;  
extern int g_nCapValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];
extern int g_nCapBaseValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];
extern int g_nCapChgValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];

extern int g_nCapStatusValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];
extern int g_nCapBaseDataValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];

typedef struct tagCapInfo
{
	int nLineNumH;
	int nLineNumL;
	int nBaseValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];
	int nChgValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];
}CAPINFO, *PCAPINFO;

//extern HANDLE g_hCapShareMemory;
//extern CAPINFO *g_pCapInfo;

// TODO: reference additional headers your program requires here
//#define HHT_LOG(fmt, ...)             LogToDebugFile(__FUNCTION__, fmt, __VA_ARGS__)
typedef enum dbglevel_t
{
	EN_EMERG = 0,
	EN_ALERT = 1,
	EN_CRIT = 2,
	EN_ERR = 3,
	EN_WARNING = 4,
	EN_NOTICE = 5,
	EN_INFO = 6,
	EN_DEBUG = 7,
	EN_BUT = 8
}LOG_LEVEL_T;
//void LogToDebugFile(const VOS_INT8 *pcFunc,
//	LOG_LEVEL_T endebugLevel, const VOS_INT8 *fmt, ...);