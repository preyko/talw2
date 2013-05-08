#ifndef RLCODEBROWSER_H
#define RLCODEBROWSER_H

#include <QFrame>

#include <QFile>
#include <QVector>

#include <QColor>
#include <QTextEdit>
#include <QTextBlockFormat>

namespace Ui {
class RLCodeBrowser;
}

class RLCodeBrowser : public QFrame
{
    Q_OBJECT
    
public:
    explicit RLCodeBrowser(QWidget *parent = 0);
    ~RLCodeBrowser();

public:
    void readContent(QFile& file);
    void writeContent(QFile& file);

    void clear();

    bool eventFilter(QObject*,QEvent* ev);

    void unselectAll();
    void selectLine(int line, QColor color = QColor(Qt::red));

public slots:
    void lineNumberChanged();

    void scrollBarActivate();
    void scrollBarDesctivate();

    void scrollBySlider(int val);
    void scrollByCb(int val);

private:
    Ui::RLCodeBrowser *ui;

    int currentLineCount_;
    bool scrollBarActive_;

    QVector<QTextCursor> selectedPositions_;
    QTextBlockFormat defaultFormat_;
};

#endif // RLCODEBROWSER_H
