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
    static QPixmap edificiosParis(int ancho, int alto);
    static QPixmap arcoDelTriunfo(int ancho, int alto);
    static QPixmap arboledaAvenida(int ancho, int alto);
    static QPixmap marcasAvenida(int ancho, int alto);
    static QPixmap lineaDeDefensa(int ancho, int alto);
};

#endif // FABRICAFONDOS_H