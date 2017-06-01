#include "capTest.h"
#include "global.h"
#include "guiGlobal.h"
#include "mainwindow.h"
//#include "ss_gui.h"
#include "math.h"
//#include "../Pub/Bulk_cap.h"

int g_gui_nXID = 0;
int g_gui_nYID = 0;

#define CAP_MAX_VALUE 256

CapTest::CapTest(QWidget *parent)
{
    Q_UNUSED(parent);
    OnInitWidget();
    setAttribute(Qt::WA_DeleteOnClose);
}

/***********************************************************
Function     :   OnInitWidget
Description :   初始化函数
Input         :   无
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.22
***********************************************************/

void CapTest::OnInitWidget()
{
    //int i = 0;
    //m_nDeskTopWidth = QApplication::desktop()->width();
    //m_nDeskTopHeight = QApplication::desktop()->height();
    m_nDeskTopWidth = g_nMainScreenX;
    m_nDeskTopHeight = g_nMainScreenY;
    
    capImg = new CapImgWidget;

    updateSpeedTimer = new QTimer;
    refreshImgTimer = new QTimer;
    //saveDataTimer = new QTimer;
    updateDataTimer = new QTimer;
    //badPtTimer = new QTimer;
    
    getParmBtn = new QPushButton(tr("获取参数"));
    getParmBtn->setFixedSize(100, 50);
    //getParmBtn->setEnabled(false);
    startSaveBtn = new QPushButton(tr("开始保存"));
    startSaveBtn->setFixedSize(100, 50);
    startSaveBtn->setEnabled(false);
    stopBtn = new QPushButton(tr("停止"));
    stopBtn->setFixedSize(100, 50);
    stopBtn->setEnabled(false);
    quitBtn = new QPushButton(tr("退出"));
    quitBtn->setFixedSize(100, 50);

    labelRefreshTime = new QLabel("刷新时间(s)");
    refreshTimeSlider = new QSlider(Qt::Horizontal);
    refreshTimeSlider->setMinimum(1);
    refreshTimeSlider->setMaximum(180);
    m_nRefreshTime = 10;
    refreshTimeLineEdit = new QLineEdit;
    refreshTimeSlider->setValue(m_nRefreshTime);
    refreshTimeLineEdit->setText(QString::number(m_nRefreshTime));
    
    resultBrowser = new QTextBrowser;
    resultBrowser->setFixedHeight(100);
    parmBrowser = new QTextBrowser;
    parmBrowser->setFixedHeight(100);

    labelBadPtInfo = new QLabel;

    realDataBtn = new QRadioButton("实时值");
    diffDataBtn = new QRadioButton("差值");
    diffDataBtn->setChecked(true);
    m_nDataType = 1;

    capFile = NULL;
    
    topHLayout = new QHBoxLayout;
    mainLayout = new QVBoxLayout;

    topHLayout->addWidget(getParmBtn);
    //topHLayout->addWidget(startSaveBtn);	//数据采集功能不再需要
    //topHLayout->addWidget(stopBtn);
    topHLayout->addWidget(quitBtn);
    topHLayout->addWidget(realDataBtn);
    topHLayout->addWidget(diffDataBtn);

    topHLayout->addWidget(labelRefreshTime);
    topHLayout->addWidget(refreshTimeLineEdit);
    topHLayout->addWidget(refreshTimeSlider);
    
    topHLayout->addWidget(resultBrowser);
    topHLayout->addWidget(parmBrowser);
    topHLayout->addWidget(labelBadPtInfo);
    mainLayout->addLayout(topHLayout);
    mainLayout->addWidget(capImg);
    //mainLayout->addWidget(capTestView);

    connect(getParmBtn, SIGNAL(clicked()), this, SLOT(DealWithGetParm()));
    //connect(startSaveBtn, SIGNAL(clicked()), this, SLOT(DealWithStartSave()));
    //connect(stopBtn, SIGNAL(clicked()), this, SLOT(DealWithStop()));
    connect(quitBtn, SIGNAL(clicked()), this, SLOT(close()));
    connect(updateSpeedTimer, SIGNAL(timeout()), this, SLOT(DealWithUpdateSpeed()));
    connect(refreshImgTimer, SIGNAL(timeout()), this, SLOT(RefreshImg()));
    connect(updateDataTimer, SIGNAL(timeout()), this, SLOT(UpdateData()));
    //connect(badPtTimer, SIGNAL(timeout()), this, SLOT(SlotShowBadPtInfo()));
    //connect(saveDataTimer, SIGNAL(timeout()), this, SLOT(DealWithSaveData()));
    connect(realDataBtn, SIGNAL(toggled(bool)), this, SLOT(DealWithRealBtnToggled(bool)));
    connect(diffDataBtn, SIGNAL(toggled(bool)), this, SLOT(DealWithDifBtnToggled(bool)));
    connect(refreshTimeSlider, SIGNAL(valueChanged(int)), this, SLOT(SlotRefreshTimeSliderChanged(int)));
    connect(refreshTimeLineEdit, SIGNAL(editingFinished()), this, SLOT(SlotRefreshTimeEditingfished()));
    setFixedSize(m_nDeskTopWidth, m_nDeskTopHeight);
    setLayout(mainLayout);
    //g_nCapGuiFlag = 1;
    SlotUpdateWidgetLangShow(g_gui_nLEDLangType);
}


/***********************************************************
Function     :   ResultBrowserAppendText
Description :   resultBrowser控件appendText
Input         :   szText 要追加的内容
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.22
***********************************************************/

void CapTest::ResultBrowserAppendText(QString szText)
{
    resultBrowser->append(szText);
}

/***********************************************************
Function     :   ResultBrowserClearText
Description :   resultBrowser控件清空内容
Input         :   无
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.22
***********************************************************/

void CapTest::ResultBrowserClearText()
{
    resultBrowser->clear();
}

/***********************************************************
Function     :   ParmBrowserAppendText
Description :   parmBrowser控件appendText
Input         :   szText 要追加的内容
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.22
***********************************************************/

void CapTest::ParmBrowserAppendText(QString szText)
{
    parmBrowser->append(szText);
}

/***********************************************************
Function     :   ParmBrowserClearText
Description :   parmBrowser控件清空内容
Input         :   无
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.22
***********************************************************/

void CapTest::ParmBrowserClearText()
{
    parmBrowser->clear();
}

void CapTest::SetGetParmBtnEnabled(bool bEnabled)
{
    getParmBtn->setEnabled(bEnabled);
}


/***********************************************************
Function     :   DealWithGetParm
Description :   点击"获取参数"按钮后触发的处理
Input         :   无
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.24
***********************************************************/

void CapTest::DealWithGetParm()
{
    //int i, j;

    QPen pen;
    QColor color;
    QBrush brush(Qt::SolidPattern);
    pen.setColor(Qt::lightGray);

    if (false == updateSpeedTimer->isActive())
    {
        updateSpeedTimer->start(2000);
    }

    //m_nLineNumH = g_cCapBaseInfo.LineNumH;
    //m_nLineNumL = g_cCapBaseInfo.LineNumL;
    #if 0
    m_nLineNumH = g_nCapLineNumH;
    m_nLineNumL = g_nCapLineNumL;
    #else
    m_nLineNumH = g_nCapLineNumL;
    m_nLineNumL = g_nCapLineNumH;
    #endif //0
    HHT_LOG(EN_INFO, "  LineNumH:%d, LineNumL:%d", m_nLineNumH, m_nLineNumL);
    if ((m_nLineNumH <=0) || (m_nLineNumL <= 0))
    {
        HHT_LOG(EN_ERR, "   Invalid Param");
        return;
    }

    if (false == refreshImgTimer->isActive())
    {
        refreshImgTimer->start(50);
    }

    if (false == updateDataTimer->isActive())
    {
        updateDataTimer->start(10);
    }
    /*
    if (!badPtTimer->isActive())
    {
        badPtTimer->start(m_nRefreshTime*1000);
        HHT_LOG(EN_INFO, "  start badPtTime set g_nCapBadPtNum = 0");
        g_nCapBadPtNum = 0;
    }
    */
    
    capImg->SetLineNum(m_nLineNumH, m_nLineNumL);
    g_gui_nCapMode = 1;
    startSaveBtn->setEnabled(true);
}

/***********************************************************
Function     :   DealWithUpdateSpeed
Description :   updateSpeedTimer发出timeout信号后触发的处理
Input         :   无
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.24
***********************************************************/

void CapTest::DealWithUpdateSpeed()
{
    //int nSpeed = 0;
    //int nLostFrameNum = 0;
    QString szSpeed = "";
    QString szLineNum = "";
    
    //nSpeed = g_cCapDataProcess.GetScanSpeed()/2.0;
    //nLostFrameNum = g_cCapDataProcess.GetLostFrameNum();
    //GUI_LOG(EN_INFO, "  Speed:%d", nSpeed);

    if (0 == g_gui_nLEDLangType)
    {
        //szSpeed.append("帧速:");
        //szSpeed.append(QString::number(nSpeed));
        //szSpeed.append(" ,丢失帧数:");
        //szSpeed.append(QString::number(nLostFrameNum));
        
        szLineNum.append("纵向线条数:");
        szLineNum.append(QString::number(m_nLineNumH));
        szLineNum.append("，横向线条数:");
        szLineNum.append(QString::number(m_nLineNumL));
    }
    else
    {
        //szSpeed.append("Frame Rete:");
        //szSpeed.append(QString::number(nSpeed));
        //szSpeed.append(" ,The num of frame lost:");
        //szSpeed.append(QString::number(nLostFrameNum));
        
        szLineNum.append("Horizonal Line Num:");
        szLineNum.append(QString::number(m_nLineNumH));
        szLineNum.append(", Vertical Line Num:");
        szLineNum.append(QString::number(m_nLineNumL));
    }

    ParmBrowserClearText();
    ParmBrowserAppendText(szLineNum);
    ParmBrowserAppendText(szSpeed);
}


/***********************************************************
Function     :   RefreshImg
Description :   refreshTimer发出timeout信号后触发的处理
Input         :   无
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.24
***********************************************************/

void CapTest::RefreshImg()
{
    if (1 == g_gui_nCapMode)
    {
        capImg->update();
    }
}

/***********************************************************
Function     :   UpdateData
Description :   updateDataTimer发出timeout信号后触发的处理
Input         :   无
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.08.15
***********************************************************/

void CapTest::UpdateData()
{
    capImg->UpdateData();
}
#if 0
/***********************************************************
Function     :   DealWithStartSave
Description :   电容测试界面点击"开始保存"后触发的处理
Input         :   无
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.29
***********************************************************/

void CapTest::DealWithStartSave()
{
    startSaveBtn->setEnabled(false);
    stopBtn->setEnabled(true);

    QString szFileName = "";

    szFileName.append(g_szConfigPath);
    szFileName.append("/");
    szFileName.append("cap.dat");

    capFile = new QFile(szFileName);

    if (!capFile->open(QIODevice::WriteOnly| QFile::Truncate))
    {
        HHT_LOG(EN_ERR, "capFile open error !");
    }

    g_bytearrayCapRealData.clear();
    g_nCapDataSaveFlag = 1;
}

/***********************************************************
Function     :   DealWithStop
Description :   电容测试界面点击"停止"后触发的处理
Input         :   无
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.29
***********************************************************/

void CapTest::DealWithStop()
{
    int i = 0;
    //m_nSaveStatus = 0;
    //capImg->SetSaveStatus(0);
    g_nCapDataSaveFlag = 0;
    stopBtn->setEnabled(false);
    startSaveBtn->setEnabled(true);
    
    QDataStream out(capFile);
    //长度2bytes + 宽度2bytes + 508bytes填0 作为数据头
    out<<(quint16)m_nLineNumL;
    out<<(quint16)m_nLineNumH;

    for (i=0; i<508; i++)
    {
        out<<(quint8)0;
    }
    //out<<g_gui_capDataArray;
    out<<g_bytearrayCapRealData;
    //capFile->flush();
}
void CapTest::SlotShowBadPtInfo()
{
    int nMin = 0;
    int nMax = 0;
    QString strText = "";
    g_listCapBadPtNum.append(g_nCapBadPtNum);
    HHT_LOG(EN_INFO, "  g_nCapBadPtNum=%d", g_nCapBadPtNum);

    nMin = MinValueOfQList(g_listCapBadPtNum);
    nMax = MaxValueOfQList(g_listCapBadPtNum);

    strText.append("异常点个数:");
    strText.append(QString::number(g_nCapBadPtNum));
    strText.append(", 最大值:");
    strText.append(QString::number(nMax));
    strText.append(",最小值:");
    strText.append(QString::number(nMin));
    labelBadPtInfo->setText(strText);
    HHT_LOG(EN_INFO, " issac, set g_nCapBadPtNum = 0");
    g_nCapBadPtNum = 0;
}
#endif //0


void CapTest::SlotRefreshTimeSliderChanged(int nValue)
{
    m_nRefreshTime = nValue;
    refreshTimeLineEdit->setText(QString::number(nValue));
    /*
    if (badPtTimer->isActive())
    {
        badPtTimer->stop();
        badPtTimer->start(nValue * 1000);
    }
    */
}

void CapTest::SlotRefreshTimeEditingfished()
{
    m_nRefreshTime = refreshTimeLineEdit->text().toInt();
    refreshTimeSlider->setValue(m_nRefreshTime);
    /*
    if (badPtTimer->isActive())
    {
        badPtTimer->stop();
        badPtTimer->start(m_nRefreshTime * 1000);
    }
    */
}



#if 0
/***********************************************************
Function     :   DealWithSaveData
Description :   saveDataTimer发出timeout信号后触发的处理
Input         :   无
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.08.01
***********************************************************/

void CapTest::DealWithSaveData()
{
    int i, j;
    static int nCount = 0;
    
    if  (1 == m_nSaveStatus)
    {   
        //out.setDevice(capFile);
        //存储的时候是一行一行存储
        for (j=0; j<m_nLineNumH; j++)
        {
            for (i=0; i<m_nLineNumL; i++)
            {
                //out<<(quint8)" ";
                //out<<(quint8)g_gui_nCapValue[i][j];
                capDataArray += (quint8)g_gui_nCapValue[i][j];
            }
        }
    }

    nCount++;
    if (nCount >= 6000)
    {
        m_nSaveStatus = 0;
    }
}
#endif //0


/***********************************************************
Function     :   DealWithRealBtnToggled
Description :   电容白板调试界面实时值单选框状态
                     发生变化触发的函数
Input         :   bChecked 该单选框是否被选上
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.31
***********************************************************/

void CapTest::DealWithRealBtnToggled(bool bChecked)
{
    if (true == bChecked)
    {
        m_nDataType = 0;
        g_gui_nCapDataType = 0;
    }
}

/***********************************************************
Function     :   DealWithDifBtnToggled
Description :   电容白板调试界面差值单选框状态
                     发生变化触发的函数
Input         :   bChecked 该单选框是否被选上
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.31
***********************************************************/

void CapTest::DealWithDifBtnToggled(bool bChecked)
{
    if (true == bChecked)
    {
        m_nDataType = 1;
        g_gui_nCapDataType = 1;
    }
}

void CapTest::SlotUpdateWidgetLangShow(int nLangType)
{
    if (0 == nLangType)
    {
        getParmBtn->setText(tr("获取参数"));
        startSaveBtn->setText(tr("开始保存"));
        stopBtn->setText(tr("停止"));
        quitBtn->setText(tr("退出"));
        realDataBtn->setText("实时值");
        diffDataBtn->setText("差值");
        labelRefreshTime ->setText("刷新时间(s)");
    }
    else
    {
        getParmBtn->setText(tr("Get Params"));
        startSaveBtn->setText(tr("Start Save"));
        stopBtn->setText(tr("Stop"));
        quitBtn->setText(tr("Exit"));
        realDataBtn->setText("Real Value");
        diffDataBtn->setText("Diff Value");
        labelRefreshTime->setText("Refresh time(s)");
    }
}

/***********************************************************
Function     :   closeEvent
Description :   电容白板调试界面调用close后继续调用的函数
                     这里对默认的closeEvent 进行重载以便在界面
                     退出时进行特殊动作
Input         :   QCloseEvent 
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.22
***********************************************************/

void CapTest::closeEvent(QCloseEvent * event)
{
    event->accept();
    //g_gui_nWindowOpened = 0;
    g_gui_nCapMode = 0;
    if (updateSpeedTimer->isActive())
    {
        updateSpeedTimer->stop();
    }
    if (refreshImgTimer->isActive())
    {
        refreshImgTimer->stop();
    }
    if (updateDataTimer->isActive())
    {
        updateDataTimer->stop();
    }

    /*
    if (badPtTimer->isActive())
    {
        badPtTimer->stop();
    }

    g_listCapBadPtNum.clear();
    */
    //g_nCapGuiFlag = 0;
    //CapRtValueTest_Stop();
    //StopScan();
    /*
    CAPBRDTest_Stop();
    GUI_LOG(EN_INFO, "  CAPBRDTest_Stop");
    */
}

/***********************************************************
Function     :   keyPressEvent
Description :   按键处理函数
                     这里对默认的keyPressEvent 进行重载以便
                     对ESC按键进行捕获,主要是给g_gui_nWindowOpened
                     变量赋值，以免其他窗口打不开
Input         :   QKeyEvent 
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.22
***********************************************************/

void CapTest::keyPressEvent(QKeyEvent * event)
{
    switch(event->key())
    {
        case Qt::Key_Escape:
            //g_gui_nWindowOpened = 0;
            QWidget::close();
            break;
        default:
            QWidget::keyPressEvent(event);
    }
}


CapTest::~CapTest()
{    
    delete capImg;
    delete getParmBtn;
    delete startSaveBtn;
    delete stopBtn;
    delete quitBtn;
    if (NULL != capFile)
    {
        delete capFile;
    }
    delete resultBrowser;
    delete parmBrowser;
    if (true == updateSpeedTimer->isActive())
    {
        updateSpeedTimer->stop();
    }
    delete updateSpeedTimer;
    
    if (true == refreshImgTimer->isActive())
    {
        refreshImgTimer->stop();
    }
    delete refreshImgTimer;

    if (updateDataTimer->isActive())
    {
        updateDataTimer->stop();
    }
    delete updateDataTimer;
/*
    if (badPtTimer->isActive())
    {
        badPtTimer->stop();
    }
    delete badPtTimer;
*/
    /*    
    if (true == saveDataTimer->isActive())
    {
        saveDataTimer->stop();
    }
    delete saveDataTimer;
    */
    
    delete realDataBtn;
    delete diffDataBtn;
    delete labelRefreshTime;
    delete refreshTimeLineEdit;
    delete refreshTimeSlider;
    delete labelBadPtInfo;
    delete topHLayout;
    delete mainLayout;
}

//################################################################################################
//################################################################################################

#if 0
CapTestView::CapTestView(QGraphicsScene *scene, QWidget *parent):
        QGraphicsView(scene, parent)
{
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    OnInitView();
    setAttribute(Qt::WA_DeleteOnClose);
}

/***********************************************************
Function     :   OnInitView
Description :   初始化函数
Input         :   无
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.22
***********************************************************/

void CapTestView::OnInitView()
{
    centerItem = NULL;
}


/***********************************************************
Function     :   wheelEvent
Description :   重载wheelEvent函数
Input         :   QWheelEvent
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.15
***********************************************************/

void CapTestView::wheelEvent(QWheelEvent * event)
{
    double dNumDegrees = event->delta()/8.0;
    double dNumSteps = dNumDegrees/15.0;
    double dFactor = pow(1.125, dNumSteps);
    scale(dFactor, dFactor);
    if (centerItem)
    {
        centerOn(centerItem);
    }
}

void CapTestView::UpdateData()
{
    
}

/***********************************************************
Function     :   mouseDoubleClickEvent
Description :   重载QGraphicsView的mouseDoubleClickEvent函数
Input         :   QMouseEvent
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.19
***********************************************************/

void CapTestView::mouseDoubleClickEvent(QMouseEvent * event)
{
    curCursorPt = event->posF();
    centerItem = itemAt(event->pos());
    //qDebug()<<"curCursorPt:"<<curCursorPt;

    QGraphicsView::mouseDoubleClickEvent (event);
}

/*
void CapTestView::mouseMoveEvent(QMouseEvent * event)
{
    CrossEllipseItem::mouseMoveEvent (event);
}
*/



CapTestView::~CapTestView()
{
}

//################################################################################################
//################################################################################################

/*
CapTestLine::CapTestLine(QLineF line)
{
    setLine(line);
}

CapTestLine::~CapTestLine()
{
    
}
*/

CrossEllipseItem::CrossEllipseItem(QGraphicsEllipseItem *parent)
{
    setMouseTracking(true);
    setAttribute(Qt::WA_DeleteOnClose);
}

/***********************************************************
Function     :   SetID
Description :   设置当前交点的水平编号和垂直编号
Input         :   nXID 交点水平编号,YID交点垂直编号
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.19
***********************************************************/

void CrossEllipseItem::SetID(int nXID, int nYID)
{
    m_nXID = nXID;
    m_nYID = nYID;
}

/***********************************************************
Function     :   GetID
Description :   获取当前交点的水平编号和垂直编号
Input         :   nXID 交点水平编号,YID交点垂直编号
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.19
***********************************************************/

void CrossEllipseItem::GetID(int & nXID, int & nYID)
{
    nXID = m_nXID;
    nYID = m_nYID;
}

/***********************************************************
Function     :   mousePressEvent
Description :   重载QGraphicsEllipse的mousePressEvent函数
Input         :   QGraphicsSceneMouseEvent
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.19
***********************************************************/

void CrossEllipseItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    QString szText = "";
    GetID(g_gui_nXID, g_gui_nYID);
    szText.append("XID:");
    szText.append(QString::number(g_gui_nXID));
    szText.append(", YID:");
    szText.append(QString::number(g_gui_nYID));
    szText.append(", Value:");
    szText.append(QString::number(g_nCapValue[g_gui_nXID][g_gui_nYID]));
    GUI_SS->guiCapTest->ResultBrowserClearText();
    GUI_SS->guiCapTest->ResultBrowserAppendText(szText);
    QGraphicsEllipseItem::mousePressEvent(event);
}

/***********************************************************
Function     :   mouseMoveEvent
Description :   重载QGraphicsEllipse的mouseMoveEvent函数
Input         :   QMouseEvent
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.19
***********************************************************/

void CrossEllipseItem::mouseMoveEvent(QMouseEvent * event)
{
    QString szText = "";
    GetID(g_gui_nXID, g_gui_nYID);
    szText.append("XID:");
    szText.append(QString::number(g_gui_nXID));
    szText.append(", YID:");
    szText.append(QString::number(g_gui_nYID));
    GUI_SS->guiCapTest->ResultBrowserClearText();
    GUI_SS->guiCapTest->ResultBrowserAppendText(szText);
    QWidget::mouseMoveEvent(event);
} 

CrossEllipseItem::~CrossEllipseItem()
{
}
#endif //0

//################################################################################################
//################################################################################################

//电容测试图像类
CapImgWidget::CapImgWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    setAttribute(Qt::WA_DeleteOnClose);
    setMouseTracking(true);
    OnInitWidget();
}

/***********************************************************
Function     :   OnInitWidget
Description :   初始化函数
Input         :   无
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.22
***********************************************************/

void CapImgWidget::OnInitWidget()
{
    //m_nDeskTopWidth = QApplication::desktop()->width();
    //m_nDeskTopHeight = QApplication::desktop()->height();
    m_nDeskTopWidth = g_nMainScreenX;
    m_nDeskTopHeight = g_nMainScreenY;
    m_nWidth = m_nDeskTopWidth;
    m_nHeight = m_nDeskTopHeight-100;
    setFixedSize(m_nWidth, m_nHeight-50);
}

/***********************************************************
Function     :   SetLineNum
Description :   设置线条信息
Input         :   nLineNumH, nLineNumL
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.08.15
***********************************************************/

void CapImgWidget::SetLineNum(int nLineNumH, int nLineNumL)
{
    m_nLineNumH = nLineNumH;
    m_nLineNumL = nLineNumL;
}


/***********************************************************
Function     :   SetSaveStatus
Description :   设置数据保存状态
Input         :   nSaveStatus 是否处于数据保存状态,0:否,1:是
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.08.15
***********************************************************/

void CapImgWidget::SetSaveStatus(int nSaveStatus)
{
    m_nSaveStatus = nSaveStatus;
}

/***********************************************************
Function     :   UpdateData
Description :   更新数据
Input         :   无
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.08.15
***********************************************************/

void CapImgWidget::UpdateData()
{
    int i, j;
    int nLineH = 0;
    int nLineL = 0;

    /*
    if ((m_nLineNumH <= 0) || (m_nLineNumL <= 0))
    {
        //GUI_LOG(EN_ERR, "   Invalid Param");
        return;
    }
    */

    for (i=0; i<m_nLineNumL; i++)
    {
        for (j=0; j<m_nLineNumH; j++)
        {
            //GUI默认原点在左上角,X由左到右增加,Y由上到下增加,实际数据可能不是,故需要转换

            /*
            if (0 == g_cCapBaseInfo.dataSeq)
            {
                nLineH = i;
                nLineL = j;
            }
            else
            {
                nLineH = m_nLineNumL - j - 1;
                nLineL = m_nLineNumH - i - 1;
            }
            */

            nLineH = i;
            nLineL = j;
            //g_gui_nCapValue[nLineH][nLineL] = g_nCapValue[i][j];

            /*
            g_gui_nCapChgValue[nLineH][nLineL] = abs(g_nCapChgValue[i][j]);
            g_gui_nCapValue[nLineH][nLineL] = g_nCapBaseValue[i][j]-g_gui_nCapChgValue[nLineH][nLineL];
            */
            g_gui_nCapChgValue[i][j] = abs(g_nCapChgValue[i][j]);
            g_gui_nCapValue[i][j] = g_nCapBaseValue[i][j]-g_gui_nCapChgValue[i][j];
        }        
    }

    #if 0
    if  (1 == m_nSaveStatus)
    {   
        //out.setDevice(capFile);
        //存储的时候是一行一行存储
        for (j=0; j<m_nLineNumH; j++)
        {
            for (i=0; i<m_nLineNumL; i++)
            {
                //out<<(quint8)" ";
                //out<<(quint8)g_gui_nCapValue[i][j];
                g_gui_capDataArray += (quint8)g_gui_nCapValue[i][j];
            }
        }
    }
    #endif //0
}


/***********************************************************
Function     :   paintEvent
Description :   重载绘图函数
Input         :   无
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.25
***********************************************************/

void CapImgWidget::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);
    if (0 == g_gui_nCapMode)
    {
        return;
    }
    
    int i, j;
    
    
    QPen pen;
    QColor color;
    QColor realColor;
    QColor diffColor;
    QPainter widgetpainter(this);
    QBrush brush(Qt::SolidPattern);
    pen.setColor(Qt::lightGray);
    widgetpainter.setPen(pen);

    #if 0
    m_nLineNumH = g_cCapBaseInfo.LineNumH;
    m_nLineNumL = g_cCapBaseInfo.LineNumL;
    //GUI_LOG(EN_INFO, "  LineNumH:%d, LineNumL:%d", m_nLineNumH, m_nLineNumL);
    #endif 



    int nYInterval = m_nHeight/m_nLineNumH;
    int nXInterval = m_nWidth/m_nLineNumL;

    for (i=0; i<m_nLineNumH; i++)
    {
        lineH[i] = QLineF(10, 10+i*nYInterval, 10+(m_nLineNumL-1)*nXInterval, 10+i*nYInterval);
        widgetpainter.drawLine(lineH[i]);
    }
    for (i=0; i<m_nLineNumL; i++)
    {
        lineL[i] = QLineF(10+i*nXInterval, 10, 10+i*nXInterval, 10+(m_nLineNumH-1)*nYInterval);
        widgetpainter.drawLine(lineL[i]);
    }

    for (i=0; i<m_nLineNumL; i++)
    {
        for (j=0; j<m_nLineNumH; j++)
        {
            if (0 != g_gui_nCapChgValue[i][j])
            {
                g_gui_nCapChgValue[i][j] += 10; //徐工要求加
            }
            if (g_gui_nCapChgValue[i][j]*4 < 40)
            {
                diffColor = Qt::lightGray;
            }
            else if (g_gui_nCapChgValue[i][j]*4 >= 255)
            {
                diffColor.setRgb(255, 0, 15, 255);
            }
            else
            {
                diffColor.setRgb(g_gui_nCapChgValue[i][j]*4, 255-g_gui_nCapChgValue[i][j]*4, 15*(double)g_gui_nCapChgValue[i][j]*4/255.0, 255);
            }
            
            double dAlpha;
            int nFactor;
            if (g_gui_nCapValue[i][j] <= 38)
            {
                dAlpha = 0.0;
            }
            else if (g_gui_nCapValue[i][j] >= 193)
            {
                dAlpha = 255.0;
            }
            else
            {
                dAlpha = (double)g_gui_nCapValue[i][j]/(193-38)*255.0;
            }
            nFactor = g_gui_nCapValue[i][j]/38.0*100;
            if (g_gui_nCapValue[i][j] <= 38)
            {
                nFactor = 100;  //100表示无变化
            }
            else if (g_gui_nCapValue[i][j] >= 193)
            {
                nFactor = 508;  //508 是当value=193时算出来的factor, 193/38.0*100;
            }
            
            color = Qt::darkCyan;
            realColor = color.lighter(nFactor);
            //color.setAlphaF(dAlpha);
            //color.setRgb(255-(255-8)*(double)g_nCapValue[i][j]/255.0, (255-8)*(double)g_nCapValue[i][j]/255.0, 15*(double)g_nCapValue[i][j]/255.0, 255);
            if (0 == g_gui_nCapDataType)
            {
                pen.setColor(realColor);
                brush.setColor(realColor);
            }
            else
            {
                pen.setColor(diffColor);
                brush.setColor(diffColor);
            }
            widgetpainter.setPen(pen);
            widgetpainter.setBrush(brush);
            //widgetpainter.drawEllipse(QRectF(10+i*nXInterval-1, 10+j*nYInterval-1, 2, 2));
            widgetpainter.drawRect(QRectF(10+i*nXInterval-2, 10+(m_nLineNumH - j - 1)*nYInterval-2, 4, 4));
        }
    }
}

/***********************************************************
Function     :   mouseMoveEvent
Description :   重载QWidget的mouseMoveEvent函数
Input         :   QMouseEvent
Output       :   无
Return       :   无
Others       :  
Author       :
History      :   1. 创建函数2013.07.25
***********************************************************/

void CapImgWidget::mouseMoveEvent(QMouseEvent * event)
{
    if (0 == g_gui_nCapMode)
    {
        return;
    }
    int i, j;
    int nX = 0;
    int nY = 0;
    int nYInterval = m_nHeight/m_nLineNumH;
    int nXInterval = m_nWidth/m_nLineNumL;
    int nXID = 0;
    int nYID = 0;

    nX = event->x();
    nY = event->y();

    for (i=0; i<m_nLineNumL; i++)
    {
        for (j=0; j<m_nLineNumH; j++)
        {
            //QRectF crossRect = QRectF(10+i*nXInterval-1, 10+j*nYInterval-1, 2, 2);
            QRectF crossRect = QRectF(10+i*nXInterval-2, 10+j*nYInterval-2, 4, 4);
            if (crossRect.contains(nX, nY))
            {
                nXID = i;
                nYID = m_nLineNumH - 1 - j;
                QString szText = "";
                szText.append("XID:");
                szText.append(QString::number(nXID + 1));
                szText.append(", YID:");
                szText.append(QString::number(nYID + 1));
                szText.append(", Value:");
                szText.append(QString::number(g_gui_nCapValue[nXID][nYID]));
                szText.append(", DiffValue:");
                szText.append(QString::number(g_gui_nCapChgValue[nXID][nYID]));
                GUI_SS->guiCapTest->ResultBrowserClearText();
                GUI_SS->guiCapTest->ResultBrowserAppendText(szText);
                break;
            }
        }
    }

    QWidget::mouseMoveEvent(event);
}

CapImgWidget::~CapImgWidget()
{

}
