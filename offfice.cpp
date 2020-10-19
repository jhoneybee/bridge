#include "offfice.h"

Office::Office(MainWindow *mainWindow, OfficeStruct *officeStruct) {
    this->mainWindow = mainWindow;
    this->httpClient = new HttpClient(mainWindow);
    this->officeStruct = officeStruct;
    connect(httpClient, SIGNAL(uploadDone()), this, SLOT(uploadDone()));
    connect(httpClient, SIGNAL(downloadDone()), this, SLOT(downloadDone()));
}

Office::~Office(){
    this->httpClient->deleteLater();
    this->word->deleteLater();
}

void Office::Quit() {
    if (!isUpload) {
        isUpload = true;
        mainWindow->debug("Word Quit.");
        mainWindow->debug("Word Upload. file: ["+ officeStruct->filename+"], url: ["+officeStruct->saveUrl+"]");
        httpClient->upload(QDir::tempPath() + '/' + officeStruct->filename, officeStruct->saveUrl, officeStruct->filename);
    }
}

void Office::downloadDone(){
    QAxObject *documents = word->querySubObject("Documents");
    documents->dynamicCall("Open(const QString&)", QDir::tempPath() + '/' + officeStruct->filename);
}

void Office::uploadDone() {
    QJsonObject sendJson;
    sendJson["id"] = officeStruct->id;
    sendJson["status"] = 200;
    sendJson["message"] = "Word文件保存成功";
    QJsonDocument doc(sendJson);
    officeStruct->client->sendTextMessage(doc.toJson(QJsonDocument::Compact));
    officeStruct->client->flush();
}

void Office::editor() {
    isUpload = false;
    word = new QAxWidget("Word.Application");
    connect(word, SIGNAL(Quit()), this, SLOT(Quit()));
    word->update();
    word->setProperty("Visible", true);
    // 下载文件到临时目录
    httpClient->download(officeStruct->target, officeStruct->filename);
}

void Office::preview() {
    word = new QAxWidget("Word.Application");
    word->setProperty("Visible", true);
    QAxObject *documents = word->querySubObject("Documents");
    documents->dynamicCall("Open(const QString&)", officeStruct->target);
    QJsonObject sendJson;
    sendJson["id"] = officeStruct->id;
    sendJson["status"] = 200;
    sendJson["message"] = "预览数据成功";
    QJsonDocument doc(sendJson);
    officeStruct->client->sendTextMessage(doc.toJson(QJsonDocument::Compact));
    officeStruct->client->flush();
    word->deleteLater();
}

void Office::print() {
    word = new QAxWidget("Word.Application");
    word->setProperty("Visible", false);
    QAxObject *documents = word->querySubObject("Documents");
    documents->dynamicCall("Open(const QString&)", officeStruct->target);
    word->dynamicCall("PrintOut()");

    QJsonObject sendJson;
    sendJson["id"] = officeStruct->id;
    sendJson["status"] = 200;
    sendJson["message"] = "打印数据成功";
    QJsonDocument doc(sendJson);
    officeStruct->client->sendTextMessage(doc.toJson(QJsonDocument::Compact));
    officeStruct->client->flush();
    word->deleteLater();
}
