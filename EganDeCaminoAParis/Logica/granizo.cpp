#include "granizo.h"
#include <stdexcept>

namespace {
const double G = 9.8;

double alturaParaCaerEn(double t)
{
    if (t <= 0)
        throw std::invalid_argument("El tiempo de aviso del granizo debe ser positivo");
    return 0.5 * G * t * t;
}
}

Granizo::Granizo(double x, int carril, double tiempoAviso)
    : Proyectil(x, alturaParaCaerEn(tiempoAviso), carril, 0.0, 0.0, 2),
    alturaInicial(alturaParaCaerEn(tiempoAviso))
{
}

bool Granizo::enAviso() const { return activo && rebotes == 0; }
double Granizo::getAlturaInicial() const { return alturaInicial; }