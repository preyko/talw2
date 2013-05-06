#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPoint>
#include <QTextFormat>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->actionOpen_RL_file,SIGNAL(triggered()),SLOT(openRLFile()));
    connect(ui->actionSave_RL_file,SIGNAL(triggered()),SLOT(saveRLFile()));
    connect(ui->actionSave_as,SIGNAL(triggered()),SLOT(saveAsRLFile()));
    connect(ui->actionProcess,SIGNAL(triggered()),SLOT(startProcess()));
}

void MainWindow::openRLFile() {
    QString filename = QFileDialog::getOpenFileName(this,
                                                    QString::fromLocal8Bit("Open RLCode file."),
                                                    QDir::home().absolutePath(),
                                                    tr("RLCode file (*.rl)"));
    if(filename.isEmpty()) return;

    QFile rl_file(filename);
    fillCode_(rl_file);

    codeFileName_ = filename;
    ui->statusBar->showMessage(codeFileName_);
}

void MainWindow::saveRLFile() {
    QFile codeTmp(codeFileName_);
    ui->codeBrowser->writeContent(codeTmp);

    ui->statusBar->showMessage(codeFileName_);
}

void MainWindow::saveAsRLFile() {
    codeFileName_ = QFileDialog::getSaveFileName(this,
                                                     QString::fromLocal8Bit("Save RLCode as..."),
                                                     QDir::home().absolutePath());
    saveRLFile();
}

inline void setLineRed(QTextBrowser* tb,int line) {
    QColor wlc(Qt::red);
    wlc.setAlpha(50);
    QTextCharFormat wlf;
    wlf.setBackground(QBrush(wlc));
    tb->cursorForPosition(QPoint(0,line)).setCharFormat(wlf);
}

void MainWindow::startProcess() {
    try {
        ui->codeBrowser->unselectAll();

        QFile codeTmp("code.tmp");
        ui->codeBrowser->writeContent(codeTmp);

        RLTokenizer::Tokenize("code.tmp","tok.tmp");

        QFile tok("tok.tmp");
        fillTO_(tok);

        RLInterpreter::Initialization();

        RLPrecompiler::setPrecompilerOutput("log.tmp");
        RLPrecompiler::Precompiler("tok.tmp");

        QFile llog("log.tmp");
        fillLog_(llog);

        RLInterpreter::setApplicationOutput("appout.tmp");
        RLInterpreter::Perform();

        QFile app_out("appout.tmp");
        fillAppOut_(app_out);

    } catch(RLPerformException& exc) {
        ui->applicationOutput->append(tr("Run-time error:") +
                                      QString::number(exc.whatLine()) +
                                      tr(":") +
                                      QString(exc.what().c_str()));

        ui->codeBrowser->selectLine(exc.whatLine());

    } catch(RLPrecompiler::Exception& exc) {
        ui->precompilerLog->append(tr("Precompile error:") +
                                      QString::number(exc.whatLine()) +
                                      tr(":") +
                                      QString(exc.what().c_str()));

        ui->codeBrowser->selectLine(exc.whatLine());
    }
}

void MainWindow::fillCode_(QFile& source) {
    ui->codeBrowser->clear();
    ui->codeBrowser->readContent(source);
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

    source.open(QIODevice::ReadOnly);

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

void MainWindow::fillAppOut_(QFile& source) {
    ui->applicationOutput->clear();

    source.open(QIODevice::ReadOnly);

    if(!source.exists()) {
        QMessageBox mb(this);
        mb.setText("Such file doesn't exist.");
        mb.setDefaultButton(QMessageBox::Ok);
        mb.show();
    }

    while(!source.atEnd())
        ui->applicationOutput->append(source.readAll());

    source.close();
}

void MainWindow::codeChanged() {
    codeNotChanged_ = false;

    statusBar()->showMessage(codeFileName_ +
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
