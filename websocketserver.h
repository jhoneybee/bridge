#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <QObject>
#include <QtWebSockets>
#include <QWebSocketServer>
class WebSocketServer;

#include "websocketclient.h"

#define C_NEW_CONNECTION 'C_NEW_CONNECTION';
#define C_CLOSED 'C_CLOSED'
#define C_ACCEPT_ERROR 'C_ACCEPT_ERROR'
#define C_SERVER_ERROR 'C_SERVER_ERROR'

class WebSocketServer : public QObject{
    Q_OBJECT
public:
    WebSocketServer();
    ~WebSocketServer();
    void listen(quint16 port = 29580);
private:
    QList<WebSocketClient*> clients;
    QWebSocketServer *webSocketServer;
public slots:
    // 启动一个WebSocket连接
    void onNewConnection();
    // 用户关闭Socket
    void onClosed();
    // 获得当前socket的错误消息
    void onAcceptError(QAbstractSocket::SocketError socketError);
    // 服务器错误信息
    void onServerError(QWebSocketProtocol::CloseCode closeCode);
};

// WebSocket 发送的指令信息
struct Command {
    // 订阅的频道
    QString *channel;
    // 请求的参数信息
    QJsonArray *params;
    // 当前的服务端
    WebSocketServer *server;
    // 当前发送此消息的客户
    QWebSocket *self;
};


#endif // WEBSOCKET_H
