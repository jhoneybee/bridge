#include "httpclient.h"

HttpClient::HttpClient(MainWindow *mainWindow){
    this->avatorManager = new QNetworkAccessManager(mainWindow);
    this->mainWindow = mainWindow;
}

HttpClient::~HttpClient(){
    this->avatorManager->deleteLater();
}

void HttpClient::download(QString url, QString filename) {
    networkReply = avatorManager->get(QNetworkRequest(QUrl(url)));
    tempFile = new QFile(QDir::tempPath() + '/' + filename);
     if (tempFile->exists()) {
         tempFile->remove();
     }
     tempFile->open(QIODevice::ReadWrite);
    connect(networkReply,&QNetworkReply::readyRead,this, &HttpClient::readyRead);
    connect(networkReply,&QNetworkReply::finished,this, &HttpClient::finished);
}

void HttpClient::readyRead(){
    tempFile->write(networkReply->readAll());
}

void HttpClient::finished(){
    tempFile->flush();
    tempFile->close();
    tempFile->deleteLater();
    downloadDone();
}

void HttpClient::upload(QString targetUrl, QString filename) {
    mainWindow->debug("upload file start. targetUrl = ["+targetUrl+"], filename = ["+filename+"]");
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart filePart;
    QFile *file = new QFile(QDir::tempPath() + '/' + filename);
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/vnd.openxmlformats-officedocument.wordprocessingml.document"));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\""+ filename+"\""));
    if (!file->isOpen()) {
      file->open(QIODevice::ReadOnly);
    }
    filePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(filePart);
    QUrl url(targetUrl);
    QNetworkRequest request(targetUrl);
    QNetworkReply* networkReply = avatorManager->post(request, multiPart);
    multiPart->setParent(networkReply);
    connect(networkReply, SIGNAL(finished()), this, SLOT(uploadDone()));
}
