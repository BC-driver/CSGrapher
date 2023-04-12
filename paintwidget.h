/*****************************************************************************
* \file paintwidget.h
* \author Zirui Xu
* \date 2023/04/04
* \version v0.4
* \brief 定义了自定义画板控件
* \note Copyright (c) 2020-2030 南京理工大学
* \remarks 添加调色功能, 鼠标悬浮动态显示, 参数调整功能.
*          添加删除功能
******************************************************************************/
#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include "element.h"
#include "logic.h"


class PaintWidget : public QWidget
{
    Q_OBJECT

signals:


    void fileUpdated();


    void showDefaultInfoSignal();


    void showElementInfoSignal(Element *eptr);


public:


    PaintWidget();


    explicit PaintWidget(QWidget *parent = nullptr);


    void setElement(elementType type);


    void setFilePath(QString s);


    void initGraph();


    void initNode(QPoint pt);


    void initStack(QPoint pt, int size, QString *list);


    void initQueue(QPoint pt, int size, QString *list);


    void initArrow(NodeElement *from, NodeElement *to);


    void updateGraphFile();


    void updateLogic();


    Element* findHoverOn(QPoint pt);


    void showInfo(QPoint pt);


    void showDefaultInfo();


    void showElementInfo(Element *eptr);


    Element* getCurrentHighlightElement();


protected:


    void paintEvent(QPaintEvent *event);


    void mousePressEvent(QMouseEvent *event);


    void mouseMoveEvent(QMouseEvent *event);


private:


    elementType currentDrawingElement;


    QList <Element*> elementList;


    QString filePath;


    Element* currentHighlightElement;


    NodeElement* bufferptr;


    Logic logic;


};


#endif // PAINTWIDGET_H
