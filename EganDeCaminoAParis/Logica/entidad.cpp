#include "entidad.h"
#include <stdexcept>

Entidad::Entidad(double x, double y, int carril)
    : x(x), y(y), vx(0), vy(0), carril(1)
{
    setCarril(carril);
}

Entidad::~Entidad() {}

double Entidad::getX() const { return x; }
double Entidad::getY() const { return y; }
double Entidad::getVx() const { return vx; }
double Entidad::getVy() const { return vy; }
int Entidad::getCarril() const { return carril; }

void Entidad::setPosicion(double nx, double ny)
{
    x = nx;
    y = ny;
}

void Entidad::setVelocidad(double nvx, double nvy)
{
    vx = nvx;
    vy = nvy;
}

void Entidad::setCarril(int nuevoCarril)
{
    if (nuevoCarril < 1 || nuevoCarril > MAX_CARRILES)
        throw std::out_of_range("Carril fuera de rango (1 a 5)");
    carril = nuevoCarril;
}