#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "image_show.h"
#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QHttpPart>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for (int i = 0; i < 4; i++)
    {
        img[i] = new ImageShow(i, ui);
        connect(img[i]->btn, &QPushButton::clicked, this, [=]()
        {
            QString path = QFileDialog::getOpenFileName(this,
                                                        tr("Open image file"), "/",
                                                        tr("Image files(*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm)"));
            if(path != "")
            {
                img[i]->has_img = true;
                img[i]->img_path = path;
                img[i]->btn->setIcon(QIcon(path));
                ui->predict->setIcon(QIcon(":/img/unknown.png"));
            }
        });

        img[i]->btn->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(img[i]->btn, &QPushButton::customContextMenuRequested, [=]()
        {
            img[i]->has_img = false;
            img[i]->btn->setIcon(QIcon(":/img/open.png"));
            ui->predict->setIcon(QIcon(":/img/unknown.png"));
        });
    }

    ui->predict->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->predict, &QPushButton::customContextMenuRequested, [=]()
    {
        ui->predict->setIcon(QIcon(":/img/unknown.png"));
    });

    status_map[0] = QString(":/img/negative.png");
    status_map[1] = QString(":/img/neutral.png");
    status_map[2] = QString(":/img/positive.png");
}


MainWindow::~MainWindow()
{
    delete ui;
    for (int i = 0; i < 4; i++)
    {
        delete img[i];
    }
}


void MainWindow::on_predict_clicked()
{
    ui->predict->setIcon(QIcon(":/img/unknown.png"));
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart imagePart, textPart;
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"text\""));
    bool flag = false;
    QString path;
    for(int i = 0; i < 4; i++)
    {
        if(img[i]->has_img)
        {
            path = img[i]->img_path;
            flag = true;
            imagePart.setHeader(
                        QNetworkRequest::ContentDispositionHeader,
                        QVariant(QString("form-data; name=\"%1\"; filename=\"%2\"")\
                                 .arg("image")\
                                 .arg(QFileInfo(path).fileName()))
                        );
            break;
        }
    }

    if(!flag)
    {
        qDebug() << "No Image Error!";
        return;
    }

    textPart.setBody(ui->textInput->toPlainText().toStdString().data());
    QFile *file = new QFile(path);
    file->open(QIODevice::ReadOnly);
    imagePart.setBodyDevice(file);          //方便后续内存释放
    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, "image/png");

    multiPart->append(imagePart);
    multiPart->append(textPart);

    QNetworkRequest networkRequest;
    networkRequest.setUrl(QUrl("http://remote:12333/"));

    //发送请求
    QEventLoop loop;
    QNetworkAccessManager manager;
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QNetworkReply *reply = manager.post(networkRequest, multiPart);
    multiPart->setParent(reply); // delete the multiPart with the reply
    loop.exec();
    if(reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Error: " << reply->error();
        return;
    }
    int data = reply->readAll().toStdString()[0] - '0';
    qDebug() << data;

    int ret = data;
    ui->predict->setIcon(QIcon(status_map[ret]));
}
