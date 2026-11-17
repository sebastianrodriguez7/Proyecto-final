#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Nivel1;
class QGraphicsView;
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Nivel1 *escena;
    QGraphicsView *vista;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
