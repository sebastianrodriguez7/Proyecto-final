#include "gestorcoleccionables.h"
#include <stdexcept>

GestorColeccionables::~GestorColeccionables()
{
    limpiar();
}

void GestorColeccionables::agregar(Coleccionable *c)
{
    if (c == nullptr)
        throw std::invalid_argument("No se puede agregar un coleccionable nulo");
    items.push_back(c);
}

void GestorColeccionables::eliminarRecogidos()
{
    for (auto it = items.begin(); it != items.end(); ) {
        if ((*it)->estaRecogido()) {
            delete *it;
            it = items.erase(it);
        } else {
            ++it;
        }
    }
}

void GestorColeccionables::descartarAntesDe(double xLimite)
{
    for (auto it = items.begin(); it != items.end(); ) {
        if ((*it)->getX() < xLimite) {
            delete *it;
            it = items.erase(it);
        } else {
            ++it;
        }
    }
}

void GestorColeccionables::limpiar()
{
    for (Coleccionable *c : items)
        delete c;
    items.clear();
}

int GestorColeccionables::cantidad() const { return static_cast<int>(items.size()); }
const std::vector<Coleccionable *> &GestorColeccionables::lista() const { return items; }