#ifndef RIVAL_H
#define RIVAL_H

#include "entidad.h"
#include "fisicaoscilatoria.h"

enum class TipoRival { NORMAL, VELOZ, GREGARIO };

class Rival : public Entidad
{
public:
    static constexpr double X_INICIAL = 36.0;
    static constexpr double LINEA_DEFENSA = 0.0;
    static constexpr double VELOCIDAD_BASE = 3.5;
    static constexpr double DURACION_ATURDIMIENTO = 1.5;
    static constexpr double EMPUJE_INICIAL = 6.0;
    static constexpr double ROZAMIENTO_EMPUJE = 2.5;
    static constexpr double AMPLITUD_TAMBALEO = 20.0;

    Rival(int id, TipoRival tipo, int carril, double velocidad, int resistencia,
          double x = X_INICIAL);

    void actualizar(double dt) override;
    void recibirBidon();

    int getId() const;
    TipoRival getTipo() const;
    double getVelocidad() const;
    int getResistencia() const;
    bool estaAturdido() const;
    bool estaDetenido() const;
    bool cruzoLinea() const;
    bool estaActivo() const;
    double getAnguloTambaleo() const;

private:
    int id;
    TipoRival tipo;
    double velocidad;
    int resistencia;

    double aturdimiento;
    bool detenido;
    double tiempoDetenido;
    double empuje;
    double tiempoTambaleo;
    FisicaOscilatoria fisicaTambaleo;
};

Rival *crearRival(TipoRival tipo, int id, int carril);

#endif // RIVAL_H