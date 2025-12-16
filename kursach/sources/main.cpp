#include <QApplication>
#include "headers/MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    app.setStyleSheet("QMainWindow { background-color: #ecf0f1; }");

    MainWindow window;
    window.show();

    return app.exec();
}