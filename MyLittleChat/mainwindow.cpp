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
    request.setRawHeader(QByteArray("Authorization"), QByteArray("Bearer <insert your api key here>"));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QByteArray arr;
    arr.fromStdString("{model: gpt-3.5-turbo, messages: [{role: user, content: \"Поясни за buildroot\"}], temperature: 0.7}");

    QJsonObject req = {
        {"model", "gpt-3.5-turbo"},
        {"messages", QJsonArray({ QJsonObject({{"role", "user"}, {"content", "Поясни за buildroot"}})})},
//         QJsonObject({{"role", "user"},
//                                  {"content", "Поясни за buildroot"}})},
        {"temperature", 0.7}
    };

    QJsonDocument doc(req);


    qDebug() << doc.toJson(QJsonDocument::Compact);
//    QIODevice * dev = new QIODevice() = QString("{\"model\": \"gpt-3.5-turbo\", \"messages\": [{\"role\": \"user\", \"content\": \"Поясни за buildroot\"}], \"temperature\": 0.7}");
//    for (auto & header : request.rawHeaderList())
//    {

//        ui->textEdit->setText(ui->textEdit->toPlainText() + "\n" + QString(header) + ": " + QString(request.rawHeader(header)));
//        qDebug() << QString(header) << " : " << QString(request.rawHeader(header));
//    }
    reply = manager->post(request, doc.toJson(QJsonDocument::Compact));

}

void MainWindow::replyFinished()
{
    ui->textEdit->clear();

    QByteArray response = reply->readAll();

    ui->textEdit->setText(QString(response));
}

