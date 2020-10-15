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
    void upload(QString filePath, QString saveUrl, QString filename);
private:
    MainWindow *mainWindow;
signals:
    void downloadDone();
    void uploadDone();
};

#endif // HTTPCLIENT_H
