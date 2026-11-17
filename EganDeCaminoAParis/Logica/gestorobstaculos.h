#ifndef GESTOROBSTACULOS_H
#define GESTOROBSTACULOS_H

#include <vector>
#include "proyectil.h"

class GestorObstaculos
{
public:
    GestorObstaculos() = default;
    ~GestorObstaculos();


    GestorObstaculos(const GestorObstaculos &) = delete;
    GestorObstaculos &operator=(const GestorObstaculos &) = delete;

    void agregar(Proyectil *p);
    void actualizar(double dt);
    void limpiar();

    int cantidad() const;
    const std::vector<Proyectil *> &lista() const;

private:
    std::vector<Proyectil *> obstaculos;
};

#endif // GESTOROBSTACULOS_H