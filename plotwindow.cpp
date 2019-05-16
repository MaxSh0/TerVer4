#include "plotwindow.h"
#include "ui_plotwindow.h"
#include "mainwindow.h"
PlotWindow::PlotWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlotWindow)
{
    ui->setupUi(this);

}

PlotWindow::~PlotWindow()
{
    delete ui;
}

//слот который изменяет интервал на графике
void PlotWindow::on_bildButton_clicked()
{
 double a = ui->Begin_OX->value(); //Начало интервала, где рисуем график по оси Ox
    double b = ui->end_OX->value(); //Конец интервала, где рисуем график по оси Ox
    ui->widget->xAxis->setRange(a, b);
    ui->widget->replot();
    ui->widget_2->xAxis->setRange(a, b);//Для оси Ox
    ui->widget_2->replot();
}

//слот который рисует 2 графика по данным из первого окна
void PlotWindow::recieveData(QVector<double>x ,QVector<double>y) {

    ui->Begin_OX->setValue(x[0]-1);//Начало интервала, где рисуем график по оси Ox
    ui->end_OX->setValue(x[x.size()-1]+1);//Конец интервала, где рисуем график по оси Ox


    ui->widget->clearGraphs();//Если нужно, но очищаем все графики
    //Добавляем один график в widget
    ui->widget->addGraph();
    //Говорим, что отрисовать нужно график по нашим двум массивам x и y


    ui->widget->graph(0)->setData(x, y, true);

       ui->widget->graph(0)->setLineStyle((QCPGraph::LineStyle)1);//убираем линии
       //формируем вид точек
       ui->widget->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));


    //Подписываем оси Ox и Oy
    ui->widget->xAxis->setLabel("Xi");
    ui->widget->yAxis->setLabel("Pi");

    //Установим область, которая будет показываться на графике
    ui->widget->xAxis->setRange(x[0]-1, x[x.size()-1]+1);//Для оси Ox
    ui->widget->yAxis->setRange(0, 1);//Для оси Oy
    //И перерисуем график на нашем widget
    ui->widget->replot();


    //ВТОРОЙ ГРАФИК
    float sumY = 0;
    //Начальный
    ui->widget_2->addGraph();
    QVector<double> x0(2), y0(2);
    y0[0] = 0;
    x0[0] = x[0];
    y0[1] = 0;
    x0[1] = -10000;
    ui->widget_2->graph(0)->setData(x0, y0);
     ui->widget_2->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));

    //середина
    QVector<double> xn(2), yn(2);
    for(int i = 0; i<x.size()-1;i++)
    {
       ui->widget_2->addGraph();
       sumY += y[i];
       yn[0] = sumY;
       yn[1] = sumY;
       xn[0] = x[i];
       xn[1] = x[i+1];
       ui->widget_2->graph(i+1)->setData(xn, yn);
       ui->widget_2->graph(i+1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
    }

    //конец
    ui->widget_2->addGraph();
    QVector<double> xend(2), yend(2);
    yend[0] = 1;
    xend[0] = x[x.size()-1];
    yend[1] = 1;
    xend[1] = 10000;
    ui->widget_2->graph(x.size())->setData(xend, yend);
    ui->widget_2->graph(x.size())->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));

    ui->widget_2->xAxis->setRange(x[0]-1, x[x.size()-1]+1);//Для оси Ox
    ui->widget_2->yAxis->setRange(0, 1);//Для оси Oy
    ui->widget_2->replot();
}
