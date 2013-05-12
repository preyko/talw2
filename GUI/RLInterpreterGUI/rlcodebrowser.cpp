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
    connect(ui->codeBrowser,SIGNAL(textChanged()),SLOT(lineNumberChanged()));

    connect(ui->codeBrowser->verticalScrollBar(),SIGNAL(valueChanged(int)),
            ui->lineViewer->verticalScrollBar(),SLOT(setValue(int)));

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
    cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor,line-1);
    cursor.setBlockFormat(new_format);

    ui->codeBrowser->setTextCursor(cursor);

    selectedPositions_.push_back(cursor);
}

void RLCodeBrowser::lineNumberChanged() {
    int res = ui->codeBrowser->toPlainText().count(QChar('\n'))+1;

    if(currentLineCount_ != res) {
        currentLineCount_ = res;

        ui->lineViewer->clear();
        for(int i=1;i<=currentLineCount_;i++) {
            ui->lineViewer->append(QString::number(i));
        }
    }
}
