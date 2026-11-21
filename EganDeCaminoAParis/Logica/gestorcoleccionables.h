#ifndef GESTORCOLECCIONABLES_H
#define GESTORCOLECCIONABLES_H

#include <vector>
#include "coleccionable.h"

class GestorColeccionables
{
public:
    GestorColeccionables() = default;
    ~GestorColeccionables();

    GestorColeccionables(const GestorColeccionables &) = delete;
    GestorColeccionables &operator=(const GestorColeccionables &) = delete;

    void agregar(Coleccionable *c);
    void eliminarRecogidos();
    void descartarAntesDe(double xLimite);
    void limpiar();

    int cantidad() const;
    const std::vector<Coleccionable *> &lista() const;

private:
    std::vector<Coleccionable *> items;
};

#endif // GESTORCOLECCIONABLES_H