#ifndef MEDIDOR_RECURSOS_H
#define MEDIDOR_RECURSOS_H

#include "equipo.h"
#include "grupo.h"
#include "partido.h"

class MedidorRecursos {
private:
    long long iteraciones;

public:
    MedidorRecursos();
    ~MedidorRecursos();

    void reiniciar();
    void sumarIteracion();
    void sumarIteraciones(long long cantidad);
    long long getIteraciones() const;

    long long memoriaJugador(const Jugador& jugador) const;
    long long memoriaEquipo(const Equipo& equipo) const;
    long long memoriaGrupo(const Grupo& grupo) const;
    long long memoriaPartido(const Partido& partido) const;
    long long memoriaTorneoBasica(Equipo* equipos, int cantidadEquipos, Grupo* grupos, int cantidadGrupos) const;
    void imprimirResumen(const char* funcionalidad, long long memoria) const;
};

#endif
