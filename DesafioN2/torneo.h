#ifndef TORNEO_H
#define TORNEO_H

#include "equipo.h"
#include "grupo.h"
#include "partido.h"
#include "medidor_recursos.h"

class Torneo {
private:
    Equipo* equipos;
    int cantidadEquipos;
    Grupo* grupos;
    int cantidadGrupos;
    Equipo** bombos[4];
    int cantidadesBombos[4];

    Partido* partidosGrupos;
    int cantidadPartidosGrupos;
    Equipo** clasificadosR16;
    int cantidadClasificadosR16;
    Partido* partidosR16;
    int cantidadPartidosR16;
    Partido* partidosR8;
    int cantidadPartidosR8;
    Partido* partidosQF;
    int cantidadPartidosQF;
    Partido* partidosSF;
    int cantidadPartidosSF;
    Partido* partidosFinales;
    int cantidadPartidosFinales;
    Equipo** top4;

    MedidorRecursos medidor;

    bool equipoYaJuegaEseDia(Equipo* equipo, const Fecha& fecha, int hastaIndice) const;
    bool equipoRompeReglaTresDias(Equipo* equipo, const Fecha& fecha, int hastaIndice) const;
    void reiniciarDatosDeGrupo();
    void ordenarTerceros(Equipo** terceros, int cantidad);
    void ordenarEquiposPorGrupo(Equipo** lista, int cantidad);
    Equipo* ganadorDePartido(const Partido& partido) const;
    Equipo* perdedorDePartido(const Partido& partido) const;
    void configurarPartidoEliminacion(Partido& partido, Equipo* a, Equipo* b, const char* etapa);

public:
    Torneo();
    ~Torneo();

    void cargarEquiposDesdeCSV(const char* nombreArchivo);
    void crearJugadoresDeTodosLosEquipos();
    void mostrarEquipos() const;
    void ordenarEquiposPorRanking();
    void formarBombos();
    void mostrarBombos() const;
    void sortearGrupos();
    void mostrarGrupos() const;
    void generarPartidosDeGrupos();
    void simularFaseDeGrupos();
    void mostrarTablaDeGrupos();
    void clasificarADieciseisavos();
    void simularFasesFinales();
    void mostrarEstadisticasFinales();
    void mostrarRecursosBasicos(const char* funcionalidad);
    void cargarHistoricosJugadores(const char* nombreArchivo);
    void guardarHistoricosJugadores(const char* nombreArchivo) const;

    int getCantidadEquipos() const;
    Equipo* getEquipo(int i) const;
};

#endif
