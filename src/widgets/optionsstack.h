#ifndef OPTIONSSTACK_H
#define OPTIONSSTACK_H

#include <QStackedWidget>
#include <QWidget>

#include "modeloptions.h"
#include "normalmapoptions.h"

class OptionsStack : public QStackedWidget
{
    Q_OBJECT
public:
    explicit OptionsStack(QWidget *parent) : QStackedWidget(parent) { }
    ~OptionsStack() { }

    ModelOptions* getModelOptions() {
        return (ModelOptions*) widget(0);
    }

    NormalMapOptions* getNormalMapOptions() {
        return (NormalMapOptions*) widget(1);
    }
};

#endif // OPTIONSSTACK_H
