#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "image_show.h"
#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QDebug>

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

//void MainWindow::contextMenuEvent(QContextMenuEvent *event)
//{
//    QMenu *menu = new QMenu(this);
//    menu->addAction(new QAction("Remove", this));
//    menu->show();
//    menu->move(cursor().pos())
//}


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
    // TODO
    int ret = 2;
    ui->predict->setIcon(QIcon(status_map[ret]));
}
