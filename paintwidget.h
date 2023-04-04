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


    void initArrow(QPoint pt);


    void updateGraphFile();


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


};


#endif // PAINTWIDGET_H
