#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "nivel1.h"
#include <QGraphicsView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    escena = new Nivel1(this);
    vista = new QGraphicsView(escena, this);
    vista->setFrameShape(QFrame::NoFrame);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCentralWidget(vista);
    resize(1280, 720);
}

MainWindow::~MainWindow()
{
    delete ui;
}
