#include "egan.h"
#include <stdexcept>

Egan::Egan(double x, double y, int carril, int numCarriles)
    : Entidad(x, y, carril),
    numCarriles(numCarriles),
    energia(ENERGIA_MAX),
    pedaleoFuerte(false),
    enPajara(false),
    tiempoPajara(0),
    pendiente(0)
{
    if (numCarriles < 1 || numCarriles > MAX_CARRILES)
        throw std::invalid_argument("Numero de carriles invalido para Egan");
    if (carril > numCarriles)
        throw std::out_of_range("Carril inicial mayor al numero de carriles del nivel");
}

double Egan::calcularAPedal() const
{
    double a = pedaleoFuerte ? A_PEDAL_FUERTE : A_PEDAL_NORMAL;
    if (pendiente > UMBRAL_ESCALADOR)
        a *= BONO_ESCALADOR;
    return a;
}

void Egan::actualizar(double dt)
{
    // 1. Energía y pájara
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

    // 2. Física de la pendiente
    vx = fisica.actualizarVelocidad(vx, calcularAPedal(), pendiente, dt);
    if (enPajara && vx > VELOCIDAD_PAJARA)
        vx = VELOCIDAD_PAJARA;

    // 3. Movimiento
    x += vx * dt;
    y += vy * dt;
}

void Egan::cambiarCarril(int direccion)
{
    int nuevo = carril + direccion;
    if (nuevo >= 1 && nuevo <= numCarriles)
        carril = nuevo;
}

void Egan::setPedaleoFuerte(bool activo) { pedaleoFuerte = activo && !enPajara; }
void Egan::setPendiente(double grados) { pendiente = grados; }

void Egan::recuperarEnergia(double cantidad)
{
    energia += cantidad;
    if (energia > ENERGIA_MAX) energia = ENERGIA_MAX;
}

double Egan::getEnergia() const { return energia; }
bool Egan::estaEnPajara() const { return enPajara; }
double Egan::getPendiente() const { return pendiente; }