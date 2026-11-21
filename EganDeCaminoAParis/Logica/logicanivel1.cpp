#include "logicanivel1.h"
#include "granizo.h"
#include "roca.h"
#include "detectorcolisiones.h"
#include <stdexcept>

LogicaNivel1::LogicaNivel1(Dificultad dificultad, unsigned semilla)
    : egan(0, 0, 2, NUM_CARRILES),
    estrategia(EstrategiaDificultad::crear(dificultad)),
    generador(semilla),
    estado(EstadoNivel::JUGANDO),
    tiempo(0),
    tiempoFinal(0),
    tiemposLluvia{20.0, 45.0, 70.0, 95.0},
    siguienteLluvia(0),
    granizosPendientes(0),
    temporizadorGranizo(0),
    indiceGranizo(0),
    derrumbeHecho(false)
{
    egan.setMultiplicadorConsumo(estrategia->multiplicadorConsumo());
    construirTerreno();
}

void LogicaNivel1::construirTerreno()
{
    terreno.agregarSegmento(30, 2);
    terreno.agregarSegmento(30, 4);
    terreno.agregarSegmento(30, 5);
    terreno.agregarSegmento(30, 8);
    terreno.agregarSegmento(30, 10);
    terreno.agregarSegmento(30, 7);
    terreno.agregarSegmento(30, 9);
    terreno.agregarSegmento(30, 3);
    terreno.agregarSegmento(15, 6);
    terreno.agregarSegmento(5, 5);
}

void LogicaNivel1::actualizar(double dt)
{
    if (dt < 0)
        throw std::invalid_argument("dt no puede ser negativo");
    if (estado != EstadoNivel::JUGANDO)
        return;

    tiempo += dt;

    programarEventos();
    lanzarGranizosPendientes(dt);

    egan.setPendiente(terreno.anguloEn(egan.getX()));
    egan.actualizar(dt);

    gestor.actualizar(dt);
    DetectorColisiones::procesarGolpes(egan, gestor);
    descartarObstaculosAtrasados();

    evaluarFin();
}

void LogicaNivel1::programarEventos()
{
    if (siguienteLluvia < tiemposLluvia.size() && tiempo >= tiemposLluvia[siguienteLluvia]) {
        granizosPendientes = estrategia->granizosPorLluvia();
        temporizadorGranizo = 0;
        indiceGranizo = 0;
        ++siguienteLluvia;
    }
    if (!derrumbeHecho && tiempo >= TIEMPO_DERRUMBE) {
        lanzarDerrumbe();
        derrumbeHecho = true;
    }
}

void LogicaNivel1::lanzarGranizosPendientes(double dt)
{
    if (granizosPendientes <= 0)
        return;
    temporizadorGranizo -= dt;
    if (temporizadorGranizo <= 0) {
        lanzarGranizo(indiceGranizo);
        ++indiceGranizo;
        --granizosPendientes;
        temporizadorGranizo = INTERVALO_GRANIZO;
    }
}

void LogicaNivel1::lanzarGranizo(int indice)
{
    double aviso = estrategia->tiempoAvisoGranizo();
    int carril = estrategia->elegirCarril(egan, NUM_CARRILES, indice, generador);
    double x = egan.getX() + egan.getVx() * aviso;
    gestor.agregar(new Granizo(x, carril, aviso));
}

void LogicaNivel1::lanzarDerrumbe()
{
    int cantidad = estrategia->rocasEnDerrumbe();
    for (int i = 0; i < cantidad; ++i) {
        int carril = estrategia->elegirCarril(egan, NUM_CARRILES, i, generador);
        double x = egan.getX() + DISTANCIA_DERRUMBE + i * SEPARACION_ROCAS;
        gestor.agregar(new Roca(x, carril, estrategia->velocidadRoca()));
    }
}

void LogicaNivel1::descartarObstaculosAtrasados()
{
    for (Proyectil *p : gestor.lista()) {
        if (p->getX() < egan.getX() - DISTANCIA_DESCARTE)
            p->desactivar();
    }
}

void LogicaNivel1::evaluarFin()
{
    bool llegoCumbre = egan.getX() >= terreno.longitudTotal();
    if (llegoCumbre && egan.getEnergia() > 0) {
        estado = EstadoNivel::GANADO;
        tiempoFinal = tiempo;
    } else if (tiempo >= TIEMPO_LIMITE) {
        estado = EstadoNivel::PERDIDO_TIEMPO;
    }
}

void LogicaNivel1::cambiarCarril(int direccion) { egan.cambiarCarril(direccion); }
void LogicaNivel1::saltar() { egan.saltar(); }
void LogicaNivel1::pedalearFuerte(bool activo) { egan.setPedaleoFuerte(activo); }

EstadoNivel LogicaNivel1::getEstado() const { return estado; }
double LogicaNivel1::getTiempo() const { return tiempo; }

double LogicaNivel1::getTiempoRestante() const
{
    double restante = TIEMPO_LIMITE - tiempo;
    return restante > 0 ? restante : 0;
}

double LogicaNivel1::getDistanciaRestante() const
{
    double d = terreno.longitudTotal() - egan.getX();
    return d > 0 ? d : 0;
}

double LogicaNivel1::getPendienteActual() const { return egan.getPendiente(); }

int LogicaNivel1::estrellas() const
{
    if (estado != EstadoNivel::GANADO)
        return 0;
    int e = 1;
    if (TIEMPO_LIMITE - tiempoFinal > HOLGURA_ESTRELLA) ++e;
    if (egan.getGolpes() <= GOLPES_MAX_ESTRELLA) ++e;
    return e;
}

int LogicaNivel1::vidasNivel2() const { return 2 + estrellas(); }
std::string LogicaNivel1::nombreDificultad() const { return estrategia->nombre(); }

const Egan &LogicaNivel1::getEgan() const { return egan; }
const Terreno &LogicaNivel1::getTerreno() const { return terreno; }
const GestorObstaculos &LogicaNivel1::getGestor() const { return gestor; }