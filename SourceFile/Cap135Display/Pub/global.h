#ifndef GLOBAL_H
#define GLOBAL_H
#include "os_header.h"

//----------------------------------------------------
/* macro defines here begin */
/* snprintf in linux, sprintf_s in windows */
#ifdef VOS_WINDOWS
#define vos_snprintf    sprintf_s
#define vos_closesocket closesocket
#else
#define vos_snprintf    snprintf
#define vos_closesocket       close
#endif


/* define return value */
#define RET_INVALID     -11
#define RET_SUCCESS     0
#define RET_FAILURE     1
#define RET_OPENCOMFAILURE 2
#define RET_RSPCMDERR   -1
#define RET_RSPDATERR   -2
#define RET_FILENOTEXISTS -4
#define READ_CFG_ERR_RET -100 //��ȡ��������������ص�ֵ


/* �ļ�·����󳤶� */
#define DEV_MAX_PATH_LEN        256
/* ������󳤶� */
//#define CMD_PKT_MAX_LEN         256


/* macro defines here end */
//----------------------------------------------------

//----------------------------------------------------
/* struct defines here begin */
typedef int             INT32;
typedef short           INT16;
typedef char            VOS_INT8;
typedef unsigned int    UINT32;
typedef unsigned short  UINT16;
typedef unsigned char   UINT8;
typedef long long       INT64;

/* ��־����Ĭ�ϼ��� */
typedef enum dbglevel_t
{
    EN_EMERG     = 0,
    EN_ALERT     = 1,
    EN_CRIT      = 2,
    EN_ERR       = 3,
    EN_WARNING   = 4,
    EN_NOTICE    = 5,
    EN_INFO      = 6,
    EN_DEBUG     = 7,
    EN_BUT       = 8
}LOG_LEVEL_T;




/* struct defines here end */
//----------------------------------------------------

//----------------------------------------------------
/* extern defines here begin */
extern INT32 g_nLogLevel;
extern FILE *g_pLogFile;
extern VOS_INT8    g_acCurAppRunDir[DEV_MAX_PATH_LEN];
extern int g_nMainScreenX; // ��ǰ����Ļ�Ĵ�С���������꣩
extern int g_nMainScreenY; // ��ǰ����Ļ�Ĵ�С���������꣩

/*���ݰװ������*/
#define CAP_MAX_NUMBER_H 400
#define CAP_MAX_NUMBER_L 400
extern int g_nCapLineNumH;  //�������ڵ���
extern int g_nCapLineNumL;  //�������ڵ���
extern int g_nCapValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];
extern int g_nCapBaseValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];
extern int g_nCapChgValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];

/* ���ݰװ���Ի���ʹ�� */
extern int g_nCapStatusValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];
extern int g_nCapBaseDataValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];

typedef struct tagCapInfo
{
    int nLineNumH;
    int nLineNumL;
    int nBaseValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];
    int nChgValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];
}CAPINFO, *PCAPINFO;

extern HANDLE g_hCapShareMemory;
extern CAPINFO *g_pCapInfo;


/* extern defines here end */
//----------------------------------------------------
void Pub_GetCurAppDir(QString &strPath);
void Pub_GetCurDir(VOS_INT8 *pCurDir);
int Pub_GetCurrentTimeClick();
void LogToDebugFile(const VOS_INT8 *pcFunc,
                    LOG_LEVEL_T endebugLevel, const VOS_INT8 *fmt, ...);
/* ��־���� */
#define HHT_LOG(fmt, ...)             LogToDebugFile(__FUNCTION__, fmt, __VA_ARGS__)

//----------------------------------------------------
/* public function defines here begin */
void Pub_MSleep(int micSeconds);
VOS_INT8 *Pub_ConvertHexToStr(const UINT8 *pBuf, const UINT32 nLen);
void Pub_GetMainScreenInfo();
INT32 InitCap135SharedMemory();
void DeinitCap135SharedMemory();

/* public function defines here end */
//----------------------------------------------------

#endif // GLOBAL_H

