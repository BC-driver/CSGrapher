/*****************************************************************************
* \file mainwindow.h
* \author Zirui Xu
* \date 2023/04/20
* \version v1.0
* \brief 定义了整个窗体上的各类控件
* \note Copyright (c) 2020-2030 南京理工大学
* \remarks 完成文件系统
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


    void on_directCheckBox_stateChanged(int arg1);


    void on_dfsButton_pressed();


    void on_bfsButton_pressed();


    void on_polyLayout_pressed();


    void on_treeLayout_pressed();


    void on_topoLayout_pressed();


    void on_generateButton_pressed();


    void on_newGraphAction_triggered();


    void on_saveGraphAction_triggered();


    void on_newBlockButton_pressed();
    void on_popBlockButton_pressed();
};


#endif // MAINWINDOW_H
