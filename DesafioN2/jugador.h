#ifndef JUGADOR_H
#define JUGADOR_H

class Equipo;

class Jugador {
private:
    char* nombre;
    char* apellido;
    int numeroCamiseta;
    int partidosJugados;
    int goles;
    int minutosJugados;
    int asistencias;
    int tarjetasAmarillas;
    int tarjetasRojas;
    int faltas;
    Equipo* equipo;

public:
    Jugador();
    Jugador(const char* nombre, const char* apellido, int numeroCamiseta);
    Jugador(const Jugador& otro);
    ~Jugador();
    Jugador& operator=(const Jugador& otro);
    bool operator>(const Jugador& otro) const;
    bool operator==(const Jugador& otro) const;

    void setNombre(const char* nombre);
    void setApellido(const char* apellido);
    void setNumeroCamiseta(int numero);
    void setEquipo(Equipo* equipo);
    void setPartidosJugados(int valor);
    void setGoles(int valor);
    void setMinutosJugados(int valor);
    void setAsistencias(int valor);
    void setTarjetasAmarillas(int valor);
    void setTarjetasRojas(int valor);
    void setFaltas(int valor);

    const char* getNombre() const;
    const char* getApellido() const;
    int getNumeroCamiseta() const;
    Equipo* getEquipo() const;
    int getPartidosJugados() const;
    int getGoles() const;
    int getMinutosJugados() const;
    int getAsistencias() const;
    int getTarjetasAmarillas() const;
    int getTarjetasRojas() const;
    int getFaltas() const;

    void sumarGol();
    void sumarPartido();
    void sumarMinutos(int minutos);
    void sumarAsistencia();
    void sumarAmarilla();
    void sumarRoja();
    void sumarFalta();
    void imprimir() const;
};

#endif
