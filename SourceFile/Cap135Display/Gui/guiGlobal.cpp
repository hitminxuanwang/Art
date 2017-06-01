#include "guiGlobal.h"

//电容白板调试相关
int g_gui_nCapMode = 0;  // 1:进入capMode
int g_gui_nCapDataType = 1; //上传的数据类型,0:实时数据,1:差值
int g_gui_nLEDLangType = 0;    //LED界面显示语言类型,0:中文,1:英文
int g_gui_nCapValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];
int g_gui_nCapChgValue[CAP_MAX_NUMBER_H][CAP_MAX_NUMBER_L];
QByteArray g_gui_capDataArray;  //电容白板数据保存
//电容白板测试相关
int g_gui_nCapPtTest = 0;   //是否处于电容白板测试模式,0:否,1:是
int g_gui_nShowCapTest = 0; //是否显示电容白板测试,0:否,1:是
int g_gui_nCapMinThreshold = 110;  //电容白板检测最低阈值
int g_gui_nCapMaxThreshold = 250;  //电容白板检测最高阈值
