#ifndef NIVEL1_H
#define NIVEL1_H

#include <QGraphicsScene>

class Nivel1 : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Nivel1(QObject *parent = nullptr);

private:
    void dibujarCarriles();
};

#endif // NIVEL1_H