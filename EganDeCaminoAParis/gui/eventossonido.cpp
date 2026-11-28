#include "eventossonido.h"
#include "guitarra.h"
#include <algorithm>

std::string nombreSonido(Sonido sonido)
{
    switch (sonido) {
    case Sonido::SALTO:          return "salto";
    case Sonido::GOLPE:          return "golpe";
    case Sonido::BIDON:          return "bidon";
    case Sonido::GUITARRA:       return "guitarra";
    case Sonido::PAJARA:         return "pajara";
    case Sonido::DESINFLADO:     return "desinflado";
    case Sonido::VICTORIA:       return "victoria";
    case Sonido::DERROTA:        return "derrota";
    case Sonido::LANZAR:         return "lanzar";
    case Sonido::IMPACTO:        return "impacto";
    case Sonido::RIVAL_DETENIDO: return "rival_detenido";
    case Sonido::VIDA_PERDIDA:   return "vida_perdida";
    case Sonido::AFICIONADO:     return "aficionado";
    }
    return "";
}


std::vector<Sonido> DetectorSonidosNivel1::comparar(const LogicaNivel1 &nivel)
{
    std::vector<Sonido> eventos;
    const Egan &egan = nivel.getEgan();

    std::vector<Visto> ahora;
    for (const Coleccionable *c : nivel.getColeccionables().lista())
        ahora.push_back({c, c->getX(), dynamic_cast<const Guitarra *>(c) != nullptr});

    if (iniciado) {
        if (egan.estaSaltando() && !saltando)
            eventos.push_back(Sonido::SALTO);
        if (egan.getGolpes() > golpes)
            eventos.push_back(Sonido::GOLPE);
        if (egan.estaEnPajara() && !pajara)
            eventos.push_back(Sonido::PAJARA);
        if (egan.estaDesinflado() && !desinflado)
            eventos.push_back(Sonido::DESINFLADO);

        for (const Visto &antes : coleccionables) {
            bool sigue = std::any_of(ahora.begin(), ahora.end(),
                                     [&](const Visto &v) { return v.puntero == antes.puntero; });
            if (!sigue && antes.x > egan.getX() - 3.0)
                eventos.push_back(antes.esGuitarra ? Sonido::GUITARRA : Sonido::BIDON);
        }

        if (nivel.getEstado() != estado) {
            if (nivel.getEstado() == EstadoNivel::GANADO)
                eventos.push_back(Sonido::VICTORIA);
            else if (nivel.getEstado() == EstadoNivel::PERDIDO_TIEMPO)
                eventos.push_back(Sonido::DERROTA);
        }
    }

    iniciado = true;
    golpes = egan.getGolpes();
    saltando = egan.estaSaltando();
    pajara = egan.estaEnPajara();
    desinflado = egan.estaDesinflado();
    estado = nivel.getEstado();
    coleccionables = ahora;
    return eventos;
}


std::vector<Sonido> DetectorSonidosNivel2::comparar(const LogicaNivel2 &nivel)
{
    std::vector<Sonido> eventos;

    std::vector<const Proyectil *> bidonesAhora;
    for (const Proyectil *p : nivel.getBidones().lista())
        bidonesAhora.push_back(p);

    std::vector<VistoObjeto> objetosAhora;
    for (const ObjetoCarril *o : nivel.getObjetos())
        objetosAhora.push_back({o, dynamic_cast<const GuitarraCarril *>(o) != nullptr, o->getTiempoRestante()});

    if (iniciado) {
        for (const Proyectil *p : bidonesAhora)
            if (std::find(bidones.begin(), bidones.end(), p) == bidones.end())
                eventos.push_back(Sonido::LANZAR);
        for (const Proyectil *p : bidones)
            if (std::find(bidonesAhora.begin(), bidonesAhora.end(), p) == bidonesAhora.end())
                eventos.push_back(Sonido::IMPACTO);

        if (nivel.getEgan().getGolpes() > golpes)
            eventos.push_back(Sonido::GOLPE);
        for (int i = detenidos; i < nivel.getRivalesDetenidos(); ++i)
            eventos.push_back(Sonido::RIVAL_DETENIDO);
        for (int i = cruzaron; i < nivel.getRivalesCruzaron(); ++i)
            eventos.push_back(Sonido::VIDA_PERDIDA);

        for (const VistoObjeto &antes : objetos) {
            bool sigue = std::any_of(objetosAhora.begin(), objetosAhora.end(),
                                     [&](const VistoObjeto &v) { return v.puntero == antes.puntero; });
            if (!sigue && antes.restante > 0.1)
                eventos.push_back(antes.esGuitarra ? Sonido::GUITARRA : Sonido::AFICIONADO);
        }

        if (nivel.getEstado() != estado) {
            if (nivel.getEstado() == EstadoNivel2::GANADO)
                eventos.push_back(Sonido::VICTORIA);
            else if (nivel.getEstado() == EstadoNivel2::PERDIDO)
                eventos.push_back(Sonido::DERROTA);
        }
    }

    iniciado = true;
    golpes = nivel.getEgan().getGolpes();
    detenidos = nivel.getRivalesDetenidos();
    cruzaron = nivel.getRivalesCruzaron();
    estado = nivel.getEstado();
    bidones = bidonesAhora;
    objetos = objetosAhora;
    return eventos;
}