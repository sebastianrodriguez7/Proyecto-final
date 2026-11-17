#include "gestorobstaculos.h"
#include <stdexcept>

GestorObstaculos::~GestorObstaculos()
{
    limpiar();
}

void GestorObstaculos::agregar(Proyectil *p)
{
    if (p == nullptr)
        throw std::invalid_argument("No se puede agregar un obstaculo nulo");
    obstaculos.push_back(p);
}

void GestorObstaculos::actualizar(double dt)
{
    for (auto it = obstaculos.begin(); it != obstaculos.end(); ) {
        (*it)->actualizar(dt);
        if (!(*it)->estaActivo()) {
            delete *it;
            it = obstaculos.erase(it);
        } else {
            ++it;
        }
    }
}

void GestorObstaculos::limpiar()
{
    for (Proyectil *p : obstaculos)
        delete p;
    obstaculos.clear();
}

int GestorObstaculos::cantidad() const { return static_cast<int>(obstaculos.size()); }
const std::vector<Proyectil *> &GestorObstaculos::lista() const { return obstaculos; }