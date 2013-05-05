#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->actionOpen_RL_file,SIGNAL(triggered()),SLOT(openRLFile()));
    connect(ui->actionProcess,SIGNAL(triggered()),SLOT(startProcess()));

    connect(ui->codeBrowser,SIGNAL(textChanged()),SLOT(codeChanged()));
}

void MainWindow::openRLFile() {
    ui->codeBrowser->clear();

    QString filename = QFileDialog::getOpenFileName(this,
                                                     QString::fromLocal8Bit("Открыть видео"),
                                                     QDir::home().absolutePath());
    QFile rl_file(filename);

    fillCode_(rl_file);

    rlFile_ = filename;
    ui->statusBar->showMessage(rlFile_);
}

void MainWindow::startProcess() {
    QString code = ui->codeBrowser->toPlainText();
    QFile codeTmp("code.tmp");
    codeTmp.open(QIODevice::WriteOnly);
    codeTmp.write(code.toAscii());
    codeTmp.close();

    RLTokenizer::Tokenize("code.tmp","tok.tmp");

    QFile tok("tok.tmp");
    fillTO_(tok);

    RLInterpreter::Initialization();

    std::ofstream log("log.tmp");
    RLPrecompiler::logstream = &log;
    RLPrecompiler::Precompiler("tok.tmp");
    log.close();

    QFile llog("log.tmp");
    fillLog_(llog);

    RLInterpreter::Perform();
}

void MainWindow::fillCode_(QFile& source) {
    ui->codeBrowser->clear();

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
        ui->codeBrowser->append(//QString::number(linen++) +
                                //tr(" ") +
                                res.remove(res.length()-1,1));
    }

    source.close();
}

void MainWindow::fillTO_(QFile& source) {
    ui->tokenOutput->clear();

    source.open(QIODevice::ReadOnly | QIODevice::Text);

    if(!source.exists()) {
        QMessageBox mb(this);
        mb.setText("Such file doesn't exist.");
        mb.setDefaultButton(QMessageBox::Ok);
        mb.show();
    }

    while(!source.atEnd())
        ui->tokenOutput->append(source.readAll());

    source.close();
}

void MainWindow::fillLog_(QFile& source) {
    ui->precompilerLog->clear();

    source.open(QIODevice::ReadOnly | QIODevice::Text);

    if(!source.exists()) {
        QMessageBox mb(this);
        mb.setText("Such file doesn't exist.");
        mb.setDefaultButton(QMessageBox::Ok);
        mb.show();
    }

    while(!source.atEnd())
        ui->precompilerLog->append(source.readAll());

    source.close();
}

void MainWindow::codeChanged() {
    codeNotChanged_ = false;

    statusBar()->showMessage(rlFile_ +
                   tr("*"));
}


void MainWindow::keyPressEvent(QKeyEvent* ke) {
    switch(ke->type()) {
        case Qt::Key_Space:
            startProcess();
        break;
        case Qt::Key_S:
            //TODO save rl file.
        break;
        case Qt::Key_O:
            if(ke->modifiers() == Qt::ControlModifier)
                openRLFile();
        break;
        default:
        break;
    }
}

MainWindow::~MainWindow() {
    delete ui;
}
