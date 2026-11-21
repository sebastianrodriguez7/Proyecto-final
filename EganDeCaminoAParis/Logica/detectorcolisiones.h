#ifndef DETECTORCOLISIONES_H
#define DETECTORCOLISIONES_H

#include "entidad.h"
#include "egan.h"
#include "gestorobstaculos.h"
#include "gestorcoleccionables.h"

class DetectorColisiones
{
public:
    static constexpr double ANCHO_GOLPE = 1.0;
    static constexpr double ALTURA_GOLPE = 1.0;
    static constexpr double AMPLITUD_GOLPE = 15.0;

    static bool hayColision(const Entidad &a, const Entidad &b);

    static int procesarGolpes(Egan &egan, const GestorObstaculos &gestor);

    static int procesarRecogidas(Egan &egan, const GestorColeccionables &gestor);
};

#endif // DETECTORCOLISIONES_H