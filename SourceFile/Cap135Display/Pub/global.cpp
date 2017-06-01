#include "qt_headfiles.h"

#include "global.h"


int g_nMainScreenX; // 当前主屏幕的大小（像素坐标）
int g_nMainScreenY; // 当前主屏幕的大小（像素坐标）
/*电容白板调试用*/
int g_nCapLineNumH = 0; //纵向最大节点数
int g_nCapLineNumL = 0; //横向最大节点数
int g_nCapValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];
int g_nCapBaseValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];
int g_nCapChgValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];

HANDLE g_hCapShareMemory;
CAPINFO *g_pCapInfo = NULL;

/* 日志文件指针 */
INT32       g_nLogLevel                 = 6;
FILE *g_pLogFile                        = NULL;
/* 当前app运行目录 */
VOS_INT8    g_acCurAppRunDir[DEV_MAX_PATH_LEN]        = {0,};

/***********************************************************
Function     :   Pub_GetCurrentTimeClick
Description  :   获取当前时间戳ms
Input        :
Output       :   无
Return       :   无
Others       :   单位ms
Author       :
History      :   1. 创建函数2012.03.12
***********************************************************/
int Pub_GetCurrentTimeClick()
{
    QTime time = QTime::currentTime();
    int ms = time.hour(); ms *= 60;
    ms += time.minute(); ms *= 60;
    ms += time.second(); ms *= 1000;
    ms += time.msec();

    return ms;
}

/***********************************************************
Function     :   Pub_GetCurDir
Description  :   获取当前目录
Input        :
Output       :   无
Return       :   无
Others       :   对dll是无效的,获取路径为空
Author       :
History      :   1. 创建函数2012.03.12
***********************************************************/
void Pub_GetCurDir(VOS_INT8 *pCurDir)
{
    if (NULL == pCurDir)
    {
        return;
    }

    QString strRootDir;

    strRootDir = QCoreApplication::applicationDirPath();
    vos_snprintf(pCurDir, sizeof(strRootDir.toStdString().c_str()),
                 "%s", strRootDir.toStdString().c_str());

    return;
}

/***********************************************************
Function     :   LogToDebugFile
Description  :   用于各模块输出日志
Input        :    DEBUG_LEVEL_T endebugLevel ----日志级别
                    const char *fmt   ----需要输出到日志的字符串
Output       :   无
Return       :   无
Others       :   调用处字符串不需要换行，此函数处理
Author       :
History      :   1. 创建函数2012.03.12
***********************************************************/

void LogToDebugFile(const char *pcFunc, LOG_LEVEL_T endebugLevel, const char *fmt, ...)
{
    struct tm *newtime;
    time_t ltime;
    va_list v_list;
    char acLogPath[400]  = {0,};
    //FILE *fp = NULL;
    QDir dir;

    if (NULL == pcFunc)
    {
        return;
    }

    if (0 == g_nLogLevel)
    {// 不输出调试信息
        return;
    }
    
    if (endebugLevel > g_nLogLevel)
    {
        return;   // 当前的调试输出级别不够
    }
    
    memset(acLogPath, 0, sizeof(acLogPath));
    if (0 == strlen(g_acCurAppRunDir))
    {
        QString strAppPath;
        Pub_GetCurAppDir(strAppPath);
        vos_snprintf(g_acCurAppRunDir, sizeof(g_acCurAppRunDir), "%s/log",
                    strAppPath.toStdString().c_str());
    }
    vos_snprintf(acLogPath, sizeof(acLogPath), "%s/debug_capdisplay.txt", g_acCurAppRunDir);

    if (NULL == g_pLogFile)
    {
        g_pLogFile = fopen(acLogPath, "w");
    }
    if(NULL == g_pLogFile)
    {
        return;
    }

    /* Get the time in seconds */
    time (&ltime);

    /* convert it to the structure tm */
    newtime = localtime(&ltime);

    /* flush message */
    va_start(v_list, fmt);
    fprintf(g_pLogFile, "[%02d-%02d-%02d %02d:%02d:%02d %7d][%s()]",
        (newtime->tm_year + 1900), (newtime->tm_mon + 1), newtime->tm_mday,
        newtime->tm_hour, newtime->tm_min, newtime->tm_sec,
        Pub_GetCurrentTimeClick(), pcFunc);
    vfprintf(g_pLogFile, fmt, v_list);
    /* 此处强制换行输出，保证每条日志包含时间等信息 */
    fprintf(g_pLogFile, "\n");
    fflush(g_pLogFile);
    va_end(v_list);

    return;
}

/***********************************************************
Function     :   Pub_GetCurAppDir
Description  :   获取app路径
Input        :   无
Output       :   无
Return       :   返回配置项的值
Others       :  
Author       :
History      :   1. 创建函数2012.09.03
***********************************************************/
void Pub_GetCurAppDir(QString &strAppPath)
{
    strAppPath = QDir::currentPath();
    return;
}


/***********************************************************
Function     :   Pub_ConvertHexToStr
Description  :   转换16进制为字符串方便输出
Input        :   
Output       :   无
Return       :   NULL --- 失败 , 其它值--- 转换后的字符串
Others       :   无
Author       :
History      :   1. 创建函数2012.03.20
***********************************************************/
VOS_INT8 *Pub_ConvertHexToStr(const UINT8 *pBuf, const UINT32 nLen)
{
    static VOS_INT8     acBuf[20000]      = {0,};
    VOS_INT8            acTmpBuf[10]    = {0,};
    UINT32              ulIndex         = 0;
    UINT32              ulBufLen         = 0;
    
    if ((NULL == pBuf) || (0 >= nLen))
    {
        return NULL;
    }

    ulBufLen = sizeof(acBuf)/sizeof(acTmpBuf);
    if (ulBufLen >= nLen)
    {
        ulBufLen = nLen;
    }
    
    memset(acBuf, 0, sizeof(acBuf));
    memset(acTmpBuf, 0, sizeof(acTmpBuf));
    
    for(ulIndex=0; ulIndex<ulBufLen; ulIndex++)
    {
        vos_snprintf(acTmpBuf, sizeof(acTmpBuf), "%02X ", *(pBuf + ulIndex));
        strcat(acBuf, acTmpBuf);
    }
    
    return acBuf;
}


/***********************************************************
Function     :   Pub_MSleep
Description  :   公共休眠函数
Input        :   
                 
Output       :   无
Return       :   无
Others       :   单位毫秒ms
Author       :
History      :   1. 创建函数2012.03.12
***********************************************************/
void Pub_MSleep(int micSeconds)
{
#ifdef VOS_WINDOWS
    Sleep(micSeconds);  // Windows下，Sleep 的单位是毫秒
#endif

#ifdef OS_LINUX
    //struct timespec t1, t2;
    //t1.tv_sec = seconds;
    //t1.tv_nsec = 
    //nanosleep();
    //sleep(seconds);
    
    usleep(micSeconds * 1000);  // <unistd.h>，usleep的单位是微秒
#endif

#ifdef OS_MAC
    if (1 == g_nIsMavericks)
    {//Mac10.9 Mavericks线程中休眠1ms实际延迟最高达10s,使用nanosleep/select等一样效果,故这里减少休眠时间
        if (200 >= micSeconds)
        {//特殊处理
            int pre_ms = Pub_GetCurrentTimeClick();
            int cur_ms = Pub_GetCurrentTimeClick();
            while (1)
            {
                cur_ms = Pub_GetCurrentTimeClick();
                if ((pre_ms + micSeconds) <= cur_ms)
                {//timeout
                    break;
                }
            }
            return;
        }
    }
    usleep(micSeconds * 1000);
#endif
}

/***********************************************************
Function     :   Pub_GetMainScreenInfo
Description  :   获取主屏幕分辨率信息
Input        :   无
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2015.12.01
***********************************************************/

void Pub_GetMainScreenInfo()
{
    int i = 0;
    QDesktopWidget *desktop = QApplication::desktop();
    int screenCount = desktop->screenCount();
    int primaryScreen = desktop->primaryScreen();
    HHT_LOG(EN_INFO, "screen sum[%d], primary[%d], sumX[%d], sumY[%d]",
            screenCount, primaryScreen, desktop->width(), desktop->height());
    if (2 < screenCount)
    {//only support 2 screens
        screenCount =2;
    }
    
    for (i=0; i<screenCount; i++)
    {
        QRect screen = desktop->screenGeometry(i);
        if (primaryScreen == i)
        {//primary screen
            g_nMainScreenX = screen.width();
            g_nMainScreenY = screen.height();
        }
    }
    
    HHT_LOG(EN_INFO, "Main Screen, resolution[%d*%d]", g_nMainScreenX, g_nMainScreenY);
}

INT32 InitCap135SharedMemory()
{
    LRESULT lResult;
    VOS_INT8 Object_DeviceInfo[200];

    memset(Object_DeviceInfo, 0, sizeof(Object_DeviceInfo));
    vos_snprintf(Object_DeviceInfo, "CAP2017_SharedMemory");

    g_hCapShareMemory = OpenFileMappingA(FILE_MAP_ALL_ACCESS, TRUE, Object_DeviceInfo);
    if ( (NULL == g_hCapShareMemory) || (INVALID_HANDLE_VALUE == g_hCapShareMemory))
    {
        HHT_LOG(EN_ERR, "   Share memory %s not found", Object_DeviceInfo);
        // init test status
        //memset(m_pSMDeviceInfo, 0, sizeof(m_pSMDeviceInfo));
        //g_hCapShareMemory = CreateFileMappingA(NULL, NULL, PAGE_READWRITE, 0x00, 0x100000, Object_DeviceInfo);
    }
    /*
    if ((NULL == g_hCapShareMemory) || (INVALID_HANDLE_VALUE == g_hCapShareMemory))
    {
        lResult = GetLastError();
        HHT_LOG(EN_ERR, "create share memory failed(%d)", lResult);
        DeinitCap135SharedMemory();
        return lResult;
    }
    */

    g_pCapInfo = (CAPINFO*)MapViewOfFile(g_hCapShareMemory, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (NULL == g_pCapInfo)
    {
        lResult = GetLastError();
        CloseHandle(g_hCapShareMemory);
        g_hCapShareMemory = NULL;
        HHT_LOG(EN_ERR, "MapViewOfFile failed(%d)", lResult);
        DeinitCap135SharedMemory();
        return lResult;
    }
    // init test status
    //memset(m_pSMDeviceInfo, 0, sizeof(m_pSMDeviceInfo));
    // TODO
    // 
    //
    //
    
    HHT_LOG(EN_INFO, "create share memory OK");

    return RET_SUCCESS;
}


void DeinitCap135SharedMemory()
{
    if (NULL != g_pCapInfo)
    {
        memset(g_pCapInfo, 0, sizeof(g_pCapInfo));
        UnmapViewOfFile(g_pCapInfo);
        g_pCapInfo = NULL;
    }

    if ((NULL != g_hCapShareMemory) && (INVALID_HANDLE_VALUE != g_hCapShareMemory))
    {
        CloseHandle(g_hCapShareMemory);
        g_hCapShareMemory = INVALID_HANDLE_VALUE;
    }

    return;
}

