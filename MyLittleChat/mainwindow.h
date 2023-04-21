#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QDebug>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void replyFinished();

private:
    Ui::MainWindow *ui;

    QNetworkAccessManager * manager;
    QNetworkRequest request;
    QNetworkReply * reply;
    QJsonObject req;
    QJsonArray context;
};
#endif // MAINWINDOW_H
