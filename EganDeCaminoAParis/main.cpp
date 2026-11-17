#include "mainwindow.h"
#include "granizo.h"
#include "roca.h"
#include "gestorobstaculos.h"
#include <QApplication>
#include <iostream>
#include <exception>

int main(int argc, char *argv[])
{
    // ---- PRUEBA TEMPORAL DE LA CAPA LOGICA ----
    try {
        // 1) Granizo: aviso, caida y rebotes
        Granizo g(10, 2);
        double t = 0;
        int previos = 0;
        while (g.estaActivo() && t < 20) {
            g.actualizar(0.01);
            t += 0.01;
            if (g.getRebotes() != previos) {
                previos = g.getRebotes();
                std::cout << "t=" << t << " rebote #" << previos << std::endl;
            }
        }
        std::cout << "Granizo desactivado en t=" << t << std::endl;

        // 2) Roca: avanza hacia Egan
        Roca r(100, 1, 6);
        for (int i = 0; i < 500; ++i) r.actualizar(0.01);
        std::cout << "Roca a los 5s: x=" << r.getX() << " y=" << r.getY()
                  << " rebotes=" << r.getRebotes() << std::endl;

        // 3) Gestor con memoria dinamica
        GestorObstaculos gestor;
        gestor.agregar(new Granizo(10, 1));
        gestor.agregar(new Granizo(20, 2));
        gestor.agregar(new Granizo(30, 3));
        gestor.agregar(new Roca(100, 1, 6));
        gestor.agregar(new Roca(120, 2, 5));
        std::cout << "Obstaculos al inicio: " << gestor.cantidad() << std::endl;
        for (int i = 0; i < 1000; ++i) gestor.actualizar(0.01);
        std::cout << "Obstaculos a los 10s: " << gestor.cantidad() << std::endl;

        // 4) Excepciones
        gestor.agregar(new Roca(50, 1, -5));   // debe lanzar excepcion
    } catch (const std::exception &e) {
        std::cout << "Excepcion capturada: " << e.what() << std::endl;
    }
    // ---- FIN DE LA PRUEBA ----

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}