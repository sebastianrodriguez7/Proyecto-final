#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "escenanivel1.h"
#include "menuprincipal.h"
#include "vistajuego.h"
#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    paginas(nullptr),
    menu(nullptr),
    vista(nullptr),
    escena(nullptr),
    estrellasNivel1(0)
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
    connect(menu, &MenuPrincipal::salirSolicitado, this, &QWidget::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::descartarEscena()
{
    if (escena == nullptr)
        return;
    vista->setScene(nullptr);
    escena->deleteLater();
    escena = nullptr;
}

void MainWindow::iniciarNivel1(Dificultad dificultad)
{
    descartarEscena();

    escena = new EscenaNivel1(dificultad, this);
    connect(escena, &EscenaNivel1::volverAlMenu, this, &MainWindow::volverAlMenu, Qt::QueuedConnection);
    connect(escena, &EscenaNivel1::nivelTerminado, this, &MainWindow::alTerminarNivel1, Qt::QueuedConnection);

    vista->setScene(escena);
    paginas->setCurrentWidget(vista);
    vista->ajustar();
    vista->setFocus();
}

void MainWindow::alTerminarNivel1(bool gano, int estrellas)
{
    estrellasNivel1 = gano ? estrellas : 0;
    volverAlMenu();
}

void MainWindow::volverAlMenu()
{
    descartarEscena();
    paginas->setCurrentWidget(menu);
}