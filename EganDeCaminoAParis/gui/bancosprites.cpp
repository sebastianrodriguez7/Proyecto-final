#include "bancosprites.h"
#include <QFont>
#include <QPainter>
#include <QPen>
#include <QTransform>
#include <algorithm>

QMap<QString, QPixmap> BancoSprites::cache;

bool BancoSprites::existe(const QString &nombre)
{
    return !QPixmap(":/sprites/" + nombre + ".png").isNull();
}

QPixmap BancoSprites::obtener(const QString &nombre, int ancho, int alto,
                              const QColor &colorMarcador, bool espejar)
{
    const QString clave = nombre + "|" + QString::number(ancho) + "|" + QString::number(alto)
    + "|" + QString::number(espejar ? 1 : 0);
    if (cache.contains(clave))
        return cache.value(clave);

    QPixmap resultado;
    QPixmap original(":/sprites/" + nombre + ".png");
    if (!original.isNull()) {
        resultado = original.scaled(ancho, alto, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        if (espejar)
            resultado = resultado.transformed(QTransform().scale(-1, 1));
    } else {
        resultado = crearMarcador(nombre, ancho, alto, colorMarcador);
    }
    cache.insert(clave, resultado);
    return resultado;
}

QPixmap BancoSprites::crearMarcador(const QString &nombre, int ancho, int alto, const QColor &color)
{
    QPixmap pm(ancho, alto);
    pm.fill(Qt::transparent);

    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(color);
    p.setPen(QPen(Qt::black, 2));
    p.drawRoundedRect(2, 2, ancho - 4, alto - 4, 12, 12);

    QFont fuente = p.font();
    fuente.setPixelSize(std::max(10, alto / 9));
    fuente.setBold(true);
    p.setFont(fuente);
    p.setPen(Qt::white);
    p.drawText(QRect(0, 0, ancho, alto), int(Qt::AlignCenter) | int(Qt::TextWordWrap), nombre);
    return pm;
}