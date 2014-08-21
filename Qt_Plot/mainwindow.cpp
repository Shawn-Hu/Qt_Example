#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    mydelegate=new Delegate(this);
    model=new QStandardItemModel(0,2,this); //We only read  two colums of the CSV file which is seperated by "."
    ui->tableView->setModel(model);  //Set the model to table view. MVC
    ui->tableView->setItemDelegate(mydelegate);  //Set the delegate to table view. MVC
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
  buildmodelformfile(model);
  plotdata(model);
}

void MainWindow::on_pushButton_3_clicked()
{
    QFile file(ui->lineEdit->text());
     file.open(QIODevice::WriteOnly | QIODevice::Text);
     QString writefile;
     QTextStream out(&file);
     writefile = model->horizontalHeaderItem(0)->text()+","+model->horizontalHeaderItem(1)->text();
     out << writefile;

    for (int j=0; j<1023; ++j)
    {
      writefile = model->item(j,0)->text()+","+model->item(j,1)->text();
      out<<writefile;
    }

}

void MainWindow::buildmodelformfile(QStandardItemModel *model)
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

    QString titleline=file.readLine();  //read the title line
    QStringList titlelist=titleline.split(","); //split by "." for two table header items
    int linenumber=0;

    model->setHorizontalHeaderItem(0, new QStandardItem(QString(titlelist[0]))); //Set the table header
    model->setHorizontalHeaderItem(1, new QStandardItem(QString(titlelist[1])));

    while(!file.atEnd())   //Read in the data from CSV file and fill the model with data. MVC model here
    {
        QString lines=file.readLine();  //read in oneline of the data
        QStringList lineslist=lines.split(","); //split the data
        model->appendRow(new QStandardItem(QString("0"))); //append one row to the model
        QStandardItem *Rows0 = new QStandardItem(QString(lineslist[0])); //Create hte now row with data for this line
        QStandardItem *Rows1 = new QStandardItem(QString(lineslist[1]));
        model->setItem(linenumber,0,Rows0);
        model->setItem(linenumber,1,Rows1);
        linenumber++;
    }
    file.close();
}

void MainWindow::plotdata(QStandardItemModel *model)
{
    ui->customPlot->legend->setVisible(true); // plot the data
    ui->customPlot->legend->setFont(QFont("Helvetica", 9));
    QPen pen;
      ui->customPlot->addGraph();
      pen.setColor(QColor(60, 80, 180));
      ui->customPlot->graph()->setPen(pen);
      ui->customPlot->graph()->setName(QString("test"));
      ui->customPlot->graph()->setLineStyle((QCPGraph::LineStyle)QCPGraph::lsLine);
      ui->customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));

      // generate data:
      QVector<double> x(1023), y(1023);
      for (int j=0; j<1023; ++j)
      {
        x[j] = model->item(j,0)->text().toDouble();
        y[j] = model->item(j,1)->text().toDouble();
      }
      ui->customPlot->graph()->setData(x, y);
      ui->customPlot->graph()->rescaleAxes(true);

    //  ui->customPlot->yAxis->scaleRange(1.1, ui->customPlot->yAxis->range().center());
    //  ui->customPlot->xAxis->scaleRange(1.1, ui->customPlot->xAxis->range().center());

      ui->customPlot->axisRect()->setupFullAxesBox();
      ui->customPlot->replot();
}



