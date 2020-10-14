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
}

void Word::editor() {
    // 下载文件到临时目录
    httpClient->download(wordStruct->target, wordStruct->filename);
}

void Word::quit() {
    httpClient->upload(QDir::tempPath() + '/' + wordStruct->filename, wordStruct->saveUrl);
}

void Word::downloadDone(){
    QAxWidget *word = new QAxWidget(QString::fromUtf8("Word.Application"), mainWindow, Qt::MSWindowsOwnDC);
    connect(word, SIGNAL(Quit()), this, SLOT(quit()));
    word->setProperty("Visible", true);
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
}

void Word::preview() {
    QAxWidget *word = new QAxWidget(QString::fromUtf8("Word.Application"), mainWindow, Qt::MSWindowsOwnDC);
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
}

void Word::print() {
    QAxWidget *word = new QAxWidget(QString::fromUtf8("Word.Application"), mainWindow, Qt::MSWindowsOwnDC);
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
}
