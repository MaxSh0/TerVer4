#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"

namespace Ui {
class PlotWindow;
}

class PlotWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlotWindow(QWidget *parent = 0);
    ~PlotWindow();


private slots:
    void on_bildButton_clicked();
    void recieveData(QVector<double>,QVector<double>);

private:
    Ui::PlotWindow *ui;



};

#endif // PLOTWINDOW_H
