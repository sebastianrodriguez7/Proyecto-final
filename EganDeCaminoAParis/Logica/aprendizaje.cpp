#include "aprendizaje.h"
#include <algorithm>

double Aprendizaje::calcularRecompensa(bool cruzoLinea)
{
    return cruzoLinea ? 1.0 : -1.0;
}

void Aprendizaje::registrar(const Experiencia &experiencia, TablaQ &tabla)
{
    tabla.actualizar(experiencia.estado, experiencia.carrilElegido, experiencia.recompensa, ALPHA);

    recientes.push_back(experiencia);
    if (recientes.size() > MAX_EXPERIENCIAS)
        recientes.pop_front();
}

double Aprendizaje::tasaExito() const
{
    if (recientes.empty())
        return 0.0;
    int exitos = 0;
    for (const Experiencia &e : recientes)
        if (e.recompensa > 0)
            ++exitos;
    return static_cast<double>(exitos) / static_cast<double>(recientes.size());
}

double Aprendizaje::ajustarEpsilon(double epsilon) const
{
    if (recientes.size() < MIN_PARA_AJUSTAR)
        return epsilon;

    if (tasaExito() < UMBRAL_EXITO)
        return std::min(EPSILON_MAX, epsilon + PASO_EPSILON);
    return std::max(EPSILON_MIN, epsilon - PASO_EPSILON);
}

std::size_t Aprendizaje::cantidad() const { return recientes.size(); }