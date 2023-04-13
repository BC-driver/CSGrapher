#include "paintwidget.h"
#include "element.h"
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QMouseEvent>
#include <QTextStream>
#include <QDebug>


PaintWidget::PaintWidget(QWidget *parent) : QWidget{parent}
{
    this -> setAttribute(Qt::WA_StyledBackground, true);
    currentHighlightElement = Q_NULLPTR;
    bufferptr = Q_NULLPTR;
    logic = new LogicModule();
    logic -> update(elementList);
    connect(logic, SIGNAL(updatePainter()), this, SLOT(update()));
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
    for(auto p : elementList){
        p -> paint(&painter);
    }
}


void PaintWidget::mousePressEvent(QMouseEvent *event){
    if(event -> button() == Qt::LeftButton){
        QPoint mousePos = event -> pos();
        QString temp[2] = {"1", "2"};
        Element* hoveringElement = findHoverOn(mousePos);
        bool noCollide = hoveringElement == Q_NULLPTR;
        switch(currentDrawingElement){
            case NODE:
            if(noCollide) initNode(mousePos);
            break;

            case STACK:
            if(noCollide) initStack(mousePos, 2, temp);
            break;

            case QUEUE:
            if(noCollide) initQueue(mousePos, 2, temp);
            break;

            case ARROW:
            if(!noCollide && hoveringElement -> getType() == NODE){
                if(bufferptr == Q_NULLPTR){
                    bufferptr = (NodeElement*)hoveringElement;
                }

                else if(bufferptr != hoveringElement){
                    initArrow(bufferptr, (NodeElement*)hoveringElement);
                    bufferptr = Q_NULLPTR;
                }
            }
            break;

            case CURSOR:
            showInfo(mousePos);
            break;
        }
        update();
        updateGraphFile();
        updateLogic();
    }

    else if(event -> button() == Qt::RightButton){
        QPoint mousePos = event -> pos();
        Element *eptr = findHoverOn(mousePos);
        if(eptr != Q_NULLPTR && eptr -> getType() != BLOCK){
            QVector <Element*> deleteElement;
            for(int i = 0;i < elementList.size();i++){
                Element *&ptr = elementList[i];
                if(ptr == eptr){
                    deleteElement.push_back(elementList[i]);
                    ptr = elementList[elementList.size() - 1];
                    elementList.pop_back();
                    i--;
                }
                else if(ptr -> getType() == ARROW){
                    ArrowElement* aptr = (ArrowElement*)ptr;
                    if(aptr -> getToElement() == eptr || aptr -> getFromElement() == eptr){
                        deleteElement.push_back(elementList[i]);
                        ptr = elementList[elementList.size() - 1];
                        elementList.pop_back();
                        i--;
                    }
                }
            }
            for(auto ptr : deleteElement){
                delete ptr;
            }
        }
    }
}


void PaintWidget::mouseMoveEvent(QMouseEvent *event){
    QPoint pt = event -> pos();
    if(currentHighlightElement != Q_NULLPTR){
        currentHighlightElement -> setLineWidth(currentHighlightElement -> getLineWidth() - 3);
        currentHighlightElement -> setFontSize(currentHighlightElement -> getFontSize() - 3);
    }
    currentHighlightElement = findHoverOn(pt);
    if(currentHighlightElement != Q_NULLPTR){
        currentHighlightElement -> setLineWidth(currentHighlightElement -> getLineWidth() + 3);
        currentHighlightElement -> setFontSize(currentHighlightElement -> getFontSize() + 3);
    }
    if(event -> buttons() & Qt::LeftButton && currentHighlightElement != Q_NULLPTR){
        currentHighlightElement -> setPos(event -> pos().x(), event -> pos().y());
    }
    update();
}


Element* PaintWidget::findHoverOn(QPoint pt){
    for(auto eptr : elementList){
        if(eptr -> hoverOn(pt) != Q_NULLPTR) return eptr -> hoverOn(pt);
    }
    return Q_NULLPTR;
}


void PaintWidget::updateGraphFile(){
    emit fileUpdated();
}


void PaintWidget::updateLogic(){
    logic -> update(elementList);
}


void PaintWidget::dfsFromNode(NodeElement *node){
    int idx = logic -> findNodeIndex(node);
    if(idx != -1) logic -> DFS(idx);
    logic -> clearVis();
}


void PaintWidget::bfsFromNode(NodeElement *node){
    int idx = logic -> findNodeIndex(node);
    if(idx != -1) logic -> BFS(idx);
    logic -> clearVis();
}


void PaintWidget::initNode(QPoint pt){
    elementList.push_back(new NodeElement(pt.x(), pt.y()));
    elementList.last()->setContext(QString::number(elementList.length()));
}


void PaintWidget::initQueue(QPoint pt, int size, QString *list){
    elementList.push_back(new QueueElement(pt.x(), pt.y(), size, list));
    elementList.last()->setContext(QString::number(elementList.length()));
}


void PaintWidget::initStack(QPoint pt, int size, QString *list){
    elementList.push_back(new StackElement(pt.x(), pt.y(), size, list));
    elementList.last()->setContext(QString::number(elementList.length()));
}


void PaintWidget::initArrow(NodeElement *from, NodeElement *to){
    elementList.push_back(new ArrowElement(from, to));
}


void PaintWidget::initGraph(){
    elementList.clear();
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


void PaintWidget::showInfo(QPoint pt){
    Element *eptr = findHoverOn(pt);
    if(eptr == Q_NULLPTR) showDefaultInfo();

    else showElementInfo(eptr);
}


void PaintWidget::showDefaultInfo(){
    emit showDefaultInfoSignal();
}


void PaintWidget::showElementInfo(Element *eptr){
    emit showElementInfoSignal(eptr);
}


Element* PaintWidget::getCurrentHighlightElement(){
    return currentHighlightElement;
}
