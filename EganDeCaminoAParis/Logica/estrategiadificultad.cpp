#include "estrategiadificultad.h"
#include <stdexcept>

EstrategiaDificultad::~EstrategiaDificultad() {}

std::unique_ptr<EstrategiaDificultad> EstrategiaDificultad::crear(Dificultad d)
{
    switch (d) {
    case Dificultad::FACIL:   return std::make_unique<EstrategiaFacil>();
    case Dificultad::NORMAL:  return std::make_unique<EstrategiaNormal>();
    case Dificultad::DIFICIL: return std::make_unique<EstrategiaDificil>();
    }
    throw std::invalid_argument("Dificultad desconocida");
}

int EstrategiaDificultad::carrilAleatorio(int numCarriles, std::mt19937 &gen)
{
    if (numCarriles < 1)
        throw std::invalid_argument("Debe haber al menos un carril");
    std::uniform_int_distribution<int> dist(1, numCarriles);
    return dist(gen);
}

int EstrategiaDificultad::carrilVecino(int carril, int numCarriles, std::mt19937 &gen)
{
    bool hayArriba = (carril - 1) >= 1;
    bool hayAbajo = (carril + 1) <= numCarriles;
    if (hayArriba && hayAbajo)
        return (carrilAleatorio(2, gen) == 1) ? carril - 1 : carril + 1;
    if (hayArriba) return carril - 1;
    if (hayAbajo) return carril + 1;
    return carril;
}

std::string EstrategiaFacil::nombre() const { return "Facil"; }
double EstrategiaFacil::tiempoAvisoGranizo() const { return 2.0; }
int EstrategiaFacil::granizosPorLluvia() const { return 2; }
double EstrategiaFacil::velocidadRoca() const { return 4.0; }
int EstrategiaFacil::rocasEnDerrumbe() const { return 2; }
double EstrategiaFacil::multiplicadorConsumo() const { return 0.8; }

int EstrategiaFacil::elegirCarril(const Egan &, int numCarriles, int, std::mt19937 &gen) const
{
    return carrilAleatorio(numCarriles, gen);
}


std::string EstrategiaNormal::nombre() const { return "Normal"; }
double EstrategiaNormal::tiempoAvisoGranizo() const { return 1.5; }
int EstrategiaNormal::granizosPorLluvia() const { return 3; }
double EstrategiaNormal::velocidadRoca() const { return 5.0; }
int EstrategiaNormal::rocasEnDerrumbe() const { return 3; }
double EstrategiaNormal::multiplicadorConsumo() const { return 1.0; }

int EstrategiaNormal::elegirCarril(const Egan &egan, int numCarriles, int indice, std::mt19937 &gen) const
{
    if (indice == 0)
        return egan.getCarril();          // el primero apunta a Egan
    return carrilAleatorio(numCarriles, gen);
}

std::string EstrategiaDificil::nombre() const { return "Dificil"; }
double EstrategiaDificil::tiempoAvisoGranizo() const { return 1.0; }
int EstrategiaDificil::granizosPorLluvia() const { return 4; }
double EstrategiaDificil::velocidadRoca() const { return 6.5; }
int EstrategiaDificil::rocasEnDerrumbe() const { return 4; }
double EstrategiaDificil::multiplicadorConsumo() const { return 1.2; }

int EstrategiaDificil::elegirCarril(const Egan &egan, int numCarriles, int indice, std::mt19937 &gen) const
{
    if (indice == 0)
        return egan.getCarril();
    if (indice == 1)
        return carrilVecino(egan.getCarril(), numCarriles, gen);
    return carrilAleatorio(numCarriles, gen);
}