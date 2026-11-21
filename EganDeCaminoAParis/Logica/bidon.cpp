#include "bidon.h"
#include <stdexcept>

Bidon::Bidon(double x, int carril, double energia)
    : Coleccionable(x, carril), energia(energia)
{
    if (energia <= 0)
        throw std::invalid_argument("La energia de un bidon debe ser positiva");
}

double Bidon::getEnergia() const { return energia; }

void Bidon::aplicarEfecto(Egan &egan)
{
    egan.recuperarEnergia(energia);
}