#include "razonamiento.h"
#include <cstdlib>
#include <stdexcept>

int Razonamiento::carrilMasLejano(int carrilEgan, int numCarriles)
{
    int mejor = 1;
    for (int c = 2; c <= numCarriles; ++c)
        if (std::abs(c - carrilEgan) > std::abs(mejor - carrilEgan))
            mejor = c;
    return mejor;
}

int Razonamiento::elegirCarril(const Estado &estado, const TablaQ &tabla,
                               double epsilon, std::mt19937 &gen) const
{
    if (epsilon < 0 || epsilon > 1)
        throw std::invalid_argument("Epsilon debe estar entre 0 y 1");

    std::uniform_real_distribution<double> azar(0.0, 1.0);
    if (azar(gen) < epsilon) {
        std::uniform_int_distribution<int> carril(1, TablaQ::ACCIONES);
        return carril(gen);
    }

    if (tabla.todosCero(estado))
        return carrilMasLejano(estado.getCarrilEgan());
    return tabla.mejorCarril(estado);
}