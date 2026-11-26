#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "escenanivel1.h"
#include "escenanivel2.h"
#include "menuprincipal.h"
#include "vistajuego.h"
#include <QCoreApplication>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    paginas(nullptr),
    menu(nullptr),
    vista(nullptr),
    escenaActual(nullptr)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("Egan, camino a París"));

    paginas = new QStackedWidget(this);
    menu = new MenuPrincipal(paginas);
    vista = new VistaJuego(paginas);
    paginas->addWidget(menu);
    paginas->addWidget(vista);
    setCentralWidget(paginas);
    resize(1280, 720);

    connect(menu, &MenuPrincipal::jugarSolicitado, this, &MainWindow::iniciarNivel1);
    connect(menu, &MenuPrincipal::practicarNivel2Solicitado, this, &MainWindow::practicarNivel2);
    connect(menu, &MenuPrincipal::salirSolicitado, this, &QWidget::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::rutaTablaQ() const
{
    return QCoreApplication::applicationDirPath() + QStringLiteral("/tabla_q.txt");
}

void MainWindow::descartarEscena()
{
    if (escenaActual == nullptr)
        return;
    vista->setScene(nullptr);
    escenaActual->deleteLater();
    escenaActual = nullptr;
}

void MainWindow::mostrarEscena(QGraphicsScene *nueva)
{
    escenaActual = nueva;
    vista->setScene(nueva);
    paginas->setCurrentWidget(vista);
    vista->ajustar();
    vista->setFocus();
}

void MainWindow::iniciarNivel1(Dificultad dificultad)
{
    descartarEscena();

    EscenaNivel1 *escena = new EscenaNivel1(dificultad, this);
    connect(escena, &EscenaNivel1::volverAlMenu, this, &MainWindow::volverAlMenu, Qt::QueuedConnection);
    connect(escena, &EscenaNivel1::nivelTerminado, this, &MainWindow::alTerminarNivel1, Qt::QueuedConnection);
    mostrarEscena(escena);
}

void MainWindow::alTerminarNivel1(bool gano, int estrellas)
{
    if (!gano) {
        volverAlMenu();
        return;
    }
    iniciarNivel2(2 + estrellas);
}

void MainWindow::practicarNivel2()
{
    iniciarNivel2(3);
}

void MainWindow::iniciarNivel2(int vidas)
{
    descartarEscena();

    EscenaNivel2 *escena = new EscenaNivel2(vidas, rutaTablaQ(), this);
    connect(escena, &EscenaNivel2::volverAlMenu, this, &MainWindow::volverAlMenu, Qt::QueuedConnection);
    connect(escena, &EscenaNivel2::nivelTerminado, this, &MainWindow::alTerminarNivel2, Qt::QueuedConnection);
    mostrarEscena(escena);
}

void MainWindow::alTerminarNivel2(bool gano)
{
    Q_UNUSED(gano);
    volverAlMenu();
}

void MainWindow::volverAlMenu()
{
    descartarEscena();
    paginas->setCurrentWidget(menu);
}