#ifndef EGAN_H
#define EGAN_H

#include "entidad.h"

    class Egan : public Entidad
{
public:

    Egan(double x, double y, int carril, int numCarriles);

    void actualizar(double dt) override;

    void cambiarCarril(int direccion);
    void setPedaleoFuerte(bool activo);
    void recuperarEnergia(double cantidad);

    double getEnergia() const;
    bool estaEnPajara() const;

    static constexpr double ENERGIA_MAX = 100.0;
    static constexpr double CONSUMO_PEDALEO = 15.0;
    static constexpr double DURACION_PAJARA = 3.0;
    static constexpr double ENERGIA_TRAS_PAJARA = 15.0;

private:
    int numCarriles;
    double energia;
    bool pedaleoFuerte;
    bool enPajara;
    double tiempoPajara;
};

#endif // EGAN_H