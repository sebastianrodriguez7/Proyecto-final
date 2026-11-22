#ifndef EGANDEFENSOR_H
#define EGANDEFENSOR_H

#include <random>
#include "entidad.h"
#include "fisicaoscilatoria.h"
#include "sentimental.h"

class EganDefensor : public Entidad
{
public:
    explicit EganDefensor(int numCarriles = 5, unsigned semilla = std::random_device{}());

    void actualizar(double dt) override;

    void cambiarCarril(int direccion);
    void iniciarCarga();
    bool soltarCarga(double &alcance);
    void recibirBidones(int cantidad);
    bool recibirGolpe();
    bool activarSentimental();

    int getBidones() const;
    bool estaCargando() const;
    double getTiempoCarga() const;
    double getAlcancePrevisto() const;
    double getProgresoRecarga() const;
    bool estaTambaleando() const;
    double getAnguloTambaleo() const;
    int getGolpes() const;
    bool estaSentimental() const;
    bool estaDesinflado() const;
    double getTiempoSentimental() const;

    static constexpr double X = 3.0;
    static constexpr int MAX_BIDONES = 5;
    static constexpr double RECARGA_BASE = 3.0;
    static constexpr double FACTOR_ESCALADOR = 1.15;
    static constexpr double ALCANCE_MIN = 5.0;
    static constexpr double ALCANCE_MAX = 30.0;
    static constexpr double TIEMPO_CARGA_MAX = 1.5;
    static constexpr double ERROR_SENTIMENTAL = 0.25;
    static constexpr double DURACION_TAMBALEO = 1.5;
    static constexpr double AMPLITUD_TAMBALEO = 15.0;

private:
    double alcanceParaCarga(double tiempo) const;

    int numCarriles;
    int bidones;
    double tiempoRecarga;
    bool cargando;
    double tiempoCarga;

    bool tambaleando;
    double tiempoTambaleo;
    int golpes;
    FisicaOscilatoria fisicaTambaleo;

    Sentimental sentimental;
    std::mt19937 generador;
};

#endif // EGANDEFENSOR_H