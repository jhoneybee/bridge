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
signals:
    void downloadDone();
    void uploadDone();
public slots:
    void finished();
};

#endif // HTTPCLIENT_H
