#ifndef OBJETOCARRIL_H
#define OBJETOCARRIL_H

#include "entidad.h"
#include "egandefensor.h"

class ObjetoCarril : public Entidad
{
public:
    static constexpr double DURACION = 6.0;

    explicit ObjetoCarril(int carril, double tiempoDisponible = DURACION);

    void actualizar(double dt) override;

    void ofrecerA(EganDefensor &egan);
    bool estaActivo() const;
    bool fueTomado() const;
    double getTiempoRestante() const;

protected:
    virtual void aplicarEfecto(EganDefensor &egan) = 0;

private:
    double tiempoRestante;
    bool tomado;
};

class Aficionado : public ObjetoCarril
{
public:
    static constexpr int BIDONES_QUE_OFRECE = 2;
    explicit Aficionado(int carril, double tiempoDisponible = DURACION);

protected:
    void aplicarEfecto(EganDefensor &egan) override;
};

class GuitarraCarril : public ObjetoCarril
{
public:
    explicit GuitarraCarril(int carril, double tiempoDisponible = DURACION);

protected:
    void aplicarEfecto(EganDefensor &egan) override;
};

#endif // OBJETOCARRIL_H