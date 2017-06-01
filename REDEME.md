# 说明
程序分成两部分，包括DataFeeder以及reacTIVision-master，其中DataFeeder需要预先启动board_driver.exe该文件"135电容白板相关\服务程序Maintain版本(3.17.03.20-M)For135Debug"路径下，用来不断将电容白板数据传输到共享内存中，在SourceFile文件夹中有相关说明文档。DataFeeder，用来将共享内存中的数
据读到二维数组中，相关数据保存在g_pCapInfo变量中。rectTIVison-master是读取摄像头拍摄的图片并交给TUIO处理的程序（http://reactivision.sourceforge.net/）。接下来的任务需要将DataFeeder读取到的g_pCapInfo替换进rectTIVison程序里实现用电容白板代替摄像头输入的目标。美院机器账户new 密码 cscg























