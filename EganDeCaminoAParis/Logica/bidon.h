#ifndef BIDON_H
#define BIDON_H

#include "coleccionable.h"

class Bidon : public Coleccionable
{
public:
    static constexpr double ENERGIA_POR_BIDON = 15.0;

    Bidon(double x, int carril, double energia = ENERGIA_POR_BIDON);

    double getEnergia() const;

protected:
    void aplicarEfecto(Egan &egan) override;

private:
    double energia;
};

#endif // BIDON_H