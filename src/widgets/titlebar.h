#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>

class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
};

#endif // TITLEBAR_H
