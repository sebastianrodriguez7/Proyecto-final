#include "proyeccion.h"
#include <stdexcept>

namespace {
const double N1_PIES[3]   = {470.0, 560.0, 660.0};
const double N1_TOPE[3]   = {385.0, 480.0, 575.0};
const double N1_FONDO[3]  = {480.0, 575.0, 690.0};
const double N1_ESCALA[3] = {0.80, 0.90, 1.00};

const double N2_PIES[5]   = {330.0, 415.0, 505.0, 600.0, 700.0};
const double N2_TOPE[5]   = {255.0, 340.0, 425.0, 515.0, 610.0};
const double N2_FONDO[5]  = {340.0, 425.0, 515.0, 610.0, 710.0};
const double N2_ESCALA[5] = {0.80, 0.85, 0.90, 0.95, 1.00};

double interpolar(const double *tabla, int n, double carril)
{
    const double margen = 1e-9;
    if (carril < 1.0 - margen || carril > n + margen)
        throw std::out_of_range("Carril fuera de rango en la proyeccion");
    if (carril < 1.0) carril = 1.0;
    if (carril > n) carril = n;

    int bajo = static_cast<int>(carril);          // 1..n
    if (bajo >= n)
        return tabla[n - 1];
    double fraccion = carril - bajo;
    return tabla[bajo - 1] + (tabla[bajo] - tabla[bajo - 1]) * fraccion;
}

double entero(const double *tabla, int n, int carril)
{
    if (carril < 1 || carril > n)
        throw std::out_of_range("Carril fuera de rango en la proyeccion");
    return tabla[carril - 1];
}
}

PuntoPantalla Proyeccion::nivel1(double xMundo, double xCamara, double carril, double altura)
{
    PuntoPantalla p;
    p.escala = interpolar(N1_ESCALA, 3, carril);
    p.x = N1_X_EGAN + (xMundo - xCamara) * N1_PIXELES_POR_METRO;
    p.y = interpolar(N1_PIES, 3, carril) - altura * N1_PIXELES_ALTURA * p.escala;
    return p;
}

double Proyeccion::nivel1Tope(int carril) { return entero(N1_TOPE, 3, carril); }
double Proyeccion::nivel1Fondo(int carril) { return entero(N1_FONDO, 3, carril); }

PuntoPantalla Proyeccion::nivel2(double xMundo, double carril, double altura)
{
    PuntoPantalla p;
    p.escala = interpolar(N2_ESCALA, 5, carril);
    p.x = N2_X_LINEA + xMundo * N2_PIXELES_POR_METRO;
    p.y = interpolar(N2_PIES, 5, carril) - altura * N2_PIXELES_POR_METRO * p.escala;
    return p;
}

double Proyeccion::nivel2Tope(int carril) { return entero(N2_TOPE, 5, carril); }
double Proyeccion::nivel2Fondo(int carril) { return entero(N2_FONDO, 5, carril); }