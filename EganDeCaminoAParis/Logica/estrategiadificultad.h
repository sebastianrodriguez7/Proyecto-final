#ifndef ESTRATEGIADIFICULTAD_H
#define ESTRATEGIADIFICULTAD_H

#include <memory>
#include <random>
#include <string>
#include "egan.h"

enum class Dificultad { FACIL, NORMAL, DIFICIL };


class EstrategiaDificultad
{
public:
    virtual ~EstrategiaDificultad();

    virtual std::string nombre() const = 0;
    virtual double tiempoAvisoGranizo() const = 0;
    virtual int granizosPorLluvia() const = 0;
    virtual double velocidadRoca() const = 0;
    virtual int rocasEnDerrumbe() const = 0;
    virtual double multiplicadorConsumo() const = 0;


    virtual int elegirCarril(const Egan &egan, int numCarriles,
                             int indice, std::mt19937 &gen) const = 0;


    static std::unique_ptr<EstrategiaDificultad> crear(Dificultad d);

protected:
    static int carrilAleatorio(int numCarriles, std::mt19937 &gen);
    static int carrilVecino(int carril, int numCarriles, std::mt19937 &gen);
};

class EstrategiaFacil : public EstrategiaDificultad
{
public:
    std::string nombre() const override;
    double tiempoAvisoGranizo() const override;
    int granizosPorLluvia() const override;
    double velocidadRoca() const override;
    int rocasEnDerrumbe() const override;
    double multiplicadorConsumo() const override;
    int elegirCarril(const Egan &egan, int numCarriles, int indice, std::mt19937 &gen) const override;
};

class EstrategiaNormal : public EstrategiaDificultad
{
public:
    std::string nombre() const override;
    double tiempoAvisoGranizo() const override;
    int granizosPorLluvia() const override;
    double velocidadRoca() const override;
    int rocasEnDerrumbe() const override;
    double multiplicadorConsumo() const override;
    int elegirCarril(const Egan &egan, int numCarriles, int indice, std::mt19937 &gen) const override;
};

class EstrategiaDificil : public EstrategiaDificultad
{
public:
    std::string nombre() const override;
    double tiempoAvisoGranizo() const override;
    int granizosPorLluvia() const override;
    double velocidadRoca() const override;
    int rocasEnDerrumbe() const override;
    double multiplicadorConsumo() const override;
    int elegirCarril(const Egan &egan, int numCarriles, int indice, std::mt19937 &gen) const override;
};

#endif // ESTRATEGIADIFICULTAD_H