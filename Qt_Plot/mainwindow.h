#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QLabel>
#include <QTextStream>
#include <QHeaderView>
#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QFileSystemWatcher>
#include "delegate.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void plotdata(QStandardItemModel *model);
    void buildmodelformfile(QStandardItemModel *model);
        ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void trackChanged( const QString & entry );

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model; //Creat model for MVC here
    Delegate *mydelegate;
    QFileSystemWatcher watcher;
};

#endif // MAINWINDOW_H
