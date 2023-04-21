#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::replyFinished);

    request.setUrl(QUrl("https://api.openai.com/v1/chat/completions"));
    request.setRawHeader(QByteArray("Content-Type"), QByteArray("application/json"));
    //                                                                   здесь необходимо указать api ключ
    request.setRawHeader(QByteArray("Authorization"), QByteArray("Bearer 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"));

    req = {
        {"model", "gpt-3.5-turbo"},
        {"messages", QJsonArray()},
        {"temperature", 0.5}
    };
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    auto message = ui->textEdit->toPlainText();

    auto messages = req["messages"].toArray();
    messages.append(QJsonObject({{"role", "user"}, {"content", message}}));
    req["messages"] = messages;
    QJsonDocument doc(req);
    reply = manager->post(request, doc.toJson(QJsonDocument::Compact));
}

void MainWindow::replyFinished()
{
    ui->textEdit_2->clear();
    QByteArray response = reply->readAll();
    QString resp = QJsonDocument::fromJson(response).object()["choices"][0]["message"]["content"].toString();
    ui->textEdit_2->setText(resp);
}

