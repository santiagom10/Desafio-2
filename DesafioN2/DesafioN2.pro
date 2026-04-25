TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        grupo.cpp \
        equipo.cpp \
        jugador.cpp \
        main.cpp \
        partido.cpp \
        torneo.cpp \
        utilidades.cpp

HEADERS += \
    equipo.h \
    grupo.h \
    jugador.h \
    partido.h \
    torneo.h \
    utilidades.h
