#pragma once

#include <fstream>
#include <iostream>

#include <QMainWindow>
#include <QMessageBox>
#include <QWidget>
#include <QLabel>

#include <QFileDialog>
#include <QFile>

#include <QPainter>
#include <QPixmap>

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

    void openMazeFile();
    void clearMaze();

    void startProcess();

    void codeChanged();

    void showMaze();
    
    void setTitle(QString title = "");

private:
    Ui::MainWindow *ui;

    QString codeFileName_;

    bool codeNotChanged_;
};
