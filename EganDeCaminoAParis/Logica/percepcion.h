#ifndef PERCEPCION_H
#define PERCEPCION_H

#include "egandefensor.h"
#include "estado.h"

class Percepcion
{
public:
    Estado percibir(const EganDefensor &egan) const;
};

#endif // PERCEPCION_H