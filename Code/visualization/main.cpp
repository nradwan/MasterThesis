#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(MainWindow::tr("Image Viewer"));
    QCommandLineParser commandLineParser;
    commandLineParser.addHelpOption();
    commandLineParser.addPositionalArgument(MainWindow::tr("[file]"), MainWindow::tr("Image file to open."));
    commandLineParser.process(QCoreApplication::arguments());
    MainWindow imageViewer;
    /*if (!commandLineParser.positionalArguments().isEmpty()
        && !imageViewer.loadFile(commandLineParser.positionalArguments().front())) {
        return -1;
    }*/

    imageViewer.show();
    //QObject::connect(imageViewer, imageViewer.finishedDownload(), imageViewer.imageLabel);
    imageViewer.updateGui("40.737102,-73.990318");
    //sleep(15);
    app.processEvents();

    imageViewer.updateGui("40.749825,-73.987963");
    //sleep(15);
    app.processEvents();

    imageViewer.updateGui("40.752946,-73.987384");
    //sleep(15);
    app.processEvents();

    imageViewer.updateGui("40.755823,-73.986397");
    sleep(5);
    app.processEvents();

    return app.exec();
}
