#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setFixedSize(926,468); //use fix size for the moment
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

void MainWindow::on_pushButton_2_clicked()
{
    close();
}

void MainWindow::on_pushButton_3_clicked()
{
    QFile file(ui->lineEdit->text());
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QString writefile;
    QTextStream out(&file);
    writefile = model->horizontalHeaderItem(0)->text()
            +","
            +model->horizontalHeaderItem(1)->text(); //write back the header of the CSV file
    out << writefile+"\n";
    for (int j=0; j<(model->rowCount()); ++j)
    {
      writefile = model->item(j,0)->text()+","+model->item(j,1)->text();
      out<<writefile+"\n";
    }
    file.close();  //write back to file and close
}

void MainWindow::trackChanged( const QString & entry )
{
    plotdata(model);
}

void MainWindow::buildmodelformfile(QStandardItemModel *model)
{
    model->removeRows(0,model->rowCount());  //We  need to empty the model every time the file is loaded
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open csv file"), "./", tr("CSV Files (*.csv);; ALL Files (*.*)"));
    QFile file(fileName);
    if(file.exists())
    {
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
          QMessageBox::information(0, "error", file.errorString());
        }
        watcher.addPath(fileName); //Creat a watcher for the file, if the file changes, the plot will be updated if we save the data
        QObject::connect( &watcher, SIGNAL(fileChanged(const QString&)),this,SLOT(trackChanged(const QString)));
    }
    else
    {
        QMessageBox::information(0, "error", "file not exists");
    }
    ui->lineEdit->setText(fileName);  //set the path string

    QString titleline=file.readLine();  //read the title line
    QStringList titlelist=titleline.split(","); //split by "." for two table header items
    int linenumber=0;
    titlelist[1].remove(QRegExp("[\\n\\t\\r]")); //delete the line ending
    model->setHorizontalHeaderItem(0, new QStandardItem(QString(titlelist[0]))); //Set the table header
    model->setHorizontalHeaderItem(1, new QStandardItem(QString(titlelist[1])));

    while(!file.atEnd())   //Read in the data from CSV file and fill the model with data. MVC model here
    {
        QString lines=file.readLine();  //Read in one line of the data
        QStringList lineslist=lines.split(","); //Split the data
        model->appendRow(new QStandardItem(QString("0"))); //Append one row to the model
        lineslist[1].remove(QRegExp("[\\n\\t\\r]"));  //Remove the line ending of current string
        if((!lineslist[0].isEmpty())&&(!lineslist[1].isEmpty())) //If we have both the data
        {
        QStandardItem *Rows0 = new QStandardItem(QString(lineslist[0])); //Create the row with data for this line
        QStandardItem *Rows1 = new QStandardItem(QString(lineslist[1]));
        model->setItem(linenumber,0,Rows0);
        model->setItem(linenumber,1,Rows1);
        }
        linenumber++;
    }
    file.close();
}

void MainWindow::plotdata(QStandardItemModel *model)
{
    ui->customPlot->clearGraphs();  //Every time we plot, we need to remove the previous plot
    ui->customPlot->legend->setVisible(true); // plot the data
    ui->customPlot->legend->setFont(QFont("Helvetica", 9));
    QPen pen;
      ui->customPlot->addGraph();
      pen.setColor(QColor(60, 80, 180));
      ui->customPlot->graph()->setPen(pen);
      ui->customPlot->graph()->setName(model->horizontalHeaderItem(0)->text()
                                       + " "
                                       + model->horizontalHeaderItem(1)->text());
      ui->customPlot->graph()->setLineStyle((QCPGraph::LineStyle)QCPGraph::lsLine);
      ui->customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));

      // generate data:
      QVector<double> x(model->rowCount()), y((model->rowCount()));
      for (int j=0; j<(model->rowCount()); ++j)
      {
        x[j] = model->item(j,0)->text().toDouble();
        y[j] = model->item(j,1)->text().toDouble();
      }
      ui->customPlot->graph()->setData(x, y);
      ui->customPlot->graph()->rescaleAxes(true);
      ui->customPlot->axisRect()->setupFullAxesBox();
      ui->customPlot->replot();
}


