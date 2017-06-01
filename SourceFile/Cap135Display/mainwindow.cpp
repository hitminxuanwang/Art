#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "global.h"

MainWindow *GUI_SS;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    //ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
    HHT_LOG(EN_INFO, "Construct");
    readShareMemTimer = new QTimer;
    connect(readShareMemTimer, SIGNAL(timeout()), this, SLOT(SlotReadShareMemory()));

    readShareMemTimer->start(10);
}

MainWindow::~MainWindow()
{
    //delete ui;
    HHT_LOG(EN_INFO, "Destruct");
    if (readShareMemTimer->isActive())
    {
        readShareMemTimer->stop();
    }
    delete readShareMemTimer;
}

void MainWindow::SlotReadShareMemory()
{
    INT32 i = 0;
    INT32 j = 0;

    if (NULL == g_pCapInfo)
    {
        HHT_LOG(EN_ERR, "   g_pCapInfo invalid");
        return;
    }
    g_nCapLineNumL = g_pCapInfo->nLineNumL;
    g_nCapLineNumH = g_pCapInfo->nLineNumH;
    HHT_LOG(EN_INFO, "   LineNumL:%d, LineNumHL:%d", g_nCapLineNumL, g_nCapLineNumH);

    for (i=0; i<g_nCapLineNumH; i++)
    {
        for (j=0; j<g_nCapLineNumL; j++)
        {
            g_nCapBaseValue[i][j] = g_pCapInfo->nBaseValue[i][j];
        }
    }

    for (i=0; i<g_nCapLineNumH; i++)
    {
        for (j=0; j<g_nCapLineNumL; j++)
        {
            g_nCapChgValue[i][j] = g_pCapInfo->nChgValue[i][j];
        }
    }
}