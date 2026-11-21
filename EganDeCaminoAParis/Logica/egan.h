#ifndef EGAN_H
#define EGAN_H

#include <random>
#include "entidad.h"
#include "fisicapendiente.h"
#include "fisicaparabolica.h"
#include "fisicaoscilatoria.h"
#include "sentimental.h"

class Egan : public Entidad
{
public:
    Egan(double x, double y, int carril, int numCarriles,
         unsigned semilla = std::random_device{}());

    void actualizar(double dt) override;

    void cambiarCarril(int direccion);
    void setPedaleoFuerte(bool activo);
    void setPendiente(double grados);
    void recuperarEnergia(double cantidad);
    void setMultiplicadorConsumo(double multiplicador);

    void saltar();
    bool recibirGolpe(double amplitud);
    bool activarSentimental();

    double getEnergia() const;
    bool estaEnPajara() const;
    double getPendiente() const;
    bool estaSaltando() const;
    bool estaTambaleando() const;
    int getGolpes() const;
    double getAnguloTambaleo() const;

    bool estaSentimental() const;
    bool estaDesinflado() const;
    double getTiempoSentimental() const;
    double getTiempoDesinflado() const;
    double getVelocidadEfectiva() const;

    static constexpr double ENERGIA_MAX = 100.0;
    static constexpr double CONSUMO_PEDALEO = 15.0;
    static constexpr double DURACION_PAJARA = 3.0;
    static constexpr double ENERGIA_TRAS_PAJARA = 15.0;

    static constexpr double A_PEDAL_NORMAL = 2.0;
    static constexpr double A_PEDAL_FUERTE = 4.0;
    static constexpr double UMBRAL_ESCALADOR = 6.0;
    static constexpr double BONO_ESCALADOR = 1.25;
    static constexpr double VELOCIDAD_PAJARA = 0.3;

    static constexpr double V_SALTO = 6.0;
    static constexpr double DURACION_TAMBALEO = 1.5;
    static constexpr double FACTOR_VELOCIDAD_GOLPE = 0.5;

    static constexpr double FACTOR_VELOCIDAD_SENTIMENTAL = 1.2;
    static constexpr double PROB_CRUCE_CARRIL = 0.4;

private:
    double calcularAPedal() const;

    int numCarriles;
    double energia;
    bool pedaleoFuerte;
    bool enPajara;
    double tiempoPajara;
    double pendiente;
    FisicaPendiente fisica;

    FisicaParabolica fisicaSalto;
    bool enSalto;

    FisicaOscilatoria fisicaTambaleo;
    bool tambaleando;
    double amplitudTambaleo;
    double tiempoTambaleo;
    int golpes;
    double multiplicadorConsumo;

    Sentimental sentimental;
    std::mt19937 generador;
};

#endif // EGAN_H