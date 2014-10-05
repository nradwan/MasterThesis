#include "mainwindow.h"
#include <QtWidgets>


MainWindow::MainWindow()
{
    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    setCentralWidget(scrollArea);

    createActions();
    createMenus();

    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);

    m_netwManager = new QNetworkAccessManager(this);
    connect(m_netwManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_netwManagerFinished(QNetworkReply*)));

    initial_string = "http://maps.googleapis.com/maps/api/staticmap?center=47.37366,8.5370498&zoom=15&size=400x400";
    marker_string = "";
    path_string = "";
    url_string  = "http://maps.googleapis.com/maps/api/staticmap?center=47.37366,8.5370498&zoom=15&size=400x400";

    downloadImage();
}


void MainWindow::slot_netwManagerFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Error in" << reply->url() << ":" << reply->errorString();
        reply->deleteLater();
        return;
    }
    QVariant redir = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (redir.isValid()) {
        QUrl url = redir.toUrl();
        qDebug() << "must go to:" << url;
        if (url.isRelative()) {
            url.setScheme(reply->url().scheme());
            //url.setEncodedHost(reply->url().encodedHost());
        }
        QNetworkRequest req(url);
        m_netwManager->get(req);
        reply->deleteLater();
        return;
    }
    qDebug() << "ContentType:" << reply->header(QNetworkRequest::ContentTypeHeader).toString();

    std::string format = reply->header(QNetworkRequest::ContentTypeHeader).toString().toStdString();
    int index = format.find('/');
    format = format.substr(index+1);
    boost::to_upper(format);
    QByteArray jpegData = reply->readAll();

    pixmap.loadFromData(jpegData, format.c_str());
    loadFile();
    reply->deleteLater();
    emit finishedDownload();
}

void MainWindow::downloadImage()
{
    QUrl url(url_string.c_str());
    qDebug() << "Starting to download" << url;
    QNetworkRequest request(url);
    m_netwManager->get(request);
}

bool MainWindow::loadFile()
{
    imageLabel->setPixmap(pixmap);
    scaleFactor = 1.0;

    fitToWindowAct->setEnabled(true);
    updateActions();

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();

    return true;
}

void MainWindow::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void MainWindow::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}

void MainWindow::createActions()
{
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    normalSizeAct = new QAction(tr("&Normal Size"), this);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);
    connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

    fitToWindowAct = new QAction(tr("&Fit to Window"), this);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));
    connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

}

void MainWindow::createMenus()
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(exitAct);

    viewMenu = new QMenu(tr("&View"), this);
    viewMenu->addAction(normalSizeAct);
    viewMenu->addSeparator();
    viewMenu->addAction(fitToWindowAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(viewMenu);
}

void MainWindow::updateActions()
{
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void MainWindow::scaleImage(double factor)
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

void MainWindow::updateGui(std::string appended_point)
{
    if(marker_string.empty()){
        marker_string = "markers=";
    }
    else{
        marker_string += "&markers=";
    }
    marker_string += appended_point;


    if(path_string.empty()){
        path_string = "path=";
    }
    else{
        path_string += "|";
    }
    path_string += appended_point;

    url_string = initial_string + "&" + marker_string +
                 "&" + path_string;


    downloadImage();
}
