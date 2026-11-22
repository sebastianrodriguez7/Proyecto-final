#ifndef FISICAPARABOLICA_H
#define FISICAPARABOLICA_H

class FisicaParabolica
{
public:
    FisicaParabolica(double g = 9.8, double rebote = 0.4);

    bool paso(double &x, double &y, double &vx, double &vy, double dt) const;

    double alcance(double v0, double anguloGrados) const;

    double velocidadParaAlcance(double alcance, double anguloGrados) const;

    double getG() const;
    double getRebote() const;

private:
    double g;
    double e;
};

#endif // FISICAPARABOLICA_H