#ifndef ESTADO_H
#define ESTADO_H

#include <string>

class Estado
{
public:
    static constexpr int NUM_CARRILES = 5;
    static constexpr int UMBRAL_POCOS = 2;
    static constexpr int NUM_ESTADOS = 10;

    explicit Estado(int carrilEgan = 1, int bidones = UMBRAL_POCOS);

    int getCarrilEgan() const;
    bool pocosBidones() const;
    std::string clave() const;
    int indice() const;

private:
    int carrilEgan;
    bool pocos;
};

struct Experiencia
{
    Estado estado;
    int carrilElegido = 1;
    double recompensa = 0;
};

#endif // ESTADO_H