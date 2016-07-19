#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_importLowButton_clicked();

    void on_importHighButton_clicked();

    void on_bakeNormalButton_clicked();

    void on_exportNormalButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
