/*****************************************************************************
* \file mainwindow.h
* \author Zirui Xu
* \date 2023/03/21
* \version v0.2
* \brief 定义了整个窗体上的各类控件
* \note Copyright (c) 2020-2030 南京理工大学
* \remarks 整理代码格式
******************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <element.h>
#include <paintwidget.h>
#include <QPushButton>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:


    MainWindow(QWidget *parent = nullptr);


    ~MainWindow();


    void setCurrentButton(QPushButton *button);


    QString getIdelColor();


    QString getActiveColor();


    void updateGraphFile();


private:


    Ui::MainWindow *ui;


    QPushButton *currentButton;


    QString filePath;


    QString idelColor, activeColor;


private slots:


    void on_graphSetButton_clicked();


    void on_stackSetButton_clicked();


    void renderImageActionSlot();


    void on_cursorSetButton_clicked();


    void on_queueSetButton_clicked();


    void updateGraphDataText();


    void on_openGraphAction_triggered();


    void displayDefaultInfo();


    void displayElementInfo(Element* eptr);


};


#endif // MAINWINDOW_H
