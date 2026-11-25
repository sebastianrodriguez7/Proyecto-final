#include "logicanivel2.h"
#include "bidonlanzado.h"
#include <cmath>
#include <stdexcept>

LogicaNivel2::LogicaNivel2(int vidas, const std::string &archivoQ, unsigned semilla)
    : egan(NUM_CARRILES, semilla + 2),
    agente(semilla + 1),
    generador(semilla),
    siguienteOleada(0),
    siguienteObjeto(0),
    estado(EstadoNivel2::JUGANDO),
    tiempo(0),
    vidas(vidas),
    detenidos(0),
    cruzaron(0),
    archivoQ(archivoQ),
    cargada(false),
    danada(false),
    guardado(false)
{
    if (vidas < 1)
        throw std::invalid_argument("Se necesita al menos una vida");

    using T = TipoRival;
    oleadas = {
        {5.0,  {T::NORMAL, T::NORMAL}},
        {25.0, {T::NORMAL, T::NORMAL, T::VELOZ}},
        {45.0, {T::NORMAL, T::VELOZ, T::GREGARIO, T::NORMAL}},
        {65.0, {T::VELOZ, T::GREGARIO, T::NORMAL, T::VELOZ, T::GREGARIO}},
        {80.0, {T::GREGARIO, T::VELOZ, T::VELOZ, T::NORMAL, T::GREGARIO, T::VELOZ}}
    };
    eventosObjetos = { {12.0, false}, {32.0, false}, {40.0, true}, {52.0, false}, {72.0, false} };

    if (!this->archivoQ.empty()) {
        try {
            cargada = agente.cargar(this->archivoQ);
        } catch (const std::runtime_error &) {
            danada = true;
        }
    }
}

LogicaNivel2::~LogicaNivel2()
{
    for (Rival *r : rivales) delete r;
    for (ObjetoCarril *o : objetos) delete o;
}

void LogicaNivel2::actualizar(double dt)
{
    if (dt < 0)
        throw std::invalid_argument("dt no puede ser negativo");
    if (estado != EstadoNivel2::JUGANDO)
        return;

    tiempo += dt;

    programarEventos();

    egan.actualizar(dt);
    agente.actualizar(dt);
    enviarRivales();

    for (Rival *r : rivales) r->actualizar(dt);
    bidones.actualizar(dt);

    resolverImpactos();
    resolverRivales();
    actualizarObjetos(dt);
    evaluarFin();
}

void LogicaNivel2::programarEventos()
{
    if (siguienteOleada < oleadas.size() && tiempo >= oleadas[siguienteOleada].tiempo) {
        for (TipoRival t : oleadas[siguienteOleada].tipos)
            cola.push_back(t);
        ++siguienteOleada;
    }

    if (siguienteObjeto < eventosObjetos.size() && tiempo >= eventosObjetos[siguienteObjeto].tiempo) {
        std::uniform_int_distribution<int> carril(1, NUM_CARRILES);
        int c = carril(generador);
        if (eventosObjetos[siguienteObjeto].esGuitarra)
            objetos.push_back(new GuitarraCarril(c));
        else
            objetos.push_back(new Aficionado(c));
        ++siguienteObjeto;
    }
}

void LogicaNivel2::enviarRivales()
{
    if (!cola.empty() && agente.puedeEnviar()) {
        rivales.push_back(agente.enviarRival(egan, cola.front()));
        cola.pop_front();
    }
}

void LogicaNivel2::resolverImpactos()
{
    for (Proyectil *p : bidones.lista()) {
        BidonLanzado *b = dynamic_cast<BidonLanzado *>(p);
        if (b == nullptr || !b->estaActivo() || !b->haImpactado())
            continue;

        for (Rival *r : rivales) {
            if (r->estaDetenido() || r->getCarril() != b->getCarril())
                continue;
            if (std::fabs(r->getX() - b->getX()) < RADIO_IMPACTO)
                r->recibirBidon();
        }
        b->desactivar();
    }
}

void LogicaNivel2::resolverRivales()
{
    for (Rival *r : rivales) {
        int id = r->getId();
        if (resueltos.count(id))
            continue;

        if (r->estaDetenido()) {
            agente.rivalTerminado(id, false);
            ++detenidos;
            resueltos.insert(id);
        } else if (r->cruzoLinea()) {
            agente.rivalTerminado(id, true);
            ++cruzaron;
            --vidas;
            resueltos.insert(id);
        } else if (r->getCarril() == egan.getCarril()
                   && std::fabs(r->getX() - EganDefensor::X) < RADIO_CONTACTO) {
            egan.recibirGolpe();
        }
    }

    for (auto it = rivales.begin(); it != rivales.end(); ) {
        if (!(*it)->estaActivo()) {
            resueltos.erase((*it)->getId());
            delete *it;
            it = rivales.erase(it);
        } else {
            ++it;
        }
    }
}

void LogicaNivel2::actualizarObjetos(double dt)
{
    for (auto it = objetos.begin(); it != objetos.end(); ) {
        ObjetoCarril *o = *it;
        o->actualizar(dt);

        if (o->estaActivo() && o->getCarril() == egan.getCarril() && !egan.estaTambaleando())
            o->ofrecerA(egan);

        if (!o->estaActivo()) {
            delete o;
            it = objetos.erase(it);
        } else {
            ++it;
        }
    }
}

void LogicaNivel2::evaluarFin()
{
    if (vidas <= 0) {
        estado = EstadoNivel2::PERDIDO;
        guardarAprendizaje();
    } else if (tiempo >= DURACION) {
        estado = EstadoNivel2::GANADO;
        guardarAprendizaje();
    }
}

void LogicaNivel2::guardarAprendizaje()
{
    if (archivoQ.empty())
        return;
    try {
        agente.guardar(archivoQ);
        guardado = true;
    } catch (const std::runtime_error &) {
        guardado = false;
    }
}

void LogicaNivel2::cambiarCarril(int direccion) { egan.cambiarCarril(direccion); }
void LogicaNivel2::iniciarCarga() { egan.iniciarCarga(); }

void LogicaNivel2::soltarCarga()
{
    double alcance = 0;
    if (egan.soltarCarga(alcance))
        bidones.agregar(new BidonLanzado(EganDefensor::X, egan.getCarril(), alcance));
}

EstadoNivel2 LogicaNivel2::getEstado() const { return estado; }
double LogicaNivel2::getTiempo() const { return tiempo; }
int LogicaNivel2::getVidas() const { return vidas; }
int LogicaNivel2::getRivalesDetenidos() const { return detenidos; }
int LogicaNivel2::getRivalesCruzaron() const { return cruzaron; }
bool LogicaNivel2::tablaQCargada() const { return cargada; }
bool LogicaNivel2::tablaQDanada() const { return danada; }
bool LogicaNivel2::aprendizajeGuardado() const { return guardado; }

const EganDefensor &LogicaNivel2::getEgan() const { return egan; }
const AgenteDirector &LogicaNivel2::getAgente() const { return agente; }
const std::vector<Rival *> &LogicaNivel2::getRivales() const { return rivales; }
const GestorObstaculos &LogicaNivel2::getBidones() const { return bidones; }
const std::vector<ObjetoCarril *> &LogicaNivel2::getObjetos() const { return objetos; }