#ifndef WORD_H
#define WORD_H

#include <QObject>
#include <QAxWidget>
#include <QAxObject>
#include <QDialog>
#include <QVBoxLayout>


class Office;

#include "mainwindow.h"
#include "httpclient.h"

// office的类型
enum OfficeType {
    WORD, EXCEL, PPT
};

struct OfficeStruct {
    QString target;
    QString saveUrl;
    QString filename;
    QString id;
    OfficeType type;
    QWebSocket *client;
};

class Office : public QObject{
    Q_OBJECT
public:
    Office(MainWindow *mainWindow, OfficeStruct *wordStruct);
    ~Office();

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
    OfficeStruct *officeStruct;
    QAxWidget *word;
    bool isUpload = true;
    QString getTypeName();
    const char* getWork();
    QDialog *dialog;
    QVBoxLayout *boxLayout;
public slots:
    void uploadDone();
    void downloadDone();
    void Quit();
};

#endif // WORD_H
