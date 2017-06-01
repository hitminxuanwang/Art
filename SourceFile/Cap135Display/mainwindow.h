#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "capTest.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void SlotReadShareMemory();

private:
    Ui::MainWindow *ui;
    QTimer *readShareMemTimer;
public:
    CapTest *guiCapTest;
};

extern MainWindow *GUI_SS;

#endif // MAINWINDOW_H
