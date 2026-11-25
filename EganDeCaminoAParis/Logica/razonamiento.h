#ifndef RAZONAMIENTO_H
#define RAZONAMIENTO_H

#include <random>
#include "estado.h"
#include "tablaq.h"

class Razonamiento
{
public:
    int elegirCarril(const Estado &estado, const TablaQ &tabla,
                     double epsilon, std::mt19937 &gen) const;

    static int carrilMasLejano(int carrilEgan, int numCarriles = Estado::NUM_CARRILES);
};

#endif // RAZONAMIENTO_H