#ifndef EGAN_H
#define EGAN_H

#include "entidad.h"
#include "fisicapendiente.h"

class Egan : public Entidad
{
public:
    Egan(double x, double y, int carril, int numCarriles);

    void actualizar(double dt) override;

    void cambiarCarril(int direccion);
    void setPedaleoFuerte(bool activo);
    void setPendiente(double grados);
    void recuperarEnergia(double cantidad);

    double getEnergia() const;
    bool estaEnPajara() const;
    double getPendiente() const;

    static constexpr double ENERGIA_MAX = 100.0;
    static constexpr double CONSUMO_PEDALEO = 15.0;
    static constexpr double DURACION_PAJARA = 3.0;
    static constexpr double ENERGIA_TRAS_PAJARA = 15.0;

    static constexpr double A_PEDAL_NORMAL = 2.0;
    static constexpr double A_PEDAL_FUERTE = 4.0;
    static constexpr double UMBRAL_ESCALADOR = 6.0;
    static constexpr double BONO_ESCALADOR = 1.25;
    static constexpr double VELOCIDAD_PAJARA = 0.3;

private:
    double calcularAPedal() const;

    int numCarriles;
    double energia;
    bool pedaleoFuerte;
    bool enPajara;
    double tiempoPajara;
    double pendiente;
    FisicaPendiente fisica;
};

#endif // EGAN_H