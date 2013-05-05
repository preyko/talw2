#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->actionOpen_RL_file,SIGNAL(triggered()),SLOT(openRLFile()));
    connect(ui->actionProcess,SIGNAL(triggered()),SLOT(startProcess()));
}

void MainWindow::openRLFile() {
    ui->codeBrowser->clear();

    QString filename = QFileDialog::getOpenFileName(this,
                                                     QString::fromLocal8Bit("Открыть видео"),
                                                     QDir::home().absolutePath());
    QFile rl_file(filename);

    fillCode_(rl_file);
}

void MainWindow::startProcess() {
    //RLTokenizer::Tokenize()
}

void MainWindow::fillCode_(QFile& source) {
    source.open(QIODevice::ReadOnly | QIODevice::Text);

    if(!source.exists()) {
        QMessageBox mb(this);
        mb.setText("Such file doesn't exist.");
        mb.setDefaultButton(QMessageBox::Ok);
        mb.show();
    }

    int linen = 0;
    while(!source.atEnd()) {
        QString res = source.readLine();
        ui->codeBrowser->append(QString::number(linen++) +
                                tr(" ") +
                                res.remove(res.length()-1,1));
    }

    source.close();
}

void MainWindow::fillLog_(QFile& source) {
    source.open(QIODevice::ReadOnly | QIODevice::Text);

    if(!source.exists()) {
        QMessageBox mb(this);
        mb.setText("Such file doesn't exist.");
        mb.setDefaultButton(QMessageBox::Ok);
        mb.show();
    }

    while(!source.atEnd())
        ui->codeBrowser->append(source.readAll());

    source.close();
}

MainWindow::~MainWindow() {
    delete ui;
}
