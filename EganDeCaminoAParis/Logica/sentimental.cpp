#include "sentimental.h"
#include <stdexcept>

bool Sentimental::activar()
{
    if (tiempoDesinflado > 0)
        return false;
    tiempoActivo = DURACION;
    return true;
}

void Sentimental::actualizar(double dt)
{
    if (dt < 0)
        throw std::invalid_argument("dt no puede ser negativo");

    if (tiempoActivo > 0) {
        tiempoActivo -= dt;
        if (tiempoActivo <= 0) {
            tiempoActivo = 0;
            tiempoDesinflado = DURACION_DESINFLADO;
        }
    } else if (tiempoDesinflado > 0) {
        tiempoDesinflado -= dt;
        if (tiempoDesinflado < 0)
            tiempoDesinflado = 0;
    }
}

bool Sentimental::estaActivo() const { return tiempoActivo > 0; }
bool Sentimental::estaDesinflado() const { return tiempoDesinflado > 0; }
double Sentimental::getTiempoActivo() const { return tiempoActivo; }
double Sentimental::getTiempoDesinflado() const { return tiempoDesinflado; }