#include "medidor_recursos.h"
#include "utilidades.h"
#include <iostream>

using namespace std;

MedidorRecursos::MedidorRecursos() {
    iteraciones = 0;
}

MedidorRecursos::~MedidorRecursos() {}

void MedidorRecursos::reiniciar() {
    iteraciones = 0;
}

void MedidorRecursos::sumarIteracion() {
    iteraciones++;
}

void MedidorRecursos::sumarIteraciones(long long cantidad) {
    iteraciones += cantidad;
}

long long MedidorRecursos::getIteraciones() const {
    return iteraciones;
}

long long MedidorRecursos::memoriaJugador(const Jugador& jugador) const {
    long long total = sizeof(jugador);
    total += longitudCadena(jugador.getNombre()) + 1;
    total += longitudCadena(jugador.getApellido()) + 1;
    return total;
}

long long MedidorRecursos::memoriaEquipo(const Equipo& equipo) const {
    long long total = sizeof(equipo);
    total += longitudCadena(equipo.getPais()) + 1;
    total += longitudCadena(equipo.getConfederacion()) + 1;
    total += longitudCadena(equipo.getDirectorTecnico()) + 1;
    for (int i = 0; i < equipo.getCantidadJugadores(); i++) {
        total += memoriaJugador(equipo.getPlantilla()[i]);
    }
    return total;
}

long long MedidorRecursos::memoriaGrupo(const Grupo& grupo) const {
    long long total = sizeof(grupo);
    total += 4 * sizeof(Equipo*);
    return total;
}

long long MedidorRecursos::memoriaPartido(const Partido& partido) const {
    return sizeof(partido);
}

long long MedidorRecursos::memoriaTorneoBasica(Equipo* equipos, int cantidadEquipos, Grupo* grupos, int cantidadGrupos) const {
    long long total = 0;
    for (int i = 0; i < cantidadEquipos; i++) total += memoriaEquipo(equipos[i]);
    for (int i = 0; i < cantidadGrupos; i++) total += memoriaGrupo(grupos[i]);
    return total;
}

void MedidorRecursos::imprimirResumen(const char* funcionalidad, long long memoria) const {
    cout << "[Recursos] " << funcionalidad << endl;
    cout << "Iteraciones: " << iteraciones << endl;
    cout << "Memoria estimada: " << memoria << " bytes" << endl;
}
