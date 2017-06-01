#include "mainwindow.h"
#include <QApplication>
#include "guiGlobal.h"
#include "global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    Pub_GetMainScreenInfo();
    InitCap135SharedMemory();
    GUI_SS = new MainWindow();
    GUI_SS->setMaximumSize(0, 0);
    GUI_SS->showMinimized();
    GUI_SS->hide();

    CapTest *capTest = new CapTest;
    if (NULL == capTest)
    {
        HHT_LOG(EN_ERR, "   capTest NULL");
    }
    else
    {
        GUI_SS->guiCapTest = capTest;
        GUI_SS->guiCapTest->setWindowFlags(Qt::FramelessWindowHint);
        GUI_SS->guiCapTest->move(QPoint(0, 0));
        GUI_SS->guiCapTest->resize(g_nMainScreenX, g_nMainScreenY);
        GUI_SS->guiCapTest->show();
        HHT_LOG(EN_INFO, "  cap test show ok");
    }

    return a.exec();
}
