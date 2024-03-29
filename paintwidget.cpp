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
        Element* hoveringElement = findHoverOn(mousePos);
        bool noCollide = hoveringElement == Q_NULLPTR;
        switch(currentDrawingElement){
            case NODE:
            if(noCollide) initNode(mousePos);
            break;

            case STACK:
            if(noCollide) initStack(mousePos, 0, Q_NULLPTR);
            break;

            case QUEUE:
            if(noCollide) initQueue(mousePos, 0, Q_NULLPTR);
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
    emit modified();
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


void PaintWidget::polyLayout(NodeElement *node){
    int idx = logic -> findNodeIndex(node);
    if(idx != -1) logic -> polyLayout(idx);
    logic -> clearVis();
}


void PaintWidget::treeLayout(NodeElement *node){
    int idx = logic -> findNodeIndex(node);
    if(idx != -1) logic -> treeLayout(idx);
    logic -> clearVis();
}


void PaintWidget::topoLayout(NodeElement *node){
    int idx = logic -> findNodeIndex(node);
    if(idx != -1) logic -> topoLayout(idx);
    logic -> clearVis();
}


Element* PaintWidget::initNode(QPoint pt){
    elementList.push_back(new NodeElement(pt.x(), pt.y()));
    elementList.last()->setContext(QString::number(elementList.length()));
    return elementList.last();
}


Element* PaintWidget::initQueue(QPoint pt, int size, QString *list){
    elementList.push_back(new QueueElement(pt.x(), pt.y(), size, list));
    elementList.last()->setContext(QString::number(elementList.length()));
    return elementList.last();
}


Element* PaintWidget::initStack(QPoint pt, int size, QString *list){
    elementList.push_back(new StackElement(pt.x(), pt.y(), size, list));
    elementList.last()->setContext(QString::number(elementList.length()));
    return elementList.last();
}


Element* PaintWidget::initArrow(NodeElement *from, NodeElement *to){
    elementList.push_back(new ArrowElement(from, to));
    return elementList.last();
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


void PaintWidget::saveFile(QTextStream& in){
    QVector <NodeElement*> nodes;
    QVector <ArrowElement*> arrows;
    QVector <StackElement*> stacks;
    QVector <QueueElement*> queues;
    for(auto eptr : elementList){
        switch(eptr -> getType()){
        case NODE:
            nodes.push_back((NodeElement*)eptr);
            break;
        case ARROW:
            arrows.push_back((ArrowElement*)eptr);
            break;
        case QUEUE:
            queues.push_back((QueueElement*)eptr);
            break;
        case STACK:
            stacks.push_back((StackElement*)eptr);
        default:
            break;
        }
    }
    in << "NODE\n" << nodes.size() << "\n";
    for(int i = 0;i < nodes.size();i++){
        NodeElement* ptr = nodes[i];
        in << ptr -> getXPos() << " " << ptr -> getYPos() << " ";
        in << ptr -> getRadius() << " ";
        in << ptr -> getFontSize() << " " << ptr -> getLineWidth() << " |" << ptr -> getContext() << "| ";
        in << ptr -> getEdgeColor().red() << " " << ptr -> getEdgeColor().green() << " " << ptr -> getEdgeColor().blue() << " ";
        in << ptr -> getFontColor().red() << " " << ptr -> getFontColor().green() << " " << ptr -> getFontColor().blue() << " ";
        in << "\n";
    }
    in << "ARROW\n" << arrows.size() << "\n";
    for(int i = 0;i < arrows.size();i++){
        ArrowElement* ptr = arrows[i];
        int u = -1, v = -1;
        for(int j = 0;j < nodes.size();j++){
            if(nodes[j] == ptr -> getFromElement()) u = j;
            else if(nodes[j] == ptr -> getToElement()) v = j;
        }
        in << u << " " << v << " " << ptr -> getIsDirected()<<" ";
        in << ptr -> getFontSize() << " " << ptr -> getLineWidth() << " |" << ptr -> getContext() << "| ";
        in << ptr -> getEdgeColor().red() << " " << ptr -> getEdgeColor().green() << " " << ptr -> getEdgeColor().blue() << " ";
        in << ptr -> getFontColor().red() << " " << ptr -> getFontColor().green() << " " << ptr -> getFontColor().blue() << " ";
        in << "\n";
    }
    in << "STACK\n" << stacks.size() << "\n";
    for(int i = 0;i < stacks.size();i++){
        StackElement* ptr = stacks[i];
        in << ptr -> getXPos() << " " << ptr -> getYPos() << " ";
        in << ptr -> getBlockWidth() << " " << ptr -> getBlockHeight()<< " ";
        in << ptr -> getFontSize() << " " << ptr -> getLineWidth() << " |" << ptr -> getContext() << "| ";
        in << ptr -> getEdgeColor().red() << " " << ptr -> getEdgeColor().green() << " " << ptr -> getEdgeColor().blue() << " ";
        in << ptr -> getFontColor().red() << " " << ptr -> getFontColor().green() << " " << ptr -> getFontColor().blue() << " ";
        in << "\n";
        in << ptr -> getSize() << "\n";
        for(int i = 0;i < ptr -> getSize();i++){
            BlockElement* bptr = ptr -> getBlockAt(i);
            in << bptr -> getFontSize() << " " << ptr -> getLineWidth() << " |" << bptr -> getContext() << "| ";
            in << bptr -> getEdgeColor().red() << " " << bptr -> getEdgeColor().green() << " " << bptr -> getEdgeColor().blue() << " ";
            in << bptr -> getFontColor().red() << " " << bptr -> getFontColor().green() << " " << bptr -> getFontColor().blue() << " ";
            in << "\n";
        }
    }
    in << "QUEUE\n" << queues.size() << "\n";
    for(int i = 0;i < queues.size();i++){
        QueueElement* ptr = queues[i];
        in << ptr -> getXPos() << " " << ptr -> getYPos() << " ";
        in << ptr -> getBlockWidth() << " " << ptr -> getBlockHeight()<< " ";
        in << ptr -> getFontSize() << " " << ptr -> getLineWidth() << " |" << ptr -> getContext() << "| ";
        in << ptr -> getEdgeColor().red() << " " << ptr -> getEdgeColor().green() << " " << ptr -> getEdgeColor().blue() << " ";
        in << ptr -> getFontColor().red() << " " << ptr -> getFontColor().green() << " " << ptr -> getFontColor().blue() << " ";
        in << "\n";
        in << ptr -> getSize() << "\n";
        for(int i = 0;i < ptr -> getSize();i++){
            BlockElement* bptr = ptr -> getBlockAt(i);
            in << bptr -> getFontSize() << " " << ptr -> getLineWidth() << " |" << bptr -> getContext() << "| ";
            in << bptr -> getEdgeColor().red() << " " << bptr -> getEdgeColor().green() << " " << bptr -> getEdgeColor().blue() << " ";
            in << bptr -> getFontColor().red() << " " << bptr -> getFontColor().green() << " " << bptr -> getFontColor().blue() << " ";
            in << "\n";
        }
    }
    in << "OK";
}


void PaintWidget::openFile(QTextStream &in){
    QString type;
    in >> type;
    while(type != "OK"){
        int n;
        in >> n;
        for(int i = 0;i < n;i++){
            int x, y, fSize, lSize, fColorR, fColorG, fColorB, lColorR, lColorG, lColorB;
            if(type == "NODE"){
                int r;
                QString preContext, context = "";
                in >> x >> y >> r;
                NodeElement* newNode = new NodeElement(x, y, r);
                in >> fSize >> lSize >> preContext;
                in >> lColorR >> lColorG >> lColorB;
                in >> fColorR >> fColorG >> fColorB;
                for(int i = 1;i < preContext.size() - 1;i++) context += preContext[i];
                newNode -> setFontSize(fSize);
                newNode -> setLineWidth(lSize);
                newNode -> setContext(context);
                newNode -> setFontColor(QColor(fColorR, fColorG, fColorB));
                newNode -> setEdgeColor(QColor(lColorR, lColorG, lColorB));
                elementList.push_back(newNode);
            }

            else if(type == "ARROW"){
                int u, v, isDir;
                QString preContext, context = "";
                in >> u >> v >> isDir;
                ArrowElement* newArrow = new ArrowElement((NodeElement*)elementList[u], (NodeElement*)elementList[v], isDir);
                in >> fSize >> lSize >> preContext;
                in >> lColorR >> lColorG >> lColorB;
                in >> fColorR >> fColorG >> fColorB;
                for(int i = 1;i < preContext.size() - 1;i++) context += preContext[i];
                newArrow -> setFontSize(fSize);
                newArrow -> setLineWidth(lSize);
                newArrow -> setContext(context);
                newArrow -> setFontColor(QColor(fColorR, fColorG, fColorB));
                newArrow -> setEdgeColor(QColor(lColorR, lColorG, lColorB));
                elementList.push_back(newArrow);
            }

            else if(type == "STACK"){
                int bWidth, bHeight, s;
                QString preContext, context = "";
                in >> x >> y >> bWidth >> bHeight;
                StackElement* newStack = new StackElement(x, y, 0, nullptr, bWidth, bHeight);
                in >> fSize >> lSize >> preContext;
                in >> lColorR >> lColorG >> lColorB;
                in >> fColorR >> fColorG >> fColorB;
                for(int i = 1;i < preContext.size() - 1;i++) context += preContext[i];
                newStack -> setFontSize(fSize);
                newStack -> setLineWidth(lSize);
                newStack -> setContext(context);
                newStack -> setFontColor(QColor(fColorR, fColorG, fColorB));
                newStack -> setEdgeColor(QColor(lColorR, lColorG, lColorB));
                in >> s;
                for(int i = 0;i < s;i++){
                    in >> fSize >> lSize >> preContext;
                    in >> lColorR >> lColorG >> lColorB;
                    in >> fColorR >> fColorG >> fColorB;
                    context = "";
                    for(int i = 1;i < preContext.size() - 1;i++) context += preContext[i];
                    newStack -> newBlock(context, QColor(fColorR, fColorG, fColorB), QColor(lColorR, lColorG, lColorB), fSize, lSize);
                }
                elementList.push_back(newStack);
            }

            else if(type == "QUEUE"){
                int bWidth, bHeight, s;
                QString preContext, context = "";
                in >> x >> y >> bWidth >> bHeight;
                QueueElement* newQueue = new QueueElement(x, y, 0, nullptr, bWidth, bHeight);
                in >> fSize >> lSize >> preContext;
                in >> lColorR >> lColorG >> lColorB;
                in >> fColorR >> fColorG >> fColorB;
                for(int i = 1;i < preContext.size() - 1;i++) context += preContext[i];
                newQueue -> setFontSize(fSize);
                newQueue -> setLineWidth(lSize);
                newQueue -> setContext(context);
                newQueue -> setFontColor(QColor(fColorR, fColorG, fColorB));
                newQueue -> setEdgeColor(QColor(lColorR, lColorG, lColorB));
                in >> s;
                for(int i = 0;i < s;i++){
                    in >> fSize >> lSize >> preContext;
                    in >> lColorR >> lColorG >> lColorB;
                    in >> fColorR >> fColorG >> fColorB;
                    context = "";
                    for(int i = 1;i < preContext.size() - 1;i++) context += preContext[i];
                    newQueue -> newBlock(context, QColor(fColorR, fColorG, fColorB), QColor(lColorR, lColorG, lColorB), fSize, lSize);
                }
                elementList.push_back(newQueue);
            }
        }
        in >> type;
    }
}


void PaintWidget::clearElementList(){
    elementList.clear();
}
