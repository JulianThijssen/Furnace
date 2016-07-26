#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../modelloader.h"
#include "../model.h"

#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_importLowButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Low Poly Model"),
                                                    "/path/to/file/",tr("OBJ Files (*.obj)"));


    if (fileName.isEmpty()) {
        return;
    }

    qDebug() << fileName;

    ui->viewWidget->setLowPoly(fileName);
}

void MainWindow::on_importHighButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open High Poly Model"),
                                                    "/path/to/file/",tr("OBJ Files (*.obj)"));


    if (fileName.isEmpty()) {
        return;
    }
    qDebug() << fileName;

    ui->viewWidget->setHighPoly(fileName);
}

void MainWindow::on_bakeNormalButton_clicked()
{
    ui->viewWidget->renderNormal(1024, 1024);
}

void MainWindow::on_exportNormalButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Export Normal Map"),
                                                    "/path/to/file/", tr("Images (*.png *.tga *.bmp *.tif)"));


    if (fileName.isEmpty()) {
        return;
    }
    qDebug() << fileName;
    ui->viewWidget->save(fileName);
}

void MainWindow::on_closeButton_clicked()
{
    QApplication::quit();
}

void MainWindow::on_modelViewButton_clicked()
{

}
