#include "egan.h"
#include <stdexcept>

Egan::Egan(double x, double y, int carril, int numCarriles)
    : Entidad(x, y, carril),
    numCarriles(numCarriles),
    energia(ENERGIA_MAX),
    pedaleoFuerte(false),
    enPajara(false),
    tiempoPajara(0)
{
    if (numCarriles < 1 || numCarriles > MAX_CARRILES)
        throw std::invalid_argument("Numero de carriles invalido para Egan");
    if (carril > numCarriles)
        throw std::out_of_range("Carril inicial mayor al numero de carriles del nivel");
}

void Egan::actualizar(double dt)
{
    if (enPajara) {
        tiempoPajara -= dt;
        if (tiempoPajara <= 0) {
            enPajara = false;
            energia = ENERGIA_TRAS_PAJARA;
        }
    } else if (pedaleoFuerte) {
        energia -= CONSUMO_PEDALEO * dt;
        if (energia <= 0) {
            energia = 0;
            enPajara = true;
            tiempoPajara = DURACION_PAJARA;
            pedaleoFuerte = false;
        }
    }
    x += vx * dt;
    y += vy * dt;
}

void Egan::cambiarCarril(int direccion)
{
    int nuevo = carril + direccion;
    if (nuevo >= 1 && nuevo <= numCarriles)
        carril = nuevo;
}

void Egan::setPedaleoFuerte(bool activo)
{
    pedaleoFuerte = activo && !enPajara;
}

void Egan::recuperarEnergia(double cantidad)
{
    energia += cantidad;
    if (energia > ENERGIA_MAX) energia = ENERGIA_MAX;
}

double Egan::getEnergia() const { return energia; }
bool Egan::estaEnPajara() const { return enPajara; }