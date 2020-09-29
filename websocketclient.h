#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QtWebSockets>
#include <QJsonDocument>
#include <QSystemTrayIcon>

class WebSocketClient;
struct Command;

#include "websocketserver.h"

class WebSocketClient : public QObject{
    Q_OBJECT
public:
    WebSocketClient(QWebSocket *client, WebSocketServer *server);
    ~WebSocketClient();
private:
    // 客户端
    QWebSocket *client;
    WebSocketServer *server;
public slots:
    // 接受到的消息信息
    void onTextMessageReceived(QString message);
signals:
    // 提供的事件注册
    void receivedCommand(Command *command);
};

#endif // WEBSOCKETCLIENT_H
