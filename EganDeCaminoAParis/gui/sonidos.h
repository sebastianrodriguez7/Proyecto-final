#ifndef SONIDOS_H
#define SONIDOS_H

#include <map>
#include <QObject>
#include <QString>
#include "eventossonido.h"

class QAudioOutput;
class QMediaPlayer;
class QSoundEffect;

class Sonidos : public QObject
{
    Q_OBJECT

public:
    explicit Sonidos(QObject *parent = nullptr);

    void reproducir(Sonido sonido);
    void iniciarMusicaFondo(const QString &nombre);
    void musicaSentimental(bool activa);
    void pausar(bool pausa);
    void alternarSilencio();
    bool estaSilenciado() const;

private:
    struct Musica
    {
        QMediaPlayer *reproductor = nullptr;
        QAudioOutput *salida = nullptr;
    };

    static QString buscarMusica(const QString &nombre);
    void crearMusica(Musica &musica, const QString &nombre);
    void aplicarVolumenes();

    std::map<Sonido, QSoundEffect *> efectos;
    Musica fondo;
    Musica sentimental;
    bool sentimentalActiva;
    bool silenciado;
};

#endif // SONIDOS_H