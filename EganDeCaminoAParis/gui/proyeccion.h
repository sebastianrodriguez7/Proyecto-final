#ifndef PROYECCION_H
#define PROYECCION_H


struct PuntoPantalla
{
    double x;
    double y;
    double escala;
};

class Proyeccion
{
public:
    static constexpr double ANCHO = 1280.0;
    static constexpr double ALTO = 720.0;

    static constexpr double N1_X_EGAN = 320.0;
    static constexpr double N1_PIXELES_POR_METRO = 48.0;
    static constexpr double N1_PIXELES_ALTURA = 50.0;

    static PuntoPantalla nivel1(double xMundo, double xCamara, double carril, double altura = 0.0);
    static double nivel1Tope(int carril);
    static double nivel1Fondo(int carril);
    static constexpr double N2_X_LINEA = 150.0;
    static constexpr double N2_PIXELES_POR_METRO = 28.5;

    static PuntoPantalla nivel2(double xMundo, double carril, double altura = 0.0);
    static double nivel2Tope(int carril);
    static double nivel2Fondo(int carril);
};

#endif // PROYECCION_H