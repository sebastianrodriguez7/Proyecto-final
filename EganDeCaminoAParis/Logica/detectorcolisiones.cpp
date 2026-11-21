#include "detectorcolisiones.h"
#include <cmath>

bool DetectorColisiones::hayColision(const Entidad &a, const Entidad &b)
{
    if (a.getCarril() != b.getCarril())
        return false;
    return std::fabs(a.getX() - b.getX()) < ANCHO_GOLPE
           && std::fabs(a.getY() - b.getY()) < ALTURA_GOLPE;
}

int DetectorColisiones::procesarGolpes(Egan &egan, const GestorObstaculos &gestor)
{
    int registrados = 0;
    for (Proyectil *p : gestor.lista()) {
        if (!p->estaActivo()) continue;
        if (hayColision(egan, *p) && egan.recibirGolpe(AMPLITUD_GOLPE)) {
            p->desactivar();
            ++registrados;
        }
    }    
    return registrados;
}

int DetectorColisiones::procesarRecogidas(Egan &egan, const GestorColeccionables &gestor)
{
    int recogidos = 0;
    for (Coleccionable *c : gestor.lista()) {
        if (c->estaRecogido()) continue;
        if (hayColision(egan, *c)) {
            c->recoger(egan);
            ++recogidos;
        }
    }
    return recogidos;
}