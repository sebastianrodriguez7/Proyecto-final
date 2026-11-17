#ifndef ROCA_H
#define ROCA_H

#include "proyectil.h"

class Roca : public Proyectil
{
public:
    Roca(double x, int carril, double velocidad);
};

#endif // ROCA_H