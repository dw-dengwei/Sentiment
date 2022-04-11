#include "ui_mainwindow.h"
#include <QtWidgets/QPushButton>

#ifndef IMAGE_SHOW_H
#define IMAGE_SHOW_H
class ImageShow
{
public:
    QPushButton *btn;
    bool has_img;
    QString img_path;
    ImageShow(int id, Ui::MainWindow* ui);
    ~ImageShow();
};
#endif // IMAGE_SHOW_H
