#ifndef FABRICAFONDOS_H
#define FABRICAFONDOS_H

#include <QColor>
#include <QPixmap>

class FabricaFondos
{
public:
    static QPixmap cielo(int ancho, int alto);
    static QPixmap montanas(int periodo, int alto, const QColor &color, double fase);
    static QPixmap bosque(int periodo, int alto);
    static QPixmap marcasCarretera(int periodo, int alto);
};

#endif // FABRICAFONDOS_H