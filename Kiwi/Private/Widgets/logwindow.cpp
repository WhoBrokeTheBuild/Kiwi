#include "logwindow.h"
#include "ui_logwindow.h"

LogWindow::LogWindow(QWidget * parent) :
    QFrame(parent),
    _ui(new Ui::LogWindow)
{
    _ui->setupUi(this);

}

LogWindow::~LogWindow()
{
    delete _ui;
}
