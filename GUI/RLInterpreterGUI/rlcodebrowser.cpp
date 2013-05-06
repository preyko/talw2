#include "rlcodebrowser.h"
#include "ui_rlcodebrowser.h"

#include <QDebug>

RLCodeBrowser::RLCodeBrowser(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::RLCodeBrowser) {
    ui->setupUi(this);

    ui->lineViewer->installEventFilter(this);

    //connect(ui->scroll,SIGNAL(valueChanged(int)),ui->codeBrowser->verticalScrollBar(),SLOT(setValue(int)));
    //connect(ui->scroll,SIGNAL(valueChanged(int)),ui->lineViewer->verticalScrollBar(),SLOT(setValue(int)));
    connect(ui->codeBrowser->verticalScrollBar(),SIGNAL(rangeChanged(int,int)),SLOT(rangeChanged(int,int)));
    connect(ui->codeBrowser,SIGNAL(textChanged()),SLOT(lineNumberChanged()));

    connect(ui->scroll,SIGNAL(sliderPressed()),SLOT(scrollBarActivate()));
    connect(ui->scroll,SIGNAL(sliderReleased()),SLOT(scrollBarDesctivate()));

    connect(ui->scroll,SIGNAL(valueChanged(int)),SLOT(scrollBySlider(int)));
    connect(ui->codeBrowser->verticalScrollBar(),SIGNAL(valueChanged(int)),SLOT(scrollByCb(int)));

    rangeChanged(ui->codeBrowser->verticalScrollBar()->minimum(),ui->codeBrowser->verticalScrollBar()->maximum());

    scrollBarActive_ = false;

    defaultFormat_ = ui->codeBrowser->textCursor().blockFormat();
}

RLCodeBrowser::~RLCodeBrowser() {
    delete ui;
}

void RLCodeBrowser::readContent(QFile& file) {
    file.open(QIODevice::ReadOnly);
    ui->codeBrowser->setPlainText(file.readAll());
    file.close();
}

void RLCodeBrowser::writeContent(QFile &file) {
    file.open(QIODevice::WriteOnly);
    file.write(ui->codeBrowser->toPlainText().toAscii());
    file.close();
}

void RLCodeBrowser::clear() {
    ui->codeBrowser->clear();
}

bool RLCodeBrowser::eventFilter(QObject* target, QEvent* ev) {
    return false;
}

void RLCodeBrowser::unselectAll() {
    for(int i=0; i<selectedPositions_.size(); i++) {
        ui->codeBrowser->setTextCursor(selectedPositions_[i]);

        QTextCursor cursor = ui->codeBrowser->textCursor();

        QTextBlockFormat new_format;
        new_format.setBackground(QBrush(Qt::white));
        cursor.setBlockFormat(new_format);
    }
}

void RLCodeBrowser::selectLine(int line, QColor color) {
    QTextCursor cursor = ui->codeBrowser->textCursor();

    QTextBlockFormat new_format; color.setAlpha(50);
    new_format.setBackground(QBrush(color));

    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor,line);
    cursor.setBlockFormat(new_format);

    ui->codeBrowser->setTextCursor(cursor);

    selectedPositions_.push_back(cursor);
}

void RLCodeBrowser::rangeChanged(int min,int max) {
    ui->scroll->setRange(min,max);
    ui->lineViewer->verticalScrollBar()->setRange(min,max);
}

void RLCodeBrowser::lineNumberChanged() {
    int codeLines = ui->codeBrowser->toPlainText().count(QChar('\n'))+1;

    ui->lineViewer->clear();
    for(int i=1;i<=codeLines;i++) {
        ui->lineViewer->append(QString::number(i));
    }
}

void RLCodeBrowser::scrollBarActivate() {
    scrollBarActive_ = true;
}

void RLCodeBrowser::scrollBarDesctivate() {
    scrollBarActive_ = false;
}

void RLCodeBrowser::scrollBySlider(int val) {
    //qDebug() << "By slider:" << val;

    if(!scrollBarActive_) return;

    ui->lineViewer->verticalScrollBar()->setValue(val);
    ui->codeBrowser->verticalScrollBar()->setValue(val);
}

void RLCodeBrowser::scrollByCb(int val) {
    //qDebug() << "By code browser:" << val;

    if(scrollBarActive_) return;

    ui->lineViewer->verticalScrollBar()->setValue(val);
    ui->scroll->setValue(val);
}
