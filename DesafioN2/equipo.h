#ifndef EQUIPO_H
#define EQUIPO_H
<<<<<<< HEAD

=======
>>>>>>> df98f771bdfd689014c5c580dd31190156b2dfa3
#include "jugador.h"
class Equipo {
private:
    char* pais;
    char* confederacion;
    char* directorTecnico;
    int rankingFIFA;
    int golesFavorHistoricos;
    int golesContraHistoricos;
    int partidosGanados;
    int partidosEmpatados;
    int partidosPerdidos;
    int tarjetasAmarillas;
    int tarjetasRojas;
    int faltas;
    Jugador* plantilla;
    int cantidadJugadores;
    int puntosGrupo;
    int golesFavorGrupo;
    int golesContraGrupo;
<<<<<<< HEAD

public:
    Equipo();
    Equipo(const char* pais, const char* confederacion, const char*
           directorTecnico, int rankingFIFA,
           int gf, int gc, int pg, int pe, int pp, int ta, int tr, int faltas);
=======
public:
    Equipo();
    Equipo(const char* pais, const char* confederacion, const char*
directorTecnico, int rankingFIFA,
            int gf, int gc, int pg, int pe, int pp, int ta, int tr, int faltas);
>>>>>>> df98f771bdfd689014c5c580dd31190156b2dfa3
    Equipo(const Equipo& otro);
    ~Equipo();
    Equipo& operator=(const Equipo& otro);
    bool operator<(const Equipo& otro) const;
    void setPais(const char* pais);
    void setConfederacion(const char* confederacion);
    void setDirectorTecnico(const char* directorTecnico);
    void setRankingFIFA(int ranking);
    void setGolesFavorHistoricos(int valor);
    void setGolesContraHistoricos(int valor);
    void setPartidosGanados(int valor);
    void setPartidosEmpatados(int valor);
    void setPartidosPerdidos(int valor);
    void setTarjetasAmarillas(int valor);
    void setTarjetasRojas(int valor);
    void setFaltas(int valor);
    const char* getPais() const;
    const char* getConfederacion() const;
    const char* getDirectorTecnico() const;
    int getRankingFIFA() const;
    int getGolesFavorHistoricos() const;
    int getGolesContraHistoricos() const;
    int getPartidosGanados() const;
    int getPartidosEmpatados() const;
    int getPartidosPerdidos() const;
    int getTarjetasAmarillas() const;
    int getTarjetasRojas() const;
    int getFaltas() const;
    int getCantidadJugadores() const;
    Jugador* getPlantilla() const;
    int getPuntosGrupo() const;
    int getGolesFavorGrupo() const;
    int getGolesContraGrupo() const;
    int getDiferenciaGolesGrupo() const;
    void crearPlantillaArtificial();
    void repartirGolesHistoricos();
    void imprimirPlantilla() const;
    void imprimirResumen() const;
    void reiniciarDatosGrupo();
    void registrarResultadoGrupo(int gf, int gc);
    int* obtenerOnceAleatorio() const;
};
<<<<<<< HEAD

=======
>>>>>>> df98f771bdfd689014c5c580dd31190156b2dfa3
#endif
