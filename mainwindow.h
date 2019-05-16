#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "plotwindow.h"
#include <QDialog>
#include <QtWidgets/QMainWindow>
#include <qtextedit.h>
#include <QTableWidget>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int Xi[100];
    float Pi[100];
    int SumElem;

signals:
    //здесь описывается кастомный сигнал
    void sendData(QVector<double>,QVector<double>);

private slots:

    void on_ButtonSend();

    void on_AddColumn_clicked();

    void on_Clear_clicked();

    int on_Calculate_clicked();

    void error();

    void on_Help_clicked();

private:

    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
