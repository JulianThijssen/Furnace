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
    ui->viewWidget->bake(0);
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
    ui->viewWidget->save(fileName, 0);
}

void MainWindow::on_closeButton_clicked()
{
    QApplication::quit();
}

void MainWindow::on_modelViewButton_pressed()
{
    ui->normalViewButton->setChecked(false);
    ui->optionsStack->setCurrentIndex(0);
}

void MainWindow::on_normalViewButton_pressed()
{
    ui->modelViewButton->setChecked(false);
    ui->optionsStack->setCurrentIndex(1);
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    int resolution;

    switch (index) {
    case 0: resolution = 128; break;
    case 1: resolution = 256; break;
    case 2: resolution = 512; break;
    case 3: resolution = 1024; break;
    case 4: resolution = 2048; break;
    case 5: resolution = 4096; break;
    case 6: resolution = 8192; break;
    default: {
        resolution = 1024;
        qDebug() << "Unrecognised resolution setting";
    }
    }

    ui->viewWidget->setResolution(resolution);
}
