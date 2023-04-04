#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "element.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui -> board, &PaintWidget::showDefaultInfoSignal, this, &MainWindow::displayDefaultInfo);
    connect(ui -> board, &PaintWidget::showElementInfoSignal, this, &MainWindow::displayElementInfo);
    connect(ui -> renderImageAction, &QAction::triggered, this, &MainWindow::renderImageActionSlot);

    currentButton = ui -> cursorSetButton;
    idelColor = "(255, 255, 255)";
    activeColor = "(240, 240, 240)";

    ui -> cursorSetButton -> setStyleSheet("background-color: rgb(255, 255, 255)");
    ui -> stackSetButton -> setStyleSheet("background-color: rgb(255, 255, 255)");
    ui -> queueSetButton -> setStyleSheet("background-color: rgb(255, 255, 255)");
    ui -> graphSetButton -> setStyleSheet("background-color: rgb(255, 255, 255)");

    this -> setWindowTitle("CSGraph");

    ui -> board -> setMouseTracking(true);

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
    this -> currentButton -> setStyleSheet("background-color: rgb" + getIdelColor() + ";");
    ui -> board -> setElement(CURSOR);
    setCurrentButton(ui -> cursorSetButton);
    this -> currentButton -> setStyleSheet("background-color: rgb" + getActiveColor() + ";");
}


void MainWindow::on_graphSetButton_clicked()
{
    this -> currentButton -> setStyleSheet("background-color: rgb" + getIdelColor() + ";");
    ui -> board -> setElement(NODE);
    setCurrentButton(ui -> graphSetButton);
    this -> currentButton -> setStyleSheet("background-color: rgb" + getActiveColor() + ";");
}


void MainWindow::on_stackSetButton_clicked()
{
    this -> currentButton -> setStyleSheet("background-color: rgb" + getIdelColor() + ";");
    ui -> board -> setElement(STACK);
    setCurrentButton(ui -> stackSetButton);
    this -> currentButton -> setStyleSheet("background-color: rgb" + getActiveColor() + ";");
}


void MainWindow::on_queueSetButton_clicked()
{
    this -> currentButton -> setStyleSheet("background-color: rgb" + getIdelColor() + ";");
    ui -> board -> setElement(QUEUE);
    setCurrentButton(ui -> queueSetButton);
    this -> currentButton -> setStyleSheet("background-color: rgb" + getActiveColor() + ";");
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
    ui -> MapInfo -> setText(arr);
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
    ui -> InfoTitle -> setText("默认参数:");
    ui -> lineWidthlineEdit -> setText(QString::number(Element::getDefaultLineWidth()));
    QColor edgecolor = Element::getDefaultEdgeColor();
    ui -> rLineColorLineEdit -> setText(QString::number(edgecolor.red()));
    ui -> gLineColorLineEdit -> setText(QString::number(edgecolor.green()));
    ui -> bLineColorLineEdit -> setText(QString::number(edgecolor.blue()));
    ui -> lineColorDisplayScreen -> setStyleSheet("background-color: rgb" + QString::number(edgecolor.red()) + ","
                                                  + QString::number(edgecolor.green()) + ","
                                                  + QString::number(edgecolor.blue()) + ");");
}


void MainWindow::displayElementInfo(Element *eptr){
    //TODO
}
