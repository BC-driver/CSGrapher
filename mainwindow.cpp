#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "element.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui -> board, &PaintWidget::showDefaultInfoSignal, this, &MainWindow::displayDefaultInfo);
    connect(ui -> board, &PaintWidget::showElementInfoSignal, this, &MainWindow::displayElementInfo);
    connect(ui -> renderImageAction, &QAction::triggered, this, &MainWindow::renderImageActionSlot);


    idelColor = "(255, 255, 255)";
    activeColor = "(240, 240, 240)";

//    ui -> cursorSetButton -> setStyleSheet("background-color: rgb(255, 255, 255)");
//    ui -> stackSetButton -> setStyleSheet("background-color: rgb(255, 255, 255)");
//    ui -> queueSetButton -> setStyleSheet("background-color: rgb(255, 255, 255)");
//    ui -> graphSetButton -> setStyleSheet("background-color: rgb(255, 255, 255)");
//    ui -> arrowSetButton -> setStyleSheet("background-color: rgb(255, 255, 255)");

    this -> setWindowTitle("CSGraph");
    this -> displayDefaultInfo();

    ui -> board -> setMouseTracking(true);

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


QString MainWindow::getIdelColor(){
    return idelColor;
}


QString MainWindow::getActiveColor(){
    return activeColor;
}


void MainWindow::on_cursorSetButton_clicked()
{
    // this -> currentButton -> setStyleSheet("background-color: rgb" + getIdelColor() + ";");
    ui -> board -> setElement(CURSOR);
    // setCurrentButton(ui -> cursorSetButton);
    // this -> currentButton -> setStyleSheet("background-color: rgb" + getActiveColor() + ";");
}


void MainWindow::on_graphSetButton_clicked()
{
    // this -> currentButton -> setStyleSheet("background-color: rgb" + getIdelColor() + ";");
    ui -> board -> setElement(NODE);
    // setCurrentButton(ui -> graphSetButton);
    // this -> currentButton -> setStyleSheet("background-color: rgb" + getActiveColor() + ";");
}


void MainWindow::on_stackSetButton_clicked()
{
    // this -> currentButton -> setStyleSheet("background-color: rgb" + getIdelColor() + ";");
    ui -> board -> setElement(STACK);
    // setCurrentButton(ui -> stackSetButton);
    // this -> currentButton -> setStyleSheet("background-color: rgb" + getActiveColor() + ";");
}


void MainWindow::on_queueSetButton_clicked()
{
    // this -> currentButton -> setStyleSheet("background-color: rgb" + getIdelColor() + ";");
    ui -> board -> setElement(QUEUE);
    // setCurrentButton(ui -> queueSetButton);
    // this -> currentButton -> setStyleSheet("background-color: rgb" + getActiveColor() + ";");
}


void MainWindow::on_arrowSetButton_clicked()
{
    // this -> currentButton -> setStyleSheet("background-color: rgb" + getIdelColor() + ";");
    ui -> board -> setElement(ARROW);
    // setCurrentButton(ui -> arrowSetButton);
    // this -> currentButton -> setStyleSheet("background-color: rgb" + getActiveColor() + ";");
}


void MainWindow::renderImageActionSlot(){
    filePath = QFileDialog::getSaveFileName(this, "Save",
                                 "C:\\",
                                 "image(*.png)");
    if(filePath.isEmpty()){
        QMessageBox::warning(this, "Waring", "Please choose a vaild file path");
    }
    else{
        QFile file(filePath);
        if(file.exists()){
            QMessageBox::warning(this, "Waring", "This name has been used");
        }
        else{
            QPixmap pixmap = QPixmap::grabWidget(ui -> board);
            pixmap.save(filePath);
        }
    }

}


void MainWindow::updateGraphDataText(){
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QByteArray arr = file.readAll();
    ui -> quickInputTextEdit -> setText(arr);
}


void MainWindow::on_openGraphAction_triggered(){
    filePath = QFileDialog::getOpenFileName(this, "Open",
                                 "C:\\",
                                 "text(*.txt)");
    if(filePath.isEmpty()){
        QMessageBox::warning(this, "Waring", "Please choose a vaild file path");
    }
    else{
        ui -> board -> setFilePath(filePath);
        ui -> board -> initGraph();
        updateGraphDataText();
    }
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
