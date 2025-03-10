#include "mainwindow.h"
#include "qjsondocument.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <signup.h>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QDebug>
#include <QSettings>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings settings("Firebase", "Qt");
    settings.beginGroup("user");
    if(settings.value("loggedIn").toString() == "true"){
        ui->textBrowser->setHtml("Welcome <b>"+settings.value("name").toString()+" </b>You are already\nlogged in as <b>"+settings.value("email").toString()+"</b>");
        ui->lineEdit->setText("Logged in at "+settings.value("time").toString());
        ui->lineEdit_2->setVisible(false);
        ui->lineEdit_3->setVisible(false);
        ui->pushButton->setText("Logout");
    }
    settings.endGroup();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    if(ui->pushButton->text() == "Logout"){
        //logout code
        QSettings settings("Firebase", "Qt");
        settings.beginGroup("user");
        settings.remove("");
        settings.endGroup();
        ui->textBrowser->setText("Successfully logged out of the application.");
        ui->lineEdit->setText("Logged out at "+QTime::currentTime().toString());
        ui->lineEdit_2->setVisible(true);
        ui->lineEdit_3->setVisible(true);
        ui->pushButton->setText("Login");
        return;
    }
    m_accessmanager = new QNetworkAccessManager();
    reply = m_accessmanager->get(QNetworkRequest(QUrl("https://qtfirebase-18b69-default-rtdb.firebaseio.com/users.json")));
    connect(reply, &QNetworkReply::readyRead, this, &MainWindow::replyReady);

}


void MainWindow::on_pushButton_2_clicked()
{
    signup *s = new signup();
    s->show();
    s->exec();
}

void MainWindow::replyReady(){
    QFile f("data.json");
    f.open(QIODevice::WriteOnly);
    QTextStream out(&f);
    out << reply->readAll();
    f.close();
    QString email = ui->lineEdit_2->text();
    QString pass = ui->lineEdit_3->text();

    QString name = checkCredentials(email, pass);
    if (!name.isEmpty()) {
        ui->textBrowser->setHtml("Welcome <b>"+name+" </b>You are successfully\nlogged in as <b>"+email+"</b>");
        ui->lineEdit->setText("Logged in at "+QTime::currentTime().toString());
        ui->lineEdit_2->setVisible(false);
        ui->lineEdit_3->setVisible(false);
        ui->pushButton->setText("Logout");
        QSettings settings("Firebase", "Qt");
        settings.beginGroup("user");
        settings.setValue("name", name);
        settings.setValue("email", email);
        settings.setValue("time", QTime::currentTime().toString());
        settings.setValue("loggedIn", "true");
        settings.endGroup();
    } else {
        QMessageBox::information(this, "Error", "Invalid email or password!", 0);
        return;
    }

}

QString MainWindow::checkCredentials(QString email, QString password) {
    // Read JSON file
    QFile file("data.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file.";
        return "";
    }
    QByteArray jsonData = file.readAll();
    file.close();
    QFile::remove("data.json");

    // Parse JSON
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isObject()) {
        qDebug() << "Invalid JSON format.";
        return "";
    }
    QJsonObject obj = doc.object();

    // Search for matching email and password
    for (const auto& key : obj.keys()) {
        QJsonObject user = obj.value(key).toObject();
        if (user.value("email").toString() == email && user.value("password").toString() == password) {
            return user.value("name").toString();
        }
    }

    // No match found
    return "";
}

