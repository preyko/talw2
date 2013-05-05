#include <QDebug>
#include <QtGui/QApplication>

#include <iostream>

#include "mainwindow.h"


class RLApplication : public QApplication {
public:
    RLApplication(int& argc, char ** argv) :
        QApplication(argc, argv) { }
    virtual ~RLApplication() { }

    virtual bool notify(QObject * receiver, QEvent * event) {
        try {
            return QApplication::notify(receiver, event);
        } catch(std::exception& e) {
            qCritical() << "Exception thrown:" << e.what();
        } catch(const char* e) {
            qCritical() << "Exception thrown:" << e;
        }

        return false;
    }
};

int main(int argc, char *argv[]) {
    RLApplication a(argc, argv);
    a.setApplicationName("RL Interpreter");
    MainWindow w;
    w.show();
    
    return a.exec();
}
