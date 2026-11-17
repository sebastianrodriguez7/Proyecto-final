#include "fisicaparabolica.h"
#include <cmath>
#include <stdexcept>

namespace {
const double PI = 3.14159265358979323846;
const double VELOCIDAD_MINIMA_REBOTE = 0.5;
}

FisicaParabolica::FisicaParabolica(double g, double rebote)
    : g(g), e(rebote)
{
    if (g <= 0)
        throw std::invalid_argument("La gravedad debe ser positiva");
    if (rebote < 0 || rebote >= 1)
        throw std::invalid_argument("El coeficiente de rebote debe estar en [0, 1)");
}

bool FisicaParabolica::paso(double &x, double &y, double &vx, double &vy, double dt) const
{
    if (dt < 0)
        throw std::invalid_argument("dt no puede ser negativo");

    vy -= g * dt;
    x += vx * dt;
    y += vy * dt;

    if (y <= 0) {
        y = 0;
        double vImpacto = -vy;
        if (vImpacto > VELOCIDAD_MINIMA_REBOTE) {
            vy = e * vImpacto;
            return true;
        }
        vy = 0;
    }
    return false;
}

double FisicaParabolica::alcance(double v0, double anguloGrados) const
{
    double theta = anguloGrados * PI / 180.0;
    return v0 * v0 * std::sin(2 * theta) / g;
}

double FisicaParabolica::getG() const { return g; }
double FisicaParabolica::getRebote() const { return e; }