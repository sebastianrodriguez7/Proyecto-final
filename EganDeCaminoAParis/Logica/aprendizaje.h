#ifndef APRENDIZAJE_H
#define APRENDIZAJE_H

#include <cstddef>
#include <deque>
#include "estado.h"
#include "tablaq.h"

class Aprendizaje
{
public:
    static constexpr double ALPHA = 0.2;
    static constexpr std::size_t MAX_EXPERIENCIAS = 20;
    static constexpr std::size_t MIN_PARA_AJUSTAR = 5;
    static constexpr double UMBRAL_EXITO = 0.3;
    static constexpr double PASO_EPSILON = 0.02;
    static constexpr double EPSILON_INICIAL = 0.3;
    static constexpr double EPSILON_MIN = 0.05;
    static constexpr double EPSILON_MAX = 0.5;

    static double calcularRecompensa(bool cruzoLinea);

    void registrar(const Experiencia &experiencia, TablaQ &tabla);
    double tasaExito() const;
    double ajustarEpsilon(double epsilon) const;
    std::size_t cantidad() const;

private:
    std::deque<Experiencia> recientes;
};

#endif // APRENDIZAJE_H