#ifndef NORMALMAPOPTIONS_H
#define NORMALMAPOPTIONS_H

#include <QWidget>

namespace Ui {
class NormalMapOptions;
}

class NormalMapOptions : public QWidget
{
    Q_OBJECT

public:
    explicit NormalMapOptions(QWidget *parent = 0);
    ~NormalMapOptions();

private:
    Ui::NormalMapOptions *ui;
};

#endif // NORMALMAPOPTIONS_H
