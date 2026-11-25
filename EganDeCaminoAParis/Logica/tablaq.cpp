#include "tablaq.h"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <stdexcept>

namespace {
const char *ENCABEZADO = "TABLAQ_V1";
}

void TablaQ::validarCarril(int carril)
{
    if (carril < 1 || carril > ACCIONES)
        throw std::out_of_range("Carril fuera de rango en la tabla Q");
}

double TablaQ::valor(const Estado &estado, int carril) const
{
    validarCarril(carril);
    return q[estado.indice()][carril - 1];
}

void TablaQ::actualizar(const Estado &estado, int carril, double recompensa, double alfa)
{
    validarCarril(carril);
    if (alfa <= 0 || alfa > 1)
        throw std::invalid_argument("Alfa debe estar en (0, 1]");

    double &celda = q[estado.indice()][carril - 1];
    celda += alfa * (recompensa - celda);
}

int TablaQ::mejorCarril(const Estado &estado) const
{
    const auto &fila = q[estado.indice()];
    int mejor = 0;
    for (int c = 1; c < ACCIONES; ++c)
        if (fila[c] > fila[mejor])
            mejor = c;
    return mejor + 1;
}

bool TablaQ::todosCero(const Estado &estado) const
{
    for (double v : q[estado.indice()])
        if (v != 0.0)
            return false;
    return true;
}

int TablaQ::situacionesVistas() const
{
    int vistas = 0;
    for (const auto &fila : q) {
        for (double v : fila) {
            if (v != 0.0) { ++vistas; break; }
        }
    }
    return vistas;
}

void TablaQ::guardar(const std::string &ruta) const
{
    std::ofstream archivo(ruta);
    if (!archivo)
        throw std::runtime_error("No se pudo escribir el archivo de la tabla Q");

    archivo << ENCABEZADO << "\n" << std::setprecision(10);
    for (const auto &fila : q) {
        for (int c = 0; c < ACCIONES; ++c)
            archivo << fila[c] << (c + 1 < ACCIONES ? " " : "\n");
    }
    if (!archivo)
        throw std::runtime_error("Error al escribir la tabla Q");
}

bool TablaQ::cargar(const std::string &ruta)
{
    std::ifstream archivo(ruta);
    if (!archivo)
        return false;

    std::string encabezado;
    archivo >> encabezado;
    if (encabezado != ENCABEZADO)
        throw std::runtime_error("El archivo de la tabla Q esta danado (encabezado)");

    std::array<std::array<double, ACCIONES>, Estado::NUM_ESTADOS> leida{};
    for (auto &fila : leida) {
        for (double &v : fila) {
            if (!(archivo >> v) || !std::isfinite(v) || v < -1.0001 || v > 1.0001)
                throw std::runtime_error("El archivo de la tabla Q esta danado (valores)");
        }
    }
    q = leida;
    return true;
}