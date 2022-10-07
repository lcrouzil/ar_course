#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

#include "Engine.h"
#include "2DGraphics/GEngine.h"

namespace Ui {
class Window;
}

class Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window();

private:
    Ui::Window *ui;

private slots:
    void resetEngine(bool);
};

#endif // WINDOW_H