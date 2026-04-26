TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        fecha.cpp \
        grupo.cpp \
        equipo.cpp \
        jugador.cpp \
        main.cpp \
        medidor_recursos.cpp \
        partido.cpp \
        registro_jugador_partido.cpp \
        torneo.cpp \
        utilidades.cpp

HEADERS += \
    equipo.h \
    fecha.h \
    grupo.h \
    jugador.h \
    medidor_recursos.h \
    partido.h \
    registro_jugador_partido.h \
    torneo.h \
    utilidades.h
