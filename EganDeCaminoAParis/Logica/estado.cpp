#include "estado.h"
#include <stdexcept>

Estado::Estado(int carrilEgan, int bidones)
    : carrilEgan(carrilEgan), pocos(bidones < UMBRAL_POCOS)
{
    if (carrilEgan < 1 || carrilEgan > NUM_CARRILES)
        throw std::out_of_range("Carril de Egan fuera de rango en el Estado");
    if (bidones < 0)
        throw std::invalid_argument("La cantidad de bidones no puede ser negativa");
}

int Estado::getCarrilEgan() const { return carrilEgan; }
bool Estado::pocosBidones() const { return pocos; }

std::string Estado::clave() const
{
    return std::to_string(carrilEgan) + (pocos ? "|pocos" : "|muchos");
}

int Estado::indice() const
{
    return (carrilEgan - 1) * 2 + (pocos ? 0 : 1);
}