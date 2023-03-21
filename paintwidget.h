/*****************************************************************************
* \file paintwidget.h
* \author Zirui Xu
* \date 2023/03/21
* \version v0.2
* \brief 定义了自定义画板控件
* \note Copyright (c) 2020-2030 南京理工大学
* \remarks 整理代码格式
******************************************************************************/
#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <element.h>
#include <QMouseEvent>


class PaintWidget : public QWidget
{
    Q_OBJECT

signals:


    void fileUpdated();


public:


    PaintWidget();


    explicit PaintWidget(QWidget *parent = nullptr);


    void setElement(elementType type);


    void setFilePath(QString s);


    void initGraph();


    void initNode(QPoint pt);


    void initStack(QPoint pt);


    void initQueue(QPoint pt);


    void initArrow(QPoint pt);


    void updateGraphFile();


protected:


    void paintEvent(QPaintEvent *event);


    void mousePressEvent(QMouseEvent *event);


private:


    elementType currentDrawingElement;


    QList <NodeElement*> nodeList;


    QList <StackElement*> stackList;


    QList <QueueElement*> queueList;


    QString filePath;


};


#endif // PAINTWIDGET_H
