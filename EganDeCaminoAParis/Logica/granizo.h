#ifndef GRANIZO_H
#define GRANIZO_H

#include "proyectil.h"

class Granizo : public Proyectil
{
public:
    static constexpr double TIEMPO_AVISO = 1.5;


    Granizo(double x, int carril, double tiempoAviso = TIEMPO_AVISO);

    bool enAviso() const;
    double getAlturaInicial() const;

private:
    double alturaInicial;
};

#endif // GRANIZO_H