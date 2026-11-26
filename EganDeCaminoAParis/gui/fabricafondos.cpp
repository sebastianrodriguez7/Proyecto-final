#include "fabricafondos.h"
#include "proyeccion.h"
#include <QLinearGradient>
#include <QPainter>
#include <QPointF>
#include <QPolygonF>
#include <cmath>

QPixmap FabricaFondos::cielo(int ancho, int alto)
{
    QPixmap pm(ancho, alto);
    QPainter p(&pm);
    QLinearGradient degradado(0, 0, 0, alto);
    degradado.setColorAt(0.0, QColor(70, 130, 200));
    degradado.setColorAt(1.0, QColor(205, 228, 245));
    p.fillRect(0, 0, ancho, alto, degradado);
    return pm;
}

QPixmap FabricaFondos::montanas(int periodo, int alto, const QColor &color, double fase)
{
    QPixmap pm(periodo, alto);
    pm.fill(Qt::transparent);

    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    p.setBrush(color);

    const double dosPi = 6.283185307179586;
    QPolygonF poligono;
    poligono << QPointF(0, alto);
    for (int x = 0; x <= periodo; x += 8) {
        double t = dosPi * x / periodo;
        double relieve = 0.50 * std::sin(t + fase)
                         + 0.28 * std::sin(3 * t + 2 * fase)
                         + 0.14 * std::sin(7 * t + 3 * fase);
        double y = alto * (0.55 - 0.30 * relieve);
        poligono << QPointF(x, y);
    }
    poligono << QPointF(periodo, alto);
    p.drawPolygon(poligono);
    return pm;
}

QPixmap FabricaFondos::bosque(int periodo, int alto)
{
    QPixmap pm(periodo, alto);
    pm.fill(Qt::transparent);

    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);

    const int paso = 64;
    const int base = alto - 20;
    for (int i = 0; i < periodo / paso; ++i) {
        int cx = i * paso + paso / 2;
        int altura = 70 + (i * 37) % 45;

        p.setBrush(QColor(90, 60, 35));
        p.drawRect(cx - 4, base - 18, 8, 18);

        p.setBrush(QColor(25, 90 + (i * 13) % 30, 50));
        for (int piso = 0; piso < 3; ++piso) {
            int anchoPiso = 54 - piso * 12;
            int y0 = base - 14 - piso * (altura / 3);
            QPolygonF triangulo;
            triangulo << QPointF(cx - anchoPiso / 2, y0)
                      << QPointF(cx + anchoPiso / 2, y0)
                      << QPointF(cx, y0 - altura / 2);
            p.drawPolygon(triangulo);
        }
    }
    p.setBrush(QColor(20, 70, 40));
    p.drawRect(0, base, periodo, 20);
    return pm;
}

QPixmap FabricaFondos::marcasCarretera(int periodo, int alto)
{
    QPixmap pm(periodo, alto);
    pm.fill(Qt::transparent);

    QPainter p(&pm);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(255, 255, 255, 170));

    const int largo = 80;
    const int hueco = 80;
    const int y1 = static_cast<int>(Proyeccion::nivel1Fondo(1));
    const int y2 = static_cast<int>(Proyeccion::nivel1Fondo(2));
    for (int x = 0; x < periodo; x += largo + hueco) {
        p.drawRect(x, y1 - 2, largo, 4);
        p.drawRect(x, y2 - 2, largo, 4);
    }
    return pm;
}