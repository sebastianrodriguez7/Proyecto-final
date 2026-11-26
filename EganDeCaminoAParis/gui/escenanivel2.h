#ifndef ESCENANIVEL2_H
#define ESCENANIVEL2_H

#include <map>
#include <vector>
#include <QElapsedTimer>
#include <QGraphicsScene>
#include <QString>
#include <QTimer>
#include "logicanivel2.h"

class QGraphicsEllipseItem;
class QGraphicsPixmapItem;
class QGraphicsRectItem;
class QGraphicsSimpleTextItem;
class QKeyEvent;

class EscenaNivel2 : public QGraphicsScene
{
    Q_OBJECT

public:
    EscenaNivel2(int vidas, const QString &archivoQ, QObject *parent = nullptr);
    ~EscenaNivel2() override;

signals:
    void nivelTerminado(bool gano);
    void volverAlMenu();

protected:
    void keyPressEvent(QKeyEvent *evento) override;
    void keyReleaseEvent(QKeyEvent *evento) override;

private slots:
    void cuadro();

private:
    struct ItemRival
    {
        QGraphicsPixmapItem *sprite = nullptr;
        QGraphicsEllipseItem *sombra = nullptr;
        double tiempoDetenido = 0;
    };

    struct ItemBidon
    {
        QGraphicsPixmapItem *sprite = nullptr;
        QGraphicsEllipseItem *sombra = nullptr;
    };

    struct ItemObjeto
    {
        QGraphicsPixmapItem *sprite = nullptr;
        QGraphicsRectItem *barra = nullptr;
        bool esGuitarra = false;
    };

    void construirFondo();
    void construirEntidades();
    void construirHUD();
    void ponerSprite(QGraphicsPixmapItem *item, const QString &nombre, int lado,
                     const QColor &color, bool espejar);

    void sincronizar(double dt);
    void sincronizarEgan();
    void sincronizarRivales(double dt);
    void sincronizarBidones();
    void sincronizarObjetos();
    void actualizarHUD();
    void mostrarResultado();

    ItemRival crearItemRival(TipoRival tipo);
    ItemObjeto crearItemObjeto(bool esGuitarra);
    void liberarRival(ItemRival &item);
    void liberarBidon(ItemBidon &item);
    void liberarObjeto(ItemObjeto &item);

    LogicaNivel2 *logica;
    QTimer temporizador;
    QElapsedTimer reloj;
    bool pausado;
    bool resultadoMostrado;
    bool mostrarAgente;
    double carrilVisual;

    QGraphicsPixmapItem *spriteEgan;
    QGraphicsEllipseItem *sombraEgan;
    QGraphicsEllipseItem *marcador;
    std::map<int, ItemRival> rivales;
    std::map<const Proyectil *, ItemBidon> bidones;
    std::map<const ObjetoCarril *, ItemObjeto> objetos;

    QGraphicsSimpleTextItem *textoVidas;
    QGraphicsSimpleTextItem *textoTiempo;
    QGraphicsSimpleTextItem *textoStats;
    QGraphicsSimpleTextItem *textoAviso;
    QGraphicsSimpleTextItem *textoAgente;
    QGraphicsSimpleTextItem *textoAyuda;
    QGraphicsRectItem *fondoAviso;
    QGraphicsRectItem *barraRecargaFondo;
    QGraphicsRectItem *barraRecarga;
    QGraphicsRectItem *barraFuerzaFondo;
    QGraphicsRectItem *barraFuerza;
    std::vector<QGraphicsPixmapItem *> iconosBidon;
    QGraphicsRectItem *velo;
    QGraphicsSimpleTextItem *textoResultado;
};

#endif // ESCENANIVEL2_H