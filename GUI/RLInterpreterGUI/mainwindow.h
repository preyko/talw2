#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMessageBox>
#include <QFileDialog>
#include <QAction>
#include <QFile>

#include "RLTokenizer.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openRLFile();
    void startProcess();
    
private:
    Ui::MainWindow *ui;

    void fillCode_(QFile& source);
    void fillLog_(QFile& source);
    //QString
};

#endif // MAINWINDOW_H
