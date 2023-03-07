#ifndef ELEMENT_H
#define ELEMENT_H

#include <QPainter>

class Element
{

protected:
    int xPos, yPos, width;
    QColor edgeColor, fontColor;
    QString context;

public:
    Element();

    void setPos(int x, int y){
        xPos = x; yPos = y;
        return;
    }

    void setEdgeClor(QColor color){
        edgeColor = color;
        return;
    }

    void setFontColor(QColor color){
        fontColor = color;
        return;
    }

    void setContext(QString s){
        context = s;
        return;
    }

};


class Node : public Element{

private:
    int radius;

public:
    Node(int x, int y, int r = 10, int w = 4,
         QColor eColor = QColor(0, 0, 0), QColor fColor = QColor(0, 0, 0)){
        xPos = x; yPos = y; radius = r; width = w;
        edgeColor = eColor; fontColor = fColor;
    }


};


class StackElement : public Element{

private:
    int blockWidth, blockHeight;

public:
    StackElement()
};


class Stack : public Element{

private:
    int size, blockWidth, blockHeight;
    QVector <StackElement*> blocks;

public:
    Stack(int x, int y, QString *list, QColor eColor = QColor(0, 0, 0),
          QColor fColor = QColor(0, 0, 0), int bw = 20, int bh = 10){
        xPos = x; yPos = y;
    }
};


class QueueElement : public Element{

private:

};


class Queue : public Element{

private:
    int size;
    QVector <QueueElement*> blocks;


};



#endif // ELEMENT_H
