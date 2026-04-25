#include "jugador.h"
#include "utilidades.h"
#include <iostream>

using namespace std;

Jugador::Jugador() {
    nombre = copiarCadena("");
    apellido = copiarCadena("");
    numeroCamiseta = 0;
    partidosJugados = 0;
    goles = 0;
    minutosJugados = 0;
    asistencias = 0;
    tarjetasAmarillas = 0;
    tarjetasRojas = 0;
    faltas = 0;
    equipo = 0;
}

Jugador::Jugador(const char* nombre, const char* apellido, int numeroCamiseta) {
    this->nombre = copiarCadena(nombre);
    this->apellido = copiarCadena(apellido);
    this->numeroCamiseta = numeroCamiseta;
    partidosJugados = 0;
    goles = 0;
    minutosJugados = 0;
    asistencias = 0;
    tarjetasAmarillas = 0;
    tarjetasRojas = 0;
    faltas = 0;
    equipo = 0;
}

Jugador::Jugador(const Jugador& otro) {
    nombre = copiarCadena(otro.nombre);
    apellido = copiarCadena(otro.apellido);
    numeroCamiseta = otro.numeroCamiseta;
    partidosJugados = otro.partidosJugados;
    goles = otro.goles;
    minutosJugados = otro.minutosJugados;
    asistencias = otro.asistencias;
    tarjetasAmarillas = otro.tarjetasAmarillas;
    tarjetasRojas = otro.tarjetasRojas;
    faltas = otro.faltas;
    equipo = otro.equipo;
}

Jugador::~Jugador() {
    delete[] nombre;
    delete[] apellido;
}

Jugador& Jugador::operator=(const Jugador& otro) {
    if (this != &otro) {
        delete[] nombre;
        delete[] apellido;
        nombre = copiarCadena(otro.nombre);
        apellido = copiarCadena(otro.apellido);
        numeroCamiseta = otro.numeroCamiseta;
        partidosJugados = otro.partidosJugados;
        goles = otro.goles;
        minutosJugados = otro.minutosJugados;
        asistencias = otro.asistencias;
        tarjetasAmarillas = otro.tarjetasAmarillas;
        tarjetasRojas = otro.tarjetasRojas;
        faltas = otro.faltas;
        equipo = otro.equipo;
    }
    return *this;
}

bool Jugador::operator>(const Jugador& otro) const {
    return goles > otro.goles;
}

bool Jugador::operator==(const Jugador& otro) const {
    return numeroCamiseta == otro.numeroCamiseta;
}

void Jugador::setNombre(const char* nombre) {
    delete[] this->nombre;
    this->nombre = copiarCadena(nombre);
}

void Jugador::setApellido(const char* apellido) {
    delete[] this->apellido;
    this->apellido = copiarCadena(apellido);
}

void Jugador::setNumeroCamiseta(int numero) { numeroCamiseta = numero; }
void Jugador::setEquipo(Equipo* equipo) { this->equipo = equipo; }
void Jugador::setPartidosJugados(int valor) { partidosJugados = valor; }
void Jugador::setGoles(int valor) { goles = valor; }
void Jugador::setMinutosJugados(int valor) { minutosJugados = valor; }
void Jugador::setAsistencias(int valor) { asistencias = valor; }
void Jugador::setTarjetasAmarillas(int valor) { tarjetasAmarillas = valor; }
void Jugador::setTarjetasRojas(int valor) { tarjetasRojas = valor; }
void Jugador::setFaltas(int valor) { faltas = valor; }

const char* Jugador::getNombre() const { return nombre; }
const char* Jugador::getApellido() const { return apellido; }
int Jugador::getNumeroCamiseta() const { return numeroCamiseta; }
Equipo* Jugador::getEquipo() const { return equipo; }
int Jugador::getPartidosJugados() const { return partidosJugados; }
int Jugador::getGoles() const { return goles; }
int Jugador::getMinutosJugados() const { return minutosJugados; }
int Jugador::getAsistencias() const { return asistencias; }
int Jugador::getTarjetasAmarillas() const { return tarjetasAmarillas; }
int Jugador::getTarjetasRojas() const { return tarjetasRojas; }
int Jugador::getFaltas() const { return faltas; }

void Jugador::sumarGol() { goles++; }
void Jugador::sumarPartido() { partidosJugados++; }
void Jugador::sumarMinutos(int minutos) { minutosJugados += minutos; }
void Jugador::sumarAsistencia() { asistencias++; }
void Jugador::sumarAmarilla() { tarjetasAmarillas++; }
void Jugador::sumarRoja() { tarjetasRojas++; }
void Jugador::sumarFalta() { faltas++; }

void Jugador::imprimir() const {
    cout << "Camiseta: " << numeroCamiseta
         << " | " << nombre << " " << apellido
         << " | PJ: " << partidosJugados
         << " | Goles: " << goles
         << " | Min: " << minutosJugados
         << " | A: " << asistencias
         << " | TA: " << tarjetasAmarillas
         << " | TR: " << tarjetasRojas
         << " | F: " << faltas << endl;
}
