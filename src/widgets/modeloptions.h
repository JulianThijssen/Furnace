#ifndef MODELOPTIONS_H
#define MODELOPTIONS_H

#include <QWidget>

namespace Ui {
class ModelOptions;
}

class ModelOptions : public QWidget
{
    Q_OBJECT

public:
    explicit ModelOptions(QWidget *parent = 0);
    ~ModelOptions();

private:
    Ui::ModelOptions *ui;
};

#endif // MODELOPTIONS_H
