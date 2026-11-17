QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Logica/egan.cpp \
    Logica/entidad.cpp \
    Logica/fisicapendiente.cpp \
    Logica/terreno.cpp \
    gui/mainwindow.cpp \
    gui/nivel1.cpp \
    main.cpp

HEADERS += \
    Logica/egan.h \
    Logica/entidad.h \
    Logica/fisicapendiente.h \
    Logica/terreno.h \
    gui/mainwindow.h \
    gui/nivel1.h

FORMS += \
    gui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
INCLUDEPATH += $$PWD/gui $$PWD/logica