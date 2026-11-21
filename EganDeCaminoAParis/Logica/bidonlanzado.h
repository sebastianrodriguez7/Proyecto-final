#ifndef BIDONLANZADO_H
#define BIDONLANZADO_H

#include "proyectil.h"

class BidonLanzado : public Proyectil
{
public:
    static constexpr double ANGULO = 45.0;

    BidonLanzado(double x, int carril, double alcance);

    bool haImpactado() const;
    double getAlcancePrevisto() const;

private:
    double alcancePrevisto;
};

#endif // BIDONLANZADO_H