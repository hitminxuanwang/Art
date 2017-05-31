// DataFeeder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;



HANDLE g_hCapShareMemory;
CAPINFO *g_pCapInfo;


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
INT32 InitCap135SharedMemory()
{
	LRESULT lResult;
	VOS_INT8 Object_DeviceInfo[200];

	memset(Object_DeviceInfo, 0, sizeof(Object_DeviceInfo));
	vos_snprintf(Object_DeviceInfo, "CAP2017_SharedMemory");

	g_hCapShareMemory = OpenFileMappingA(FILE_MAP_ALL_ACCESS, TRUE, Object_DeviceInfo);
	if ((NULL == g_hCapShareMemory) || (INVALID_HANDLE_VALUE == g_hCapShareMemory))
	{
		//HHT_LOG(EN_ERR, "   Share memory %s not found", Object_DeviceInfo);
		cout << "test1" << endl;
	}


	g_pCapInfo = (CAPINFO*)MapViewOfFile(g_hCapShareMemory, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (NULL == g_pCapInfo)
	{
		lResult = GetLastError();
		CloseHandle(g_hCapShareMemory);
		g_hCapShareMemory = NULL;
		//HHT_LOG(EN_ERR, "MapViewOfFile failed(%d)", lResult);
		cout << "test2" << endl;
		DeinitCap135SharedMemory();
		return lResult;
	}


	//HHT_LOG(EN_INFO, "create share memory OK");
	cout << "test3" << endl;

	return RET_SUCCESS;
}



int main(int argc, _TCHAR* argv[])
{
	int test=InitCap135SharedMemory();



	return 0;
}

