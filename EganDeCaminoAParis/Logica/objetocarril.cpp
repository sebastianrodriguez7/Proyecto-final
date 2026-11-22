#include "objetocarril.h"
#include <stdexcept>

ObjetoCarril::ObjetoCarril(int carril, double tiempoDisponible)
    : Entidad(EganDefensor::X, 0.0, carril),
    tiempoRestante(tiempoDisponible),
    tomado(false)
{
    if (tiempoDisponible <= 0)
        throw std::invalid_argument("El tiempo disponible debe ser positivo");
}

void ObjetoCarril::actualizar(double dt)
{
    if (tiempoRestante > 0)
        tiempoRestante -= dt;
}

void ObjetoCarril::ofrecerA(EganDefensor &egan)
{
    if (!estaActivo()) return;
    aplicarEfecto(egan);
    tomado = true;
}

bool ObjetoCarril::estaActivo() const { return !tomado && tiempoRestante > 0; }
bool ObjetoCarril::fueTomado() const { return tomado; }
double ObjetoCarril::getTiempoRestante() const { return tiempoRestante > 0 ? tiempoRestante : 0; }

Aficionado::Aficionado(int carril, double tiempoDisponible)
    : ObjetoCarril(carril, tiempoDisponible) {}

void Aficionado::aplicarEfecto(EganDefensor &egan)
{
    egan.recibirBidones(BIDONES_QUE_OFRECE);
}

GuitarraCarril::GuitarraCarril(int carril, double tiempoDisponible)
    : ObjetoCarril(carril, tiempoDisponible) {}

void GuitarraCarril::aplicarEfecto(EganDefensor &egan)
{
    egan.activarSentimental();
}