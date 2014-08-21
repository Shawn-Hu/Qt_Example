#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::on_pushButton_clicked()
{



    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open csv file"), "./", tr("CSV Files (*.csv);; ALL Files (*.*)"));
    QFile file(fileName);
    if(file.exists())
    {
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
          QMessageBox::information(0, "error", file.errorString());
        }
    }
    else
    {
        QMessageBox::information(0, "error", "file not exists");
    }
    ui->lineEdit->setText(fileName);  //set the path string

    //QTextStream in(&file);
    model=new QStandardItemModel(0,2,this); //We only read  two colums of the CSV file which is seperated by "."
    QString titleline=file.readLine();
    QStringList titlelist=titleline.split(",");
    int linenumber=0;
   //while(!file.atEnd())
   // {

    //}

    model->setHorizontalHeaderItem(0, new QStandardItem(QString(titlelist[0]))); //Set the table header of the item
    model->setHorizontalHeaderItem(1, new QStandardItem(QString(titlelist[1])));

    while(!file.atEnd())   //Read in the data from CSV file and fill the model with data. MVC model here
    {
        QString lines=file.readLine();
        QStringList lineslist=lines.split(",");
        model->appendRow(new QStandardItem(QString("0")));
        QStandardItem *Rows0 = new QStandardItem(QString(lineslist[0]));
        QStandardItem *Rows1 = new QStandardItem(QString(lineslist[1]));
        model->setItem(linenumber,0,Rows0);
        model->setItem(linenumber,1,Rows1);
        linenumber++;
    }
    ui->tableView->setModel(model);

    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setFont(QFont("Helvetica", 9));
    QPen pen;
      ui->customPlot->addGraph();
      pen.setColor(QColor(60, 80, 180));
      ui->customPlot->graph()->setPen(pen);
      ui->customPlot->graph()->setName("Data plot");
      ui->customPlot->graph()->setLineStyle((QCPGraph::LineStyle)QCPGraph::lsLine);
      ui->customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));

      // generate data:
      QVector<double> x(linenumber), y(linenumber);
      for (int j=0; j<linenumber; ++j)
      {
        x[j] = model->item(j,0)->text().toDouble();
        y[j] = model->item(j,1)->text().toDouble();
      }
      ui->customPlot->graph()->setData(x, y);
      ui->customPlot->graph()->rescaleAxes(true);

      ui->customPlot->yAxis->scaleRange(1.1, ui->customPlot->yAxis->range().center());
      ui->customPlot->xAxis->scaleRange(1.1, ui->customPlot->xAxis->range().center());

      ui->customPlot->axisRect()->setupFullAxesBox();
      ui->customPlot->replot();


}


