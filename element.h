/*****************************************************************************
* \file element.h
* \author Zirui Xu
* \date 2023/04/20
* \version v1.0
* \brief 定义了绘图的各类基本元素
* \note Copyright (c) 2020-2030 南京理工大学
* \remarks 添加了newBlock的重载函数, 使其用法更灵活
******************************************************************************/
#ifndef ELEMENT_H
#define ELEMENT_H


#include <QPainter>


enum elementType{
    NODE,
    STACK,
    QUEUE,
    ARROW,
    BLOCK,
    CURSOR
};


// Base Class for graphs
class Element
{


public:


    Element(int x, int y, elementType tp);


    virtual ~Element();


    void setPos(int x, int y);


    void setPos(QPointF pt);


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


    elementType getType();


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


    virtual Element* hoverOn(QPoint pt) = 0;


protected:


    int xPos, yPos, lineWidth, fontSize;


    QColor edgeColor, fontColor;


    QString context;


    elementType type;


    static int defaultLineWidth, defaultFontSize;


    static QColor defaultEdgeColor, defaultFontColor;


    static QString defaultContext;


};


// Auxiliary class for Stack and Queue
class BlockElement : public Element{


public:


    BlockElement(int x, int y, int bw = 50, int bh = 30);


    ~BlockElement();


    void setBlockWidth(int blockWidth);


    void setBlockHeight(int blockHeight);


    int getBlockWidth();


    int getBlockHeight();


    void paint(QPainter *painter);


    Element* hoverOn(QPoint pt);


private:


    int blockWidth, blockHeight;


};


class NodeElement : public Element{


public:


    NodeElement(int x, int y, int r = 20);


    ~NodeElement();


    int getRadius();


    void setRadius(int r);


    void paint(QPainter* painter);


    Element* hoverOn(QPoint pt);


private:


    int radius;

};


class StackElement : public Element{


public:


    StackElement(int x, int y, int n, QString *list, int bw = 50, int bh = 30);


    ~StackElement();


    void setBlockWidth(int blockWidth);


    void setBlockHeight(int blockHeight);


    void newBlock(QString context);


    void newBlock(QString context, QColor fontColor, QColor edgeColor, int fontSize, int lineWidth);


    void popBlock();


    int getBlockWidth();


    int getBlockHeight();


    int getSize();


    BlockElement* getBlockAt(int i);


    void paint(QPainter* painter);


    Element* hoverOn(QPoint pt);


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


    void newBlock(QString context);


    void newBlock(QString context, QColor fontColor, QColor edgeColor, int fontSize, int lineWidth);


    void popBlock();


    int getBlockWidth();


    int getBlockHeight();


    int getSize();


    BlockElement* getBlockAt(int i);


    void paint(QPainter* painter);


    Element* hoverOn(QPoint pt);


private:


    int size, blockWidth, blockHeight;
    QVector <BlockElement*> blocks;


};


class ArrowElement : public Element{


public:


    ArrowElement(NodeElement *from, NodeElement *to, bool d = 1);


    ~ArrowElement();


    Element* getToElement();


    Element* getFromElement();


    bool getIsDirected();


    void setIsDirected(bool state);


    void setToElement(NodeElement* nptr);


    void setFromElement(NodeElement* nptr);


    void paint(QPainter* painter);


    Element *hoverOn(QPoint pt);


private:


    NodeElement *fromElement, *toElement;


    bool isDirected;


};





#endif // ELEMENT_H
