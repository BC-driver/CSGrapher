/*****************************************************************************
* \file element.h
* \author Zirui Xu
* \date 2023/03/21
* \version v0.2
* \brief 定义了绘图的各类基本元素
* \note Copyright (c) 2020-2030 南京理工大学
* \remarks 整理代码格式
******************************************************************************/
#ifndef ELEMENT_H
#define ELEMENT_H


#include <QPainter>


// Base Class for graphs
class Element
{


public:


    Element(int x, int y);


    void setPos(int x, int y);


    void setEdgeColor(QColor color);


    void setFontColor(QColor color);


    void setContext(QString s);


    void setLineWidth(int linew);


    void setFontSize(int fonts);


    static void setDefaultLineWidth(int width);


    static void setDefaultFontSize(int fontsize);


    static void setDefaultEdgeColor(QColor color);


    static void setDefaultFontColor(QColor color);


    static void setDefaultContext(QString s);


    int getXPos();


    int getYPos();


    QColor getEdgeColor();


    QColor getFontColor();


    QString getContext();


    int getLineWidth();


    int getFontSize();


    static int getDefaultLineWidth();


    static int getDefaultFontSize();


    static QColor getDefaultEdgeColor();


    static QColor getDefaultFontColor();


    static QString getDefaultContext();


    virtual void paint(QPainter *painter) = 0;


    virtual bool hoverOn(QPoint pt) = 0;


protected:


    int xPos, yPos, lineWidth, fontSize;


    QColor edgeColor, fontColor;


    QString context;


    static int defaultLineWidth, defaultFontSize;


    static QColor defaultEdgeColor, defaultFontColor;


    static QString defaultContext;


};


// Auxiliary class for Stack and Queue
class BlockElement{


public:


    BlockElement(QColor ec = QColor(0, 0, 0), QColor fc = QColor(0, 0, 0), QString context = "");


    void setEdgeColor(QColor ec);


    void setFontColor(QColor fc);


    void setContext(QString context);


    QColor getEdgeColor();


    QColor getFontColor();


    QString getcontext();


private:


    QString context;


    QColor edgeColor, fontColor;


};


class NodeElement : public Element{


public:


    NodeElement(int x, int y, int r = 20);


    int getRadius();


    void paint(QPainter* painter);


    bool hoverOn(QPoint pt);


private:


    int radius;

};


class StackElement : public Element{


public:


    StackElement(int x, int y, int n, QString *list, int bw = 50, int bh = 30);


    ~StackElement();


    void setBlockWidth(int blockWidth);


    void setBlockHeight(int blockHeight);


    int getBlockWidth();


    int getBlockHeight();


    void paint(QPainter* painter);


    bool hoverOn(QPoint pt);


private:


    int size, blockWidth, blockHeight;
    QVector <BlockElement*> blocks;


};


class QueueElement : public Element{


public:


    QueueElement(int x, int y, int n, QString *list, int bw = 50, int bh = 30);


    ~QueueElement();


    void setBlockWidth(int blockWidth);


    void setBlockHeight(int blockHeight);


    int getBlockWidth();


    int getBlockHeight();


    void paint(QPainter* painter);


    bool hoverOn(QPoint pt);


private:


    int size, blockWidth, blockHeight;
    QVector <BlockElement*> blocks;


};


class ArrowElement : public Element{


public:


    ArrowElement(int xStartPos, int yStartPos, int xEndPos, int yEndPos);


    void setEndPos(int x, int y);


    void setStartPos(int x, int y);


    int getXEndPos();


    int getYEndPos();


    int getXStartPos();


    int getYStartPos();


    void paint(QPainter* painter);


private:


    int xEndPos, yEndPos;


};


enum elementType{
    NODE,
    STACK,
    QUEUE,
    ARROW,
    CURSOR
};


#endif // ELEMENT_H
