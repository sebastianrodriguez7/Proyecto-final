#ifndef FISICAPENDIENTE_H
#define FISICAPENDIENTE_H

class FisicaPendiente
{
public:
    FisicaPendiente(double g = 9.8, double rozamiento = 0.5);

    double calcularAceleracion(double aPedal, double anguloGrados, double v) const;

    double actualizarVelocidad(double v, double aPedal, double anguloGrados, double dt) const;

private:
    double g;
    double c;
};

#endif // FISICAPENDIENTE_H