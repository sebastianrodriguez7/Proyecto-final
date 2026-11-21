#ifndef GUITARRA_H
#define GUITARRA_H

#include "coleccionable.h"

class Guitarra : public Coleccionable
{
public:
    Guitarra(double x, int carril);

protected:
    void aplicarEfecto(Egan &egan) override;
};

#endif // GUITARRA_H