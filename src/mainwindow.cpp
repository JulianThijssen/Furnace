#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "modelloader.h"
#include "model.h"

#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_importLowButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
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
                                                    tr("Open File"),
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
