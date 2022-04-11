#include "image_show.h"
#include "mainwindow.h"

ImageShow::ImageShow(int id, Ui::MainWindow *ui)
{
    switch (id)
    {
    case 0:
        btn = ui->open0;
        break;
    case 1:
        btn = ui->open1;
        break;
    case 2:
        btn = ui->open2;
        break;
    case 3:
        btn = ui->open3;
        break;
    default:
        break;
    }
    has_img = false;
}

ImageShow::~ImageShow()
{
    delete btn;
}
