#include "fisicaoscilatoria.h"
#include <cmath>
#include <stdexcept>

FisicaOscilatoria::FisicaOscilatoria(double gamma, double omega)
    : gamma(gamma), omega(omega)
{
    if (gamma <= 0)
        throw std::invalid_argument("El amortiguamiento debe ser positivo");
    if (omega <= 0)
        throw std::invalid_argument("La frecuencia debe ser positiva");
}

double FisicaOscilatoria::angulo(double amplitud, double t) const
{
    if (amplitud < 0)
        throw std::invalid_argument("La amplitud no puede ser negativa");
    if (t < 0)
        throw std::invalid_argument("El tiempo no puede ser negativo");

    return amplitud * std::exp(-gamma * t) * std::cos(omega * t);
}

double FisicaOscilatoria::getGamma() const { return gamma; }
double FisicaOscilatoria::getOmega() const { return omega; }