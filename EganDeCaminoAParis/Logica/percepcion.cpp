#include "percepcion.h"

Estado Percepcion::percibir(const EganDefensor &egan) const
{
    return Estado(egan.getCarril(), egan.getBidones());
}