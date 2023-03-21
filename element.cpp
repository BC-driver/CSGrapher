#include "element.h"
#include <QPainter>


int Element::defaultLineWidth = 3;
int Element::defaultFontSize = 12;
QColor Element::defaultEdgeColor = QColor(0, 0, 0);
QColor Element::defaultFontColor = QColor(0, 0, 0);
QString Element::defaultContext = "";


// Element's functions
Element::Element(int x, int y){
    this -> xPos = x;
    this -> yPos = y;
    this -> lineWidth = defaultLineWidth;
    this -> fontSize  = defaultFontSize;
    this -> fontColor = defaultFontColor;
    this -> edgeColor = defaultEdgeColor;
    this -> context   = defaultContext;
}


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


void Element::setDefaultLineWidth(int width){
    defaultLineWidth = width;
}


void Element::setDefaultFontSize(int fontsize){
    fontSize = fontsize;
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


QColor Element::getEdgeColor(){
    return edgeColor;
}


QColor Element::getFontColor(){
    return fontColor;
}


QString Element::getContext(){
    return context;
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
BlockElement::BlockElement(QColor ec, QColor fc, QString context){
    this -> edgeColor = ec;
    this -> fontColor = fc;
    this -> context = context;
}


void BlockElement::setEdgeColor(QColor ec){
    this -> edgeColor = ec;
    return;
}


void BlockElement::setFontColor(QColor fc){
    this -> fontColor = fc;
    return;
}


void BlockElement::setContext(QString context){
    this -> context = context;
}


QColor BlockElement::getEdgeColor(){
    return this -> edgeColor;
}


QColor BlockElement::getFontColor(){
    return this -> fontColor;
}


QString BlockElement::getcontext(){
    return this -> context;
}


// NodeElement's functions
NodeElement::NodeElement(int x, int y, int r) :
    Element(x, y){
    this -> radius = r;
}


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


// StackElement's functions
StackElement::StackElement(int x, int y, int n, QString *list, int bw, int bh) :
    Element(x, y){
    this -> size = n;
    this -> blockWidth = bw;
    this -> blockHeight = bh;
    for(int i = 0;i < this -> size;i++){
        blocks.push_back(new BlockElement(this -> edgeColor, this -> fontColor, *(list + i)));
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
        BlockElement &cur = *(this -> blocks[i]);
        pen.setColor(cur.getEdgeColor());
        painter -> setPen(pen);
        painter -> drawRect(this -> xPos + this -> blockWidth * (-0.5), this -> yPos + this -> blockHeight * (i + 0.5),
                            this -> blockWidth, this ->blockHeight);
        pen.setColor(this -> fontColor);
        painter -> setPen(pen);
        font.setWeight(this -> fontSize);
        painter -> setFont(font);
        painter -> drawText(this -> xPos + this -> blockWidth * (-0.5), this -> yPos + this -> blockHeight * (i + 0.5),
                            this -> blockWidth, this -> blockHeight,
                            Qt::AlignHCenter | Qt::AlignVCenter, cur.getcontext());
    }

    // draw the description

}


// QueueElement's functions
QueueElement::QueueElement(int x, int y, int n, QString *list, int bw, int bh) :
    Element(x, y){
    this -> size = n;
    this -> blockWidth = bw;
    this -> blockHeight = bh;
    for(int i = 0;i < n;i++){
        blocks.push_back(new BlockElement(this -> edgeColor, this -> fontColor, *(list + i)));
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
    for(int i = 0;i < this -> size;i++){
        BlockElement &cur = *(this -> blocks[i]);
        pen.setColor(cur.getEdgeColor());
        painter -> setPen(pen);
        painter -> drawRect(this -> xPos + this -> blockWidth * (i + 0.25),
                            this -> yPos - this -> blockHeight * 0.5,
                            this -> blockWidth, this -> blockHeight);
    }

}


// ArrowElement's functions
ArrowElement::ArrowElement(int xStartPos, int yStartPos, int xEndPos, int yEndPos) :
    Element(xStartPos, yStartPos){
    this -> xEndPos = xEndPos;
    this -> yEndPos = yEndPos;
}


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