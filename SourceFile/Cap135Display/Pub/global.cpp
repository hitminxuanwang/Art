#include "qt_headfiles.h"

#include "global.h"


int g_nMainScreenX; // ��ǰ����Ļ�Ĵ�С���������꣩
int g_nMainScreenY; // ��ǰ����Ļ�Ĵ�С���������꣩
/*���ݰװ������*/
int g_nCapLineNumH = 0; //�������ڵ���
int g_nCapLineNumL = 0; //�������ڵ���
int g_nCapValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];
int g_nCapBaseValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];
int g_nCapChgValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];

HANDLE g_hCapShareMemory;
CAPINFO *g_pCapInfo = NULL;

/* ��־�ļ�ָ�� */
INT32       g_nLogLevel                 = 6;
FILE *g_pLogFile                        = NULL;
/* ��ǰapp����Ŀ¼ */
VOS_INT8    g_acCurAppRunDir[DEV_MAX_PATH_LEN]        = {0,};

/***********************************************************
Function     :   Pub_GetCurrentTimeClick
Description  :   ��ȡ��ǰʱ���ms
Input        :
Output       :   ��
Return       :   ��
Others       :   ��λms
Author       :
History      :   1. ��������2012.03.12
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
Description  :   ��ȡ��ǰĿ¼
Input        :
Output       :   ��
Return       :   ��
Others       :   ��dll����Ч��,��ȡ·��Ϊ��
Author       :
History      :   1. ��������2012.03.12
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
Description  :   ���ڸ�ģ�������־
Input        :    DEBUG_LEVEL_T endebugLevel ----��־����
                    const char *fmt   ----��Ҫ�������־���ַ���
Output       :   ��
Return       :   ��
Others       :   ���ô��ַ�������Ҫ���У��˺�������
Author       :
History      :   1. ��������2012.03.12
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
    {// �����������Ϣ
        return;
    }
    
    if (endebugLevel > g_nLogLevel)
    {
        return;   // ��ǰ�ĵ���������𲻹�
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
    /* �˴�ǿ�ƻ����������֤ÿ����־����ʱ�����Ϣ */
    fprintf(g_pLogFile, "\n");
    fflush(g_pLogFile);
    va_end(v_list);

    return;
}

/***********************************************************
Function     :   Pub_GetCurAppDir
Description  :   ��ȡapp·��
Input        :   ��
Output       :   ��
Return       :   �����������ֵ
Others       :  
Author       :
History      :   1. ��������2012.09.03
***********************************************************/
void Pub_GetCurAppDir(QString &strAppPath)
{
    strAppPath = QDir::currentPath();
    return;
}


/***********************************************************
Function     :   Pub_ConvertHexToStr
Description  :   ת��16����Ϊ�ַ����������
Input        :   
Output       :   ��
Return       :   NULL --- ʧ�� , ����ֵ--- ת������ַ���
Others       :   ��
Author       :
History      :   1. ��������2012.03.20
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
Description  :   �������ߺ���
Input        :   
                 
Output       :   ��
Return       :   ��
Others       :   ��λ����ms
Author       :
History      :   1. ��������2012.03.12
***********************************************************/
void Pub_MSleep(int micSeconds)
{
#ifdef VOS_WINDOWS
    Sleep(micSeconds);  // Windows�£�Sleep �ĵ�λ�Ǻ���
#endif

#ifdef OS_LINUX
    //struct timespec t1, t2;
    //t1.tv_sec = seconds;
    //t1.tv_nsec = 
    //nanosleep();
    //sleep(seconds);
    
    usleep(micSeconds * 1000);  // <unistd.h>��usleep�ĵ�λ��΢��
#endif

#ifdef OS_MAC
    if (1 == g_nIsMavericks)
    {//Mac10.9 Mavericks�߳�������1msʵ���ӳ���ߴ�10s,ʹ��nanosleep/select��һ��Ч��,�������������ʱ��
        if (200 >= micSeconds)
        {//���⴦��
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
Description  :   ��ȡ����Ļ�ֱ�����Ϣ
Input        :   ��
Output       :   ��
Return       :   ��
Others       :  
Author       :
History      :   1. ��������2015.12.01
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

