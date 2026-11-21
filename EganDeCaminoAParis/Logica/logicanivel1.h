#ifndef LOGICANIVEL1_H
#define LOGICANIVEL1_H

#include <memory>
#include <random>
#include <vector>
#include "egan.h"
#include "terreno.h"
#include "gestorobstaculos.h"
#include "estrategiadificultad.h"

enum class EstadoNivel { JUGANDO, GANADO, PERDIDO_TIEMPO };

class LogicaNivel1
{
public:
    static constexpr int NUM_CARRILES = 3;
    static constexpr double TIEMPO_LIMITE = 120.0;
    static constexpr double HOLGURA_ESTRELLA = 20.0;
    static constexpr int GOLPES_MAX_ESTRELLA = 2;
    static constexpr double TIEMPO_DERRUMBE = 60.0;
    static constexpr double INTERVALO_GRANIZO = 0.7;
    static constexpr double DISTANCIA_DERRUMBE = 25.0;
    static constexpr double SEPARACION_ROCAS = 8.0;
    static constexpr double DISTANCIA_DESCARTE = 6.0;

    explicit LogicaNivel1(Dificultad dificultad, unsigned semilla = std::random_device{}());

    LogicaNivel1(const LogicaNivel1 &) = delete;
    LogicaNivel1 &operator=(const LogicaNivel1 &) = delete;

    void actualizar(double dt);

    void cambiarCarril(int direccion);
    void saltar();
    void pedalearFuerte(bool activo);

    EstadoNivel getEstado() const;
    double getTiempo() const;
    double getTiempoRestante() const;
    double getDistanciaRestante() const;
    double getPendienteActual() const;
    int estrellas() const;
    int vidasNivel2() const;
    std::string nombreDificultad() const;

    const Egan &getEgan() const;
    const Terreno &getTerreno() const;
    const GestorObstaculos &getGestor() const;

private:
    void construirTerreno();
    void programarEventos();
    void lanzarGranizosPendientes(double dt);
    void lanzarGranizo(int indice);
    void lanzarDerrumbe();
    void descartarObstaculosAtrasados();
    void evaluarFin();

    Terreno terreno;
    GestorObstaculos gestor;
    Egan egan;
    std::unique_ptr<EstrategiaDificultad> estrategia;
    std::mt19937 generador;

    EstadoNivel estado;
    double tiempo;
    double tiempoFinal;

    std::vector<double> tiemposLluvia;
    std::size_t siguienteLluvia;
    int granizosPendientes;
    double temporizadorGranizo;
    int indiceGranizo;
    bool derrumbeHecho;
};

#endif // LOGICANIVEL1_H