#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plotwindow.h"
#include <qtextedit.h>
#include <QMessageBox>
#include <QTableWidget>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->Plot->setEnabled(false);

    //создаем второе окно (но не открываем его!!!)
    PlotWindow *window = new PlotWindow;

    //устанавливаем картинки в лейауты (делать надо так для того чтоб в екзкшнике были картинки)
    QPixmap myPixmap1("tv4_MatO.png");
    ui->label_Mx->setPixmap(myPixmap1);
    QPixmap myPixmap2("tv4_Disp.png");
    ui->label_Dx->setPixmap(myPixmap2);
    QPixmap myPixmap3("tv4_sigma.png");
    ui->label_Sigma->setPixmap(myPixmap3);

    //не даем пользователю редактировать первый столбец
    ui->Table->item(0,0)->setFlags(Qt::ItemIsEnabled);
    ui->Table->item(1,0)->setFlags(Qt::ItemIsEnabled);

    //коннекты!!! (все остальные Qt creator сам прописал)
    connect(ui->Plot, SIGNAL(clicked()), this, SLOT(on_ButtonSend()));//тут формируется сигнал
    connect(this, SIGNAL(sendData(QVector<double>,QVector<double>)), window, SLOT(recieveData(QVector<double>,QVector<double>)));//тут сигнал отправляется во вторую форму в слот recieveData
    connect(ui->Plot, SIGNAL(clicked()), window, SLOT(show()));//тут при нажатии на кнопку "Построить график" открывается второе окно

}

MainWindow::~MainWindow()
{
    delete ui;
}

//слот который добавляет к таблице столбец
void MainWindow::on_AddColumn_clicked()
{
    ui->Table->setColumnCount(ui->Table->columnCount()+1); 
    QTableWidgetItem *NextItem = new QTableWidgetItem("0");
    QTableWidgetItem *NextItem2 = new QTableWidgetItem("0");
    ui->Table->setItem(1,ui->Table->columnCount()-1,NextItem);
    ui->Table->setItem(0,ui->Table->columnCount()-1,NextItem2);
}

//слот который отчищает все поля от данных и в массивы записывает нули
void MainWindow::on_Clear_clicked()
{
    QTableWidgetItem *VoidItem = new QTableWidgetItem("0");
    QTableWidgetItem *VoidItem2 = new QTableWidgetItem("0");

    ui->Table->setColumnCount(2);
    ui->Table->setItem(1,1,VoidItem);
    ui->Table->setItem(0,1,VoidItem2);
    for(int i = 0; i<100;i++){
        Pi[i] = 0;
        Xi[i] = 0;
    }
    ui->List->clear();
    ui->Mx->clear();
    ui->Dx->clear();
    ui->Sigma->clear();
    ui->Plot->setEnabled(false);

}

//Слот расчета всех вычисляемых значений
int MainWindow::on_Calculate_clicked()
{
    float Mx = 0;
    float Mx2 = 0;
    float Dx;
    float Sigma;
    float sumPi = 0;
    int indexFx = 0;

    SumElem = ui->Table->columnCount()-1; //кол-во элементов в строке таблицы

     ui->List->clear();
    //заносим в массивы данные с таблицы
     for(int i = 0; i<ui->Table->columnCount()-1; i++){
        QTableWidgetItem *Item  = ui->Table->item(0,i+1);
        Xi[i] = (Item->text()).toInt();
        QTableWidgetItem *Item2  = ui->Table->item(1,i+1);
        Pi[i] = (Item2->text()).toFloat();
    }

    //считаем сумму всех Pi
    for(int i = 0; i<ui->Table->columnCount()-1; i++)
    {
        sumPi += Pi[i];
    }
    //если она не равна 1, то выводим ошибку и прекращаем выполнение функции
    if(sumPi != 1)
    {
        error();
        return 0;
    }
    //обнуляем сумму т.к. эта переменная будет использоваться в дальнейшем
    sumPi = 0;

    //M(X)
    for(int i = 0; i<ui->Table->columnCount()-1; i++){
        Mx += Pi[i]*Xi[i];
    }

    //D(X)
    for(int i = 0; i<ui->Table->columnCount()-1; i++){
        Mx2 += Pi[i]*Xi[i]*Xi[i];
    }
    Dx = Mx2 - Mx*Mx;

    //Sigma
    Sigma = sqrt(Dx);


    //установка значений в виджеты
    ui->Mx->setText(QString::number(Mx));
    ui->Dx->setText(QString::number(Dx));
    ui->Sigma->setText(QString::number(Sigma));

    //первое значение всегда нуль
    ui->List->insertItem(1,QString::number(0,10)+QString("\t")+QString("если")+QString("   ")+QString("x<")+QString::number(Xi[0],10));
    //все остальные значения находим и записываем в лист в цикле
    for(int i = 1; i<ui->Table->columnCount()-1; i++){
    indexFx = i;
    sumPi+=Pi[i-1];
    ui->List->insertItem(i+1,QString::number(sumPi)+QString("\t")+QString("если")+QString("   ")+QString::number(Xi[i-1],10)+QString("<x<=")+QString::number(Xi[i],10));
    }
    //последнее значение всегда единица
    ui->List->insertItem(indexFx+1,QString::number(1,10)+QString("\t")+QString("если")+QString("   ")+QString("x>")+QString::number(Xi[indexFx],10));

    //обнуляем сумму (нужно для последовательных вычислений)
    sumPi = 0;

     ui->Plot->setEnabled(true);
}

//слот ошибки
void MainWindow::error()
{
    QMessageBox::about(this,"Ошибка","Сумма всех Pi должна быть равна единице!!!");
}

//слот вызова справки
void MainWindow::on_Help_clicked()
{
     QMessageBox::about(this,"Справка","1)Сумма всех Pi = 1\n2)Таблица не должна содержать пустых полей\n3)Построение графика возможно только после проведения всех расчётов\n4)Для удобства просмотра графиков реализовано изменение интервала по оси OX");
}

//слот при выполнении которого формируется массивы (QVector) координат (массив иксов и массив игриков) и испускает сигнал содержащий эти массивы
void MainWindow::on_ButtonSend(){
    QVector<double> x(SumElem),y(SumElem);
    for(int i = 0; i<ui->Table->columnCount()-1; i++)
    {
        x[i] = Pi[i];
        y[i] = Xi[i];
    }

    emit sendData(y,x);//тут испускается сигнал
}

