#pragma once

#include <fstream>
#include <iostream>

#include <QMainWindow>

#include <QMessageBox>
#include <QFileDialog>
#include <QFile>

#include <QPoint>

#include <QDebug>

#include "RLType.h"
#include "RLCommand.h"
#include "RLInterpreter.h"

#include "RLTokenizer.h"
#include "RLPrecompiler.h"

#include "RLOStream.h"

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
    void saveRLFile();
    void saveAsRLFile();

    void startProcess();

    void codeChanged();

private:
    Ui::MainWindow *ui;

    void fillCode_(QFile& source);
    void fillTO_(QFile& source);
    void fillLog_(QFile& source);
    void fillAppOut_(QFile& source);

    QString codeFileName_;

    bool codeNotChanged_;
};
