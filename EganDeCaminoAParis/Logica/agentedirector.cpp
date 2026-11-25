#include "agentedirector.h"

AgenteDirector::AgenteDirector(unsigned semilla)
    : epsilon(Aprendizaje::EPSILON_INICIAL),
    generador(semilla),
    siguienteId(1)
{
}

void AgenteDirector::actualizar(double dt) { accion.actualizar(dt); }
bool AgenteDirector::puedeEnviar() const { return accion.lista(); }

Rival *AgenteDirector::enviarRival(const EganDefensor &egan, TipoRival tipo)
{
    Estado estado = percepcion.percibir(egan);
    int carril = razonamiento.elegirCarril(estado, tabla, epsilon, generador);
    int id = siguienteId++;
    Rival *rival = accion.enviar(tipo, carril, id);

    Decision d;
    d.estado = estado;
    d.carril = carril;
    pendientes[id] = d;
    return rival;
}

void AgenteDirector::rivalTerminado(int idRival, bool cruzoLinea)
{
    auto it = pendientes.find(idRival);
    if (it == pendientes.end())
        return;

    Experiencia e;
    e.estado = it->second.estado;
    e.carrilElegido = it->second.carril;
    e.recompensa = Aprendizaje::calcularRecompensa(cruzoLinea);
    pendientes.erase(it);

    aprendizaje.registrar(e, tabla);
    epsilon = aprendizaje.ajustarEpsilon(epsilon);
}

void AgenteDirector::guardar(const std::string &ruta) const { tabla.guardar(ruta); }
bool AgenteDirector::cargar(const std::string &ruta) { return tabla.cargar(ruta); }

double AgenteDirector::getEpsilon() const { return epsilon; }
double AgenteDirector::getTasaExito() const { return aprendizaje.tasaExito(); }
const TablaQ &AgenteDirector::getTabla() const { return tabla; }
std::size_t AgenteDirector::getExperienciasRecientes() const { return aprendizaje.cantidad(); }
std::size_t AgenteDirector::getDecisionesPendientes() const { return pendientes.size(); }