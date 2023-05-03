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
    QFile file("api_key");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "file not opened";
        return;
    }
    QString key_header = "Bearer " + file.readAll();
    key_header = key_header.trimmed();
    request.setRawHeader(QByteArray("Authorization"),
                         QByteArray(qPrintable(key_header)));
    file.close();

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
    ui->textEdit_2->setText(QJsonDocument::fromJson(reply->readAll()).object()["choices"].toArray()[0].toObject()["message"].toObject()["content"].toString());
}
