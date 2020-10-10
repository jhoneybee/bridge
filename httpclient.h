#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QHttpMultiPart>
#include <QFile>
#include <QDir>
#include <QNetworkAccessManager>

class HttpClient;

#include "mainwindow.h"

class HttpClient : public QObject
{
    Q_OBJECT
public:
     HttpClient(MainWindow *parent);
     ~HttpClient();
    void download(QString url, QString filename);
    void upload(QString targetUrl, QString filename);
private:
    QNetworkReply* networkReply;
    QNetworkAccessManager *avatorManager;
    QFile* tempFile;
    MainWindow *mainWindow;
public slots:
    void readyRead();
    void finished();
signals:
    void downloadDone();
    void uploadDone();
};

#endif // HTTPCLIENT_H
