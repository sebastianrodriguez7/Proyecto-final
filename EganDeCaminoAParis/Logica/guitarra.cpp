#include "guitarra.h"

Guitarra::Guitarra(double x, int carril)
    : Coleccionable(x, carril)
{
}

void Guitarra::aplicarEfecto(Egan &egan)
{
    egan.activarSentimental();
}