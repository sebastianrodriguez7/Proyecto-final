#ifndef FISICAOSCILATORIA_H
#define FISICAOSCILATORIA_H

class FisicaOscilatoria
{
public:
    FisicaOscilatoria(double gamma = 2.5, double omega = 12.0);

    double angulo(double amplitud, double t) const;

    double getGamma() const;
    double getOmega() const;

private:
    double gamma;
    double omega;
};

#endif // FISICAOSCILATORIA_H