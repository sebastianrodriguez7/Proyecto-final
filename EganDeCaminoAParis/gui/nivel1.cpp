#include "nivel1.h"
#include <QBrush>
#include <QPen>
#include <QColor>

Nivel1::Nivel1(QObject *parent)
    : QGraphicsScene(parent)
{
    setSceneRect(0, 0, 1280, 720);
    setBackgroundBrush(QColor(135, 180, 220));
    dibujarCarriles();
}

void Nivel1::dibujarCarriles()
{
    const int altos[3] = {100, 120, 140};
    const QColor colores[3] = {QColor(90, 90, 95), QColor(75, 75, 80), QColor(60, 60, 65)};

    int y = 360;
    for (int i = 0; i < 3; ++i) {
        addRect(0, y, 1280, altos[i], QPen(Qt::white), QBrush(colores[i]));
        y += altos[i];
    }
}