#include "rival.h"
#include <stdexcept>

Rival::Rival(int id, TipoRival tipo, int carril, double velocidad, int resistencia, double x)
    : Entidad(x, 0.0, carril),
    id(id),
    tipo(tipo),
    velocidad(velocidad),
    resistencia(resistencia),
    aturdimiento(0),
    detenido(false),
    tiempoDetenido(0),
    empuje(0),
    tiempoTambaleo(-1),
    fisicaTambaleo(3.0, 10.0)
{
    if (velocidad <= 0)
        throw std::invalid_argument("La velocidad del rival debe ser positiva");
    if (resistencia < 1)
        throw std::invalid_argument("La resistencia del rival debe ser al menos 1");
}

void Rival::actualizar(double dt)
{
    if (!estaActivo()) return;

    if (detenido)
        tiempoDetenido -= dt;
    else if (aturdimiento > 0)
        aturdimiento -= dt;
    else
        x -= velocidad * dt;

    if (empuje > 0) {
        x += empuje * dt;
        empuje -= ROZAMIENTO_EMPUJE * empuje * dt;
        if (empuje < 0.01) empuje = 0;
    }

    if (tiempoTambaleo >= 0) {
        tiempoTambaleo += dt;
        if (tiempoTambaleo >= DURACION_ATURDIMIENTO)
            tiempoTambaleo = -1;
    }
}

void Rival::recibirBidon()
{
    if (detenido) return;
    --resistencia;
    aturdimiento = DURACION_ATURDIMIENTO;
    empuje = EMPUJE_INICIAL;
    tiempoTambaleo = 0;
    if (resistencia <= 0) {
        detenido = true;
        tiempoDetenido = DURACION_ATURDIMIENTO;
    }
}

int Rival::getId() const { return id; }
TipoRival Rival::getTipo() const { return tipo; }
double Rival::getVelocidad() const { return velocidad; }
int Rival::getResistencia() const { return resistencia; }
bool Rival::estaAturdido() const { return aturdimiento > 0; }
bool Rival::estaDetenido() const { return detenido; }
bool Rival::cruzoLinea() const { return !detenido && x <= LINEA_DEFENSA; }

bool Rival::estaActivo() const
{
    if (cruzoLinea()) return false;
    if (detenido && tiempoDetenido <= 0) return false;
    return true;
}

double Rival::getAnguloTambaleo() const
{
    if (tiempoTambaleo < 0) return 0;
    return fisicaTambaleo.angulo(AMPLITUD_TAMBALEO, tiempoTambaleo);
}

Rival *crearRival(TipoRival tipo, int id, int carril)
{
    switch (tipo) {
    case TipoRival::NORMAL:   return new Rival(id, tipo, carril, Rival::VELOCIDAD_BASE, 1);
    case TipoRival::VELOZ:    return new Rival(id, tipo, carril, Rival::VELOCIDAD_BASE * 2, 1);
    case TipoRival::GREGARIO: return new Rival(id, tipo, carril, Rival::VELOCIDAD_BASE, 2);
    }
    throw std::invalid_argument("Tipo de rival desconocido");
}