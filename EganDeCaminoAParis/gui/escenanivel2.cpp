#include "escenanivel2.h"
#include "bancosprites.h"
#include "fabricafondos.h"
#include "proyeccion.h"
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
const int LADO_EGAN = 170;
const int LADO_RIVAL = 160;
const int LADO_GREGARIO = 185;
const int LADO_BIDON = 48;
const int LADO_ICONO = 36;
const int LADO_AFICIONADO = 128;
const int LADO_GUITARRA = 96;
const double ANCHO_BARRA = 206.0;

QString minutosSegundos(double segundos)
{
    if (segundos < 0)
        segundos = 0;
    int total = static_cast<int>(std::ceil(segundos));
    return QString::asprintf("%d:%02d", total / 60, total % 60);
}

int zDeCarril(double carril, int extra)
{
    return 10 + static_cast<int>(std::lround(carril)) * 10 + extra;
}

int ladoRival(TipoRival tipo)
{
    return (tipo == TipoRival::GREGARIO) ? LADO_GREGARIO : LADO_RIVAL;
}

QString nombreRival(TipoRival tipo)
{
    switch (tipo) {
    case TipoRival::VELOZ:    return "rival_veloz";
    case TipoRival::GREGARIO: return "rival_gregario";
    default:                  return "rival_normal";
    }
}

QColor colorRival(TipoRival tipo)
{
    switch (tipo) {
    case TipoRival::VELOZ:    return QColor(30, 190, 220);
    case TipoRival::GREGARIO: return QColor(205, 140, 40);
    default:                  return QColor(60, 70, 150);
    }
}
}


EscenaNivel2::EscenaNivel2(int vidas, const QString &archivoQ, QObject *parent)
    : QGraphicsScene(parent),
    logica(new LogicaNivel2(vidas, archivoQ.toStdString())),
    pausado(false),
    resultadoMostrado(false),
    mostrarAgente(false),
    carrilVisual(3.0),
    spriteEgan(nullptr),
    sombraEgan(nullptr),
    marcador(nullptr),
    textoVidas(nullptr),
    textoTiempo(nullptr),
    textoStats(nullptr),
    textoAviso(nullptr),
    textoAgente(nullptr),
    textoAyuda(nullptr),
    fondoAviso(nullptr),
    barraRecargaFondo(nullptr),
    barraRecarga(nullptr),
    barraFuerzaFondo(nullptr),
    barraFuerza(nullptr),
    velo(nullptr),
    textoResultado(nullptr)
{
    setSceneRect(0, 0, Proyeccion::ANCHO, Proyeccion::ALTO);
    setBackgroundBrush(Qt::black);

    construirFondo();
    construirEntidades();
    construirHUD();

    connect(&temporizador, &QTimer::timeout, this, &EscenaNivel2::cuadro);
    reloj.start();
    temporizador.start(16);
}

EscenaNivel2::~EscenaNivel2()
{
    temporizador.stop();
    delete logica;
}

void EscenaNivel2::construirFondo()
{
    QGraphicsPixmapItem *cielo = addPixmap(FabricaFondos::cielo(1280, 300));
    cielo->setZValue(0);

    QGraphicsPixmapItem *edificios = addPixmap(FabricaFondos::edificiosParis(1280, 190));
    edificios->setPos(0, 90);
    edificios->setZValue(1);

    QGraphicsPixmapItem *arco = addPixmap(FabricaFondos::arcoDelTriunfo(300, 205));
    arco->setPos(540, 55);
    arco->setZValue(2);

    QGraphicsPixmapItem *arboles = addPixmap(FabricaFondos::arboledaAvenida(1280, 140));
    arboles->setPos(0, Proyeccion::nivel2Tope(1) - 140);
    arboles->setZValue(3);

    // Los cinco carriles
    const QColor colores[5] = {QColor(96, 96, 104), QColor(86, 86, 94), QColor(76, 76, 84),
                               QColor(66, 66, 74), QColor(58, 58, 66)};
    for (int carril = 1; carril <= 5; ++carril) {
        double tope = Proyeccion::nivel2Tope(carril);
        double fondo = Proyeccion::nivel2Fondo(carril);
        QGraphicsRectItem *franja = addRect(0, tope, Proyeccion::ANCHO, fondo - tope,
                                            QPen(Qt::NoPen), QBrush(colores[carril - 1]));
        franja->setZValue(4);
    }
    QGraphicsRectItem *acera = addRect(0, Proyeccion::nivel2Fondo(5), Proyeccion::ANCHO,
                                       Proyeccion::ALTO - Proyeccion::nivel2Fondo(5),
                                       QPen(Qt::NoPen), QBrush(QColor(150, 140, 125)));
    acera->setZValue(4);

    QGraphicsPixmapItem *marcas = addPixmap(FabricaFondos::marcasAvenida(1280, 720));
    marcas->setZValue(5);

    const double altoLinea = Proyeccion::nivel2Fondo(5) - Proyeccion::nivel2Tope(1);
    QGraphicsPixmapItem *linea = addPixmap(FabricaFondos::lineaDeDefensa(16, static_cast<int>(altoLinea)));
    linea->setPos(Proyeccion::N2_X_LINEA - 8, Proyeccion::nivel2Tope(1));
    linea->setZValue(6);
}

void EscenaNivel2::ponerSprite(QGraphicsPixmapItem *item, const QString &nombre, int lado,
                               const QColor &color, bool espejar)
{
    QPixmap pm = BancoSprites::obtener(nombre, lado, lado, color, espejar);
    item->setPixmap(pm);
    item->setOffset(-pm.width() / 2.0, -pm.height());
}

void EscenaNivel2::construirEntidades()
{
    sombraEgan = addEllipse(-50, -9, 100, 18, QPen(Qt::NoPen), QBrush(QColor(0, 0, 0, 110)));
    sombraEgan->setZValue(7);

    spriteEgan = addPixmap(QPixmap());
    ponerSprite(spriteEgan, "egan_defensor", LADO_EGAN, QColor(250, 210, 40), false);

    marcador = addEllipse(-34, -10, 68, 20, QPen(QColor(252, 209, 22), 3), QBrush(Qt::NoBrush));
    marcador->setZValue(8);
    marcador->setVisible(false);
}

void EscenaNivel2::construirHUD()
{
    QFont fuente("Segoe UI", 18, QFont::Bold);

    textoVidas = addSimpleText(QString(), QFont("Segoe UI", 22, QFont::Bold));
    textoTiempo = addSimpleText(QString(), fuente);
    textoStats = addSimpleText(QString(), fuente);
    textoAviso = addSimpleText(QString(), QFont("Segoe UI", 26, QFont::Bold));
    textoAgente = addSimpleText(QString(), QFont("Segoe UI", 13));
    textoAyuda = addSimpleText(QStringLiteral("↑ ↓ carril     Espacio: mantener y soltar para lanzar     "
                                              "D: ver al Director     P: pausa"),
                               QFont("Segoe UI", 12));

    QGraphicsSimpleTextItem *textos[] = {textoVidas, textoTiempo, textoStats, textoAviso, textoAgente, textoAyuda};
    for (QGraphicsSimpleTextItem *t : textos) {
        t->setBrush(QBrush(Qt::white));
        t->setZValue(100);
    }
    textoVidas->setPos(20, 6);
    textoStats->setPos(Proyeccion::ANCHO - 260, 10);
    textoAgente->setPos(Proyeccion::ANCHO - 300, 90);
    textoAgente->setVisible(false);
    textoAyuda->setBrush(QBrush(QColor(255, 255, 255, 190)));
    textoAyuda->setPos((Proyeccion::ANCHO - textoAyuda->boundingRect().width()) / 2.0, 690);

    fondoAviso = addRect(0, 0, 10, 10, QPen(Qt::NoPen), QBrush(QColor(0, 0, 0, 150)));
    fondoAviso->setZValue(99);
    fondoAviso->setVisible(false);

    for (int i = 0; i < EganDefensor::MAX_BIDONES; ++i) {
        QGraphicsPixmapItem *icono = addPixmap(BancoSprites::obtener("bidon", LADO_ICONO, LADO_ICONO,
                                                                     QColor(60, 160, 220)));
        icono->setPos(20 + i * 42, 46);
        icono->setZValue(100);
        iconosBidon.push_back(icono);
    }
    barraRecargaFondo = addRect(20, 88, ANCHO_BARRA, 8, QPen(Qt::NoPen), QBrush(QColor(0, 0, 0, 120)));
    barraRecarga = addRect(20, 88, 0, 8, QPen(Qt::NoPen), QBrush(QColor(60, 160, 220)));
    barraFuerzaFondo = addRect(20, 102, ANCHO_BARRA, 12, QPen(QColor(255, 255, 255), 2), QBrush(QColor(0, 0, 0, 120)));
    barraFuerza = addRect(20, 102, 0, 12, QPen(Qt::NoPen), QBrush(QColor(252, 209, 22)));
    barraRecargaFondo->setZValue(100);
    barraRecarga->setZValue(101);
    barraFuerzaFondo->setZValue(100);
    barraFuerza->setZValue(101);
}


void EscenaNivel2::cuadro()
{
    double dt = reloj.nsecsElapsed() / 1.0e9;
    reloj.start();
    if (dt > 0.05)
        dt = 0.05;
    if (pausado)
        return;

    if (logica->getEstado() == EstadoNivel2::JUGANDO)
        logica->actualizar(dt);
    sincronizar(dt);
    actualizarHUD();

    if (!resultadoMostrado && logica->getEstado() != EstadoNivel2::JUGANDO)
        mostrarResultado();
}

void EscenaNivel2::keyPressEvent(QKeyEvent *evento)
{
    if (evento->isAutoRepeat())
        return;

    const bool jugando = !pausado && logica->getEstado() == EstadoNivel2::JUGANDO;
    switch (evento->key()) {
    case Qt::Key_Up:     if (jugando) logica->cambiarCarril(-1); break;
    case Qt::Key_Down:   if (jugando) logica->cambiarCarril(+1); break;
    case Qt::Key_Space:  if (jugando) logica->iniciarCarga(); break;
    case Qt::Key_P:      pausado = !pausado; break;
    case Qt::Key_D:      mostrarAgente = !mostrarAgente; break;
    case Qt::Key_Escape: emit volverAlMenu(); break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        if (resultadoMostrado)
            emit nivelTerminado(logica->getEstado() == EstadoNivel2::GANADO);
        break;
    default:
        QGraphicsScene::keyPressEvent(evento);
    }
}

void EscenaNivel2::keyReleaseEvent(QKeyEvent *evento)
{
    if (evento->isAutoRepeat())
        return;
    if (evento->key() == Qt::Key_Space) {
        if (!pausado && logica->getEstado() == EstadoNivel2::JUGANDO)
            logica->soltarCarga();
    } else {
        QGraphicsScene::keyReleaseEvent(evento);
    }
}


void EscenaNivel2::sincronizar(double dt)
{
    carrilVisual += (logica->getEgan().getCarril() - carrilVisual) * std::min(1.0, dt * 14.0);
    carrilVisual = std::min(std::max(carrilVisual, 1.0), double(LogicaNivel2::NUM_CARRILES));

    sincronizarEgan();
    sincronizarRivales(dt);
    sincronizarBidones();
    sincronizarObjetos();
}

void EscenaNivel2::sincronizarEgan()
{
    const EganDefensor &egan = logica->getEgan();

    PuntoPantalla p = Proyeccion::nivel2(EganDefensor::X, carrilVisual);
    spriteEgan->setPos(p.x, p.y);
    spriteEgan->setScale(p.escala);
    spriteEgan->setRotation(egan.getAnguloTambaleo());
    spriteEgan->setZValue(zDeCarril(carrilVisual, 5));
    sombraEgan->setPos(p.x, p.y);
    sombraEgan->setScale(p.escala);

    if (egan.estaCargando()) {
        PuntoPantalla m = Proyeccion::nivel2(EganDefensor::X + egan.getAlcancePrevisto(), carrilVisual);
        marcador->setPos(m.x, m.y);
        marcador->setScale(m.escala);
        marcador->setVisible(true);
    } else {
        marcador->setVisible(false);
    }
}

EscenaNivel2::ItemRival EscenaNivel2::crearItemRival(TipoRival tipo)
{
    ItemRival item;
    item.sombra = addEllipse(-45, -9, 90, 18, QPen(Qt::NoPen), QBrush(QColor(0, 0, 0, 100)));
    item.sombra->setZValue(7);

    item.sprite = addPixmap(QPixmap());
    ponerSprite(item.sprite, nombreRival(tipo), ladoRival(tipo), colorRival(tipo), true);
    return item;
}

void EscenaNivel2::liberarRival(ItemRival &item)
{
    removeItem(item.sprite);
    delete item.sprite;
    removeItem(item.sombra);
    delete item.sombra;
    item.sprite = nullptr;
    item.sombra = nullptr;
}

void EscenaNivel2::sincronizarRivales(double dt)
{
    std::set<int> vigentes;

    for (const Rival *r : logica->getRivales()) {
        const int id = r->getId();
        vigentes.insert(id);

        auto it = rivales.find(id);
        if (it == rivales.end())
            it = rivales.emplace(id, crearItemRival(r->getTipo())).first;
        ItemRival &item = it->second;

        PuntoPantalla p = Proyeccion::nivel2(r->getX(), r->getCarril());
        item.sprite->setPos(p.x, p.y);
        item.sprite->setScale(p.escala);
        item.sprite->setRotation(r->getAnguloTambaleo());
        item.sprite->setZValue(zDeCarril(r->getCarril(), 3));
        item.sombra->setPos(p.x, p.y);
        item.sombra->setScale(p.escala);

        if (r->estaDetenido()) {
            item.tiempoDetenido += dt;
            item.sprite->setOpacity(std::max(0.0, 1.0 - item.tiempoDetenido / Rival::DURACION_ATURDIMIENTO));
        } else {
            item.sprite->setOpacity(r->estaAturdido() ? 0.75 : 1.0);
        }
    }

    for (auto it = rivales.begin(); it != rivales.end(); ) {
        if (vigentes.count(it->first) == 0) {
            liberarRival(it->second);
            it = rivales.erase(it);
        } else {
            ++it;
        }
    }
}

void EscenaNivel2::liberarBidon(ItemBidon &item)
{
    removeItem(item.sprite);
    delete item.sprite;
    removeItem(item.sombra);
    delete item.sombra;
    item.sprite = nullptr;
    item.sombra = nullptr;
}

void EscenaNivel2::sincronizarBidones()
{
    std::set<const Proyectil *> vigentes;

    for (const Proyectil *p : logica->getBidones().lista()) {
        vigentes.insert(p);

        auto it = bidones.find(p);
        if (it == bidones.end()) {
            ItemBidon nuevo;
            nuevo.sombra = addEllipse(-20, -6, 40, 12, QPen(Qt::NoPen), QBrush(QColor(0, 0, 0, 110)));
            nuevo.sombra->setZValue(7);
            nuevo.sprite = addPixmap(QPixmap());
            QPixmap pm = BancoSprites::obtener("bidon", LADO_BIDON, LADO_BIDON, QColor(60, 160, 220));
            nuevo.sprite->setPixmap(pm);
            nuevo.sprite->setOffset(-pm.width() / 2.0, -pm.height() / 2.0);
            it = bidones.emplace(p, nuevo).first;
        }
        ItemBidon &item = it->second;

        PuntoPantalla suelo = Proyeccion::nivel2(p->getX(), p->getCarril(), 0.0);
        PuntoPantalla aire = Proyeccion::nivel2(p->getX(), p->getCarril(), p->getY());
        const double mitad = LADO_BIDON / 2.0 * aire.escala;

        item.sprite->setPos(aire.x, aire.y - mitad);
        item.sprite->setScale(aire.escala);
        item.sprite->setRotation(p->getX() * 40.0);
        item.sprite->setZValue(zDeCarril(p->getCarril(), 9));

        item.sombra->setPos(suelo.x, suelo.y);
        item.sombra->setScale(suelo.escala * std::max(0.4, 1.0 - p->getY() / 12.0));
    }

    for (auto it = bidones.begin(); it != bidones.end(); ) {
        if (vigentes.count(it->first) == 0) {
            liberarBidon(it->second);
            it = bidones.erase(it);
        } else {
            ++it;
        }
    }
}

EscenaNivel2::ItemObjeto EscenaNivel2::crearItemObjeto(bool esGuitarra)
{
    ItemObjeto item;
    item.esGuitarra = esGuitarra;

    item.sprite = addPixmap(QPixmap());
    const int lado = esGuitarra ? LADO_GUITARRA : LADO_AFICIONADO;
    QPixmap pm = BancoSprites::obtener(esGuitarra ? "guitarra" : "aficionado", lado, lado,
                                       esGuitarra ? QColor(200, 120, 40) : QColor(230, 190, 40));
    item.sprite->setPixmap(pm);
    item.sprite->setOffset(-pm.width() / 2.0, -pm.height());

    item.barra = addRect(-30, 0, 60, 6, QPen(Qt::NoPen), QBrush(QColor(252, 209, 22)));
    return item;
}

void EscenaNivel2::liberarObjeto(ItemObjeto &item)
{
    removeItem(item.sprite);
    delete item.sprite;
    removeItem(item.barra);
    delete item.barra;
    item.sprite = nullptr;
    item.barra = nullptr;
}

void EscenaNivel2::sincronizarObjetos()
{
    std::set<const ObjetoCarril *> vigentes;

    for (const ObjetoCarril *o : logica->getObjetos()) {
        vigentes.insert(o);
        const bool esGuitarra = (dynamic_cast<const GuitarraCarril *>(o) != nullptr);

        auto it = objetos.find(o);
        if (it != objetos.end() && it->second.esGuitarra != esGuitarra) {
            liberarObjeto(it->second);
            objetos.erase(it);
            it = objetos.end();
        }
        if (it == objetos.end())
            it = objetos.emplace(o, crearItemObjeto(esGuitarra)).first;
        ItemObjeto &item = it->second;

        PuntoPantalla p = Proyeccion::nivel2(EganDefensor::X, o->getCarril());
        const double x = p.x - 70.0 * p.escala;
        item.sprite->setPos(x, p.y);
        item.sprite->setScale(p.escala);
        item.sprite->setZValue(zDeCarril(o->getCarril(), 1));

        const double fraccion = std::min(1.0, o->getTiempoRestante() / ObjetoCarril::DURACION);
        const double altoSprite = (esGuitarra ? LADO_GUITARRA : LADO_AFICIONADO) * p.escala;
        item.barra->setRect(-30, 0, 60 * fraccion, 6);
        item.barra->setPos(x, p.y - altoSprite - 12);
        item.barra->setZValue(zDeCarril(o->getCarril(), 1));
    }

    for (auto it = objetos.begin(); it != objetos.end(); ) {
        if (vigentes.count(it->first) == 0) {
            liberarObjeto(it->second);
            it = objetos.erase(it);
        } else {
            ++it;
        }
    }
}

void EscenaNivel2::actualizarHUD()
{
    const EganDefensor &egan = logica->getEgan();

    QString corazones;
    for (int i = 0; i < logica->getVidas(); ++i)
        corazones += QStringLiteral("♥ ");
    textoVidas->setText(QStringLiteral("Vidas: ") + corazones);

    textoTiempo->setText(QStringLiteral("Tiempo: ")
                         + minutosSegundos(LogicaNivel2::DURACION - logica->getTiempo()));
    textoTiempo->setPos((Proyeccion::ANCHO - textoTiempo->boundingRect().width()) / 2.0, 8);

    textoStats->setText(QStringLiteral("Detenidos: ") + QString::number(logica->getRivalesDetenidos())
                        + QStringLiteral("\nPasaron: ") + QString::number(logica->getRivalesCruzaron()));

    for (int i = 0; i < static_cast<int>(iconosBidon.size()); ++i)
        iconosBidon[i]->setOpacity(i < egan.getBidones() ? 1.0 : 0.2);

    barraRecarga->setRect(20, 88, ANCHO_BARRA * std::min(1.0, egan.getProgresoRecarga()), 8);
    double fuerza = egan.estaCargando() ? egan.getTiempoCarga() / EganDefensor::TIEMPO_CARGA_MAX : 0.0;
    barraFuerza->setRect(20, 102, ANCHO_BARRA * std::min(1.0, fuerza), 12);

    QString aviso;
    QColor color(255, 255, 255);
    if (pausado) {
        aviso = QStringLiteral("PAUSA");
    } else if (egan.estaTambaleando()) {
        aviso = QStringLiteral("¡Te tocaron!");
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
    if (aviso.isEmpty()) {
        fondoAviso->setVisible(false);
    } else {
        QRectF caja = textoAviso->boundingRect();
        const double x = (Proyeccion::ANCHO - caja.width()) / 2.0;
        textoAviso->setPos(x, 44);
        fondoAviso->setRect(x - 14, 40, caja.width() + 28, caja.height() + 8);
        fondoAviso->setVisible(true);
    }

    textoAgente->setVisible(mostrarAgente);
    if (mostrarAgente) {
        const AgenteDirector &agente = logica->getAgente();
        QString estadoTabla = logica->tablaQDanada() ? QStringLiteral("dañada (empieza de cero)")
                                                     : (logica->tablaQCargada() ? QStringLiteral("cargada de la partida anterior")
                                                                                : QStringLiteral("nueva"));
        textoAgente->setText(QStringLiteral("DIRECTOR DE INEOS\n")
                             + QStringLiteral("Exploración (ε): ") + QString::number(agente.getEpsilon(), 'f', 2)
                             + QStringLiteral("\nÉxito reciente: ")
                             + QString::number(static_cast<int>(std::lround(agente.getTasaExito() * 100)))
                             + QStringLiteral(" %\nSituaciones aprendidas: ")
                             + QString::number(agente.getTabla().situacionesVistas())
                             + QStringLiteral(" de 10\nTabla Q: ") + estadoTabla);
    }
}

void EscenaNivel2::mostrarResultado()
{
    resultadoMostrado = true;

    velo = addRect(0, 0, Proyeccion::ANCHO, Proyeccion::ALTO, QPen(Qt::NoPen), QBrush(QColor(0, 0, 0, 170)));
    velo->setZValue(200);

    const bool gano = (logica->getEstado() == EstadoNivel2::GANADO);
    QString texto = gano ? QStringLiteral("¡Llegaste a París!") : QStringLiteral("Ineos pasó la línea de defensa");
    texto += QStringLiteral("\n\nRivales detenidos: ") + QString::number(logica->getRivalesDetenidos());
    texto += QStringLiteral("\nRivales que pasaron: ") + QString::number(logica->getRivalesCruzaron());
    texto += QStringLiteral("\nVidas restantes: ") + QString::number(logica->getVidas());
    if (logica->aprendizajeGuardado())
        texto += QStringLiteral("\n\nEl Director guardó lo que aprendió.");
    texto += QStringLiteral("\n\nEnter: continuar     Esc: menú");

    textoResultado = addSimpleText(texto, QFont("Segoe UI", 28, QFont::Bold));
    textoResultado->setBrush(QBrush(QColor(252, 209, 22)));
    textoResultado->setZValue(201);
    QRectF caja = textoResultado->boundingRect();
    textoResultado->setPos((Proyeccion::ANCHO - caja.width()) / 2.0,
                           (Proyeccion::ALTO - caja.height()) / 2.0);
}