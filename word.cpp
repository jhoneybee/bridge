#include "word.h"

Word::Word(MainWindow *mainWindow) {
    this->mainWindow = mainWindow;

}

Word::~Word(){
    word->clear();
    delete word;
}

void Word::editor(QString target, QString saveUrl) {
}

void Word::preview(QString target) {
    if (dialog) {
        delete(dialog);
    }

    dialog = new QDialog(mainWindow);
    QVBoxLayout *layout = new QVBoxLayout;
    dialog->setLayout(layout);
    dialog->showMaximized();

    this->word = new QAxWidget(QString::fromUtf8("Word.Application"), dialog);
    this->word->dynamicCall("Visible", false);
    QAxObject *documents = word->querySubObject("Documents");
    documents->dynamicCall("Open(const QString&)",target);
}

void Word::print(QString target) {
    if (dialog) {
        delete(dialog);
    }

    dialog = new QDialog(mainWindow);
    QVBoxLayout *layout = new QVBoxLayout;
    dialog->setLayout(layout);
    dialog->showMaximized();
    this->word = new QAxWidget(QString::fromUtf8("Word.Application"), dialog);
    this->word->dynamicCall("Visible", false);
    QAxObject *documents = word->querySubObject("Documents");
    documents->dynamicCall("Open(const QString&)",target);
    documents->dynamicCall("PrintOut()");
}
