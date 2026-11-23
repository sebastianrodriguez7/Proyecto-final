#include "accion.h"
#include <stdexcept>

void Accion::actualizar(double dt)
{
    if (espera > 0)
        espera -= dt;
}

bool Accion::lista() const { return espera <= 0; }

Rival *Accion::enviar(TipoRival tipo, int carril, int id)
{
    if (!lista())
        throw std::logic_error("La accion todavia esta en espera");
    espera = ESPERA;
    return crearRival(tipo, id, carril);
}