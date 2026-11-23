#ifndef ACCION_H
#define ACCION_H

#include "rival.h"

class Accion
{
public:
    static constexpr double ESPERA = 1.2;

    void actualizar(double dt);
    bool lista() const;

    Rival *enviar(TipoRival tipo, int carril, int id);

private:
    double espera = 0;
};

#endif // ACCION_H