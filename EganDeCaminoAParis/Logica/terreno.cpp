#include "terreno.h"
#include <stdexcept>

void Terreno::agregarSegmento(double longitud, double angulo)
{
    if (longitud <= 0)
        throw std::invalid_argument("La longitud del segmento debe ser positiva");
    if (angulo < ANGULO_MIN || angulo > ANGULO_MAX)
        throw std::out_of_range("La pendiente debe estar entre 2 y 10 grados");

    segmentos.push_back({longitud, angulo});
    total += longitud;
}

double Terreno::anguloEn(double posicion) const
{
    if (segmentos.empty())
        throw std::logic_error("El terreno no tiene segmentos");
    if (posicion < 0)
        throw std::out_of_range("Posicion negativa en el terreno");

    double acumulado = 0;
    for (const Segmento &s : segmentos) {
        acumulado += s.longitud;
        if (posicion < acumulado)
            return s.angulo;
    }
    return segmentos.back().angulo;
}

double Terreno::longitudTotal() const { return total; }
int Terreno::cantidadSegmentos() const { return static_cast<int>(segmentos.size()); }