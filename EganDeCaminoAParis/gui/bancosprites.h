#ifndef BANCOSPRITES_H
#define BANCOSPRITES_H

#include <QColor>
#include <QMap>
#include <QPixmap>
#include <QString>

class BancoSprites
{
public:
    static QPixmap obtener(const QString &nombre, int ancho, int alto,
                           const QColor &colorMarcador, bool espejar = false);

    static bool existe(const QString &nombre);

private:
    static QPixmap crearMarcador(const QString &nombre, int ancho, int alto, const QColor &color);

    static QMap<QString, QPixmap> cache;
};

#endif // BANCOSPRITES_H