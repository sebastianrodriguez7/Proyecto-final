#ifndef VISTAJUEGO_H
#define VISTAJUEGO_H

#include <QGraphicsView>

class QResizeEvent;

class VistaJuego : public QGraphicsView
{
    Q_OBJECT

public:
    explicit VistaJuego(QWidget *parent = nullptr);

    void ajustar();

protected:
    void resizeEvent(QResizeEvent *evento) override;
};

#endif // VISTAJUEGO_H