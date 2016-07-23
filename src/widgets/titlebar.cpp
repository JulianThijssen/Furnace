#include "titlebar.h"

#include <QStyleOption>
#include <QPainter>
#include <QDebug>

TitleBar::TitleBar(QWidget *parent) :
    QWidget(parent),
    moving(false)
{

}

void TitleBar::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void TitleBar::mousePressEvent(QMouseEvent *event) {
    pos = event->pos();
    moving = true;
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    moving = false;
}

void TitleBar::mouseMoveEvent(QMouseEvent *event) {
    if (moving) {
        QPoint diff = event->pos() - pos;
        QPoint newPos = parentWidget()->parentWidget()->pos() + diff;
        parentWidget()->parentWidget()->move(newPos);
    }
}
