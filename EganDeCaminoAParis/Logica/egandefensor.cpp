#include "egandefensor.h"
#include <algorithm>
#include <stdexcept>

EganDefensor::EganDefensor(int numCarriles, unsigned semilla)
    : Entidad(X, 0.0, (numCarriles + 1) / 2),
    numCarriles(numCarriles),
    bidones(MAX_BIDONES),
    tiempoRecarga(0),
    cargando(false),
    tiempoCarga(0),
    tambaleando(false),
    tiempoTambaleo(0),
    golpes(0),
    fisicaTambaleo(3.0, 10.0),
    generador(semilla)
{
    if (numCarriles < 1 || numCarriles > MAX_CARRILES)
        throw std::invalid_argument("Numero de carriles invalido para EganDefensor");
}

void EganDefensor::actualizar(double dt)
{
    sentimental.actualizar(dt);

    if (bidones < MAX_BIDONES) {
        double intervalo = RECARGA_BASE * FACTOR_ESCALADOR;
        tiempoRecarga += dt * (sentimental.estaActivo() ? 2.0 : 1.0);
        if (tiempoRecarga >= intervalo) {
            ++bidones;
            tiempoRecarga -= intervalo;
        }
    } else {
        tiempoRecarga = 0;
    }

    if (cargando)
        tiempoCarga = std::min(tiempoCarga + dt, TIEMPO_CARGA_MAX);

    if (tambaleando) {
        tiempoTambaleo += dt;
        if (tiempoTambaleo >= DURACION_TAMBALEO) {
            tambaleando = false;
            tiempoTambaleo = 0;
        }
    }
}

void EganDefensor::cambiarCarril(int direccion)
{
    if (tambaleando) return;
    int nuevo = carril + direccion;
    if (nuevo >= 1 && nuevo <= numCarriles)
        carril = nuevo;
}

double EganDefensor::alcanceParaCarga(double tiempo) const
{
    double fraccion = std::min(tiempo / TIEMPO_CARGA_MAX, 1.0);
    return ALCANCE_MIN + (ALCANCE_MAX - ALCANCE_MIN) * fraccion;
}

void EganDefensor::iniciarCarga()
{
    if (tambaleando || bidones <= 0) return;
    cargando = true;
    tiempoCarga = 0;
}

bool EganDefensor::soltarCarga(double &alcance)
{
    if (!cargando) return false;
    cargando = false;

    double a = alcanceParaCarga(tiempoCarga);
    tiempoCarga = 0;
    --bidones;

    if (sentimental.estaActivo()) {
        std::uniform_real_distribution<double> error(-ERROR_SENTIMENTAL, ERROR_SENTIMENTAL);
        a *= 1.0 + error(generador);
    }
    alcance = a;
    return true;
}

void EganDefensor::recibirBidones(int cantidad)
{
    if (cantidad < 0)
        throw std::invalid_argument("La cantidad de bidones no puede ser negativa");
    bidones = std::min(MAX_BIDONES, bidones + cantidad);
}

bool EganDefensor::recibirGolpe()
{
    if (tambaleando) return false;
    tambaleando = true;
    tiempoTambaleo = 0;
    cargando = false;
    tiempoCarga = 0;
    ++golpes;
    return true;
}

bool EganDefensor::activarSentimental() { return sentimental.activar(); }

int EganDefensor::getBidones() const { return bidones; }
bool EganDefensor::estaCargando() const { return cargando; }
double EganDefensor::getTiempoCarga() const { return tiempoCarga; }
double EganDefensor::getAlcancePrevisto() const { return alcanceParaCarga(tiempoCarga); }

double EganDefensor::getProgresoRecarga() const
{
    if (bidones >= MAX_BIDONES) return 1.0;
    return tiempoRecarga / (RECARGA_BASE * FACTOR_ESCALADOR);
}

bool EganDefensor::estaTambaleando() const { return tambaleando; }

double EganDefensor::getAnguloTambaleo() const
{
    if (!tambaleando) return 0;
    return fisicaTambaleo.angulo(AMPLITUD_TAMBALEO, tiempoTambaleo);
}

int EganDefensor::getGolpes() const { return golpes; }
bool EganDefensor::estaSentimental() const { return sentimental.estaActivo(); }
bool EganDefensor::estaDesinflado() const { return sentimental.estaDesinflado(); }
double EganDefensor::getTiempoSentimental() const { return sentimental.getTiempoActivo(); }