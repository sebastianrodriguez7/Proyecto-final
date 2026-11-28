#include "sonidos.h"
#include <QAudioOutput>
#include <QFile>
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QUrl>

namespace {
const float VOLUMEN_EFECTOS = 0.8f;
const float VOLUMEN_FONDO = 0.35f;
const float VOLUMEN_FONDO_BAJO = 0.10f;
const float VOLUMEN_SENTIMENTAL = 0.8f;
}

Sonidos::Sonidos(QObject *parent)
    : QObject(parent),
    sentimentalActiva(false),
    silenciado(false)
{
}

void Sonidos::reproducir(Sonido sonido)
{
    if (silenciado)
        return;

    auto it = efectos.find(sonido);
    if (it == efectos.end()) {
        QSoundEffect *efecto = nullptr;
        const QString nombre = QString::fromStdString(nombreSonido(sonido));
        if (QFile::exists(":/audio/" + nombre + ".wav")) {
            efecto = new QSoundEffect(this);
            efecto->setSource(QUrl("qrc:/audio/" + nombre + ".wav"));
            efecto->setVolume(VOLUMEN_EFECTOS);
        }
        it = efectos.emplace(sonido, efecto).first;
    }
    if (it->second != nullptr)
        it->second->play();
}

QString Sonidos::buscarMusica(const QString &nombre)
{
    const char *extensiones[] = {".mp3", ".ogg", ".wav"};
    for (const char *extension : extensiones) {
        const QString ruta = ":/audio/" + nombre + extension;
        if (QFile::exists(ruta))
            return QStringLiteral("qrc") + ruta;
    }
    return QString();
}

void Sonidos::crearMusica(Musica &musica, const QString &nombre)
{
    const QString url = buscarMusica(nombre);
    if (url.isEmpty())
        return;

    musica.salida = new QAudioOutput(this);
    musica.reproductor = new QMediaPlayer(this);
    musica.reproductor->setAudioOutput(musica.salida);
    musica.reproductor->setSource(QUrl(url));
    musica.reproductor->setLoops(QMediaPlayer::Infinite);
}

void Sonidos::aplicarVolumenes()
{
    if (fondo.salida != nullptr) {
        float volumen = VOLUMEN_FONDO;
        if (sentimentalActiva && sentimental.reproductor != nullptr)
            volumen = VOLUMEN_FONDO_BAJO;
        fondo.salida->setVolume(silenciado ? 0.0f : volumen);
    }
    if (sentimental.salida != nullptr)
        sentimental.salida->setVolume(silenciado ? 0.0f : VOLUMEN_SENTIMENTAL);
}

void Sonidos::iniciarMusicaFondo(const QString &nombre)
{
    crearMusica(fondo, nombre);
    aplicarVolumenes();
    if (fondo.reproductor != nullptr)
        fondo.reproductor->play();
}

void Sonidos::musicaSentimental(bool activa)
{
    if (activa == sentimentalActiva)
        return;
    sentimentalActiva = activa;

    if (activa) {
        if (sentimental.reproductor == nullptr)
            crearMusica(sentimental, "musica_sentimental");
        if (sentimental.reproductor != nullptr)
            sentimental.reproductor->play();
    } else if (sentimental.reproductor != nullptr) {
        sentimental.reproductor->stop();
    }
    aplicarVolumenes();
}

void Sonidos::pausar(bool pausa)
{
    if (fondo.reproductor != nullptr) {
        if (pausa) fondo.reproductor->pause();
        else       fondo.reproductor->play();
    }
    if (sentimental.reproductor != nullptr && sentimentalActiva) {
        if (pausa) sentimental.reproductor->pause();
        else       sentimental.reproductor->play();
    }
}

void Sonidos::alternarSilencio()
{
    silenciado = !silenciado;
    aplicarVolumenes();
}

bool Sonidos::estaSilenciado() const { return silenciado; }