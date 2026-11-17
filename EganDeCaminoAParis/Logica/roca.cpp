#include "roca.h"
#include <stdexcept>

Roca::Roca(double x, int carril, double velocidad)
    : Proyectil(x, 0.0, carril, -velocidad, 3.0, -1)
{
    if (velocidad <= 0)
        throw std::invalid_argument("La velocidad de la roca debe ser positiva");
}