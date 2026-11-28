#ifndef ESCENANIVEL1_H
#define ESCENANIVEL1_H

#include <map>
#include <vector>
#include <QElapsedTimer>
#include <QGraphicsScene>
#include <QString>
#include <QTimer>
#include "logicanivel1.h"
#include "eventossonido.h"
#include "sonidos.h"

class QGraphicsEllipseItem;
class QGraphicsPixmapItem;
class QGraphicsRectItem;
class QGraphicsSimpleTextItem;
class QKeyEvent;

class EscenaNivel1 : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit EscenaNivel1(Dificultad dificultad, QObject *parent = nullptr);
    ~EscenaNivel1() override;

signals:
    void nivelTerminado(bool gano, int estrellas);
    void volverAlMenu();

protected:
    void keyPressEvent(QKeyEvent *evento) override;
    void keyReleaseEvent(QKeyEvent *evento) override;

private slots:
    void cuadro();

private:
    struct Capa
    {
        QGraphicsPixmapItem *a = nullptr;
        QGraphicsPixmapItem *b = nullptr;
        double periodo = 1280;
        double factor = 1.0;
        double y = 0;
    };

    struct ItemObstaculo
    {
        QGraphicsPixmapItem *sprite = nullptr;
        QGraphicsEllipseItem *sombra = nullptr;
        bool esGranizo = false;
    };

    void construirFondo();
    void construirEntidades();
    void construirHUD();
    Capa crearCapa(const QPixmap &imagen, double y, double factor, double z);
    void ponerSprite(QGraphicsPixmapItem *item, const QString &nombre, int lado, const QColor &color);

    void sincronizar(double dt);
    void sincronizarFondo(double xCamara);
    void sincronizarEgan(double xCamara);
    void sincronizarObstaculos(double xCamara);
    void sincronizarColeccionables(double xCamara);
    void actualizarHUD();
    void mostrarResultado();

    ItemObstaculo crearObstaculo(bool esGranizo);
    void liberarObstaculo(ItemObstaculo &item);

    LogicaNivel1 *logica;
    QTimer temporizador;
    QElapsedTimer reloj;
    bool pausado;
    bool teclaFuerte;
    bool resultadoMostrado;
    double carrilVisual;
    Sonidos sonidos;
    DetectorSonidosNivel1 detectorSonidos;

    std::vector<Capa> capas;
    QGraphicsPixmapItem *spriteEgan;
    QGraphicsEllipseItem *sombraEgan;
    QString nombreEgan;
    std::map<const Proyectil *, ItemObstaculo> obstaculos;
    std::map<const Coleccionable *, QGraphicsPixmapItem *> coleccionables;

    QGraphicsSimpleTextItem *textoTiempo;
    QGraphicsSimpleTextItem *textoDistancia;
    QGraphicsSimpleTextItem *textoPendiente;
    QGraphicsSimpleTextItem *textoAviso;
    QGraphicsSimpleTextItem *textoEnergia;
    QGraphicsRectItem *barraFondo;
    QGraphicsRectItem *barraRelleno;
    QGraphicsRectItem *velo;
    QGraphicsSimpleTextItem *textoResultado;
};

#endif // ESCENANIVEL1_H