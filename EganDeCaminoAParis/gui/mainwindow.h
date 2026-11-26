#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "estrategiadificultad.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QGraphicsScene;
class QStackedWidget;
class MenuPrincipal;
class VistaJuego;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void iniciarNivel1(Dificultad dificultad);
    void alTerminarNivel1(bool gano, int estrellas);
    void iniciarNivel2(int vidas);
    void practicarNivel2();
    void alTerminarNivel2(bool gano);
    void volverAlMenu();

private:
    void mostrarEscena(QGraphicsScene *nueva);
    void descartarEscena();
    QString rutaTablaQ() const;

    Ui::MainWindow *ui;
    QStackedWidget *paginas;
    MenuPrincipal *menu;
    VistaJuego *vista;
    QGraphicsScene *escenaActual;
};
#endif // MAINWINDOW_H