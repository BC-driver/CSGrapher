#include "element.h"
#include <QPainter>


int Element::defaultLineWidth = 3;
int Element::defaultFontSize = 12;
QColor Element::defaultEdgeColor = QColor(0, 0, 0);
QColor Element::defaultFontColor = QColor(0, 0, 0);
QString Element::defaultContext = "";


// Element's functions
Element::Element(int x, int y, elementType tp){
    this -> xPos = x;
    this -> yPos = y;
    this -> type = tp;
    this -> lineWidth = defaultLineWidth;
    this -> fontSize  = defaultFontSize;
    this -> fontColor = defaultFontColor;
    this -> edgeColor = defaultEdgeColor;
    this -> context   = defaultContext;
}


Element::~Element(){}


void Element::setPos(int x, int y){
    xPos = x; yPos = y;
}


void Element::setEdgeColor(QColor color){
    edgeColor = color;
}


void Element::setFontColor(QColor color){
    fontColor = color;
}


void Element::setContext(QString s){
    context = s;
}


void Element::setFontSize(int fonts){
    fontSize = fonts;
}


void Element::setLineWidth(int linew){
    lineWidth = linew;
}


void Element::setDefaultLineWidth(int width){
    defaultLineWidth = width;
}


void Element::setDefaultFontSize(int fontsize){
    defaultFontSize = fontsize;
}


void Element::setDefaultEdgeColor(QColor color){
    defaultEdgeColor = color;
}


void Element::setDefaultFontColor(QColor color){
    defaultFontColor = color;
}


void Element::setDefaultContext(QString s){
    defaultContext = s;
}


int Element::getXPos(){
    return xPos;
}


int Element::getYPos(){
    return yPos;
}


elementType Element::getType(){
    return type;
}


QColor Element::getEdgeColor(){
    return edgeColor;
}


QColor Element::getFontColor(){
    return fontColor;
}


QString Element::getContext(){
    return context;
}


int Element::getFontSize(){
    return fontSize;
}


int Element::getLineWidth(){
    return lineWidth;
}


int Element::getDefaultLineWidth(){
    return defaultLineWidth;
}


int Element::getDefaultFontSize(){
    return defaultFontSize;
}


QColor Element::getDefaultEdgeColor(){
    return defaultEdgeColor;
}


QColor Element::getDefaultFontColor(){
    return defaultFontColor;
}


QString Element::getDefaultContext(){
    return defaultContext;
}


// BlockElement's functions
BlockElement::BlockElement(int x, int y, int bw, int bh):
    Element(x, y, BLOCK){
    this -> blockWidth = bw;
    this -> blockHeight = bh;
}


BlockElement::~BlockElement(){}


void BlockElement::paint(QPainter *painter){

    // init the pen
    QPen pen;
    pen.setColor(getEdgeColor());
    pen.setWidth(getLineWidth());
    painter->setPen(pen);

    // draw the block
    painter -> drawRect(xPos + blockWidth * (-0.5), yPos + blockHeight * (-0.5),
                        blockWidth, blockHeight);

    // draw the descrption
    painter -> setPen(getFontColor());
    QFont font("Consolas", getFontSize(), QFont::Bold, false);
    painter -> setFont(font);
    painter -> drawText(this -> xPos + this -> blockWidth * (-0.5), this -> yPos + this -> blockHeight * (-0.5),
                        this -> blockWidth, this -> blockHeight,
                        Qt::AlignHCenter | Qt::AlignVCenter, getContext());
}


Element* BlockElement::hoverOn(QPoint pt){
    bool inXRange = xPos - blockWidth * 0.5 <= pt.x() && pt.x() <= xPos + blockWidth * 0.5;
    bool inYRange = yPos - blockHeight * 0.5 <= pt.y() && pt.y() <= yPos + blockHeight * 0.5;
    if(inXRange && inYRange) return this;

    else return Q_NULLPTR;
}


// NodeElement's functions
NodeElement::NodeElement(int x, int y, int r) :
    Element(x, y, NODE){
    this -> radius = r;
}


NodeElement::~NodeElement(){}


int NodeElement::getRadius(){
    return this -> radius;
}


void NodeElement::paint(QPainter* painter){

    //init pen
    QPen pen;
    pen.setColor(this -> edgeColor);
    pen.setWidth(this -> lineWidth);
    painter -> setPen(pen);
    int xCenter = this -> xPos - this -> radius, yCenter = this -> yPos - this -> radius;

    //draw the Node
    painter -> drawEllipse(xCenter, yCenter, this -> radius * 2, this -> radius * 2);

    //draw the descrption
    painter -> setPen(this -> fontColor);
    QFont font("Consolas", this -> fontSize, QFont::Bold, false);
    painter -> setFont(font);
    painter -> drawText(xCenter, yCenter, this -> radius * 2,
                        this -> radius * 2, Qt::AlignHCenter | Qt::AlignVCenter, this -> context);
}


Element* NodeElement::hoverOn(QPoint pt){
    return (pt.x() - xPos) * (pt.x() - xPos) + (pt.y() - yPos) * (pt.y() - yPos) < radius * radius ? this : Q_NULLPTR;
}


// StackElement's functions
StackElement::StackElement(int x, int y, int n, QString *list, int bw, int bh) :
    Element(x, y, STACK){
    this -> size = n;
    this -> blockWidth = bw;
    this -> blockHeight = bh;
    for(int i = 1;i <= this -> size;i++){
        BlockElement* bptr = new BlockElement(x, y + blockHeight * i);
        blocks.push_back(bptr);
        bptr -> setContext(list[i - 1]);
    }
}


StackElement::~StackElement(){
    for(int i = 0;i < this -> size;i++){
        delete blocks[i];
    }
}


void StackElement::setBlockWidth(int blockWidth){
    this -> blockWidth = blockWidth;
}


void StackElement::setBlockHeight(int blockHeight){
    this -> blockHeight = blockHeight;
}


int StackElement::getBlockWidth(){
    return this -> blockWidth;
}


int StackElement::getBlockHeight(){
    return this -> blockHeight;
}


void StackElement::paint(QPainter *painter){

    // init pen
    QPen pen;
    pen.setColor(this -> edgeColor);
    pen.setWidth(this -> lineWidth);
    painter -> setPen(pen);
    QFont font("Consolas", this -> fontSize, QFont::Bold, false);

    // draw the frame
    painter -> drawLine(this -> xPos - this -> blockWidth * 0.6, this -> yPos - this -> blockHeight * 0.5,
                        this -> xPos - this -> blockWidth * 0.5, this -> yPos - this -> blockHeight * 0.5);
    painter -> drawLine(this -> xPos + this -> blockWidth * 0.6, this -> yPos - this -> blockHeight * 0.5,
                        this -> xPos + this -> blockWidth * 0.5, this -> yPos - this -> blockHeight * 0.5);
    painter -> drawLine(this -> xPos - this -> blockWidth * 0.5, this -> yPos - this -> blockHeight * 0.5,
                        this -> xPos - this -> blockWidth * 0.5, this -> yPos + this -> blockHeight * (this -> size + 0.5));
    painter -> drawLine(this -> xPos + this -> blockWidth * 0.5, this -> yPos - this -> blockHeight * 0.5,
                        this -> xPos + this -> blockWidth * 0.5, this -> yPos + this -> blockHeight * (this -> size + 0.5));
    painter -> drawLine(this -> xPos - this -> blockWidth * 0.5, this -> yPos + this -> blockHeight * (this -> size + 0.5),
                        this -> xPos + this -> blockWidth * 0.5, this -> yPos + this -> blockHeight * (this -> size + 0.5));

    // draw the blocks
    for(int i = 0;i < this -> size;i++){
        blocks[i]->paint(painter);
    }

    // draw the description
    painter -> setPen(getFontColor());
    painter -> setFont(font);
    painter -> drawText(this -> xPos + this -> blockWidth * (-0.5), this -> yPos + this -> blockHeight * (0.5 + size),
                        this -> blockWidth, this -> blockHeight,
                        Qt::AlignHCenter | Qt::AlignVCenter, getContext());

}


Element* StackElement::hoverOn(QPoint pt){
    bool inXRange = xPos - blockWidth * 0.5 <= pt.x() && pt.x() <= xPos + blockWidth * 0.5;
    bool inYRange = yPos - blockHeight * 0.5 <= pt.y() && pt.y() <= yPos + blockHeight * (0.5 + size);
    if(inXRange && inYRange){
        if (pt.y() <= yPos + blockHeight * 0.5) return this;
        for(auto b : blocks){
            if(b -> hoverOn(pt) != Q_NULLPTR)
                return b;
        }
    }

    else return Q_NULLPTR;
}


// QueueElement's functions
QueueElement::QueueElement(int x, int y, int n, QString *list, int bw, int bh) :
    Element(x, y, QUEUE){
    this -> size = n;
    this -> blockWidth = bw;
    this -> blockHeight = bh;
    for(int i = 1;i <= this -> size;i++){
        BlockElement* bptr = new BlockElement(x + blockWidth * (i - 0.25), y);
        blocks.push_back(bptr);
        bptr -> setContext(list[i - 1]);
    }
}


QueueElement::~QueueElement(){
    for(int i = 0;i < this -> size;i++){
        delete blocks[i];
    }
}


void QueueElement::setBlockWidth(int blockWidth){
    this -> blockWidth = blockWidth;
}


void QueueElement::setBlockHeight(int blockHeight){
    this -> blockHeight = blockHeight;
}


int QueueElement::getBlockWidth(){
    return this -> blockWidth;
}


int QueueElement::getBlockHeight(){
    return this -> blockHeight;
}


void QueueElement::paint(QPainter *painter){

    // init pen
    QPen pen;
    pen.setColor(this -> edgeColor);
    pen.setWidth(this -> lineWidth);
    QFont font("Consolas", this -> fontSize, QFont::Bold, false);
    painter -> setPen(pen);

    // draw the frame
    painter -> drawLine(this -> xPos - this -> blockWidth * 0.25, this -> yPos - this -> blockHeight * 0.6,
                        this -> xPos - this -> blockWidth * 0.25, this -> yPos - this -> blockHeight * 0.5);
    painter -> drawLine(this -> xPos - this -> blockWidth * 0.25, this -> yPos + this -> blockHeight * 0.6,
                        this -> xPos - this -> blockWidth * 0.25, this -> yPos + this -> blockHeight * 0.5);
    painter -> drawLine(this -> xPos - this -> blockWidth * 0.25, this -> yPos - this -> blockHeight * 0.5,
                        this -> xPos + this -> blockWidth * (this -> size + 0.75), this -> yPos - this -> blockHeight * 0.5);
    painter -> drawLine(this -> xPos - this -> blockWidth * 0.25, this -> yPos + this -> blockHeight * 0.5,
                        this -> xPos + this -> blockWidth * (this -> size + 0.75), this -> yPos + this -> blockHeight * 0.5);
    painter -> drawLine(this -> xPos + this -> blockWidth * (this -> size + 0.75), this -> yPos - this -> blockHeight * 0.6,
                        this -> xPos + this -> blockWidth * (this -> size + 0.75), this -> yPos - this -> blockHeight * 0.5);
    painter -> drawLine(this -> xPos + this -> blockWidth * (this -> size + 0.75), this -> yPos + this -> blockHeight * 0.6,
                        this -> xPos + this -> blockWidth * (this -> size + 0.75), this -> yPos + this -> blockHeight * 0.5);

    // draw the blocks
    for(auto b : blocks){
        b -> paint(painter);
    }

    // draw the description
    painter -> setPen(getFontColor());
    painter -> setFont(font);
    painter -> drawText(this -> xPos + this -> blockWidth * (-0.5), this -> yPos + this -> blockHeight * 0.5,
                        this -> blockWidth, this -> blockHeight,
                        Qt::AlignHCenter | Qt::AlignVCenter, getContext());


}


Element* QueueElement::hoverOn(QPoint pt){
    bool inXRange = xPos - blockWidth * 0.25 <= pt.x() && pt.x() <= xPos + blockWidth * (0.75 + size);
    bool inYRange = yPos - blockHeight * 0.5 <= pt.y() && pt.y() <= yPos + blockHeight * 0.5;
    if(inXRange && inYRange){
        if (pt.x() <= xPos + blockWidth * 0.25 || pt.x() >= xPos + blockWidth * (0.25 + size)) return this;
        for(auto b : blocks){
            if(b -> hoverOn(pt) != Q_NULLPTR)
                return b;
        }
    }

    else return Q_NULLPTR;
}


// ArrowElement's functions
ArrowElement::ArrowElement(int xStartPos, int yStartPos, int xEndPos, int yEndPos) :
    Element(xStartPos, yStartPos, ARROW){
    this -> xEndPos = xEndPos;
    this -> yEndPos = yEndPos;
}


ArrowElement::~ArrowElement(){}

void ArrowElement::setEndPos(int x, int y){
    this -> xEndPos = x;
    this -> yEndPos = y;
}


void ArrowElement::setStartPos(int x, int y){
    setPos(x, y);
}


int ArrowElement::getXEndPos(){
    return this -> xEndPos;
}


int ArrowElement::getYEndPos(){
    return this -> yEndPos;
}


int ArrowElement::getXStartPos(){
    return getXPos();
}


int ArrowElement::getYStartPos(){
    return getYPos();
}


void ArrowElement::paint(QPainter *painter){

    // init pen
    QPen pen;
    pen.setColor(this -> edgeColor);
    pen.setWidth(this -> lineWidth);
    painter -> setPen(pen);

    // draw the arrow

}
