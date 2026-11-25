#ifndef LOGICANIVEL2_H
#define LOGICANIVEL2_H

#include <deque>
#include <random>
#include <set>
#include <string>
#include <vector>
#include "egandefensor.h"
#include "rival.h"
#include "objetocarril.h"
#include "gestorobstaculos.h"
#include "agentedirector.h"

enum class EstadoNivel2 { JUGANDO, GANADO, PERDIDO };

class LogicaNivel2
{
public:
    static constexpr int NUM_CARRILES = 5;
    static constexpr double DURACION = 90.0;
    static constexpr double RADIO_IMPACTO = 1.5;
    static constexpr double RADIO_CONTACTO = 1.0;

    LogicaNivel2(int vidas, const std::string &archivoQ = "",
                 unsigned semilla = std::random_device{}());
    ~LogicaNivel2();

    LogicaNivel2(const LogicaNivel2 &) = delete;
    LogicaNivel2 &operator=(const LogicaNivel2 &) = delete;

    void actualizar(double dt);

    void cambiarCarril(int direccion);
    void iniciarCarga();
    void soltarCarga();

    EstadoNivel2 getEstado() const;
    double getTiempo() const;
    int getVidas() const;
    int getRivalesDetenidos() const;
    int getRivalesCruzaron() const;
    bool tablaQCargada() const;
    bool tablaQDanada() const;
    bool aprendizajeGuardado() const;

    const EganDefensor &getEgan() const;
    const AgenteDirector &getAgente() const;
    const std::vector<Rival *> &getRivales() const;
    const GestorObstaculos &getBidones() const;
    const std::vector<ObjetoCarril *> &getObjetos() const;

private:
    struct Oleada { double tiempo; std::vector<TipoRival> tipos; };
    struct EventoObjeto { double tiempo; bool esGuitarra; };

    void programarEventos();
    void enviarRivales();
    void resolverImpactos();
    void resolverRivales();
    void actualizarObjetos(double dt);
    void evaluarFin();
    void guardarAprendizaje();

    EganDefensor egan;
    AgenteDirector agente;
    GestorObstaculos bidones;
    std::vector<Rival *> rivales;
    std::vector<ObjetoCarril *> objetos;
    std::set<int> resueltos;
    std::deque<TipoRival> cola;
    std::mt19937 generador;

    std::vector<Oleada> oleadas;
    std::size_t siguienteOleada;
    std::vector<EventoObjeto> eventosObjetos;
    std::size_t siguienteObjeto;

    EstadoNivel2 estado;
    double tiempo;
    int vidas;
    int detenidos;
    int cruzaron;

    std::string archivoQ;
    bool cargada;
    bool danada;
    bool guardado;
};

#endif // LOGICANIVEL2_H