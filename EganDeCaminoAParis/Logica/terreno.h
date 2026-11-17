#ifndef TERRENO_H
#define TERRENO_H

#include <vector>

struct Segmento
{
    double longitud;
    double angulo;
};

class Terreno
{
public:
    static constexpr double ANGULO_MIN = 2.0;
    static constexpr double ANGULO_MAX = 10.0;

    void agregarSegmento(double longitud, double angulo);
    double anguloEn(double posicion) const;
    double longitudTotal() const;
    int cantidadSegmentos() const;

private:
    std::vector<Segmento> segmentos;
    double total = 0;
};

#endif // TERRENO_H