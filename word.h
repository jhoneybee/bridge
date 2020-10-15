#ifndef WORD_H
#define WORD_H

#include <QObject>
#include <QAxWidget>
#include <QAxObject>
#include <QDialog>
#include <QVBoxLayout>

class Word;

#include "mainwindow.h"
#include "httpclient.h"

struct WordStruct {
    QString target;
    QString saveUrl;
    QString filename;
    QString id;
    QWebSocket *client;
};

class Word : public QObject{
    Q_OBJECT
public:
    Word(MainWindow *mainWindow, WordStruct *wordStruct);
    ~Word();

    /**
     * @brief editor 编辑Word
     * @param url 要编辑url的地址
     */
    void editor();

    /**
     * @brief preview 预览office
     * @param url 要预览的office地址
     * @param saveUrl 保存到服务器的URL地址
     */
    void preview();

    /**
     * @brief print 将Word打印
     * @param target word的预览地址
     */
    void print();

private:
    MainWindow *mainWindow;
    HttpClient *httpClient;
    WordStruct *wordStruct;
    QAxWidget *word;
    bool isUpload = false;
public slots:
    void uploadDone();
    void downloadDone();
    void Quit();
};

#endif // WORD_H
