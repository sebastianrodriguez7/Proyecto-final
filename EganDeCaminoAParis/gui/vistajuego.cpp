#include "vistajuego.h"
#include <QPainter>
#include <QResizeEvent>

VistaJuego::VistaJuego(QWidget *parent)
    : QGraphicsView(parent)
{
    setFrameShape(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
    setFocusPolicy(Qt::StrongFocus);
    setBackgroundBrush(QBrush(Qt::black));
}

void VistaJuego::ajustar()
{
    if (scene() != nullptr)
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}

void VistaJuego::resizeEvent(QResizeEvent *evento)
{
    QGraphicsView::resizeEvent(evento);
    ajustar();
}