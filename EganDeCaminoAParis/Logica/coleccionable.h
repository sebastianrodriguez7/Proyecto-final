#ifndef COLECCIONABLE_H
#define COLECCIONABLE_H

#include "entidad.h"
#include "egan.h"

class Coleccionable : public Entidad
{
public:
    Coleccionable(double x, int carril);

    void actualizar(double dt) override;

    void recoger(Egan &egan);
    bool estaRecogido() const;

protected:
    virtual void aplicarEfecto(Egan &egan) = 0;

private:
    bool recogido;
};

#endif // COLECCIONABLE_H