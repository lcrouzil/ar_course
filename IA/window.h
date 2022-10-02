#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

#include "2DGraphics/MapWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Window; }
QT_END_NAMESPACE

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);
    ~Window();

private:
    Ui::Window *ui;
};
#endif // WINDOW_H
