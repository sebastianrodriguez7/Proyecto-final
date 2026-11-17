#include "proyectil.h"

Proyectil::Proyectil(double x, double altura, int carril,
                     double velX, double velY, int maxRebotes, double rebote)
    : Entidad(x, altura, carril),
    fisica(9.8, rebote),
    activo(true),
    rebotes(0),
    maxRebotes(maxRebotes)
{
    setVelocidad(velX, velY);
}

void Proyectil::actualizar(double dt)
{
    if (!activo) return;

    if (fisica.paso(x, y, vx, vy, dt)) {
        ++rebotes;
        if (maxRebotes >= 0 && rebotes >= maxRebotes)
            activo = false;
    }
}

bool Proyectil::estaActivo() const { return activo; }
void Proyectil::desactivar() { activo = false; }
int Proyectil::getRebotes() const { return rebotes; }