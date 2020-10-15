#include "httpclient.h"

HttpClient::HttpClient(MainWindow *mainWindow){
    this->avatorManager = new QNetworkAccessManager(mainWindow);
    this->mainWindow = mainWindow;
}

HttpClient::~HttpClient(){
    this->avatorManager->deleteLater();
}

void HttpClient::download(QString url, QString filename) {
    // 构建及发送请求
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    QNetworkReply *pReply = manager->get(request);
    QEventLoop eventLoop;
    QObject::connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    QByteArray bytes = pReply->readAll();
    QString filePath = QDir::tempPath() + '/' + filename;
    mainWindow->debug("Write: " + filePath);
    tempFile = new QFile(filePath);
     if (tempFile->exists()) {
         tempFile->remove();
     }
     tempFile->open(QIODevice::ReadWrite);
     tempFile->write(bytes);
     tempFile->flush();
     tempFile->close();
     downloadDone();
}

void HttpClient::upload(QString targetUrl, QString filename) {
    mainWindow->debug("Upload file start. targetUrl = ["+targetUrl+"], filename = ["+filename+"]");
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
