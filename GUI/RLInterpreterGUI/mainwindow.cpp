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

    connect(ui->actionShow_Maze,SIGNAL(triggered()),SLOT(showMaze()));
    connect(ui->actionLoad_Maze_Shceme,SIGNAL(triggered()),SLOT(openMazeFile()));
    connect(ui->actionClear_Maze,SIGNAL(triggered()),SLOT(clearMaze()));
    
    connect(ui->codeBrowser,SIGNAL(codeChanged()),SLOT(codeChanged()));
    
    setTitle();

    RLRoboMaze::Init();
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

void MainWindow::openMazeFile() {
    QString filename = QFileDialog::getOpenFileName(this,
                                                    QString::fromLocal8Bit("Open RLMazeScheme file."),
                                                    QDir::home().absolutePath(),
                                                    tr("RLMazeScheme file (*.rlscm)"));
    if(filename.isEmpty()) return;

    try {
    RLRoboMaze::loadMaze(filename.toStdString());
    } catch(std::exception exc) {
        QMessageBox::warning(this,tr("Parse error"),tr("Error occures on loading of the maze scheme."));
    }
}

void MainWindow::clearMaze() {
    RLRoboMaze::clearMaze();
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

void MainWindow::showMaze() {
    const int cellWidth = 20;
    const int cellHeight = 20;

    const int lineThick = 2;

    const QColor borderColor(0,255,0,255);

    const QColor backgroundColor(255,255,255);
    const QColor visitedCellColor(0,255,0,70);
    const QColor occupiedCellColor(255,0,0,70);
    const QColor roboCellColor(0,0,0,155);

    int maze_pic_w = RLRoboMaze::getSize().w*cellWidth;
    int maze_pic_h = RLRoboMaze::getSize().h*cellHeight;

    QPixmap maze_pic(maze_pic_w,maze_pic_h);
    maze_pic.fill(backgroundColor);

    QPainter painter(&maze_pic);

    // Draw cell content
    painter.setPen(Qt::transparent);

    for(int x = 0; x < RLRoboMaze::getSize().w; x++) {
        for(int y = 0; y < RLRoboMaze::getSize().h; y++) {
            RLRoboMaze::CellState state = RLRoboMaze::getCellState(x,y);

            if(state == RLRoboMaze::OOR)
                throw "Ebat' ti loh";
            else if(state == RLRoboMaze::Visited) {
                painter.setBrush(QBrush(visitedCellColor));
                painter.drawRect(x*cellWidth,y*cellHeight,cellWidth,cellHeight);
            } else if(state == RLRoboMaze::Occupied) {
                painter.setBrush(QBrush(occupiedCellColor));
                painter.drawRect(x*cellWidth,y*cellHeight,cellWidth,cellHeight);
            }
        }
    }

    // Draw robot end position
    RLRoboMaze::RoboPosition robo_pos = RLRoboMaze::getRoboPosition();
    painter.setBrush(QBrush(roboCellColor));
    //if(RLRoboMaze::getCellState(robo_pos.x,robo_pos.y) == RLRoboMaze)
        painter.drawRect(robo_pos.x*cellWidth,robo_pos.y*cellHeight,cellWidth,cellHeight);

    // Draw cell lines
    painter.setPen(QPen(QBrush(borderColor),lineThick));

    for(int x = cellWidth; x < maze_pic_w; x += cellWidth)
        painter.drawLine(x,0,x,maze_pic_h-1);
    for(int y = cellWidth; y < maze_pic_h; y += cellHeight)
        painter.drawLine(0,y,maze_pic_w-1,y);

    painter.end();

    //ui->mazeScheme->setPixmap(maze_pic);
    ui->mazeScheme->setPixmap(maze_pic.scaled(ui->mazeScheme->size()));
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
