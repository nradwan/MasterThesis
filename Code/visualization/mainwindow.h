#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <string.h>

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    bool loadFile();
    void start();
    void updateUrl(std::string appended_point);
    void updateGui(std::string image_name);

    QLabel *imageLabel;
    std::string url_string;

    //char* appended_point;

private slots:
    void normalSize();
    void fitToWindow();
    void slot_netwManagerFinished(QNetworkReply *reply);

signals:
    void finishedDownload();

private:
    void createActions();
    void createMenus();
    void updateActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    void downloadImage();


    QScrollArea *scrollArea;
    double scaleFactor;

    QPixmap pixmap;

    QAction *exitAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;

    QMenu *fileMenu;
    QMenu *viewMenu;

    QNetworkAccessManager *m_netwManager;
    //char* initial_string = "http://maps.googleapis.com/maps/api/staticmap?center=47.37366,8.5370498&zoom=15&size=400x400";
    std::string initial_string;
    std::string marker_string;
    std::string path_string;
};


#endif // MAINWINDOW_H
