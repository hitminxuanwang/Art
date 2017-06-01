#ifndef CAPTEST_H
#define CAPTEST_H

#include "qt_headfiles.h"
#include "global.h"

class QGraphicsLineItem;
class QPushButton;
class QTextBrowser;
class QGraphicsEllipseItem;
class QHBoxLayout;
class QVBoxLayout;
class QRadioButton;
//class CapTestView;
class CapImgWidget;
//class CapTestLine;
//class CrossEllipseItem;

class CapTest:public QWidget
{
    Q_OBJECT
public:
    CapTest(QWidget *parent=0);
    ~CapTest();
    void OnInitWidget();
    void ResultBrowserAppendText(QString szText);
    void ResultBrowserClearText();
    void ParmBrowserAppendText(QString szText);
    void ParmBrowserClearText();
    void SetGetParmBtnEnabled(bool bEnabled);
    //void UpdateData();
    void closeEvent(QCloseEvent *event);
protected:
    void keyPressEvent(QKeyEvent *event);
public slots:
    void DealWithGetParm();
    void DealWithUpdateSpeed();
    void RefreshImg();
    void UpdateData();
    //void DealWithStartSave();
    //void DealWithStop();
    //void DealWithSaveData();
    void DealWithRealBtnToggled(bool);
    void DealWithDifBtnToggled(bool);
    void SlotUpdateWidgetLangShow(int);
    //void SlotShowBadPtInfo();
    void SlotRefreshTimeSliderChanged(int);
    void SlotRefreshTimeEditingfished();
private:
    //CapTestView *capTestView;
    CapImgWidget *capImg;
    //QGraphicsScene *m_Scene;
    //QGraphicsLineItem *m_lineItemH[200];
    //QGraphicsLineItem *m_lineItemL[200];
    //QGraphicsEllipseItem m_CrossPt[200][200];
    //CrossEllipseItem *m_CrossPt[200][200];
    QLineF lineH[CAP_MAX_NUMBER_H];
    QLineF lineL[CAP_MAX_NUMBER_L];
    
    int m_nLineNumH;    //垂直线条数(垂直方向线条数,线是水平方向)
    int m_nLineNumL;    //水平线条数(水平方向线条数,线是垂直方向)
    int m_nDeskTopWidth;
    int m_nDeskTopHeight;
    int m_nWidth;
    int m_nHeight;
    int m_nDataType;    //数据类型,0:实时数据,1:差值数据
    int m_nSaveStatus;  //是否处于保存数据状态,0:否,1:是
    int m_nRefreshTime;
    //int m_nIsSceneEmpty;    //场景里是否为空,0:否,1:是

    QFile *capFile;
    QDataStream out;
    //QByteArray capDataArray;
    QTimer *updateSpeedTimer;
    QTimer *refreshImgTimer;
    QTimer *updateDataTimer;
    QTimer *badPtTimer;
    //QTimer *saveDataTimer;
    QPushButton *getParmBtn;
    QPushButton *startSaveBtn;
    QPushButton *stopBtn;
    QPushButton *quitBtn;
    QTextBrowser *resultBrowser;
    QTextBrowser *parmBrowser;
    QLabel *labelBadPtInfo;
    QLabel*labelRefreshTime;
    QSlider *refreshTimeSlider;
    QLineEdit *refreshTimeLineEdit;
    QRadioButton *realDataBtn;
    QRadioButton *diffDataBtn;

    QVBoxLayout *mainLayout;
    QHBoxLayout *topHLayout;
};

/*
class CapTestView: public QGraphicsView
{
    Q_OBJECT
public:
    CapTestView(QGraphicsScene *scene, QWidget *parent = 0);
    ~CapTestView();
    void OnInitView();
    void wheelEvent(QWheelEvent *event);
    void UpdateData();
protected:
    void mouseDoubleClickEvent (QMouseEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);

private:
    QPointF curCursorPt;
    QGraphicsItem *centerItem;
};
*/

//################################################################################################
//################################################################################################
/*
class CapTestLine: public QGraphicsLineItem
{
    Q_OBJECT
public:
    CapTestLine(QLineF line);
    ~CapTestLine();
};
*/

/*
class CrossEllipseItem: public QWidget,public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    CrossEllipseItem(QGraphicsEllipseItem *parent = 0);
    ~CrossEllipseItem();
    void SetID(int nXID, int nYID);
    void GetID(int &nXID, int &nYID);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    int m_nXID;
    int m_nYID;
};
*/

class CapImgWidget: public QWidget
{
    Q_OBJECT
public:
    CapImgWidget(QWidget *parent = 0);
    void OnInitWidget();
    void SetLineNum(int nLineNumH, int nLineNumL);
    void SetSaveStatus(int nSaveStatus);
    void UpdateData();
    ~CapImgWidget();
protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    
private:
    QLineF lineH[CAP_MAX_NUMBER_H];
    QLineF lineL[CAP_MAX_NUMBER_L];
    
    int m_nLineNumH;    //垂直线条数(垂直方向线条数,线是水平方向)
    int m_nLineNumL;    //水平线条数(水平方向线条数,线是垂直方向)
    int m_nDeskTopWidth;
    int m_nDeskTopHeight;
    int m_nWidth;
    int m_nHeight;
    int m_nSaveStatus;  //是否处于数据保存状态,0:否,1:是
};

#endif //CAPTEST_H
