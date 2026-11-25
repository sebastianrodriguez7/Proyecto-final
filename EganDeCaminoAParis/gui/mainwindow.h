#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "estrategiadificultad.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QStackedWidget;
class MenuPrincipal;
class VistaJuego;
class EscenaNivel1;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void iniciarNivel1(Dificultad dificultad);
    void alTerminarNivel1(bool gano, int estrellas);
    void volverAlMenu();

private:
    void descartarEscena();

    Ui::MainWindow *ui;
    QStackedWidget *paginas;
    MenuPrincipal *menu;
    VistaJuego *vista;
    EscenaNivel1 *escena;
    int estrellasNivel1;
};
#endif // MAINWINDOW_H