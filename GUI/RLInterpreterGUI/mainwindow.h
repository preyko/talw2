#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMessageBox>
#include <QKeyEvent>
#include <QFileDialog>
#include <QAction>
#include <QFile>

#include <QDebug>

#include "RLType.h"
#include "RLCommand.h"
#include "RLInterpreter.h"

#include "RLTokenizer.h"
#include "RLPrecompiler.h"


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

    void codeChanged();
    
protected:
    void keyPressEvent(QKeyEvent* ke);

private:
    Ui::MainWindow *ui;

    void fillCode_(QFile& source);
    void fillTO_(QFile& source);
    void fillLog_(QFile& source);

    QString rlFile_;

    bool codeNotChanged_;
};

#endif // MAINWINDOW_H
