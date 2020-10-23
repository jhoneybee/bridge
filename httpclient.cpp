#include "httpclient.h"

HttpClient::HttpClient(MainWindow *mainWindow){
    this->mainWindow = mainWindow;
}

HttpClient::~HttpClient(){
}

void HttpClient::download(QString url, QString filename) {
    // 构建及发送请求
    QNetworkAccessManager manager;
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    QNetworkReply *pReply = manager.get(request);
    QEventLoop eventLoop;
    connect(&manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    QByteArray bytes = pReply->readAll();
    QString filePath = this->mainWindow->getTempDownload() + '/' + filename;
    mainWindow->debug("Write: " + filePath);
    QFile file(filePath);
     if (file.exists()) {
         file.remove();
     }
     file.open(QIODevice::ReadWrite);
     file.write(bytes);
     file.flush();
     file.close();
     downloadDone();
}

void HttpClient::upload(QString filePath, QString saveUrl, QString filename) {
    mainWindow->debug("Upload file start. filePath = ["+filePath+"], saveUrl = ["+saveUrl+"]");
    QNetworkAccessManager manager;
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/vnd.openxmlformats-officedocument.wordprocessingml.document"));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\""+ filename+"\""));
    QFile file(filePath);
    if (!file.isOpen()) {
      file.open(QIODevice::ReadOnly);
    }
    filePart.setBodyDevice(&file);
    file.setParent(multiPart);
    multiPart->append(filePart);

    QUrl url(saveUrl);
    qDebug() << url.toString();
    QNetworkRequest request(url);
    QNetworkReply *pReply = manager.post(request, multiPart);
    multiPart->setParent(pReply);
    QEventLoop eventLoop;
    connect(&manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    mainWindow->debug("Upload Error: ["+pReply->errorString()+"]");
    mainWindow->debug("Upload file end. "+ QString(pReply->readAll()));
    uploadDone();
}
