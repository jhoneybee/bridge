#include "websocketserver.h"

WebSocketServer::WebSocketServer(MainWindow *mainWindow) {
    this->mainWindow = mainWindow;
    this->webSocketServer = new QWebSocketServer(QStringLiteral("bridge"), QWebSocketServer::NonSecureMode);
    this->mainWindow = mainWindow;
    this->word = new Word(mainWindow);
}

WebSocketServer::~WebSocketServer() {
    // 清除内存数据
    this->clients.clear();
    qDeleteAll(this->clients);
}

void WebSocketServer::listen(quint16 port){
    this->webSocketServer->listen(QHostAddress::Any,port);

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
    mainWindow->debug("Text message received : " + message);

    QJsonParseError json_error;
    QJsonDocument document = QJsonDocument::fromJson(message.toUtf8(), &json_error);

    // 如果JSON格式错误，则输出对应的错误信息
    if (json_error.error != QJsonParseError::NoError) {
        mainWindow->debug("JSON format is incorrect : " + message);
        return;
    }
    QJsonObject jsonObject = document.object();

    // 订阅的频道信息
    QString channel = jsonObject.value("channel").toString();
    // 参数信息
    QJsonObject params = jsonObject.value("params").toObject();

    if (channel == "OFFICE_OPEN_WORD_EDITOR") {
        // 打开 OFFICE 的 Word 功能
        // 设置文件目标地址,以及保存地址,编辑完文件后自动保存上传到服务器。
        word->editor(params.value("target").toString(), params.value("saveUrl").toString());
        return;
    }else if (channel == "OFFICE_OPEN_WORD_PREVIEW") {
        // 预览Word的文件
        word->preview(params.value("target").toString());
        return;
    }else if (channel == "OFFICE_OPEN_WORD_PRINT") {
        // 打印 Word
        word->print(params.value("target").toString());
        return;
    }
}
