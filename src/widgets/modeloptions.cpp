#include "modeloptions.h"
#include "ui_modeloptions.h"

ModelOptions::ModelOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModelOptions)
{
    ui->setupUi(this);
}

ModelOptions::~ModelOptions()
{
    delete ui;
}
