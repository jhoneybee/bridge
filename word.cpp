#include "word.h"

Word::Word(MainWindow *mainWindow, WordStruct *wordStruct) {
    this->mainWindow = mainWindow;
    this->httpClient = new HttpClient(mainWindow);
    this->wordStruct = wordStruct;
    connect(httpClient, SIGNAL(uploadDone()), this, SLOT(uploadDone()));
    connect(httpClient, SIGNAL(downloadDone()), this, SLOT(downloadDone()));
}

Word::~Word(){
    this->httpClient->deleteLater();
    this->word->deleteLater();
}

void Word::Quit() {
    if (!isUpload) {
        isUpload = true;
        mainWindow->debug("Word Quit.");
        mainWindow->debug("Word Upload. file: ["+ wordStruct->filename+"], url: ["+wordStruct->saveUrl+"]");
        httpClient->upload(QDir::tempPath() + '/' + wordStruct->filename, wordStruct->saveUrl, wordStruct->filename);
    }
}

void Word::downloadDone(){
    QAxObject *documents = word->querySubObject("Documents");
    documents->dynamicCall("Open(const QString&)", QDir::tempPath() + '/' + wordStruct->filename);
}

void Word::uploadDone() {
    QJsonObject sendJson;
    sendJson["id"] = wordStruct->id;
    sendJson["status"] = 200;
    sendJson["message"] = "Word文件保存成功";
    QJsonDocument doc(sendJson);
    wordStruct->client->sendTextMessage(doc.toJson(QJsonDocument::Compact));
    wordStruct->client->flush();
    isUpload = false;
}

void Word::editor() {
    word = new QAxWidget("Word.Application");
    connect(word, SIGNAL(Quit()), this, SLOT(Quit()));
    word->update();
    word->setProperty("Visible", true);
    // 下载文件到临时目录
    httpClient->download(wordStruct->target, wordStruct->filename);
}

void Word::preview() {
    word = new QAxWidget("Word.Application");
    word->setProperty("Visible", true);
    QAxObject *documents = word->querySubObject("Documents");
    documents->dynamicCall("Open(const QString&)", wordStruct->target);
    QJsonObject sendJson;
    sendJson["id"] = wordStruct->id;
    sendJson["status"] = 200;
    sendJson["message"] = "预览数据成功";
    QJsonDocument doc(sendJson);
    wordStruct->client->sendTextMessage(doc.toJson(QJsonDocument::Compact));
    wordStruct->client->flush();
    word->deleteLater();
}

void Word::print() {
    word = new QAxWidget("Word.Application");
    word->setProperty("Visible", false);
    QAxObject *documents = word->querySubObject("Documents");
    documents->dynamicCall("Open(const QString&)", wordStruct->target);
    word->dynamicCall("PrintOut()");

    QJsonObject sendJson;
    sendJson["id"] = wordStruct->id;
    sendJson["status"] = 200;
    sendJson["message"] = "打印数据成功";
    QJsonDocument doc(sendJson);
    wordStruct->client->sendTextMessage(doc.toJson(QJsonDocument::Compact));
    wordStruct->client->flush();
    word->deleteLater();
}
