#include "signup.h"
#include "ui_signup.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QDebug>

signup::signup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::signup)
{
    ui->setupUi(this);
}

signup::~signup()
{
    delete ui;
}

void signup::on_pushButton_clicked()
{
    m_accessmanager = new QNetworkAccessManager();
    reply = m_accessmanager->get(QNetworkRequest(QUrl("https://qtfirebase-18b69-default-rtdb.firebaseio.com/users.json")));
    connect(reply, &QNetworkReply::readyRead, this, &signup::replyReady);
}

void signup::replyReady(){
    QString response = reply->readAll();
    QString name = ui->lineEdit->text();
    QString email = ui->lineEdit_2->text();
    QString password = ui->lineEdit_3->text();
    QString cpassword = ui->lineEdit_4->text();
    if(name.isEmpty() || email.isEmpty() || password.isEmpty() || cpassword.isEmpty()){
        QMessageBox::information(this, "Error", "Please fill all the fields", 0);
        return;
    }
    else if(!email.contains("@") || email.size() <= 5){
        QMessageBox::information(this, "Error", "Enter correct email address", 0);
        return;
    }
    else if(password != cpassword){
        QMessageBox::information(this,"Error", "Password doesn't match.",0);
        return;
    }
    else if(response.contains(email)){
        QMessageBox::information(this, "Error", "Account with this email exists already.", 0);
        return;
    }
    else{

        QVariantMap map;
        map["name"] = name;
        map["password"] = password;
        map["email"] = email;
        QJsonDocument jsonDoc = QJsonDocument::fromVariant(map);
        QNetworkRequest request(QUrl("https://qtfirebase-18b69-default-rtdb.firebaseio.com/users.json"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
        m_accessmanager->post(request, jsonDoc.toJson());
        QMessageBox::information(this, "Success", "Account created successfully", 0);
        this->close();
    }
}

