#include "paintwidget.h"
#include "element.h"
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QMouseEvent>
#include <QTextStream>


PaintWidget::PaintWidget(QWidget *parent) : QWidget{parent}
{
    this->setAttribute(Qt::WA_StyledBackground, true);
}


void PaintWidget::setElement(elementType type){
    currentDrawingElement = type;
}


void PaintWidget::setFilePath(QString s){
    filePath = s;
}


void PaintWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    for(int i = 0;i < nodeList.size();i++){
        NodeElement* currentNode = nodeList[i];
        currentNode -> paint(&painter);
    }
    for(int i = 0;i < stackList.size();i++){
        StackElement* currentStack = stackList[i];
        currentStack -> paint(&painter);
    }
    for(int i = 0;i < queueList.size();i++){
        QueueElement* currentQueue = queueList[i];
        currentQueue -> paint(&painter);
    }
}


void PaintWidget::mousePressEvent(QMouseEvent *event){
    if(event -> button() == Qt::LeftButton){
        QPoint mousePos = event -> pos();
        int index = nodeList.size() + 1;
        QString temp[1] = {"1"};
        switch(currentDrawingElement){
            case NODE:
            initNode(mousePos);
            break;

            case STACK:
            stackList.push_back(new StackElement(mousePos.x(), mousePos.y(), 1, temp));
            break;

            case QUEUE:
            queueList.push_back(new QueueElement(mousePos.x(), mousePos.y(), 1, temp));
            break;

            case ARROW:
            break;

            case CURSOR:
            break;
        }
        update();
        updateGraphFile();
    }
}


void PaintWidget::updateGraphFile(){
    emit fileUpdated();
}


void PaintWidget::initNode(QPoint pt){
    // todo;
}


void PaintWidget::initGraph(){
    nodeList.clear();
    stackList.clear();
    queueList.clear();
    QFile file(filePath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString currentElementType;
    in >> currentElementType;
    while(currentElementType != "OK"){
        if(currentElementType == "NODE"){
            //todo;
        }
        else if(currentElementType == "STACK"){
            //todo;
        }
        else if(currentElementType == "QUEUE"){
            //todo;
        }
    }
}
