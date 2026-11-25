#ifndef TABLAQ_H
#define TABLAQ_H

#include <array>
#include <string>
#include "estado.h"


class TablaQ
{
public:
    static constexpr int ACCIONES = 5;

    double valor(const Estado &estado, int carril) const;
    void actualizar(const Estado &estado, int carril, double recompensa, double alfa);

    int mejorCarril(const Estado &estado) const;
    bool todosCero(const Estado &estado) const;
    int situacionesVistas() const;

    void guardar(const std::string &ruta) const;
    bool cargar(const std::string &ruta);

private:
    static void validarCarril(int carril);

    std::array<std::array<double, ACCIONES>, Estado::NUM_ESTADOS> q{};
};

#endif // TABLAQ_H