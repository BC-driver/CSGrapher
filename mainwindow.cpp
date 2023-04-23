#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "element.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QShortcut>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QShortcut  *newFileShortcut= new QShortcut(QKeySequence("ctrl+N"), this),
               *openFileShortcut = new QShortcut(QKeySequence("ctrl+O"), this),
               *renderImageShortcut= new QShortcut(QKeySequence("ctrl+I"), this),
               *saveFileShortcut= new QShortcut(QKeySequence("ctrl+S"), this);
    connect(newFileShortcut, SIGNAL(activated()), this,SLOT(on_newGraphAction_triggered()));
    connect(openFileShortcut, SIGNAL(activated()), this,SLOT(on_openGraphAction_triggered()));
    connect(renderImageShortcut, SIGNAL(activated()), this,SLOT(renderImageActionSlot()));
    connect(saveFileShortcut, SIGNAL(activated()), this,SLOT(on_saveGraphAction_triggered()));
    connect(ui -> board, &PaintWidget::modified, this, &MainWindow::displayUnsave);
    connect(ui -> board, &PaintWidget::showDefaultInfoSignal, this, &MainWindow::displayDefaultInfo);
    connect(ui -> board, &PaintWidget::showElementInfoSignal, this, &MainWindow::displayElementInfo);
    connect(ui -> renderImageAction, &QAction::triggered, this, &MainWindow::renderImageActionSlot);

    this -> setWindowTitle("CSGraph");
    this -> displayDefaultInfo();

    ui -> board -> setMouseTracking(true);
    ui -> board -> setVisible(0);
    setDrawingButtonEnable(false);

    currentFocusElement = Q_NULLPTR;
    currentButton = ui -> cursorSetButton;

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setCurrentButton(QPushButton *button){
    this -> currentButton = button;
}


void MainWindow::on_cursorSetButton_clicked()
{
    ui -> board -> setElement(CURSOR);
}


void MainWindow::on_graphSetButton_clicked()
{
    ui -> board -> setElement(NODE);
}


void MainWindow::on_stackSetButton_clicked()
{
    ui -> board -> setElement(STACK);
}


void MainWindow::on_queueSetButton_clicked()
{
    ui -> board -> setElement(QUEUE);
}


void MainWindow::on_arrowSetButton_clicked()
{
    ui -> board -> setElement(ARROW);
}


void MainWindow::renderImageActionSlot(){
    if(filePath == ""){
        QMessageBox::warning(this, "警告", "目前没有打开的画板");
        return;
    }
    QString imagePath = QFileDialog::getSaveFileName(this, "渲染为","C:\\","image(*.png)");
    if(imagePath.isEmpty()){
        QMessageBox::warning(this, "警告", "请选择一个合法路径");
        return;
    }
    QFile file(imagePath);
    if(file.exists()){
        QMessageBox::warning(this, "警告", "此文件名已存在");
        return;
    }
    QPixmap pixmap = QPixmap::grabWidget(ui -> board);
    pixmap.save(imagePath);
}


void MainWindow::on_newGraphAction_triggered()
{
    filePath = QFileDialog::getSaveFileName(this, "新建","C:\\","graph(*.grf)");
    if(filePath.isEmpty()){
        QMessageBox::warning(this, "警告", "请选择一个合法路径");
        filePath = "";
        return;
    }
    QFile file(filePath);
    if(file.exists()){
        QMessageBox::warning(this, "警告", "此文件名已存在");
        filePath = "";
        return;
    }
    ui -> board -> clearElementList();
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream input(&file);
    input << "OK";
    QFileInfo fileInfo(file);
    this -> setWindowTitle(fileInfo.fileName() + " - CSGraph");
    ui -> board -> setVisible(1);
    ui -> openFileHintWidget -> setVisible(0);
    file.close();
    setDrawingButtonEnable(true);
}


void MainWindow::on_openGraphAction_triggered(){
    filePath = QFileDialog::getOpenFileName(this, "打开","C:\\","graph(*.grf)");
    QFile file(filePath);
    if(!file.exists()){
        QMessageBox::warning(this, "警告", "此文件名不存在");
        filePath = "";
        return;
    }
    ui -> board -> setVisible(1);
    ui -> board -> clearElementList();
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream input(&file);
    ui -> board -> openFile(input);
    QFileInfo fileInfo(file);
    this -> setWindowTitle(fileInfo.fileName() + " - CSGraph");
    ui -> openFileHintWidget -> setVisible(0);
    file.close();
    setDrawingButtonEnable(true);
}


void MainWindow::on_saveGraphAction_triggered(){
    if(filePath == ""){
        QMessageBox::warning(this, "警告", "目前没有打开的画板");
        return;
    }
    QFile file(filePath);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream input(&file);
    ui -> board -> saveFile(input);
    file.close();
    QFileInfo fileInfo(file);
    this -> setWindowTitle(fileInfo.fileName() + " - CSGraph 已保存!");
}


void MainWindow::displayDefaultInfo(){
    currentFocusElement = Q_NULLPTR;
    ui -> InfoTitle -> setText("默认参数:");
    ui -> elementContextLineEdit -> setText(Element::getDefaultContext());
    ui -> lineWidthlineEdit -> setText(QString::number(Element::getDefaultLineWidth()));
    QColor edgecolor = Element::getDefaultEdgeColor();
    ui -> rLineColorLineEdit -> setText(QString::number(edgecolor.red()));
    ui -> gLineColorLineEdit -> setText(QString::number(edgecolor.green()));
    ui -> bLineColorLineEdit -> setText(QString::number(edgecolor.blue()));
    ui -> lineColorDisplayScreen -> setStyleSheet("background-color: rgb(" + QString::number(edgecolor.red()) + ","
                                                  + QString::number(edgecolor.green()) + ","
                                                  + QString::number(edgecolor.blue()) + ");");
    ui -> fontSizelineEdit -> setText(QString::number(Element::getDefaultFontSize()));
    QColor fontcolor = Element::getDefaultFontColor();
    ui -> rFontColorLineEdit -> setText(QString::number(fontcolor.red()));
    ui -> gFontColorLineEdit -> setText(QString::number(fontcolor.green()));
    ui -> bFontColorLineEdit -> setText(QString::number(fontcolor.blue()));
    ui -> fontColorDisplayScreen -> setStyleSheet("background-color: rgb(" + QString::number(fontcolor.red()) + ","
                                                  + QString::number(fontcolor.green()) + ","
                                                  + QString::number(fontcolor.blue()) + ");");
    ui -> specialAttributeOneLabel -> setVisible(0);
    ui -> specialAttributeOneLineedit -> setVisible(0);
    ui -> specialAttributeTwoLabel -> setVisible(0);
    ui -> specialAttributeTwoLineedit -> setVisible(0);
    ui -> directLabel -> setVisible(0);
    ui -> directCheckBox -> setVisible(0);
    ui -> dfsButton -> setVisible(0);
    ui -> bfsButton -> setVisible(0);
    ui -> polyLayout -> setVisible(0);
    ui -> topoLayout -> setVisible(0);
    ui -> treeLayout -> setVisible(0);
    ui -> newBlockTag -> setVisible(0);
    ui -> newBlockButton -> setVisible(0);
    ui -> newBlockNameTag -> setVisible(0);
    ui -> newBlockLineedit -> setVisible(0);
    ui -> popBlockButton -> setVisible(0);
}


void MainWindow::displayElementInfo(Element *eptr){
    currentFocusElement = eptr;
    QString typeStr = "";
    ui -> specialAttributeOneLabel -> setVisible(0);
    ui -> specialAttributeOneLineedit -> setVisible(0);
    ui -> specialAttributeTwoLabel -> setVisible(0);
    ui -> specialAttributeTwoLineedit -> setVisible(0);
    ui -> directLabel -> setVisible(0);
    ui -> directCheckBox -> setVisible(0);
    ui -> dfsButton -> setVisible(0);
    ui -> bfsButton -> setVisible(0);
    ui -> polyLayout -> setVisible(0);
    ui -> topoLayout -> setVisible(0);
    ui -> treeLayout -> setVisible(0);
    ui -> newBlockTag -> setVisible(0);
    ui -> newBlockButton -> setVisible(0);
    ui -> newBlockNameTag -> setVisible(0);
    ui -> newBlockLineedit -> setVisible(0);
    ui -> popBlockButton -> setVisible(0);
    NodeElement* nptr = (NodeElement*) eptr;
    StackElement* sptr = (StackElement*) eptr;
    QueueElement* qptr = (QueueElement*) eptr;
    ArrowElement* aptr = (ArrowElement*) eptr;
    switch (eptr -> getType()) {
        case NODE:
        typeStr = "节点";
        ui -> specialAttributeOneLabel -> setText("节点半径:");
        ui -> specialAttributeOneLabel -> setVisible(1);
        ui -> specialAttributeOneLineedit -> setText(QString::number(nptr -> getRadius()));
        ui -> specialAttributeOneLineedit -> setVisible(1);
        ui -> dfsButton -> setVisible(1);
        ui -> bfsButton -> setVisible(1);
        ui -> polyLayout -> setVisible(1);
        ui -> topoLayout -> setVisible(1);
        ui -> treeLayout -> setVisible(1);
        break;

        case STACK:
        typeStr = "栈";
        ui -> specialAttributeOneLabel -> setText("块宽度:");
        ui -> specialAttributeTwoLabel -> setText("块高度:");
        ui -> specialAttributeOneLabel -> setVisible(1);
        ui -> specialAttributeTwoLabel -> setVisible(1);
        ui -> specialAttributeOneLineedit -> setText(QString::number(sptr -> getBlockWidth()));
        ui -> specialAttributeTwoLineedit -> setText(QString::number(sptr -> getBlockHeight()));
        ui -> specialAttributeOneLineedit -> setVisible(1);
        ui -> specialAttributeTwoLineedit -> setVisible(1);
        ui -> newBlockTag -> setVisible(1);
        ui -> newBlockButton -> setVisible(1);
        ui -> newBlockNameTag -> setVisible(1);
        ui -> newBlockLineedit -> setVisible(1);
        ui -> popBlockButton -> setVisible(1);
        break;

        case QUEUE:
        typeStr = "队列";
        ui -> specialAttributeOneLabel -> setText("块宽度:");
        ui -> specialAttributeTwoLabel -> setText("块高度:");
        ui -> specialAttributeOneLabel -> setVisible(1);
        ui -> specialAttributeTwoLabel -> setVisible(1);
        ui -> specialAttributeOneLineedit -> setText(QString::number(qptr -> getBlockWidth()));
        ui -> specialAttributeTwoLineedit -> setText(QString::number(qptr -> getBlockHeight()));
        ui -> specialAttributeOneLineedit -> setVisible(1);
        ui -> specialAttributeTwoLineedit -> setVisible(1);
        ui -> newBlockTag -> setVisible(1);
        ui -> newBlockButton -> setVisible(1);
        ui -> newBlockNameTag -> setVisible(1);
        ui -> newBlockLineedit -> setVisible(1);
        ui -> popBlockButton -> setVisible(1);
        break;

        case BLOCK:
        typeStr = "块";
        break;

        case ARROW:
        typeStr = "边";
        ui -> directLabel -> setVisible(1);
        ui -> directCheckBox -> setVisible(1);
        ui -> directCheckBox -> setChecked(aptr -> getIsDirected());
        break;

    }
    ui -> InfoTitle -> setText(typeStr + eptr -> getContext() + "参数:");
    ui -> elementContextLineEdit -> setText(eptr -> getContext());
    ui -> lineWidthlineEdit -> setText(QString::number(eptr -> getLineWidth()));
    QColor edgecolor = eptr -> getEdgeColor();
    ui -> rLineColorLineEdit -> setText(QString::number(edgecolor.red()));
    ui -> gLineColorLineEdit -> setText(QString::number(edgecolor.green()));
    ui -> bLineColorLineEdit -> setText(QString::number(edgecolor.blue()));
    ui -> lineColorDisplayScreen -> setStyleSheet("background-color: rgb(" + QString::number(edgecolor.red()) + ","
                                                  + QString::number(edgecolor.green()) + ","
                                                  + QString::number(edgecolor.blue()) + ");");
    ui -> fontSizelineEdit -> setText(QString::number(eptr -> getFontSize()));
    QColor fontcolor = eptr -> getFontColor();
    ui -> rFontColorLineEdit -> setText(QString::number(fontcolor.red()));
    ui -> gFontColorLineEdit -> setText(QString::number(fontcolor.green()));
    ui -> bFontColorLineEdit -> setText(QString::number(fontcolor.blue()));
    ui -> fontColorDisplayScreen -> setStyleSheet("background-color: rgb(" + QString::number(fontcolor.red()) + ","
                                                  + QString::number(fontcolor.green()) + ","
                                                  + QString::number(fontcolor.blue()) + ");");
}


void MainWindow::displayUnsave(){
    QFile file(filePath);
    QFileInfo fileInfo(file);
    this -> setWindowTitle(fileInfo.fileName() + "* - CSGraph");
}


void MainWindow::on_lineWidthlineEdit_returnPressed(){
    int changeValue = ui -> lineWidthlineEdit -> text().toInt();
    if(currentFocusElement == Q_NULLPTR){
        Element::setDefaultLineWidth(changeValue);
    }

    else{
        if(currentFocusElement == ui -> board -> getCurrentHighlightElement()){
            currentFocusElement -> setLineWidth(changeValue + 3);
        }

        else{
            currentFocusElement -> setLineWidth(changeValue);
        }
    }
    update();
}


void MainWindow::on_fontSizelineEdit_returnPressed()
{
    int changeValue = ui -> fontSizelineEdit -> text().toInt();
    if(currentFocusElement == Q_NULLPTR){
        Element::setDefaultFontSize(changeValue);
    }

    else{
        if(currentFocusElement == ui -> board -> getCurrentHighlightElement()){
            currentFocusElement -> setFontSize(changeValue + 3);
        }

        else{
            currentFocusElement -> setFontSize(changeValue);
        }
    }
    update();
}


void MainWindow::on_rLineColorLineEdit_returnPressed()
{
    int changeValue = ui -> rLineColorLineEdit -> text().toInt();
    QColor changeColor;
    if(currentFocusElement == Q_NULLPTR){
        changeColor = Element::getDefaultEdgeColor();
        changeColor.setRed(changeValue);
        Element::setDefaultEdgeColor(changeColor);
    }

    else{
        changeColor = currentFocusElement -> getEdgeColor();
        changeColor.setRed(changeValue);
        currentFocusElement -> setEdgeColor(changeColor);
    }
    ui -> lineColorDisplayScreen -> setStyleSheet("background-color: rgb(" + QString::number(changeColor.red()) + ","
                                                  + QString::number(changeColor.green()) + ","
                                                  + QString::number(changeColor.blue()) + ");");
    update();
}


void MainWindow::on_gLineColorLineEdit_returnPressed(){
    int changeValue = ui -> gLineColorLineEdit -> text().toInt();
    QColor changeColor;
    if(currentFocusElement == Q_NULLPTR){
        changeColor = Element::getDefaultEdgeColor();
        changeColor.setGreen(changeValue);
        Element::setDefaultEdgeColor(changeColor);
    }

    else{
        changeColor = currentFocusElement -> getEdgeColor();
        changeColor.setGreen(changeValue);
        currentFocusElement -> setEdgeColor(changeColor);
    }
    ui -> lineColorDisplayScreen -> setStyleSheet("background-color: rgb(" + QString::number(changeColor .red()) + ","
                                                  + QString::number(changeColor.green()) + ","
                                                  + QString::number(changeColor.blue()) + ");");
    update();
}


void MainWindow::on_bLineColorLineEdit_returnPressed(){
    int changeValue = ui -> bLineColorLineEdit -> text().toInt();
    QColor changeColor;
    if(currentFocusElement == Q_NULLPTR){
        changeColor = Element::getDefaultEdgeColor();
        changeColor.setBlue(changeValue);
        Element::setDefaultEdgeColor(changeColor);
    }

    else{
        changeColor = currentFocusElement -> getEdgeColor();
        changeColor.setBlue(changeValue);
        currentFocusElement -> setEdgeColor(changeColor);
    }
    ui -> lineColorDisplayScreen -> setStyleSheet("background-color: rgb(" + QString::number(changeColor .red()) + ","
                                                  + QString::number(changeColor.green()) + ","
                                                  + QString::number(changeColor.blue()) + ");");
    update();
}


void MainWindow::on_rFontColorLineEdit_returnPressed()
{
    int changeValue = ui -> rFontColorLineEdit -> text().toInt();
    QColor changeColor;
    if(currentFocusElement == Q_NULLPTR){
        changeColor = Element::getDefaultFontColor();
        changeColor.setRed(changeValue);
        Element::setDefaultFontColor(changeColor);
    }

    else{
        changeColor = currentFocusElement -> getFontColor();
        changeColor.setRed(changeValue);
        currentFocusElement -> setFontColor(changeColor);
    }
    ui -> fontColorDisplayScreen -> setStyleSheet("background-color: rgb(" + QString::number(changeColor.red()) + ","
                                                  + QString::number(changeColor.green()) + ","
                                                  + QString::number(changeColor.blue()) + ");");
    update();
}


void MainWindow::on_gFontColorLineEdit_returnPressed()
{
    int changeValue = ui -> gFontColorLineEdit -> text().toInt();
    QColor changeColor;
    if(currentFocusElement == Q_NULLPTR){
        changeColor = Element::getDefaultFontColor();
        changeColor.setGreen(changeValue);
        Element::setDefaultFontColor(changeColor);
    }

    else{
        changeColor = currentFocusElement -> getFontColor();
        changeColor.setGreen(changeValue);
        currentFocusElement -> setFontColor(changeColor);
    }
    ui -> fontColorDisplayScreen -> setStyleSheet("background-color: rgb(" + QString::number(changeColor.red()) + ","
                                                  + QString::number(changeColor.green()) + ","
                                                  + QString::number(changeColor.blue()) + ");");
    update();
}


void MainWindow::on_bFontColorLineEdit_returnPressed()
{
    int changeValue = ui -> bFontColorLineEdit -> text().toInt();
    QColor changeColor;
    if(currentFocusElement == Q_NULLPTR){
        changeColor = Element::getDefaultFontColor();
        changeColor.setBlue(changeValue);
        Element::setDefaultFontColor(changeColor);
    }

    else{
        changeColor = currentFocusElement -> getFontColor();
        changeColor.setBlue(changeValue);
        currentFocusElement -> setFontColor(changeColor);
    }
    ui -> fontColorDisplayScreen -> setStyleSheet("background-color: rgb(" + QString::number(changeColor.red()) + ","
                                                  + QString::number(changeColor.green()) + ","
                                                  + QString::number(changeColor.blue()) + ");");
    update();
}


void MainWindow::on_elementContextLineEdit_returnPressed()
{
    QString changeContext = ui -> elementContextLineEdit -> text();
    if(currentFocusElement != Q_NULLPTR){
        currentFocusElement->setContext(changeContext);
        displayElementInfo(currentFocusElement);
    }
    update();
}


void MainWindow::on_specialAttributeOneLineedit_returnPressed()
{
    int value = ui -> specialAttributeOneLineedit -> text().toInt();
    elementType type = currentFocusElement -> getType();
    if(type == NODE){
        NodeElement* ptr = (NodeElement*) currentFocusElement;
        ptr -> setRadius(value);
    }

    else if(type == STACK){
        StackElement* ptr = (StackElement*) currentFocusElement;
        ptr -> setBlockWidth(value);
    }

    else if(type == QUEUE){
        QueueElement* ptr = (QueueElement*) currentFocusElement;
        ptr -> setBlockWidth(value);
    }
    update();
}


void MainWindow::on_specialAttributeTwoLineedit_returnPressed()
{
    int value = ui -> specialAttributeTwoLineedit -> text().toInt();
    elementType type = currentFocusElement -> getType();
    if(type == STACK){
        StackElement* ptr = (StackElement*) currentFocusElement;
        ptr -> setBlockHeight(value);
    }

    else if(type == QUEUE){
        QueueElement* ptr = (QueueElement*) currentFocusElement;
        ptr -> setBlockHeight(value);
    }
    update();
}


void MainWindow::on_newBlockButton_pressed()
{
    QString value = ui -> newBlockLineedit -> text();
    elementType type = currentFocusElement -> getType();
    if(type == STACK){
        StackElement* ptr = (StackElement*) currentFocusElement;
        ptr -> newBlock(value);
    }

    else if(type == QUEUE){
        QueueElement* ptr = (QueueElement*) currentFocusElement;
        ptr -> newBlock(value);
    }
    update();
}


void MainWindow::on_popBlockButton_pressed(){
    elementType type = currentFocusElement -> getType();
    if(type == STACK){
        StackElement* ptr = (StackElement*) currentFocusElement;
        ptr -> popBlock();
    }

    else if(type == QUEUE){
        QueueElement* ptr = (QueueElement*) currentFocusElement;
        ptr -> popBlock();
    }
    update();
}


void MainWindow::on_directCheckBox_stateChanged(int arg1)
{
    if(currentFocusElement -> getType() != ARROW) return;

    ArrowElement *aptr = (ArrowElement*)currentFocusElement;
    if(arg1){
        aptr -> setIsDirected(true);
    }

    else{
        aptr -> setIsDirected(false);
    }
    update();
    ui -> board -> updateLogic();
}


void MainWindow::on_dfsButton_pressed()
{
    if(currentFocusElement != Q_NULLPTR && currentFocusElement -> getType() == NODE){
        ui -> board -> dfsFromNode((NodeElement*)currentFocusElement);
    }
}


void MainWindow::on_bfsButton_pressed()
{
    if(currentFocusElement != Q_NULLPTR && currentFocusElement -> getType() == NODE){
        ui -> board -> bfsFromNode((NodeElement*)currentFocusElement);
    }
}


void MainWindow::on_polyLayout_pressed()
{
    if(currentFocusElement != Q_NULLPTR && currentFocusElement -> getType() == NODE){
        ui -> board -> polyLayout((NodeElement*)currentFocusElement);
    }
}


void MainWindow::on_treeLayout_pressed()
{
    if(currentFocusElement != Q_NULLPTR && currentFocusElement -> getType() == NODE){
        ui -> board -> treeLayout((NodeElement*)currentFocusElement);
    }
}


void MainWindow::on_topoLayout_pressed()
{
    if(currentFocusElement != Q_NULLPTR && currentFocusElement -> getType() == NODE){
        ui -> board -> topoLayout((NodeElement*)currentFocusElement);
    }
}


class DSU{


public:


    DSU(int n){
        for(int i = 0;i <= n;i++){
            father.push_back(i);
        }
    }


    int findFather(int x){
        return father[x] == x ? x : findFather(father[x]);
    }


    void unionset(int x, int y){
        int fax = findFather(x), fay = findFather(y);
        if(fax == fay) return;
        else father[fay] = fax;
    }


private:


    QVector <int> father;


};


void MainWindow::on_generateButton_pressed()
{
    QString text = ui -> quickInputTextEdit -> toPlainText();
    QTextStream input(&text);
    QVector <NodeElement*> nodes;
    int n, m, index;
    input >> n >> m >> index;
    DSU dsu(n);
    for(int i = 1;i <= n;i++){
        NodeElement* curNode = (NodeElement*)ui -> board -> initNode(QPoint(50, 50));
        nodes.push_back(curNode);
    }
    for(int i = 1;i <= m;i++){
        int u, v;
        input >> u >> v;
        dsu.unionset(u, v);
        ui -> board -> initArrow((NodeElement*)nodes[u - index], (NodeElement*)nodes[v - index]);
    }
    ui -> board -> updateLogic();
    for(int i = index;i < nodes.size();i++){
        if(dsu.findFather(i) == i){
            ui -> board -> treeLayout(nodes[i]);
        }
    }
    displayUnsave();
}


void MainWindow::setDrawingButtonEnable(bool state){
    ui -> arrowSetButton -> setEnabled(state);
    ui -> graphSetButton -> setEnabled(state);
    ui -> cursorSetButton -> setEnabled(state);
    ui -> queueSetButton -> setEnabled(state);
    ui -> stackSetButton -> setEnabled(state);
    ui -> generateButton -> setEnabled(state);
}
