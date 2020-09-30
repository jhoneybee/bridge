#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <QObject>
#include <QtWebSockets>
#include <QWebSocketServer>
class WebSocketServer;

#include "mainwindow.h"
#include "word.h"


class WebSocketServer : public QObject{
    Q_OBJECT
public:
    WebSocketServer(MainWindow *mainWindow);
    ~WebSocketServer();
    void listen(quint16 port = 29580);
private:
    QWebSocketServer *webSocketServer;
    Word *word;
    MainWindow *mainWindow;
    QList<QWebSocket*> clients;
public slots:
    // 启动一个WebSocket连接
    void onNewConnection();
    void onDisconnected();
    void onTextMessageReceived(QString message);
    // 用户关闭Socket
    void onClosed();
    // 获得当前socket的错误消息
    void onAcceptError(QAbstractSocket::SocketError socketError);
    // 服务器错误信息
    void onServerError(QWebSocketProtocol::CloseCode closeCode);
};

#endif // WEBSOCKET_H
