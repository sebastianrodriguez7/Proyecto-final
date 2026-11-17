#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "entidad.h"
#include "fisicaparabolica.h"

class Proyectil : public Entidad
{
public:
    Proyectil(double x, double altura, int carril,
              double velX, double velY, int maxRebotes, double rebote = 0.4);

    void actualizar(double dt) override;

    bool estaActivo() const;
    void desactivar();
    int getRebotes() const;

protected:
    FisicaParabolica fisica;
    bool activo;
    int rebotes;
    int maxRebotes;
};

#endif // PROYECTIL_H