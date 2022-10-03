#include "window.h"
#include "ui_window.h"

Window::Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);

    Engine *e = new Engine();
    GEngine *ge = e->getGEngine();

    ui->verticalLayout_main->addWidget(ge);

}

Window::~Window()
{
    delete ui;
}
