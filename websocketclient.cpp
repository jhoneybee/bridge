#include "websocketclient.h"

WebSocketClient::WebSocketClient(QWebSocket *client, WebSocketServer *server) {
    this->client = client;
    this->server = server;
}

WebSocketClient::~WebSocketClient() {
}

// 获取消息,然后进行处理
void WebSocketClient::onTextMessageReceived(QString message) {
    QJsonParseError err_rpt;
    QJsonDocument document = QJsonDocument::fromJson(message.toUtf8(), &err_rpt);

    // 如果JSON格式化错误,则不执行，记录对应的日志信息
    if (err_rpt.error != QJsonParseError::NoError) {
        return;
    }

    // 获取对应的QJsonObject 对象
    QJsonObject jsonObject = document.object();
    QJsonArray params = jsonObject.value("params").toArray();
    QString channel = "TEXT_MESSAGE_RECEIVED";
    Command command;
    command.params = &params;
    command.self = this->client;
    command.channel = &channel;
    command.server = this->server;
    this->receivedCommand(&command);
}
