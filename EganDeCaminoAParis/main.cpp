#include "mainwindow.h"
#include "egan.h"
#include <QApplication>
#include <iostream>
#include <stdexcept>

int main(int argc, char *argv[])
{
    // ---- PRUEBA TEMPORAL DE LA CAPA LOGICA ----
    try {
        Egan egan(0, 0, 2, 3);
        egan.setPedaleoFuerte(true);
        for (int i = 0; i < 10; ++i) {
            egan.actualizar(1.0);   // simula 1 segundo
            std::cout << "t=" << i + 1
                      << " energia=" << egan.getEnergia()
                      << " pajara=" << egan.estaEnPajara() << std::endl;
        }
        egan.cambiarCarril(-1);
        egan.cambiarCarril(-1);     // ya está en el carril 1: no debe pasar de ahí
        std::cout << "carril=" << egan.getCarril() << std::endl;

        Egan malo(0, 0, 4, 3);      // debe lanzar excepción
    } catch (const std::exception &e) {
        std::cout << "Excepcion capturada: " << e.what() << std::endl;
    }
    // ---- FIN DE LA PRUEBA ----

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}