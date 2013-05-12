#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->actionOpen_RL_file,SIGNAL(triggered()),SLOT(openRLFile()));
    connect(ui->actionSave_RL_file,SIGNAL(triggered()),SLOT(saveRLFile()));
    connect(ui->actionSave_as,SIGNAL(triggered()),SLOT(saveAsRLFile()));
    connect(ui->actionProcess,SIGNAL(triggered()),SLOT(startProcess()));
    
    connect(ui->codeBrowser,SIGNAL(codeChanged()),SLOT(codeChanged()));
    
    setTitle();
}

void MainWindow::openRLFile() {
    QString filename = QFileDialog::getOpenFileName(this,
                                                    QString::fromLocal8Bit("Open RLCode file."),
                                                    QDir::home().absolutePath(),
                                                    tr("RLCode file (*.rl)"));
    if(filename.isEmpty()) return;

    QFile code(filename);
    ui->codeBrowser->readContent(code);
    codeFileName_ = filename;
	
    setTitle(codeFileName_);
}

void MainWindow::saveRLFile() {
    QFile codeTmp(codeFileName_);
    ui->codeBrowser->writeContent(codeTmp);

    setTitle(codeFileName_);
}

void MainWindow::saveAsRLFile() {
    codeFileName_ = QFileDialog::getSaveFileName(this,
                                                 QString::fromLocal8Bit("Save RLCode as..."),
                                                 QDir::home().absolutePath(),
                                                 tr("RLCode file (*.rl)"));
    if(!codeFileName_.contains(tr(".rl")))
        codeFileName_.append(tr(".rl"));
    
    saveRLFile();
}

void MainWindow::startProcess() {
	ui->codeBrowser->unselectAll();

    QFile codeTmp("code.tmp");
    ui->codeBrowser->writeContent(codeTmp);
	
	ui->tokenOutput->clear();
	ui->precompilerOutput->clear();
	ui->applicationOutput->clear();
	
	RLOStream tokenOutput("tok.tmp");
	RLOStream precpOutput("log.tmp");
	RLOStream appliOuptut("appout.tmp");
	
	connect(&tokenOutput,SIGNAL(sendInterceptedData(QString)),ui->tokenOutput,SLOT(append(QString)));
	connect(&precpOutput,SIGNAL(sendInterceptedData(QString)),ui->precompilerOutput,SLOT(append(QString)));
	connect(&appliOuptut,SIGNAL(sendInterceptedData(QString)),ui->applicationOutput,SLOT(append(QString)));
	
	RLTokenizer::setTokenizerOutput(tokenOutput);
	RLPrecompiler::setPrecompilerOutput(precpOutput);
	RLInterpreter::setApplicationOutput(appliOuptut);
	
    try {
        RLTokenizer::Tokenize("code.tmp");

        RLPrecompiler::Precompile("tok.tmp");
		
        RLInterpreter::Perform();

        ui->tabWidget->setCurrentWidget(ui->programOutputTab);
    } catch(RLPerformException& exc) {
        RLPrecompiler::getPrecompilerOutput() << "Run-time error:"
                                              << intToStr(exc.whatLine()) 
                                              << ":"
                                              << exc.what().c_str()
											  << std::endl;

        ui->codeBrowser->selectLine(exc.whatLine());
    } catch(RLPrecompiler::Exceptions& exc) {
        for(uint i=0;i<exc.getCollectioner().size();i++) {
            RLPrecompiler::Exception* exception = &exc.getCollectioner()[i];

            RLPrecompiler::getPrecompilerOutput() << "Precompile error:"
                                                  << exception->whatLine()
                                                  << " : " 
                                                  << exception->what().c_str()
												  << std::endl;

            ui->codeBrowser->selectLine(exception->whatLine());
        }
        ui->tabWidget->setCurrentWidget(ui->precompilerOutputTab);
    } catch(RLTokenizer::Exceptions& exc) {
        for(uint i=0;i<exc.getCollectioner().size();i++) {
            RLTokenizer::Exception* exception = &exc.getCollectioner()[i];

            RLPrecompiler::getPrecompilerOutput() << "Unexpected token:"
                                                  << exception->whatLine()
                                                  << " : " 
                                                  << exception->what().c_str()
												  << std::endl;

            ui->codeBrowser->selectLine(exception->whatLine());
        }
        ui->tabWidget->setCurrentWidget(ui->precompilerOutputTab);
    }
	
	//disconnect(&toi,SIGNAL(sendInterceptedData(QString)),ui->tokenOutput,SLOT(append(QString)));
	//disconnect(&pro,SIGNAL(sendInterceptedData(QString)),ui->precompilerOutput,SLOT(append(QString)));
	//disconnect(&apo,SIGNAL(sendInterceptedData(QString)),ui->applicationOutput,SLOT(append(QString)));
}

void MainWindow::codeChanged() {
    codeNotChanged_ = false;

    setTitle(codeFileName_ +
             tr("*"));
}

void MainWindow::setTitle(QString title) {
    QString t = "RLInterpreter";
    
    if(!title.isEmpty()) {
        t += " - ";
        t += title;
    }
    
    setWindowTitle(t);
}


MainWindow::~MainWindow() {
    delete ui;
}
