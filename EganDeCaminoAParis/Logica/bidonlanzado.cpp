#include "bidonlanzado.h"
#include "fisicaparabolica.h"
#include <cmath>

namespace {
const double PI = 3.14159265358979323846;

double velocidadX(double alcance)
{
    double v0 = FisicaParabolica().velocidadParaAlcance(alcance, BidonLanzado::ANGULO);
    return v0 * std::cos(BidonLanzado::ANGULO * PI / 180.0);
}

double velocidadY(double alcance)
{
    double v0 = FisicaParabolica().velocidadParaAlcance(alcance, BidonLanzado::ANGULO);
    return v0 * std::sin(BidonLanzado::ANGULO * PI / 180.0);
}
}

BidonLanzado::BidonLanzado(double x, int carril, double alcance)
    : Proyectil(x, 0.0, carril, velocidadX(alcance), velocidadY(alcance), -1, 0.0),
    alcancePrevisto(alcance)
{
}

bool BidonLanzado::haImpactado() const { return rebotes >= 1; }
double BidonLanzado::getAlcancePrevisto() const { return alcancePrevisto; }