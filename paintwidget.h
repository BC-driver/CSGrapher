/*****************************************************************************
* \file paintwidget.h
* \author Zirui Xu
* \date 2023/04/19
* \version v0.7
* \brief 定义了自定义画板控件
* \note Copyright (c) 2020-2030 南京理工大学
* \remarks 完成保存,打开画板功能
******************************************************************************/
#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QTextStream>
#include "element.h"
#include "logicmodule.h"


class PaintWidget : public QWidget
{
    Q_OBJECT

signals:


    void fileUpdated();


    void modified();


    void showDefaultInfoSignal();


    void showElementInfoSignal(Element *eptr);


public:


    PaintWidget();


    explicit PaintWidget(QWidget *parent = nullptr);


    void setElement(elementType type);


    void setFilePath(QString s);


    Element* initNode(QPoint pt);


    Element* initStack(QPoint pt, int size, QString *list);


    Element* initQueue(QPoint pt, int size, QString *list);


    Element* initArrow(NodeElement *from, NodeElement *to);


    void updateGraphFile();


    void updateLogic();


    void dfsFromNode(NodeElement* node);


    void bfsFromNode(NodeElement* node);


    void polyLayout(NodeElement* node);


    void treeLayout(NodeElement* node);


    void topoLayout(NodeElement* node);


    Element* findHoverOn(QPoint pt);


    void showInfo(QPoint pt);


    void showDefaultInfo();


    void showElementInfo(Element *eptr);


    Element* getCurrentHighlightElement();


    void saveFile(QTextStream& in);


    void openFile(QTextStream& in);


    void clearElementList();


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


    LogicModule* logic;


};


#endif // PAINTWIDGET_H
