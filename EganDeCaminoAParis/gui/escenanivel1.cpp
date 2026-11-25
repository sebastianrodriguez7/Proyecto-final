#include "escenanivel1.h"
#include "bancosprites.h"
#include "fabricafondos.h"
#include "proyeccion.h"
#include "granizo.h"
#include "guitarra.h"
#include <QBrush>
#include <QFont>
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QKeyEvent>
#include <QPen>
#include <algorithm>
#include <cmath>
#include <set>

namespace {
const int LADO_EGAN = 192;
const int LADO_ROCA = 96;
const int LADO_GRANIZO = 48;
const int LADO_BIDON = 64;
const int LADO_GUITARRA = 96;

QString minutosSegundos(double segundos)
{
    int total = static_cast<int>(std::ceil(segundos));
    return QString::asprintf("%d:%02d", total / 60, total % 60);
}

int zDeCarril(double carril, int extra)
{
    return 10 + static_cast<int>(std::lround(carril)) * 10 + extra;
}
}


EscenaNivel1::EscenaNivel1(Dificultad dificultad, QObject *parent)
    : QGraphicsScene(parent),
    logica(new LogicaNivel1(dificultad)),
    pausado(false),
    teclaFuerte(false),
    resultadoMostrado(false),
    carrilVisual(2.0),
    spriteEgan(nullptr),
    sombraEgan(nullptr),
    textoTiempo(nullptr),
    textoDistancia(nullptr),
    textoPendiente(nullptr),
    textoAviso(nullptr),
    textoEnergia(nullptr),
    barraFondo(nullptr),
    barraRelleno(nullptr),
    velo(nullptr),
    textoResultado(nullptr)
{
    setSceneRect(0, 0, Proyeccion::ANCHO, Proyeccion::ALTO);
    setBackgroundBrush(Qt::black);

    construirFondo();
    construirEntidades();
    construirHUD();

    connect(&temporizador, &QTimer::timeout, this, &EscenaNivel1::cuadro);
    reloj.start();
    temporizador.start(16);
}

EscenaNivel1::~EscenaNivel1()
{
    temporizador.stop();
    delete logica;
}

EscenaNivel1::Capa EscenaNivel1::crearCapa(const QPixmap &imagen, double y, double factor, double z)
{
    Capa c;
    c.a = addPixmap(imagen);
    c.b = addPixmap(imagen);
    c.a->setZValue(z);
    c.b->setZValue(z);
    c.periodo = imagen.width();
    c.factor = factor;
    c.y = y;
    return c;
}

void EscenaNivel1::construirFondo()
{
    QGraphicsPixmapItem *cielo = addPixmap(FabricaFondos::cielo(1280, 400));
    cielo->setZValue(0);

    capas.push_back(crearCapa(FabricaFondos::montanas(1280, 260, QColor(120, 140, 175), 0.7), 130, 0.06, 1));
    capas.push_back(crearCapa(FabricaFondos::montanas(1280, 200, QColor(85, 105, 140), 2.1), 190, 0.12, 1.5));
    capas.push_back(crearCapa(FabricaFondos::bosque(1280, 170), 225, 0.35, 2));

    const QColor colores[3] = {QColor(92, 92, 100), QColor(80, 80, 88), QColor(68, 68, 76)};
    for (int carril = 1; carril <= 3; ++carril) {
        double tope = Proyeccion::nivel1Tope(carril);
        double fondo = Proyeccion::nivel1Fondo(carril);
        QGraphicsRectItem *franja = addRect(0, tope, Proyeccion::ANCHO, fondo - tope,
                                            QPen(Qt::NoPen), QBrush(colores[carril - 1]));
        franja->setZValue(3);
    }
    QGraphicsRectItem *cesped = addRect(0, Proyeccion::nivel1Fondo(3), Proyeccion::ANCHO,
                                        Proyeccion::ALTO - Proyeccion::nivel1Fondo(3),
                                        QPen(Qt::NoPen), QBrush(QColor(40, 110, 50)));
    cesped->setZValue(3);
    QGraphicsRectItem *bordeSuperior = addRect(0, Proyeccion::nivel1Tope(1) - 3, Proyeccion::ANCHO, 4,
                                               QPen(Qt::NoPen), QBrush(Qt::white));
    bordeSuperior->setZValue(4);

    capas.push_back(crearCapa(FabricaFondos::marcasCarretera(1280, 720), 0, 1.0, 4));
}

void EscenaNivel1::ponerSprite(QGraphicsPixmapItem *item, const QString &nombre, int lado, const QColor &color)
{
    QPixmap pm = BancoSprites::obtener(nombre, lado, lado, color);
    item->setPixmap(pm);
    item->setOffset(-pm.width() / 2.0, -pm.height());
}

void EscenaNivel1::construirEntidades()
{
    sombraEgan = addEllipse(-45, -9, 90, 18, QPen(Qt::NoPen), QBrush(QColor(0, 0, 0, 110)));
    sombraEgan->setZValue(5);

    spriteEgan = addPixmap(QPixmap());
    nombreEgan = "egan_nivel1";
    ponerSprite(spriteEgan, nombreEgan, LADO_EGAN, QColor(250, 210, 40));
}

void EscenaNivel1::construirHUD()
{
    QFont fuente("Segoe UI", 18, QFont::Bold);

    textoTiempo = addSimpleText(QString(), fuente);
    textoDistancia = addSimpleText(QString(), fuente);
    textoPendiente = addSimpleText(QString(), fuente);
    textoEnergia = addSimpleText(QStringLiteral("Energía"), fuente);
    textoAviso = addSimpleText(QString(), QFont("Segoe UI", 28, QFont::Bold));

    QGraphicsSimpleTextItem *textos[] = {textoTiempo, textoDistancia, textoPendiente, textoEnergia, textoAviso};
    for (QGraphicsSimpleTextItem *t : textos) {
        t->setBrush(QBrush(Qt::white));
        t->setZValue(100);
    }
    textoTiempo->setPos(20, 10);
    textoEnergia->setPos(20, 50);
    textoDistancia->setPos(Proyeccion::ANCHO - 320, 10);
    textoPendiente->setPos(Proyeccion::ANCHO - 320, 50);
    textoAviso->setPos(Proyeccion::ANCHO / 2, 90);

    barraFondo = addRect(130, 56, 260, 24, QPen(QColor(255, 255, 255), 2), QBrush(QColor(0, 0, 0, 120)));
    barraRelleno = addRect(132, 58, 256, 20, QPen(Qt::NoPen), QBrush(QColor(80, 200, 90)));
    barraFondo->setZValue(100);
    barraRelleno->setZValue(101);
}



void EscenaNivel1::cuadro()
{
    double dt = reloj.nsecsElapsed() / 1.0e9;
    reloj.start();
    if (dt > 0.05)
        dt = 0.05;
    if (pausado)
        return;

    if (logica->getEstado() == EstadoNivel::JUGANDO) {
        logica->pedalearFuerte(teclaFuerte);
        logica->actualizar(dt);
    }
    sincronizar(dt);
    actualizarHUD();

    if (!resultadoMostrado && logica->getEstado() != EstadoNivel::JUGANDO)
        mostrarResultado();
}

void EscenaNivel1::keyPressEvent(QKeyEvent *evento)
{
    if (evento->isAutoRepeat())
        return;

    switch (evento->key()) {
    case Qt::Key_Up:     logica->cambiarCarril(-1); break;
    case Qt::Key_Down:   logica->cambiarCarril(+1); break;
    case Qt::Key_Right:  teclaFuerte = true; break;
    case Qt::Key_Space:  logica->saltar(); break;
    case Qt::Key_P:      pausado = !pausado; break;
    case Qt::Key_Escape: emit volverAlMenu(); break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        if (resultadoMostrado)
            emit nivelTerminado(logica->getEstado() == EstadoNivel::GANADO, logica->estrellas());
        break;
    default:
        QGraphicsScene::keyPressEvent(evento);
    }
}

void EscenaNivel1::keyReleaseEvent(QKeyEvent *evento)
{
    if (evento->isAutoRepeat())
        return;
    if (evento->key() == Qt::Key_Right)
        teclaFuerte = false;
    else
        QGraphicsScene::keyReleaseEvent(evento);
}


void EscenaNivel1::sincronizar(double dt)
{
    const Egan &egan = logica->getEgan();
    const double xCamara = egan.getX();

    carrilVisual += (egan.getCarril() - carrilVisual) * std::min(1.0, dt * 14.0);
    carrilVisual = std::min(std::max(carrilVisual, 1.0), double(LogicaNivel1::NUM_CARRILES));

    sincronizarFondo(xCamara);
    sincronizarEgan(xCamara);
    sincronizarObstaculos(xCamara);
    sincronizarColeccionables(xCamara);
}

void EscenaNivel1::sincronizarFondo(double xCamara)
{
    for (Capa &c : capas) {
        double desplazamiento = std::fmod(xCamara * Proyeccion::N1_PIXELES_POR_METRO * c.factor, c.periodo);
        c.a->setPos(-desplazamiento, c.y);
        c.b->setPos(c.periodo - desplazamiento, c.y);
    }
}

void EscenaNivel1::sincronizarEgan(double xCamara)
{
    const Egan &egan = logica->getEgan();

    QString nombre = "egan_nivel1";
    if (egan.estaEnPajara() && BancoSprites::existe("egan_nivel1_pajara"))
        nombre = "egan_nivel1_pajara";
    else if (teclaFuerte && egan.getEnergia() > 0 && BancoSprites::existe("egan_nivel1_fuerte"))
        nombre = "egan_nivel1_fuerte";
    if (nombre != nombreEgan) {
        nombreEgan = nombre;
        ponerSprite(spriteEgan, nombreEgan, LADO_EGAN, QColor(250, 210, 40));
    }

    PuntoPantalla suelo = Proyeccion::nivel1(egan.getX(), xCamara, carrilVisual, 0.0);
    PuntoPantalla aire = Proyeccion::nivel1(egan.getX(), xCamara, carrilVisual, egan.getY());

    spriteEgan->setPos(aire.x, aire.y);
    spriteEgan->setScale(aire.escala);

    spriteEgan->setRotation(-egan.getPendiente() + egan.getAnguloTambaleo());
    spriteEgan->setZValue(zDeCarril(carrilVisual, 5));

    double encogimiento = std::max(0.5, 1.0 - egan.getY() * 0.2);
    sombraEgan->setPos(suelo.x, suelo.y);
    sombraEgan->setScale(suelo.escala * encogimiento);
}

EscenaNivel1::ItemObstaculo EscenaNivel1::crearObstaculo(bool esGranizo)
{
    ItemObstaculo o;
    o.esGranizo = esGranizo;

    o.sombra = addEllipse(-30, -8, 60, 16, QPen(Qt::NoPen), QBrush(QColor(0, 0, 0, 100)));
    o.sombra->setZValue(5);

    o.sprite = addPixmap(QPixmap());
    const int lado = esGranizo ? LADO_GRANIZO : LADO_ROCA;
    QPixmap pm = BancoSprites::obtener(esGranizo ? "granizo" : "roca", lado, lado,
                                       esGranizo ? QColor(110, 190, 240) : QColor(130, 110, 90));
    o.sprite->setPixmap(pm);
    o.sprite->setOffset(-pm.width() / 2.0, -pm.height() / 2.0);
    return o;
}

void EscenaNivel1::liberarObstaculo(ItemObstaculo &item)
{
    removeItem(item.sprite);
    delete item.sprite;
    removeItem(item.sombra);
    delete item.sombra;
    item.sprite = nullptr;
    item.sombra = nullptr;
}

void EscenaNivel1::sincronizarObstaculos(double xCamara)
{
    std::set<const Proyectil *> vigentes;

    for (const Proyectil *p : logica->getGestor().lista()) {
        vigentes.insert(p);
        const Granizo *granizo = dynamic_cast<const Granizo *>(p);
        const bool esGranizo = (granizo != nullptr);

        auto it = obstaculos.find(p);
        if (it != obstaculos.end() && it->second.esGranizo != esGranizo) {
            liberarObstaculo(it->second);
            obstaculos.erase(it);
            it = obstaculos.end();
        }
        if (it == obstaculos.end())
            it = obstaculos.emplace(p, crearObstaculo(esGranizo)).first;
        ItemObstaculo &o = it->second;

        PuntoPantalla suelo = Proyeccion::nivel1(p->getX(), xCamara, p->getCarril(), 0.0);
        PuntoPantalla aire = Proyeccion::nivel1(p->getX(), xCamara, p->getCarril(), p->getY());
        const double mitad = (esGranizo ? LADO_GRANIZO : LADO_ROCA) / 2.0 * aire.escala;

        o.sprite->setPos(aire.x, aire.y - mitad);
        o.sprite->setScale(aire.escala);
        o.sprite->setZValue(zDeCarril(p->getCarril(), esGranizo ? 8 : 3));

        double tamanoSombra = suelo.escala;
        if (esGranizo) {
            double fraccion = 1.0 - std::min(1.0, p->getY() / granizo->getAlturaInicial());
            tamanoSombra *= 0.3 + 0.9 * fraccion;
        } else {
            o.sprite->setRotation(-p->getX() * 70.0);     // la roca rueda
        }
        o.sombra->setPos(suelo.x, suelo.y);
        o.sombra->setScale(tamanoSombra);
    }

    for (auto it = obstaculos.begin(); it != obstaculos.end(); ) {
        if (vigentes.count(it->first) == 0) {
            liberarObstaculo(it->second);
            it = obstaculos.erase(it);
        } else {
            ++it;
        }
    }
}

void EscenaNivel1::sincronizarColeccionables(double xCamara)
{
    std::set<const Coleccionable *> vigentes;

    for (const Coleccionable *c : logica->getColeccionables().lista()) {
        vigentes.insert(c);

        auto it = coleccionables.find(c);
        if (it == coleccionables.end()) {
            const bool esGuitarra = (dynamic_cast<const Guitarra *>(c) != nullptr);
            QGraphicsPixmapItem *item = addPixmap(QPixmap());
            const int lado = esGuitarra ? LADO_GUITARRA : LADO_BIDON;
            QPixmap pm = BancoSprites::obtener(esGuitarra ? "guitarra" : "bidon", lado, lado,
                                               esGuitarra ? QColor(200, 120, 40) : QColor(60, 160, 220));
            item->setPixmap(pm);
            item->setOffset(-pm.width() / 2.0, -pm.height());
            it = coleccionables.emplace(c, item).first;
        }

        PuntoPantalla pt = Proyeccion::nivel1(c->getX(), xCamara, c->getCarril(), 0.0);
        it->second->setPos(pt.x, pt.y);
        it->second->setScale(pt.escala);
        it->second->setZValue(zDeCarril(c->getCarril(), 2));
    }

    for (auto it = coleccionables.begin(); it != coleccionables.end(); ) {
        if (vigentes.count(it->first) == 0) {
            removeItem(it->second);
            delete it->second;
            it = coleccionables.erase(it);
        } else {
            ++it;
        }
    }
}


void EscenaNivel1::actualizarHUD()
{
    const Egan &egan = logica->getEgan();

    textoTiempo->setText(QStringLiteral("Tiempo: ") + minutosSegundos(logica->getTiempoRestante()));
    textoDistancia->setText(QStringLiteral("Cumbre: ")
                            + QString::number(static_cast<int>(std::ceil(logica->getDistanciaRestante())))
                            + QStringLiteral(" m"));
    textoPendiente->setText(QStringLiteral("Pendiente: ")
                            + QString::number(static_cast<int>(std::lround(logica->getPendienteActual())))
                            + QStringLiteral("°"));

    // Barra de energía: verde, y roja cuando queda poca
    double fraccion = std::min(1.0, std::max(0.0, egan.getEnergia() / Egan::ENERGIA_MAX));
    barraRelleno->setRect(132, 58, 256 * fraccion, 20);
    barraRelleno->setBrush(QBrush(fraccion < 0.25 ? QColor(220, 60, 50) : QColor(80, 200, 90)));

    // Aviso central
    QString aviso;
    QColor color(255, 255, 255);
    if (pausado) {
        aviso = QStringLiteral("PAUSA");
    } else if (egan.estaEnPajara()) {
        aviso = QStringLiteral("¡PÁJARA!");
        color = QColor(255, 90, 80);
    } else if (egan.estaSentimental()) {
        aviso = QStringLiteral("♪ Recuérdame ♪  ")
                + QString::number(egan.getTiempoSentimental(), 'f', 1) + QStringLiteral(" s");
        color = QColor(252, 209, 22);
    } else if (egan.estaDesinflado()) {
        aviso = QStringLiteral("Desinflado");
        color = QColor(190, 190, 190);
    }
    textoAviso->setText(aviso);
    textoAviso->setBrush(QBrush(color));
    textoAviso->setPos((Proyeccion::ANCHO - textoAviso->boundingRect().width()) / 2.0, 90);
}

void EscenaNivel1::mostrarResultado()
{
    resultadoMostrado = true;

    velo = addRect(0, 0, Proyeccion::ANCHO, Proyeccion::ALTO, QPen(Qt::NoPen), QBrush(QColor(0, 0, 0, 160)));
    velo->setZValue(200);

    const bool gano = (logica->getEstado() == EstadoNivel::GANADO);
    const int estrellas = logica->estrellas();

    QString estrellasTexto;
    for (int i = 0; i < 3; ++i)
        estrellasTexto += (i < estrellas) ? QStringLiteral("★") : QStringLiteral("☆");

    QString texto = gano ? QStringLiteral("¡Llegaste a la cumbre!") : QStringLiteral("Se acabó el tiempo");
    texto += QStringLiteral("\n\n") + estrellasTexto;
    if (gano)
        texto += QStringLiteral("\n\nVidas para el nivel 2: ") + QString::number(logica->vidasNivel2());
    texto += QStringLiteral("\n\nEnter: continuar     Esc: menú");

    textoResultado = addSimpleText(texto, QFont("Segoe UI", 30, QFont::Bold));
    textoResultado->setBrush(QBrush(QColor(252, 209, 22)));
    textoResultado->setZValue(201);
    QRectF caja = textoResultado->boundingRect();
    textoResultado->setPos((Proyeccion::ANCHO - caja.width()) / 2.0,
                           (Proyeccion::ALTO - caja.height()) / 2.0);
}