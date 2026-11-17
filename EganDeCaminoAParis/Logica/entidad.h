#ifndef ENTIDAD_H
#define ENTIDAD_H

class Entidad
{
public:
    Entidad(double x = 0, double y = 0, int carril = 1);
    virtual ~Entidad();

    virtual void actualizar(double dt) = 0;

    double getX() const;
    double getY() const;
    double getVx() const;
    double getVy() const;
    int getCarril() const;

    void setPosicion(double x, double y);
    void setVelocidad(double vx, double vy);
    void setCarril(int carril);

    static const int MAX_CARRILES = 5;

protected:
    double x, y;
    double vx, vy;
    int carril;
};

#endif // ENTIDAD_H