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


QString Office::getTypeName(){
    if (this->officeStruct->type == WORD) {
        return "Word.Application";
    }

    if (this->officeStruct->type == EXCEL) {
        return "Excel.Application";
    }

    if (this->officeStruct->type == PPT) {
        return "PowerPoint.Application";
    }
    return "";
}

const char* Office::getWork(){
    if (this->officeStruct->type == WORD) {
        return "Documents";
    }

    if (this->officeStruct->type == EXCEL) {
        return "Workbooks";
    }

    if (this->officeStruct->type == PPT) {
        return "Presentations";
    }
    return "";
}

void Office::Quit() {
    if (!isUpload) {
        isUpload = true;
        mainWindow->debug("Quit.");
        mainWindow->debug("Upload. file: ["+ officeStruct->filename+"], url: ["+officeStruct->saveUrl+"]");
        httpClient->upload(QDir::tempPath() + '/' + officeStruct->filename, officeStruct->saveUrl, officeStruct->filename);
    }
}

void Office::downloadDone(){
    QAxObject *documents = word->querySubObject(getWork());
    documents->dynamicCall("Open(const QString&)", QDir::tempPath() + '/' + officeStruct->filename);
}

void Office::uploadDone() {
    QJsonObject sendJson;
    sendJson["id"] = officeStruct->id;
    sendJson["status"] = 200;
    sendJson["message"] = "文件保存成功";
    QJsonDocument doc(sendJson);
    officeStruct->client->sendTextMessage(doc.toJson(QJsonDocument::Compact));
    officeStruct->client->flush();
}

void Office::editor() {
    isUpload = false;
    word = new QAxWidget(getTypeName());
    connect(word, SIGNAL(Quit()), this, SLOT(Quit()));
    word->update();
    word->setProperty("Visible", true);
    // 下载文件到临时目录
    httpClient->download(officeStruct->target, officeStruct->filename);
}

void Office::preview() {
    word = new QAxWidget(getTypeName());
    word->setProperty("Visible", true);
    QAxObject *documents = word->querySubObject(getWork());
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
    word = new QAxWidget(getTypeName());
    word->setProperty("Visible", false);
    QAxObject *documents = word->querySubObject(getWork());
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
