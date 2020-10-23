#include "websocketserver.h"

OfficeStruct officeStruct;

WebSocketServer::WebSocketServer(MainWindow *mainWindow) {
    this->mainWindow = mainWindow;
    this->webSocketServer = new QWebSocketServer(QStringLiteral("bridge"), QWebSocketServer::NonSecureMode);
    this->mainWindow = mainWindow;
    office = new Office(mainWindow, &officeStruct);
}

WebSocketServer::~WebSocketServer() {
    // 清除内存数据
    this->clients.clear();
    qDeleteAll(this->clients);
    office->deleteLater();
}

void WebSocketServer::listen(quint16 port){
    this->webSocketServer->listen(QHostAddress::Any,port);
    // 如果存在，则退出程序，防止多开
    if (1006 == this->webSocketServer->error()) {
        mainWindow->trayIconPtr->hide();
        exit(EXIT_FAILURE);
        return;
    }
    // 初始化事件信息
    connect(webSocketServer, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
    connect(webSocketServer, &QWebSocketServer::closed, this, &WebSocketServer::onClosed);
    connect(webSocketServer, &QWebSocketServer::acceptError, this, &WebSocketServer::onAcceptError);
    connect(webSocketServer, &QWebSocketServer::serverError, this, &WebSocketServer::onServerError);
}

void WebSocketServer::onNewConnection() {
    QWebSocket *socket = webSocketServer->nextPendingConnection();
    clients << socket;
    // 注册初始化事件
    connect(socket, &QWebSocket::textMessageReceived, this, &WebSocketServer::onTextMessageReceived);
    connect(socket, &QWebSocket::disconnected, this, &WebSocketServer::onDisconnected);
    mainWindow->debug("Connection [" + socket->peerAddress().toString() + "]");
}

void WebSocketServer::onClosed() {
    mainWindow->debug("WebSocket server closed.");
}

void WebSocketServer::onAcceptError(QAbstractSocket::SocketError socketError) {
    mainWindow->debug("WebSocket server accept error." + QString(socketError));
}

void WebSocketServer::onServerError(QWebSocketProtocol::CloseCode closeCode) {
    mainWindow->debug("WebSocket server error." + QString(closeCode));
}

void WebSocketServer::onDisconnected(){
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient){
        clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

void WebSocketServer::onTextMessageReceived(QString message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    mainWindow->debug("Text message received : " + message);

    QJsonParseError json_error;
    QJsonDocument document = QJsonDocument::fromJson(message.toUtf8(), &json_error);

    // 如果JSON格式错误，则输出对应的错误信息
    if (json_error.error != QJsonParseError::NoError) {
        QString errorMessage = "JSON format is incorrect : " + message;
        mainWindow->debug(errorMessage);

        QJsonObject sendJson;
        sendJson["status"] = 200;
        sendJson["message"] = errorMessage;

        QJsonDocument doc(sendJson);
        pClient->sendTextMessage(doc.toJson(QJsonDocument::Compact));
        pClient->flush();
        return;
    }
    QJsonObject jsonObject = document.object();

    // 订阅的频道信息
    QString channel = jsonObject.value("channel").toString();
    // 参数信息
    QJsonObject params = jsonObject.value("params").toObject();

    officeStruct.client = pClient;
    officeStruct.id = jsonObject.value("id").toString();
    officeStruct.saveUrl = params.value("saveUrl").toString();
    officeStruct.target = params.value("target").toString();

    QStringList channelParams = channel.split("_");

    if (channelParams.length() == 4) {
        if (channelParams.at(0) == "OFFICE" && channelParams.at(1) == "OPEN") {
            if (channelParams.at(2) == "WORD") {
                officeStruct.type = WORD;
            }else if (channelParams.at(2) == "EXCEL") {
                officeStruct.type = EXCEL;
            }else if (channelParams.at(2) == "PPT") {
                officeStruct.type = PPT;
            }
            if (channelParams.at(3) == "EDITOR") {
                office->editor();
            }else if (channelParams.at(3) == "PREVIEW") {
                office->preview();
            }else if (channelParams.at(3) == "PRINT") {
                office->print();
            }
        }
    }
}
