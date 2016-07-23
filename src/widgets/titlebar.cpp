#include "titlebar.h"

#include <QStyleOption>
#include <QPainter>

TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{

}

void TitleBar::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
