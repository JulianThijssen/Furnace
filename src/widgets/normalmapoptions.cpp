#include "normalmapoptions.h"
#include "ui_normalmapoptions.h"

NormalMapOptions::NormalMapOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NormalMapOptions)
{
    ui->setupUi(this);
}

NormalMapOptions::~NormalMapOptions()
{
    delete ui;
}
