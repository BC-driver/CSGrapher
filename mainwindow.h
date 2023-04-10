/*****************************************************************************
* \file mainwindow.h
* \author Zirui Xu
* \date 2023/04/04
* \version v0.4
* \brief 定义了整个窗体上的各类控件
* \note Copyright (c) 2020-2030 南京理工大学
* \remarks 添加调色功能, 鼠标悬浮动态显示, 参数调整功能.
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


    Element* currentFocusElement;


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


    void on_lineWidthlineEdit_returnPressed();


    void on_rLineColorLineEdit_returnPressed();


    void on_gLineColorLineEdit_returnPressed();


    void on_bLineColorLineEdit_returnPressed();


    void on_fontSizelineEdit_returnPressed();


    void on_rFontColorLineEdit_returnPressed();


    void on_gFontColorLineEdit_returnPressed();


    void on_bFontColorLineEdit_returnPressed();


    void on_elementContextLineEdit_returnPressed();


    void on_arrowSetButton_clicked();
    void on_specialAttributeOneLineedit_returnPressed();
    void on_specialAttributeTwoLineedit_returnPressed();
};


#endif // MAINWINDOW_H
