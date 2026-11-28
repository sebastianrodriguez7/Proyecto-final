#ifndef EVENTOSSONIDO_H
#define EVENTOSSONIDO_H

#include <string>
#include <vector>
#include "logicanivel1.h"
#include "logicanivel2.h"

enum class Sonido
{
    SALTO, GOLPE, BIDON, GUITARRA, PAJARA, DESINFLADO, VICTORIA, DERROTA,
    LANZAR, IMPACTO, RIVAL_DETENIDO, VIDA_PERDIDA, AFICIONADO
};

std::string nombreSonido(Sonido sonido);

class DetectorSonidosNivel1
{
public:
    std::vector<Sonido> comparar(const LogicaNivel1 &nivel);

private:
    struct Visto { const Coleccionable *puntero; double x; bool esGuitarra; };

    bool iniciado = false;
    int golpes = 0;
    bool saltando = false;
    bool pajara = false;
    bool desinflado = false;
    EstadoNivel estado = EstadoNivel::JUGANDO;
    std::vector<Visto> coleccionables;
};

class DetectorSonidosNivel2
{
public:
    std::vector<Sonido> comparar(const LogicaNivel2 &nivel);

private:
    struct VistoObjeto { const ObjetoCarril *puntero; bool esGuitarra; double restante; };

    bool iniciado = false;
    int golpes = 0;
    int detenidos = 0;
    int cruzaron = 0;
    EstadoNivel2 estado = EstadoNivel2::JUGANDO;
    std::vector<const Proyectil *> bidones;
    std::vector<VistoObjeto> objetos;
};

#endif // EVENTOSSONIDO_H