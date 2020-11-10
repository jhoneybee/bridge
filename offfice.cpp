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
    mainWindow->debug("Quit.");
    if (!isUpload) {
        isUpload = true;
        mainWindow->debug("Quit.");
        mainWindow->debug("Upload. file: ["+ officeStruct->filename+"], url: ["+officeStruct->saveUrl+"]");
        httpClient->upload(QDir::tempPath() + '/' + officeStruct->filename, officeStruct->saveUrl, officeStruct->filename);
    }
}

void Office::downloadDone(){
    QString path = QDir::tempPath() + '/' + officeStruct->filename;
    if (officeStruct->type == WORD) {
        QAxObject *documents = word->querySubObject(getWork());
        documents->dynamicCall("Open(const QString&)", path);
    }else {
        boxLayout = new QVBoxLayout();
        dialog = new QDialog();
        connect(dialog, SIGNAL(rejected()), this, SLOT(Quit()));
        dialog->setLayout(boxLayout);
        word = new QAxWidget();
        word->setControl(QString::fromUtf8("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
        boxLayout->addWidget(word);
        word->dynamicCall("Navigate(const QString&)", path);
        dialog->showMaximized();
    }
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
    if (officeStruct->type == WORD) {
        word = new QAxWidget();
        word = new QAxWidget(getTypeName());
        connect(word, SIGNAL(Quit()), this, SLOT(Quit()));
        word->update();
        word->setProperty("Visible", true);
    }
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
