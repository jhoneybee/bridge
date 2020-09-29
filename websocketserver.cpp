#include "websocketserver.h"

WebSocketServer::WebSocketServer() {
    this->webSocketServer = new QWebSocketServer(QStringLiteral("bridge"), QWebSocketServer::NonSecureMode);
}

WebSocketServer::~WebSocketServer() {
    // 清除内存数据
    // this->clients.clear();
    // qDeleteAll(this->clients);
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
    // QWebSocket *socket = webSocketServer->nextPendingConnection();
    // WebSocketClient *client = new WebSocketClient(socket, this);
    // this->clients->append(client);

    // 注册初始化事件
    // connect(socket, &QWebSocket::textMessageReceived, client, &WebSocketClient::onTextMessageReceived);
}

void WebSocketServer::onClosed() {
}

void WebSocketServer::onAcceptError(QAbstractSocket::SocketError socketError) {
}

void WebSocketServer::onServerError(QWebSocketProtocol::CloseCode closeCode) {
}
