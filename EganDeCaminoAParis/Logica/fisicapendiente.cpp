#include "fisicapendiente.h"
#include <cmath>
#include <stdexcept>

namespace {
const double PI = 3.14159265358979323846;
}

FisicaPendiente::FisicaPendiente(double g, double rozamiento)
    : g(g), c(rozamiento)
{
    if (g <= 0)
        throw std::invalid_argument("La gravedad debe ser positiva");
    if (rozamiento < 0)
        throw std::invalid_argument("El rozamiento no puede ser negativo");
}

double FisicaPendiente::calcularAceleracion(double aPedal, double anguloGrados, double v) const
{
    double theta = anguloGrados * PI / 180.0;
    return aPedal - g * std::sin(theta) - c * v;
}

double FisicaPendiente::actualizarVelocidad(double v, double aPedal, double anguloGrados, double dt) const
{
    if (dt < 0)
        throw std::invalid_argument("dt no puede ser negativo");
    double nueva = v + calcularAceleracion(aPedal, anguloGrados, v) * dt;
    return (nueva < 0) ? 0 : nueva;
}