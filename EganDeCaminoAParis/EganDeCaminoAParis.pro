QT += core gui widgets multimedia

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Logica/accion.cpp \
    Logica/agentedirector.cpp \
    Logica/aprendizaje.cpp \
    Logica/bidon.cpp \
    Logica/bidonlanzado.cpp \
    Logica/coleccionable.cpp \
    Logica/detectorcolisiones.cpp \
    Logica/egan.cpp \
    Logica/egandefensor.cpp \
    Logica/entidad.cpp \
    Logica/estado.cpp \
    Logica/estrategiadificultad.cpp \
    Logica/fisicaoscilatoria.cpp \
    Logica/fisicaparabolica.cpp \
    Logica/fisicapendiente.cpp \
    Logica/gestorcoleccionables.cpp \
    Logica/gestorobstaculos.cpp \
    Logica/granizo.cpp \
    Logica/guitarra.cpp \
    Logica/logicanivel1.cpp \
    Logica/logicanivel2.cpp \
    Logica/objetocarril.cpp \
    Logica/percepcion.cpp \
    Logica/proyectil.cpp \
    Logica/razonamiento.cpp \
    Logica/rival.cpp \
    Logica/roca.cpp \
    Logica/sentimental.cpp \
    Logica/tablaq.cpp \
    Logica/terreno.cpp \
    gui/bancosprites.cpp \
    gui/escenanivel1.cpp \
    gui/escenanivel2.cpp \
    gui/eventossonido.cpp \
    gui/fabricafondos.cpp \
    gui/mainwindow.cpp \
    gui/menuprincipal.cpp \
    gui/proyeccion.cpp \
    gui/sonidos.cpp \
    gui/vistajuego.cpp \
    main.cpp

HEADERS += \
    Logica/accion.h \
    Logica/agentedirector.h \
    Logica/aprendizaje.h \
    Logica/bidon.h \
    Logica/bidonlanzado.h \
    Logica/coleccionable.h \
    Logica/detectorcolisiones.h \
    Logica/egan.h \
    Logica/egandefensor.h \
    Logica/entidad.h \
    Logica/estado.h \
    Logica/estrategiadificultad.h \
    Logica/fisicaoscilatoria.h \
    Logica/fisicaparabolica.h \
    Logica/fisicapendiente.h \
    Logica/gestorcoleccionables.h \
    Logica/gestorobstaculos.h \
    Logica/granizo.h \
    Logica/guitarra.h \
    Logica/logicanivel1.h \
    Logica/logicanivel2.h \
    Logica/objetocarril.h \
    Logica/percepcion.h \
    Logica/proyectil.h \
    Logica/razonamiento.h \
    Logica/rival.h \
    Logica/roca.h \
    Logica/sentimental.h \
    Logica/tablaq.h \
    Logica/terreno.h \
    gui/bancosprites.h \
    gui/escenanivel1.h \
    gui/escenanivel2.h \
    gui/eventossonido.h \
    gui/fabricafondos.h \
    gui/mainwindow.h \
    gui/menuprincipal.h \
    gui/proyeccion.h \
    gui/sonidos.h \
    gui/vistajuego.h

FORMS += \
    gui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
INCLUDEPATH += $$PWD/gui $$PWD/logica

RESOURCES += \
    Recursos/recursos.qrc