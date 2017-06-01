/************************************************************
Copyright (C), 2012-2014, HongHe Tech.
FileName       : os_header.h
Date           : 2012.03.12
Description    : 各平台引用的头文件声明
Version        : 无
Function List  :
        1.
History:   1. 创建文件2013.03.12
***********************************************************/

#ifndef OS_HEADER_H
#define OS_HEADER_H

//define common header files across platform
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stddef.h>
#include <ctype.h>
#include <fcntl.h>

#include <QString>      //QString
#include <QStringList>
#include <QDir>
#include <QPair>
#include <QTime>
#include <QTextCodec>
#include <QCoreApplication>
#include <QSettings>


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
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <termios.h>
#include <sys/ioctl.h>          // ioctl TIOCEXCL

#endif

#ifdef VOS_WINDOWS
    #if _MSC_VER >= 1600
    #pragma execution_character_set("utf-8")    //告诉windows编译器源码的编码格式解决代码中直接赋值的中文运行时显示乱码问题,放在这个头文件里是为了确保所有界面类都可以包含到
    #endif
#endif //VOS_WINDOWS


#endif // OS_HEADER_H
