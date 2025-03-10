#ifndef SIGNUP_H
#define SIGNUP_H

#include "QtNetwork/qnetworkaccessmanager.h"
#include <QDialog>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

namespace Ui {
class signup;
}

class signup : public QDialog
{
    Q_OBJECT

public:
    explicit signup(QWidget *parent = nullptr);
    ~signup();
public slots:
    void replyReady();

private slots:
    void on_pushButton_clicked();

private:
    Ui::signup *ui;
    QNetworkAccessManager *m_accessmanager;
    QNetworkReply *reply;
};

#endif // SIGNUP_H
