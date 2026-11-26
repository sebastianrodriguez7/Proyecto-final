#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include <QWidget>
#include "estrategiadificultad.h"

class QComboBox;

class MenuPrincipal : public QWidget
{
    Q_OBJECT

public:
    explicit MenuPrincipal(QWidget *parent = nullptr);

signals:
    void jugarSolicitado(Dificultad dificultad);
    void salirSolicitado();

private slots:
    void alPresionarJugar();

private:
    QComboBox *selectorDificultad;
};

#endif // MENUPRINCIPAL_H