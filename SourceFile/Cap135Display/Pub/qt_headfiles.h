#ifndef QT_HEADFILES_H
#define QT_HEADFILES_H


/* core */
#include <QtGui>
#include <QApplication>
#include <QDesktopWidget>
#include <QAction>

/* network */
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QHostInfo>
//#include <QNetworkInterface>
//#include <QTcpSocket>
//#include <QUdpSocket>

/* process & thread */
#include <QMutex>
#include <QProcess>

/* files */
#include <QFile>
#include <QLibrary>
#include <QList>
#include <QString>
#include <QDir>
#include <QUrl>
#include <QTextCodec>
//#include <QDomDocument>
#include <QMap>

/* widgets */
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QStatusBar>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QFileDialog>
#include <QCompleter>
#include <QHeaderView>
#include <QDialog>
#include <QStylePainter>
#include <QSystemTrayIcon>
#include <QListView>
#include <QProgressDialog>
#include <QScrollBar>
#include <QMenu>
#include <QTextBrowser>

#ifdef VOS_WINDOWS
    #if _MSC_VER >= 1600
    #pragma execution_character_set("utf-8")    //告诉windows编译器源码的编码格式解决代码中直接赋值的中文运行时显示乱码问题,放在这个头文件里是为了确保所有界面类都可以包含到
    #endif
#endif //VOS_WINDOWS

#endif

