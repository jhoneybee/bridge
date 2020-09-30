#ifndef WORD_H
#define WORD_H

#include <QObject>
#include <QAxWidget>
#include <QAxObject>
#include <QDialog>
#include <QVBoxLayout>

class Word;

#include "mainwindow.h"


class Word : public QObject{
    Q_OBJECT
public:
    Word(MainWindow *mainWindow);
    ~Word();

    /**
     * @brief editor 编辑Word
     * @param url 要编辑url的地址
     */
    void editor(QString target, QString saveUrl);

    /**
     * @brief preview 预览office
     * @param url 要预览的office地址
     * @param saveUrl 保存到服务器的URL地址
     */
    void preview(QString target);

    /**
     * @brief print 将Word打印
     * @param target word的预览地址
     */
    void print(QString target);

private:
    MainWindow *mainWindow;
    QAxWidget *word;
    QDialog *dialog;
};

#endif // WORD_H
