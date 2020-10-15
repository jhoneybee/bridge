#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(),this->height());

    trayIconPtr = new QSystemTrayIcon(parent);
    QPixmap icon = QPixmap("://resources/img/svg/icon.svg");
    trayIconPtr->setIcon(icon);
    trayIconPtr->show();
    connect(trayIconPtr, &QSystemTrayIcon::activated, this, &MainWindow::activated);

    // 初始化数据信息
    this->config = new Config();
    setConfig(config);

    this->webSocketServer = new WebSocketServer(this);
    webSocketServer->listen(config->port.toInt());
    debug("WebSocket Server listen [" + config->port + "]");

}

void MainWindow::setConfig(Config *config) {
    this->ui->fProtValue->setText(config->port);
    this->ui->fWebSocketValue->setText(config->webSocket);
}

void MainWindow::activated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
        this->show();
    }
}

void MainWindow::debug(QString info) {
    if (ui->log->document()->lineCount() > 5000) {
        ui->log->clear();
    }
    ui->log->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") +" : " + info);
}

MainWindow::~MainWindow() {
    delete ui;
    delete config;
}
