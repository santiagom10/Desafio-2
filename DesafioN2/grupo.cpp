#include "grupo.h"
#include "DesafioN2Libreria.h"
#include <iostream>

using namespace std;

//Constructor
Grupo::Grupo() {
    letra = 'A';// Inicializa el grupo con la letra A
    cantidadEquipos = 0;
    equipos = new Equipo*[4];// Reserva memoria para 4 punteros a Equipo

    // Inicializa sin equipo
    for(int i = 0; i < 4; i++)
        equipos[i] = nullptr;
}

//Constructor con parámetro
Grupo::Grupo(char letra) {
    this->letra = letra;
    cantidadEquipos = 0;
    equipos = new Equipo*[4];// Reserva espacio para 4 equipos

    for(int i = 0; i < 4; i++)
        equipos[i] = nullptr;
}

//Constructor de copia
Grupo::Grupo(const Grupo& otro) {
    letra = otro.letra;
    cantidadEquipos = otro.cantidadEquipos;// Copia cantidad de equipos
    equipos = new Equipo*[4];

    //Copia cada equipo
    for(int i = 0; i < cantidadEquipos; i++) {
        equipos[i] = new Equipo(*otro.equipos[i]);
    }

    for(int i = cantidadEquipos; i < 4; i++) {
        equipos[i] = nullptr;
    }
}

//Destructor
Grupo::~Grupo() {
    for(int i = 0; i < cantidadEquipos; i++) {
        delete equipos[i];
    }

    delete[] equipos;
}

//Sobrecarga del operador
Grupo& Grupo::operator=(const Grupo& otro) {
    if(this != &otro) {

        //Libera memoria
        for(int i = 0; i < cantidadEquipos; i++) {
            delete equipos[i];
        }
        delete[] equipos;

        //Copia datos del otro objeto
        letra = otro.letra;
        cantidadEquipos = otro.cantidadEquipos;
        equipos = new Equipo*[4];

        //Copia de los equipos
        for(int i = 0; i < cantidadEquipos; i++) {
            equipos[i] = new Equipo(*otro.equipos[i]);
        }

        for(int i = cantidadEquipos; i < 4; i++) {
            equipos[i] = nullptr;
        }
    }
    return *this; // Retorna el objeto actual
}

//modificar la letra
void Grupo::setLetra(char letra) {
    this->letra = letra;
}

//consultar valor de la letra
char Grupo::getLetra() const {
    return letra;
}

//cuántos equipos hay en el grupo
int Grupo::getCantidadEquipos() const {
    return cantidadEquipos;
}

// equipo en la posicion
Equipo* Grupo::getEquipo(int i) const {
    if(i < 0 || i >= cantidadEquipos)
        return nullptr;

    return equipos[i];
}

//Verifica si se puede agregar un equipo al grupo
bool Grupo::puedeAgregarEquipo(Equipo* equipo) const {

    //No permite agregar si hay 4 equipos
    if(cantidadEquipos >= 4 || equipo == nullptr)
        return false;

    int cantidadUEFA = 0; // Cuenta equipos UEFA

    //Recorre los equipos actuales
    for(int i = 0; i < cantidadEquipos; i++) {

        //Si son de la misma confederacion
        if(sonIguales(equipos[i]->getConfederacion(), equipo->getConfederacion())) {

            //Si es UEFA, se permite maximo 2
            if(sonIguales(equipo->getConfederacion(), "UEFA")) {
                cantidadUEFA++;
            }
            else {
                return false; //No repetir otras confederaciones
            }
        }
    }

    //Verifica que no haya mas de 2 UEFA
    if(sonIguales(equipo->getConfederacion(), "UEFA") && cantidadUEFA >= 2)
        return false;

    return true;
}

//Agrega un equipo si cumple
bool Grupo::agregarEquipo(Equipo* equipo) {

    //Verifica primero si se puede agregar
    if(!puedeAgregarEquipo(equipo))
        return false;

    //Lo añade y aumenta el contador
    equipos[cantidadEquipos++] = equipo;
    return true;
}

//Imprime la información del grupo
void Grupo::imprimir() const {
    cout << "Grupo " << letra << endl;

    // Recorre e imprime cada equipo
    for(int i = 0; i < cantidadEquipos; i++) {
        cout << " - " << equipos[i]->getPais()
        << " (" << equipos[i]->getConfederacion() << ")" << endl;
    }
}

//Ordena la tabla de posiciones del grupo
void Grupo::ordenarTabla() {

    //Recorre cada posicion
    for(int i = 0; i < cantidadEquipos - 1; i++) {

        int mejor = i; //Se asume que el mejor equipo esta en la posición actual

        //Busca en el arreglo un equipo mejor
        for(int j = i + 1; j < cantidadEquipos; j++) {

            //Compara por puntos
            if(equipos[j]->getPuntosGrupo() > equipos[mejor]->getPuntosGrupo()) {
                mejor = j; //Se encontro un equipo mejor
            }
            //Si empatan en puntos
            else if(equipos[j]->getPuntosGrupo() == equipos[mejor]->getPuntosGrupo()) {

                //Desempate por diferencia de goles
                if(equipos[j]->getDiferenciaGolesGrupo() > equipos[mejor]->getDiferenciaGolesGrupo()) {
                    mejor = j;
                }
                //Si siguen empatados
                else if(equipos[j]->getDiferenciaGolesGrupo() == equipos[mejor]->getDiferenciaGolesGrupo()) {

                    //Desempate por goles
                    if(equipos[j]->getGolesFavorGrupo() > equipos[mejor]->getGolesFavorGrupo()) {
                        mejor = j;
                    }
                }
            }
        }

        //Si se encontro un mejor equipo, se intercambia
        if(mejor != i) {
            Equipo* temp = equipos[i];   // Guarda el equipo actual
            equipos[i] = equipos[mejor]; // Coloca el mejor en su posicion
            equipos[mejor] = temp;
        }
    }
}
