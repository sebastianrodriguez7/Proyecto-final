#include "fabricafondos.h"
#include "proyeccion.h"
#include <QLinearGradient>
#include <QPainter>
#include <QPointF>
#include <QPolygonF>
#include <cmath>
#include <QPainterPath>
#include <QRectF>

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
QPixmap FabricaFondos::edificiosParis(int ancho, int alto)
{
    QPixmap pm(ancho, alto);
    pm.fill(Qt::transparent);

    QPainter p(&pm);
    p.setPen(Qt::NoPen);

    const int anchoEdificio = 92;
    for (int i = 0; i * anchoEdificio < ancho; ++i) {
        int x = i * anchoEdificio;
        int altura = 105 + (i * 41) % 55;
        int y = alto - altura;

        p.setBrush(QColor(226 - (i % 3) * 8, 214 - (i % 3) * 8, 186 - (i % 3) * 6));
        p.drawRect(x, y, anchoEdificio - 2, altura);

        p.setBrush(QColor(110, 118, 135));
        QPolygonF techo;
        techo << QPointF(x - 2, y) << QPointF(x + anchoEdificio, y)
              << QPointF(x + anchoEdificio - 14, y - 22) << QPointF(x + 12, y - 22);
        p.drawPolygon(techo);

        p.setBrush(QColor(80, 95, 125, 200));
        for (int fila = 0; fila < altura / 34; ++fila)
            for (int col = 0; col < 3; ++col)
                p.drawRect(x + 12 + col * 26, y + 12 + fila * 34, 14, 20);
    }
    return pm;
}

QPixmap FabricaFondos::arcoDelTriunfo(int ancho, int alto)
{
    QPixmap pm(ancho, alto);
    pm.fill(Qt::transparent);

    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(QColor(150, 135, 105), 2));

    p.setBrush(QColor(218, 204, 172));
    p.drawRect(QRectF(ancho * 0.06, alto * 0.20, ancho * 0.88, alto * 0.80));
    p.setBrush(QColor(204, 189, 156));
    p.drawRect(QRectF(0, alto * 0.02, ancho, alto * 0.20));

    p.setBrush(QColor(190, 175, 142));
    p.drawRect(QRectF(ancho * 0.06, alto * 0.32, ancho * 0.88, alto * 0.05));
    p.drawRect(QRectF(ancho * 0.06, alto * 0.42, ancho * 0.88, alto * 0.03));

    const double ax = ancho * 0.30, aw = ancho * 0.40;
    QPainterPath arco;
    arco.moveTo(ax, alto);
    arco.lineTo(ax, alto * 0.52);
    arco.arcTo(QRectF(ax, alto * 0.52 - aw / 2, aw, aw), 180, -180);
    arco.lineTo(ax + aw, alto);
    arco.closeSubpath();
    p.setBrush(QColor(70, 62, 58));
    p.drawPath(arco);
    return pm;
}

QPixmap FabricaFondos::arboledaAvenida(int ancho, int alto)
{
    QPixmap pm(ancho, alto);
    pm.fill(Qt::transparent);

    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);

    const int paso = 110;
    for (int i = 0; i * paso < ancho + paso; ++i) {
        int cx = i * paso + paso / 2;
        int copa = 46 + (i * 17) % 14;

        p.setBrush(QColor(90, 62, 40));
        p.drawRect(cx - 5, alto - 46, 10, 46);
        p.setBrush(QColor(52, 120, 64));
        p.drawEllipse(cx - copa, alto - 46 - copa, copa * 2, copa + 18);
        p.setBrush(QColor(78, 150, 84));
        p.drawEllipse(cx - copa / 2, alto - 46 - copa - 4, copa, copa);
    }
    return pm;
}

QPixmap FabricaFondos::marcasAvenida(int ancho, int alto)
{
    QPixmap pm(ancho, alto);
    pm.fill(Qt::transparent);

    QPainter p(&pm);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(255, 255, 255, 150));

    const int largo = 60;
    const int hueco = 60;
    for (int carril = 1; carril <= 4; ++carril) {
        int y = static_cast<int>(Proyeccion::nivel2Fondo(carril));
        for (int x = 0; x < ancho; x += largo + hueco)
            p.drawRect(x, y - 2, largo, 4);
    }
    return pm;
}

QPixmap FabricaFondos::lineaDeDefensa(int ancho, int alto)
{
    QPixmap pm(ancho, alto);
    pm.fill(Qt::transparent);

    QPainter p(&pm);
    p.setPen(Qt::NoPen);
    const int lado = ancho / 2;
    for (int fila = 0; fila * lado < alto; ++fila) {
        for (int col = 0; col < 2; ++col) {
            p.setBrush(((fila + col) % 2 == 0) ? QColor(255, 255, 255) : QColor(20, 20, 20));
            p.drawRect(col * lado, fila * lado, lado, lado);
        }
    }
    return pm;
}