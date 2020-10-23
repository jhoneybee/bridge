#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

class MainWindow;

#include "websocketserver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// 当前配置信息的数据结构
struct Config {
    QString port = "29580";
    QString webSocket = "ws://127.0.0.1:29580";
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setConfig(Config *config);
    // 发送对应的消息
    void debug(QString info);
    // 获取下载文件的临时目录
    QString getTempDownload();
    QSystemTrayIcon* trayIconPtr;

private:
    Config *config;
    Ui::MainWindow *ui;
    WebSocketServer *webSocketServer;

public slots:
    // 提供的事件注册
    void activated(QSystemTrayIcon::ActivationReason reason);
};
#endif // MAINWINDOW_H
