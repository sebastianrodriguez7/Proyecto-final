#ifndef DETECTORCOLISIONES_H
#define DETECTORCOLISIONES_H

#include "entidad.h"
#include "egan.h"
#include "gestorobstaculos.h"

class DetectorColisiones
{
public:
    static constexpr double ANCHO_GOLPE = 1.0;
    static constexpr double ALTURA_GOLPE = 1.0;
    static constexpr double AMPLITUD_GOLPE = 15.0;

    static bool hayColision(const Entidad &a, const Entidad &b);

    static int procesarGolpes(Egan &egan, const GestorObstaculos &gestor);
};

#endif // DETECTORCOLISIONES_H