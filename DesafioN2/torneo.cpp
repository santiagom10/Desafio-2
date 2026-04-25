#include "torneo.h"
#include "DesafioN2Libreria.h"
#include <iostream>
#include <fstream>
using namespace std;

Torneo::Torneo() {
    equipos = 0;
    cantidadEquipos = 0;
    grupos = 0;
    cantidadGrupos = 12;
    for(int i = 0; i < 4; i++) {
        bombos[i] = 0;
        cantidadesBombos[i] = 0;
    }
    inicializarAleatorio();
}

Torneo::~Torneo() {
    delete[] equipos;
    delete[] grupos;
    for(int i = 0; i < 4; i++) {
        delete[] bombos[i];
    }
}

void Torneo::cargarEquiposDesdeCSV(const char* nombreArchivo) {
    if(equipos != 0) {
        delete[] equipos;
        equipos = 0;
    }

    ifstream archivo(nombreArchivo);
    if(!archivo.is_open()) {
        cout << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    cantidadEquipos = 48;
    equipos = new Equipo[cantidadEquipos];
        char linea[512];
        archivo.getline(linea, 512);

    int i = 0;

    while(archivo.getline(linea, 512) && i < cantidadEquipos) {
        char pais[100] = "";
        char confederacion[50] = "";
        char director[50] = "DT";
        char rankingStr[20] = "0";
        char gfStr[20] = "0";
        char gcStr[20] = "0";
        char pgStr[20] = "0";
        char peStr[20] = "0";
        char ppStr[20] = "0";
        int campo = 0;
        int pos = 0;
        char buffer[100];
        int b = 0;

        while(true) {
            char c = linea[pos];
            if(c == ',' || c == '\0') {
                buffer[b] = '\0';
                if(campo == 0) copiarCadenaEnBuffer(pais, buffer);
                else if(campo == 1) copiarCadenaEnBuffer(confederacion, buffer);
                else if(campo == 2) copiarCadenaEnBuffer(rankingStr, buffer);
                else if(campo == 3) copiarCadenaEnBuffer(gfStr, buffer);
                else if(campo == 4) copiarCadenaEnBuffer(gcStr, buffer);
                else if(campo == 5) copiarCadenaEnBuffer(pgStr, buffer);
                else if(campo == 6) copiarCadenaEnBuffer(peStr, buffer);
                else if(campo == 7) copiarCadenaEnBuffer(ppStr, buffer);
                campo++;
                b = 0;
                if(c == '\0') break;
            } else {
                buffer[b++] = c;
            }
            pos++;
        }

        equipos[i] = Equipo(
            pais,
            confederacion,
            director,
            stringAEntero(rankingStr),
            stringAEntero(gfStr),
            stringAEntero(gcStr),
            stringAEntero(pgStr),
            stringAEntero(peStr),
            stringAEntero(ppStr),
            0,
            0,
            0
            );
        i++;
    }

    archivo.close();
    cout << "Equipos cargados correctamente." << endl;
}

void Torneo::crearJugadoresDeTodosLosEquipos() {
    if(equipos == 0) {
        cout << "Primero cargue los equipos." << endl;
        return;
    }

    for(int i = 0; i < cantidadEquipos; i++) {
        equipos[i].crearPlantillaArtificial();
    }

    cout << "Jugadores creados correctamente para todos los equipos." << endl;
}

void Torneo::mostrarEquipos() const {
    if(equipos == 0) {
        cout << "No hay equipos cargados." << endl;
        return;
    }

    for(int i = 0; i < cantidadEquipos; i++) {
        cout << "[" << i << "] ";
        equipos[i].imprimirResumen();
    }

}

void Torneo::ordenarEquiposPorRanking() {
    if(equipos == 0) return;
    for(int i = 0; i < cantidadEquipos - 1; i++) {
        for(int j = 0; j < cantidadEquipos - 1 - i; j++) {
            if(equipos[j + 1] < equipos[j]) {
                Equipo temp = equipos[j];
                equipos[j] = equipos[j + 1];
                equipos[j + 1] = temp;
            }
        }
    }
}

void Torneo::formarBombos() {
    if(equipos == 0) {
        cout << "Primero cargue los equipos." << endl;
        return;
    }

    ordenarEquiposPorRanking();
    for(int i = 0; i < 4; i++) {
        delete[] bombos[i];
        bombos[i] = new Equipo*[12];
        cantidadesBombos[i] = 0;
    }

    int indiceUSA = -1;
    for(int i = 0; i < cantidadEquipos; i++) {
        if(sonIguales(equipos[i].getPais(), "EE. UU.") ||
            sonIguales(equipos[i].getPais(), "Estados Unidos")) {
            indiceUSA = i;
            break;
        }
    }

    if(indiceUSA != -1) {
        bombos[0][cantidadesBombos[0]++] = &equipos[indiceUSA];
    }

    for(int i = 0; i < cantidadEquipos; i++) {
        if(i == indiceUSA) continue;
        if(cantidadesBombos[0] < 12) bombos[0][cantidadesBombos[0]++] =
                &equipos[i];
        else if(cantidadesBombos[1] < 12) bombos[1][cantidadesBombos[1]++] =
                &equipos[i];
        else if(cantidadesBombos[2] < 12) bombos[2][cantidadesBombos[2]++] =
                &equipos[i];
        else if(cantidadesBombos[3] < 12) bombos[3][cantidadesBombos[3]++] =
                &equipos[i];
    }

        cout << "Bombos formados correctamente." << endl;
    mostrarBombos();
}

void Torneo::mostrarBombos() const {
    for(int i = 0; i < 4; i++) {
        cout << "\nBombo " << (i + 1) << endl;
        for(int j = 0; j < cantidadesBombos[i]; j++) {
            cout << " - " << bombos[i][j]->getPais()
            << " (Ranking " << bombos[i][j]->getRankingFIFA() << ")" <<
                endl;
        }
    }
}

void Torneo::sortearGrupos() {
    bool listo = false;
    int intentos = 0;
    while(!listo && intentos < 500) {
        intentos++;
        listo = true;
        delete[] grupos;
        grupos = new Grupo[cantidadGrupos];
        for(int i = 0; i < cantidadGrupos; i++) grupos[i] = Grupo(char('A' +
                                   i));
        for(int b = 0; b < 4; b++) {
            for(int i = 0; i < cantidadesBombos[b]; i++) {
                int j = aleatorioEnRango(0, cantidadesBombos[b] - 1);
                Equipo* temp = bombos[b][i];
                bombos[b][i] = bombos[b][j];
                bombos[b][j] = temp;
            }
        }
        for(int b = 0; b < 4 && listo; b++) {
            for(int e = 0; e < 12 && listo; e++) {
                bool agregado = false;
                for(int g = 0; g < 12 && !agregado; g++) {
                    if(grupos[g].getCantidadEquipos() == b &&
                        grupos[g].puedeAgregarEquipo(bombos[b][e])) {
                        grupos[g].agregarEquipo(bombos[b][e]);
                        agregado = true;
                    }
                }
                if(!agregado) listo = false;
            }
        }
    }
    if(listo) cout << "Grupos sorteados correctamente." << endl;
    else cout << "No fue posible sortear grupos validos." << endl;
}

void Torneo::mostrarGrupos() const {
    if(grupos == 0) {
        cout << "No se han sorteado grupos." << endl;
        return;
    }
    for(int i = 0; i < cantidadGrupos; i++) {
        grupos[i].imprimir();
        cout << endl;
    }
}

int Torneo::getCantidadEquipos() const {
    return cantidadEquipos;
}

Equipo* Torneo::getEquipo(int i) const {
    if(i < 0 || i >= cantidadEquipos) return 0;
    return &equipos[i];
}
