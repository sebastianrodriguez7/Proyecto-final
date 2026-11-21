#include "coleccionable.h"

Coleccionable::Coleccionable(double x, int carril)
    : Entidad(x, 0.0, carril), recogido(false)
{
}

void Coleccionable::actualizar(double) {}

void Coleccionable::recoger(Egan &egan)
{
    if (recogido) return;
    aplicarEfecto(egan);
    recogido = true;
}

bool Coleccionable::estaRecogido() const { return recogido; }