#include "egan.h"
#include <stdexcept>

Egan::Egan(double x, double y, int carril, int numCarriles)
    : Entidad(x, y, carril),
    numCarriles(numCarriles),
    energia(ENERGIA_MAX),
    pedaleoFuerte(false),
    enPajara(false),
    tiempoPajara(0),
    pendiente(0),
    fisicaSalto(9.8, 0.0),
    enSalto(false),
    tambaleando(false),
    amplitudTambaleo(0),
    tiempoTambaleo(0),
    golpes(0)
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


    if (tambaleando) {
        tiempoTambaleo += dt;
        if (tiempoTambaleo >= DURACION_TAMBALEO) {
            tambaleando = false;
            tiempoTambaleo = 0;
        }
    }

    vx = fisica.actualizarVelocidad(vx, calcularAPedal(), pendiente, dt);
    if (enPajara && vx > VELOCIDAD_PAJARA)
        vx = VELOCIDAD_PAJARA;


    if (enSalto) {
        double xLocal = 0, vxLocal = 0;
        fisicaSalto.paso(xLocal, y, vxLocal, vy, dt);
        if (y <= 0 && vy <= 0) {
            y = 0;
            vy = 0;
            enSalto = false;
        }
    }


    x += vx * dt;
}

void Egan::cambiarCarril(int direccion)
{
    if (tambaleando) return;
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

void Egan::setMultiplicadorConsumo(double multiplicador)
{
    if (multiplicador <= 0)
        throw std::invalid_argument("El multiplicador de consumo debe ser positivo");
    multiplicadorConsumo = multiplicador;
}

void Egan::saltar()
{
    if (enSalto || tambaleando) return;
    enSalto = true;
    vy = V_SALTO;
}

bool Egan::recibirGolpe(double amplitud)
{
    if (amplitud <= 0)
        throw std::invalid_argument("La amplitud del golpe debe ser positiva");
    if (tambaleando) return false;

    tambaleando = true;
    amplitudTambaleo = amplitud;
    tiempoTambaleo = 0;
    ++golpes;
    vx *= FACTOR_VELOCIDAD_GOLPE;
    return true;
}

double Egan::getEnergia() const { return energia; }
bool Egan::estaEnPajara() const { return enPajara; }
double Egan::getPendiente() const { return pendiente; }
bool Egan::estaSaltando() const { return enSalto; }
bool Egan::estaTambaleando() const { return tambaleando; }
int Egan::getGolpes() const { return golpes; }

double Egan::getAnguloTambaleo() const
{
    if (!tambaleando) return 0;
    return fisicaTambaleo.angulo(amplitudTambaleo, tiempoTambaleo);
}