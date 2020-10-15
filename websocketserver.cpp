#include "websocketserver.h"

WordStruct wordStruct;

WebSocketServer::WebSocketServer(MainWindow *mainWindow) {
    this->mainWindow = mainWindow;
    this->webSocketServer = new QWebSocketServer(QStringLiteral("bridge"), QWebSocketServer::NonSecureMode);
    this->mainWindow = mainWindow;
    word = new Word(mainWindow, &wordStruct);
}

WebSocketServer::~WebSocketServer() {
    // 清除内存数据
    this->clients.clear();
    qDeleteAll(this->clients);
    word->deleteLater();
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

    wordStruct.client = pClient;
    wordStruct.filename = params.value("filename").toString();
    wordStruct.id = jsonObject.value("id").toString();
    wordStruct.saveUrl = params.value("saveUrl").toString();
    wordStruct.target = params.value("target").toString();

    if (channel == "OFFICE_OPEN_WORD_EDITOR") {
        // 打开 OFFICE 的 Word 功能
        // 设置文件目标地址,以及保存地址,编辑完文件后自动保存上传到服务器。
        word->editor();
        return;
    }else if (channel == "OFFICE_OPEN_WORD_PREVIEW") {
        // 预览Word的文件
        word->preview();
        return;
    }else if (channel == "OFFICE_OPEN_WORD_PRINT") {
        // 打印 Word
        word->print();
        return;
    }
}
