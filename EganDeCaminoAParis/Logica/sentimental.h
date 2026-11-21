#ifndef SENTIMENTAL_H
#define SENTIMENTAL_H


class Sentimental
{
public:
    static constexpr double DURACION = 8.0;
    static constexpr double DURACION_DESINFLADO = 3.0;

    bool activar();
    void actualizar(double dt);

    bool estaActivo() const;
    bool estaDesinflado() const;
    double getTiempoActivo() const;
    double getTiempoDesinflado() const;

private:
    double tiempoActivo = 0;
    double tiempoDesinflado = 0;
};

#endif // SENTIMENTAL_H