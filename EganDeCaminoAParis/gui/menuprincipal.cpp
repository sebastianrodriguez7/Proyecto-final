#include "menuprincipal.h"
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QStringList>
#include <QVBoxLayout>

MenuPrincipal::MenuPrincipal(QWidget *parent)
    : QWidget(parent),
    selectorDificultad(nullptr)
{
    setObjectName("menu");
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet(
        "#menu { background-color: #14213d; }"
        "QLabel { color: white; font-size: 18px; }"
        "QLabel#titulo { color: #fcd116; font-size: 48px; font-weight: bold; }"
        "QLabel#subtitulo { color: #9fb3d9; font-size: 20px; }"
        "QLabel#controles { color: #c9d4ee; font-size: 16px; }"
        "QPushButton { background-color: #fcd116; color: #14213d; font-size: 22px;"
        "              font-weight: bold; padding: 10px 50px; border-radius: 8px; }"
        "QPushButton:hover { background-color: #ffe066; }"
        "QComboBox { font-size: 20px; padding: 6px; min-width: 200px; }");

    QVBoxLayout *disposicion = new QVBoxLayout(this);
    disposicion->setAlignment(Qt::AlignCenter);
    disposicion->setSpacing(16);

    QLabel *titulo = new QLabel(QStringLiteral("EGAN: camino a París"), this);
    titulo->setObjectName("titulo");
    titulo->setAlignment(Qt::AlignCenter);

    QLabel *subtitulo = new QLabel(QStringLiteral("Fantasías animadas de la historia de Colombia"), this);
    subtitulo->setObjectName("subtitulo");
    subtitulo->setAlignment(Qt::AlignCenter);

    QLabel *etiquetaDificultad = new QLabel(QStringLiteral("Dificultad del nivel 1"), this);
    etiquetaDificultad->setAlignment(Qt::AlignCenter);

    selectorDificultad = new QComboBox(this);
    selectorDificultad->addItems(QStringList() << QStringLiteral("Fácil")
                                               << QStringLiteral("Normal")
                                               << QStringLiteral("Difícil"));
    selectorDificultad->setCurrentIndex(1);

    QPushButton *botonJugar = new QPushButton(QStringLiteral("Jugar"), this);
    QPushButton *botonNivel2 = new QPushButton(QStringLiteral("Practicar nivel 2"), this);
    QPushButton *botonSalir = new QPushButton(QStringLiteral("Salir"), this);

    QLabel *controles = new QLabel(
        QStringLiteral("Nivel 1:  ↑ ↓ cambian de carril    → pedalea fuerte    Espacio salta\n"
                       "Nivel 2:  ↑ ↓ cambian de carril    Espacio (mantener y soltar) lanza el bidón\n"
                       "P: pausa      D: ver al Director (nivel 2)      Esc: volver al menú"),
        this);
    controles->setObjectName("controles");
    controles->setAlignment(Qt::AlignCenter);

    disposicion->addStretch();
    disposicion->addWidget(titulo);
    disposicion->addWidget(subtitulo);
    disposicion->addSpacing(30);
    disposicion->addWidget(etiquetaDificultad);
    disposicion->addWidget(selectorDificultad, 0, Qt::AlignCenter);
    disposicion->addSpacing(10);
    disposicion->addWidget(botonJugar, 0, Qt::AlignCenter);
    disposicion->addWidget(botonNivel2, 0, Qt::AlignCenter);
    disposicion->addWidget(botonSalir, 0, Qt::AlignCenter);
    disposicion->addSpacing(30);
    disposicion->addWidget(controles);
    disposicion->addStretch();

    connect(botonJugar, &QPushButton::clicked, this, &MenuPrincipal::alPresionarJugar);
    connect(botonNivel2, &QPushButton::clicked, this, &MenuPrincipal::practicarNivel2Solicitado);
    connect(botonSalir, &QPushButton::clicked, this, &MenuPrincipal::salirSolicitado);
}

void MenuPrincipal::alPresionarJugar()
{
    Dificultad dificultad = Dificultad::NORMAL;
    switch (selectorDificultad->currentIndex()) {
    case 0: dificultad = Dificultad::FACIL; break;
    case 2: dificultad = Dificultad::DIFICIL; break;
    default: break;
    }
    emit jugarSolicitado(dificultad);
}