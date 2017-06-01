#ifndef GUIGLOBAL_H
#define GUIGLOBAL_H
#include "global.h"

//电容白板调试相关
extern int g_gui_nCapMode;  // 1:进入capMode
extern int g_gui_nCapDataType; //上传的数据类型,0:实时数据,1:差值
extern int g_gui_nLEDLangType;    //LED界面显示语言类型,0:中文,1:英文
extern int g_gui_nCapValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];
extern int g_gui_nCapChgValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];
extern QByteArray g_gui_capDataArray;   //电容白板数据保存
//电容白板测试相关
extern int g_gui_nCapPtTest;   //是否处于电容白板测试模式,0:否,1:是
extern int g_gui_nShowCapTest; //是否显示电容白板测试,0:否,1:是
extern int g_gui_nCapMinThreshold;  //电容白板检测最低阈值
extern int g_gui_nCapMaxThreshold;  //电容白板检测最高阈值

#endif //GUIGLOBAL_H
