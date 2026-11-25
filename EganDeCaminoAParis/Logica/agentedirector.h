#ifndef AGENTEDIRECTOR_H
#define AGENTEDIRECTOR_H

#include <map>
#include <random>
#include <string>
#include "percepcion.h"
#include "razonamiento.h"
#include "accion.h"
#include "aprendizaje.h"
#include "tablaq.h"

class AgenteDirector
{
public:
    explicit AgenteDirector(unsigned semilla = std::random_device{}());

    void actualizar(double dt);
    bool puedeEnviar() const;

    Rival *enviarRival(const EganDefensor &egan, TipoRival tipo);

    // Aprende: el rival terminó su recorrido (cruzó la línea o fue detenido)
    void rivalTerminado(int idRival, bool cruzoLinea);

    void guardar(const std::string &ruta) const;
    bool cargar(const std::string &ruta);

    double getEpsilon() const;
    double getTasaExito() const;
    const TablaQ &getTabla() const;
    std::size_t getExperienciasRecientes() const;
    std::size_t getDecisionesPendientes() const;

private:
    struct Decision { Estado estado; int carril = 1; };

    Percepcion percepcion;
    Razonamiento razonamiento;
    Accion accion;
    Aprendizaje aprendizaje;
    TablaQ tabla;
    double epsilon;
    std::mt19937 generador;
    int siguienteId;
    std::map<int, Decision> pendientes;
};

#endif // AGENTEDIRECTOR_H