#include "mainwindow.h"
#include "bidonlanzado.h"
#include "egandefensor.h"
#include "rival.h"
#include "objetocarril.h"
#include <QApplication>
#include <cmath>
#include <iostream>
#include <exception>

// Distancia a la que hay que lanzar para alcanzar a un rival que sigue avanzando
static double alcanceConAdelanto(double xRival, double velRival)
{
    double R = xRival - EganDefensor::X;
    for (int i = 0; i < 50; ++i) {
        double vuelo = std::sqrt(2 * R / 9.8);
        R = (xRival - velRival * vuelo) - EganDefensor::X;
    }
    return R;
}

int main(int argc, char *argv[])
{
    // ---- PRUEBA TEMPORAL DE LA CAPA LOGICA ----
    try {
        std::cout << "--- 1) Bidon lanzado a 45 grados ---\n";
        FisicaParabolica fp;
        std::cout << "Velocidad para alcance 20: " << fp.velocidadParaAlcance(20, 45)
                  << "  alcance con esa velocidad: " << fp.alcance(fp.velocidadParaAlcance(20, 45), 45) << std::endl;
        for (double alc : {5.0, 10.0, 20.0, 30.0}) {
            BidonLanzado b(3.0, 3, alc);
            double t = 0;
            while (!b.haImpactado() && t < 10) {
                b.actualizar(0.001);
                t += 0.001;
            }
            std::cout << "Alcance " << alc << ": cae a " << b.getX() - 3.0
                      << " tras " << t << " s" << std::endl;
        }

        std::cout << "\n--- 2) EganDefensor: carga y bidones ---\n";
        EganDefensor e(5, 1);
        std::cout << "Carril inicial=" << e.getCarril() << " bidones=" << e.getBidones() << std::endl;
        for (double espera : {0.0, 0.75, 1.5, 3.0}) {
            e.recibirBidones(5);
            e.iniciarCarga();
            for (double t = 0; t < espera; t += 0.01) e.actualizar(0.01);
            double alc = 0;
            e.soltarCarga(alc);
            std::cout << "Carga " << espera << " s -> alcance " << alc << std::endl;
        }
        e.recibirBidones(5);
        int lanzados = 0;
        for (int i = 0; i < 7; ++i) {
            double alc;
            e.iniciarCarga();
            if (e.soltarCarga(alc)) ++lanzados;
        }
        std::cout << "Lanzamientos con 5 bidones (se intentan 7): " << lanzados
                  << " quedan=" << e.getBidones() << std::endl;

        double t = 0;
        while (e.getBidones() < 1) { e.actualizar(0.01); t += 0.01; }
        std::cout << "Tiempo en recargar 1 bidon: " << t << " s" << std::endl;

        EganDefensor s(5, 1);
        s.activarSentimental();
        double alc;
        for (int i = 0; i < 5; ++i) { s.iniciarCarga(); s.soltarCarga(alc); }
        t = 0;
        while (s.getBidones() < 1) { s.actualizar(0.01); t += 0.01; }
        std::cout << "Con Sentimental, recarga 1 bidon: " << t << " s" << std::endl;

        double menor = 1e9, mayor = 0;
        for (int i = 0; i < 1000; ++i) {
            EganDefensor g(5, 100 + i);
            g.activarSentimental();
            g.iniciarCarga();
            for (int k = 0; k < 150; ++k) g.actualizar(0.01);   // carga maxima = 30
            double a;
            g.soltarCarga(a);
            if (a < menor) menor = a;
            if (a > mayor) mayor = a;
        }
        std::cout << "Con Sentimental, alcance 30 varia entre " << menor << " y " << mayor << std::endl;

        std::cout << "\n--- 3) Rivales ---\n";
        Rival *n = crearRival(TipoRival::NORMAL, 1, 2);
        Rival *v = crearRival(TipoRival::VELOZ, 2, 2);
        Rival *g = crearRival(TipoRival::GREGARIO, 3, 2);
        std::cout << "Velocidades: normal=" << n->getVelocidad() << " veloz=" << v->getVelocidad()
                  << " gregario=" << g->getVelocidad()
                  << " | resistencias: " << n->getResistencia() << "/" << v->getResistencia()
                  << "/" << g->getResistencia() << std::endl;
        double tn = 0, tv = 0;
        while (!n->cruzoLinea()) { n->actualizar(0.01); tn += 0.01; }
        while (!v->cruzoLinea()) { v->actualizar(0.01); tv += 0.01; }
        std::cout << "Tiempo en cruzar la linea: normal=" << tn << " s veloz=" << tv << " s" << std::endl;

        Rival r(10, TipoRival::NORMAL, 3, Rival::VELOCIDAD_BASE, 1, 20.0);
        r.recibirBidon();
        std::cout << "Tras 1 bidon (normal): detenido=" << r.estaDetenido()
                  << " angulo=" << r.getAnguloTambaleo() << std::endl;
        for (int i = 0; i < 50; ++i) r.actualizar(0.01);
        std::cout << "0.5 s despues: angulo=" << r.getAnguloTambaleo() << " x=" << r.getX() << std::endl;
        for (int i = 0; i < 150; ++i) r.actualizar(0.01);
        std::cout << "2 s despues: x=" << r.getX() << " activo=" << r.estaActivo() << std::endl;

        Rival fuerte(11, TipoRival::GREGARIO, 3, Rival::VELOCIDAD_BASE, 2, 20.0);
        fuerte.recibirBidon();
        std::cout << "Gregario tras 1 bidon: detenido=" << fuerte.estaDetenido()
                  << " resistencia=" << fuerte.getResistencia()
                  << " aturdido=" << fuerte.estaAturdido() << std::endl;
        for (int i = 0; i < 200; ++i) fuerte.actualizar(0.01);
        std::cout << "Gregario 2 s despues: aturdido=" << fuerte.estaAturdido() << " avanza de nuevo" << std::endl;
        fuerte.recibirBidon();
        std::cout << "Gregario tras 2 bidones: detenido=" << fuerte.estaDetenido() << std::endl;
        delete n; delete v; delete g;

        std::cout << "\n--- 4) Duelo: bidon contra rival que avanza ---\n";
        double R = alcanceConAdelanto(Rival::X_INICIAL, Rival::VELOCIDAD_BASE);
        std::cout << "Alcance con adelanto: " << R << " (sin adelanto seria "
                  << Rival::X_INICIAL - EganDefensor::X << ")" << std::endl;
        for (int intento = 0; intento < 2; ++intento) {
            double alcanceUsado = (intento == 0) ? R : Rival::X_INICIAL - EganDefensor::X;
            Rival objetivo(20, TipoRival::NORMAL, 3, Rival::VELOCIDAD_BASE, 1);
            BidonLanzado bidon(EganDefensor::X, 3, alcanceUsado);
            double tt = 0;
            while (!bidon.haImpactado() && tt < 10) {
                bidon.actualizar(0.001);
                objetivo.actualizar(0.001);
                tt += 0.001;
            }
            bool acierto = std::fabs(objetivo.getX() - bidon.getX()) < 1.5;
            if (acierto) objetivo.recibirBidon();
            std::cout << (intento == 0 ? "Con adelanto" : "Sin adelanto ") << ": impacto en t=" << tt
                      << " bidon x=" << bidon.getX() << " rival x=" << objetivo.getX()
                      << " -> detenido=" << objetivo.estaDetenido() << std::endl;
        }

        std::cout << "\n--- 5) Aficionado y guitarra ---\n";
        EganDefensor u(5, 1);
        for (int i = 0; i < 4; ++i) { double a; u.iniciarCarga(); u.soltarCarga(a); }
        std::cout << "Bidones tras 4 lanzamientos: " << u.getBidones() << std::endl;
        Aficionado af(2);
        af.ofrecerA(u);
        std::cout << "Tras aficionado: " << u.getBidones() << " tomado=" << af.fueTomado() << std::endl;
        af.ofrecerA(u);
        std::cout << "Mismo aficionado otra vez: " << u.getBidones() << std::endl;
        GuitarraCarril gc(4);
        gc.ofrecerA(u);
        std::cout << "Tras guitarra: sentimental=" << u.estaSentimental() << std::endl;
        Aficionado efimero(1);
        for (int i = 0; i < 650; ++i) efimero.actualizar(0.01);
        std::cout << "Aficionado tras 6.5 s: activo=" << efimero.estaActivo() << std::endl;

        std::cout << "\n--- 6) Golpe a Egan ---\n";
        EganDefensor w(5, 1);
        w.iniciarCarga();
        std::cout << "Primer golpe: " << w.recibirGolpe() << "  segundo seguido: " << w.recibirGolpe()
                  << "  cargando=" << w.estaCargando() << std::endl;
        w.cambiarCarril(1);
        std::cout << "Carril tambaleando: " << w.getCarril() << std::endl;
        for (int i = 0; i < 160; ++i) w.actualizar(0.01);
        w.cambiarCarril(1);
        std::cout << "Carril despues: " << w.getCarril() << " golpes=" << w.getGolpes() << std::endl;

        std::cout << "\n--- 7) Excepciones ---\n";
        try { Rival malo(1, TipoRival::NORMAL, 1, -3.0, 1); }
        catch (const std::exception &ex) { std::cout << "1) " << ex.what() << std::endl; }
        try { EganDefensor malo(7); }
        catch (const std::exception &ex) { std::cout << "2) " << ex.what() << std::endl; }
        try { EganDefensor malo(5); malo.recibirBidones(-1); }
        catch (const std::exception &ex) { std::cout << "3) " << ex.what() << std::endl; }
        try { fp.velocidadParaAlcance(-5, 45); }
        catch (const std::exception &ex) { std::cout << "4) " << ex.what() << std::endl; }
    } catch (const std::exception &ex) {
        std::cout << "Error inesperado: " << ex.what() << std::endl;
    }
    // ---- FIN DE LA PRUEBA ----

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}