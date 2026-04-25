#include "torneo.h"
#include "utilidades.h"
#include <iostream>
#include <fstream>
#include <filesystem> // 👈 arriba del archivo
#include <string>

using namespace std;


static bool parseLineaJugadorHistorico(const char* linea, char* pais, int& camiseta, char* nombre, char* apellido, int& pj, int& goles, int& minutos, int& asistencias, int& amarillas, int& rojas, int& faltas) {
    char campos[11][128];
    for (int i = 0; i < 11; i++) campos[i][0] = '\0';

    int campo = 0;
    int pos = 0;
    char buffer[128];
    int b = 0;

    while (true) {
        char c = linea[pos];
        if (c == ',' || c == '\0' || c == '\n' || c == '\r') {
            buffer[b] = '\0';
            if (campo >= 0 && campo < 11) copiarCadenaEnBuffer(campos[campo], buffer);
            campo++;
            b = 0;
            if (c == '\0' || c == '\n' || c == '\r') break;
        } else {
            if (b < 127) buffer[b++] = c;
        }
        pos++;
    }

    if (campo < 11) return false;
    copiarCadenaEnBuffer(pais, campos[0]);
    camiseta = stringAEntero(campos[1]);
    copiarCadenaEnBuffer(nombre, campos[2]);
    copiarCadenaEnBuffer(apellido, campos[3]);
    pj = stringAEntero(campos[4]);
    goles = stringAEntero(campos[5]);
    minutos = stringAEntero(campos[6]);
    asistencias = stringAEntero(campos[7]);
    amarillas = stringAEntero(campos[8]);
    rojas = stringAEntero(campos[9]);
    faltas = stringAEntero(campos[10]);
    return true;
}

static bool abrirCSVConFallback(ifstream& archivo, const char* nombreArchivo) {
    archivo.open(nombreArchivo);
    if (archivo.is_open()) return true;

    const char* prefijos[] = {"../", "../../", "../../../", "../../../../"};
    for (int i = 0; i < 4; i++) {
        string ruta = string(prefijos[i]) + nombreArchivo;
        archivo.open(ruta.c_str());
        if (archivo.is_open()) return true;
    }
    return false;
}

Torneo::Torneo() {
    equipos = 0;
    cantidadEquipos = 0;
    grupos = 0;
    cantidadGrupos = 12;
    for (int i = 0; i < 4; i++) {
        bombos[i] = 0;
        cantidadesBombos[i] = 0;
    }

    partidosGrupos = 0;
    cantidadPartidosGrupos = 0;
    clasificadosR16 = 0;
    cantidadClasificadosR16 = 0;
    partidosR16 = 0;
    cantidadPartidosR16 = 16;
    partidosR8 = 0;
    cantidadPartidosR8 = 8;
    partidosQF = 0;
    cantidadPartidosQF = 4;
    partidosSF = 0;
    cantidadPartidosSF = 2;
    partidosFinales = 0;
    cantidadPartidosFinales = 2;

    top4 = new Equipo*[4];
    for (int i = 0; i < 4; i++) top4[i] = 0;

    inicializarAleatorio();
}

Torneo::~Torneo() {
    delete[] equipos;
    delete[] grupos;
    for (int i = 0; i < 4; i++) delete[] bombos[i];
    delete[] partidosGrupos;
    delete[] clasificadosR16;
    delete[] partidosR16;
    delete[] partidosR8;
    delete[] partidosQF;
    delete[] partidosSF;
    delete[] partidosFinales;
    delete[] top4;
}

void Torneo::cargarEquiposDesdeCSV(const char* nombreArchivo) {
    medidor.reiniciar();

    delete[] equipos;
    equipos = 0;
    cantidadEquipos = 0;

    ifstream archivo;
    if (!abrirCSVConFallback(archivo, nombreArchivo)) {
        cout << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    const int capacidad = 48;
    equipos = new Equipo[capacidad];
    cantidadEquipos = 0;

    char linea[512];
    archivo.getline(linea, 512); // header

    while (archivo.getline(linea, 512) && cantidadEquipos < capacidad) {
        medidor.sumarIteracion();

        char campos[8][128];
        for (int i = 0; i < 8; i++) campos[i][0] = '\0';

        int campo = 0;
        int pos = 0;
        char buffer[128];
        int b = 0;

        while (true) {
            char c = linea[pos];
            if (c == ',' || c == '\0') {
                buffer[b] = '\0';
                if (campo >= 0 && campo < 8) copiarCadenaEnBuffer(campos[campo], buffer);
                campo++;
                b = 0;
                if (c == '\0') break;
            } else {
                if (b < 127) buffer[b++] = c;
            }
            pos++;
        }

        equipos[cantidadEquipos] = Equipo(
            campos[0],
            campos[1],
            "DT",
            stringAEntero(campos[2]),
            stringAEntero(campos[3]),
            stringAEntero(campos[4]),
            stringAEntero(campos[5]),
            stringAEntero(campos[6]),
            stringAEntero(campos[7]),
            0, 0, 0
            );
        cantidadEquipos++;
    }

    archivo.close();
    cout << "Equipos cargados correctamente." << endl;
    mostrarRecursosBasicos("Carga de equipos");
}

void Torneo::crearJugadoresDeTodosLosEquipos() {
    medidor.reiniciar();
    if (equipos == 0) {
        cout << "Primero cargue los equipos." << endl;
        return;
    }
    for (int i = 0; i < cantidadEquipos; i++) {
        medidor.sumarIteracion();
        equipos[i].crearPlantillaArtificial();
    }
    cout << "Jugadores creados correctamente para todos los equipos." << endl;
    mostrarRecursosBasicos("Creacion de jugadores");
}

void Torneo::mostrarEquipos() const {
    if (equipos == 0) {
        cout << "No hay equipos cargados." << endl;
        return;
    }
    for (int i = 0; i < cantidadEquipos; i++) {
        cout << "[" << i << "] ";
        equipos[i].imprimirResumen();
    }
}

void Torneo::ordenarEquiposPorRanking() {
    if (equipos == 0) return;
    for (int i = 0; i < cantidadEquipos - 1; i++) {
        for (int j = 0; j < cantidadEquipos - 1 - i; j++) {
            medidor.sumarIteracion();
            if (equipos[j + 1] < equipos[j]) {
                Equipo temp = equipos[j];
                equipos[j] = equipos[j + 1];
                equipos[j + 1] = temp;
            }
        }
    }
}

void Torneo::formarBombos() {
    medidor.reiniciar();
    if (equipos == 0) {
        cout << "Primero cargue los equipos." << endl;
        return;
    }

    ordenarEquiposPorRanking();

    for (int i = 0; i < 4; i++) {
        delete[] bombos[i];
        bombos[i] = new Equipo*[12];
        cantidadesBombos[i] = 0;
    }

    int indiceUSA = -1;
    for (int i = 0; i < cantidadEquipos; i++) {
        medidor.sumarIteracion();
        if (sonIguales(equipos[i].getPais(), "EE. UU.") || sonIguales(equipos[i].getPais(), "Estados Unidos")) {
            indiceUSA = i;
            break;
        }
    }

    if (indiceUSA != -1) {
        bombos[0][cantidadesBombos[0]++] = &equipos[indiceUSA];
    }

    for (int i = 0; i < cantidadEquipos; i++) {
        medidor.sumarIteracion();
        if (i == indiceUSA) continue;
        if (cantidadesBombos[0] < 12) bombos[0][cantidadesBombos[0]++] = &equipos[i];
        else if (cantidadesBombos[1] < 12) bombos[1][cantidadesBombos[1]++] = &equipos[i];
        else if (cantidadesBombos[2] < 12) bombos[2][cantidadesBombos[2]++] = &equipos[i];
        else if (cantidadesBombos[3] < 12) bombos[3][cantidadesBombos[3]++] = &equipos[i];
    }

    cout << "Bombos formados correctamente." << endl;
    mostrarBombos();
    mostrarRecursosBasicos("Formacion de bombos");
}

void Torneo::mostrarBombos() const {
    for (int i = 0; i < 4; i++) {
        cout << "\nBombo " << (i + 1) << endl;
        for (int j = 0; j < cantidadesBombos[i]; j++) {
            cout << " - " << bombos[i][j]->getPais()
            << " (Ranking " << bombos[i][j]->getRankingFIFA() << ")" << endl;
        }
    }
}

void Torneo::sortearGrupos() {
    medidor.reiniciar();
    if (equipos == 0) {
        cout << "Primero cargue los equipos." << endl;
        return;
    }
    if (bombos[0] == 0) {
        cout << "Primero forme los bombos." << endl;
        return;
    }

    bool listo = false;
    int intentos = 0;

    while (!listo && intentos < 500) {
        intentos++;
        listo = true;

        delete[] grupos;
        grupos = new Grupo[cantidadGrupos];
        for (int i = 0; i < cantidadGrupos; i++) grupos[i] = Grupo(char('A' + i));

        for (int b = 0; b < 4; b++) {
            for (int i = 0; i < cantidadesBombos[b]; i++) {
                medidor.sumarIteracion();
                int j = aleatorioEnRango(0, cantidadesBombos[b] - 1);
                Equipo* temp = bombos[b][i];
                bombos[b][i] = bombos[b][j];
                bombos[b][j] = temp;
            }
        }

        for (int b = 0; b < 4 && listo; b++) {
            for (int e = 0; e < 12 && listo; e++) {
                bool agregado = false;
                for (int g = 0; g < 12 && !agregado; g++) {
                    medidor.sumarIteracion();
                    if (grupos[g].getCantidadEquipos() == b && grupos[g].puedeAgregarEquipo(bombos[b][e])) {
                        grupos[g].agregarEquipo(bombos[b][e]);
                        agregado = true;
                    }
                }
                if (!agregado) listo = false;
            }
        }
    }

    if (listo) cout << "Grupos sorteados correctamente." << endl;
    else cout << "No fue posible sortear grupos validos." << endl;

    mostrarRecursosBasicos("Sorteo de grupos");
}

void Torneo::mostrarGrupos() const {
    if (grupos == 0) {
        cout << "No se han sorteado grupos." << endl;
        return;
    }
    for (int i = 0; i < cantidadGrupos; i++) {
        grupos[i].imprimir();
        cout << endl;
    }
}

bool Torneo::equipoYaJuegaEseDia(Equipo* equipo, const Fecha& fecha, int hastaIndice) const {
    for (int i = 0; i < hastaIndice; i++) {
        if (partidosGrupos[i].getFecha() == fecha) {
            if (partidosGrupos[i].getEquipo1() == equipo || partidosGrupos[i].getEquipo2() == equipo) return true;
        }
    }
    return false;
}

bool Torneo::equipoRompeReglaTresDias(Equipo* equipo, const Fecha& fecha, int hastaIndice) const {
    for (int i = 0; i < hastaIndice; i++) {
        if (partidosGrupos[i].getEquipo1() == equipo || partidosGrupos[i].getEquipo2() == equipo) {
            int diferencia = partidosGrupos[i].getFecha().diferenciaDias(fecha);
            if (diferencia < 0) diferencia *= -1;
            if (diferencia < 3) return true;
        }
    }
    return false;
}

void Torneo::generarPartidosDeGrupos() {
    medidor.reiniciar();
    if (grupos == 0) {
        cout << "Primero sortee los grupos." << endl;
        return;
    }

    delete[] partidosGrupos;
    cantidadPartidosGrupos = 72;
    partidosGrupos = new Partido[cantidadPartidosGrupos];

    Fecha inicio(20, 6, 2026);
    Fecha fechasDisponibles[19];
    for (int i = 0; i < 19; i++) {
        fechasDisponibles[i] = inicio;
        fechasDisponibles[i].sumarDias(i);
    }

    int partidosPorDia[19];
    for (int i = 0; i < 19; i++) partidosPorDia[i] = 0;

    int indice = 0;
    for (int g = 0; g < cantidadGrupos; g++) {
        Equipo* e0 = grupos[g].getEquipo(0);
        Equipo* e1 = grupos[g].getEquipo(1);
        Equipo* e2 = grupos[g].getEquipo(2);
        Equipo* e3 = grupos[g].getEquipo(3);

        Equipo* paresA[6] = {e0, e0, e0, e1, e1, e2};
        Equipo* paresB[6] = {e1, e2, e3, e2, e3, e3};

        for (int p = 0; p < 6; p++) {
            medidor.sumarIteracion();
            partidosGrupos[indice].setEtapa("Grupos");
            partidosGrupos[indice].setHora("00:00");
            partidosGrupos[indice].setSede("nombreSede");
            partidosGrupos[indice].configurarArbitrosFijos();
            partidosGrupos[indice].setEquipo1(paresA[p]);
            partidosGrupos[indice].setEquipo2(paresB[p]);

            bool asignado = false;
            for (int d = 0; d < 19 && !asignado; d++) {
                medidor.sumarIteracion();
                if (partidosPorDia[d] >= 4) continue;
                if (equipoYaJuegaEseDia(paresA[p], fechasDisponibles[d], indice)) continue;
                if (equipoYaJuegaEseDia(paresB[p], fechasDisponibles[d], indice)) continue;
                if (equipoRompeReglaTresDias(paresA[p], fechasDisponibles[d], indice)) continue;
                if (equipoRompeReglaTresDias(paresB[p], fechasDisponibles[d], indice)) continue;
                partidosGrupos[indice].setFecha(fechasDisponibles[d]);
                partidosPorDia[d]++;
                asignado = true;
            }

            if (!asignado) {
                for (int d = 0; d < 19 && !asignado; d++) {
                    if (partidosPorDia[d] < 4) {
                        partidosGrupos[indice].setFecha(fechasDisponibles[d]);
                        partidosPorDia[d]++;
                        asignado = true;
                    }
                }
            }
            indice++;
        }
    }

    cout << "Partidos de grupos generados correctamente." << endl;
    mostrarRecursosBasicos("Generacion de partidos de grupos");
}

void Torneo::reiniciarDatosDeGrupo() {
    for (int i = 0; i < cantidadEquipos; i++) equipos[i].reiniciarDatosGrupo();
}

void Torneo::simularFaseDeGrupos() {
    medidor.reiniciar();
    if (partidosGrupos == 0) {
        cout << "Primero genere los partidos de grupos." << endl;
        return;
    }

    reiniciarDatosDeGrupo();
    for (int i = 0; i < cantidadPartidosGrupos; i++) {
        medidor.sumarIteracion();
        partidosGrupos[i].simular(true);
        partidosGrupos[i].imprimir();
        partidosGrupos[i].imprimirGoleadores();
        cout << endl;
    }

    guardarHistoricosJugadores("historicos_jugadores.csv");

    cout << "Fase de grupos simulada correctamente." << endl;
    mostrarRecursosBasicos("Simulacion fase de grupos");
}

void Torneo::mostrarTablaDeGrupos() {
    if (grupos == 0) {
        cout << "No hay grupos." << endl;
        return;
    }

    for (int i = 0; i < cantidadGrupos; i++) {
        grupos[i].ordenarTabla();
        cout << "\nTabla Grupo " << grupos[i].getLetra() << endl;
        for (int j = 0; j < grupos[i].getCantidadEquipos(); j++) {
            Equipo* e = grupos[i].getEquipo(j);
            cout << (j + 1) << ". " << e->getPais()
                 << " | Pts: " << e->getPuntosGrupo()
                 << " | DG: " << e->getDiferenciaGolesGrupo()
                 << " | GF: " << e->getGolesFavorGrupo() << endl;
        }
    }
}

void Torneo::ordenarTerceros(Equipo** terceros, int cantidad) {
    for (int i = 0; i < cantidad - 1; i++) {
        for (int j = 0; j < cantidad - 1 - i; j++) {
            bool cambiar = false;
            if (terceros[j]->getPuntosGrupo() < terceros[j + 1]->getPuntosGrupo()) cambiar = true;
            else if (terceros[j]->getPuntosGrupo() == terceros[j + 1]->getPuntosGrupo()) {
                if (terceros[j]->getDiferenciaGolesGrupo() < terceros[j + 1]->getDiferenciaGolesGrupo()) cambiar = true;
                else if (terceros[j]->getDiferenciaGolesGrupo() == terceros[j + 1]->getDiferenciaGolesGrupo()) {
                    if (terceros[j]->getGolesFavorGrupo() < terceros[j + 1]->getGolesFavorGrupo()) cambiar = true;
                }
            }
            if (cambiar) {
                Equipo* t = terceros[j];
                terceros[j] = terceros[j + 1];
                terceros[j + 1] = t;
            }
        }
    }
}

void Torneo::ordenarEquiposPorGrupo(Equipo** lista, int cantidad) {
    for (int i = 0; i < cantidad - 1; i++) {
        for (int j = 0; j < cantidad - 1 - i; j++) {
            bool cambiar = false;
            if (lista[j]->getPuntosGrupo() < lista[j + 1]->getPuntosGrupo()) cambiar = true;
            else if (lista[j]->getPuntosGrupo() == lista[j + 1]->getPuntosGrupo()) {
                if (lista[j]->getDiferenciaGolesGrupo() < lista[j + 1]->getDiferenciaGolesGrupo()) cambiar = true;
                else if (lista[j]->getDiferenciaGolesGrupo() == lista[j + 1]->getDiferenciaGolesGrupo()) {
                    if (lista[j]->getGolesFavorGrupo() < lista[j + 1]->getGolesFavorGrupo()) cambiar = true;
                }
            }
            if (cambiar) {
                Equipo* t = lista[j];
                lista[j] = lista[j + 1];
                lista[j + 1] = t;
            }
        }
    }
}

void Torneo::clasificarADieciseisavos() {
    medidor.reiniciar();
    if (grupos == 0) {
        cout << "Primero simule la fase de grupos." << endl;
        return;
    }

    delete[] clasificadosR16;
    clasificadosR16 = new Equipo*[32];
    cantidadClasificadosR16 = 0;

    Equipo* primeros[12];
    Equipo* segundos[12];
    Equipo* terceros[12];

    for (int i = 0; i < cantidadGrupos; i++) {
        medidor.sumarIteracion();
        grupos[i].ordenarTabla();
        primeros[i] = grupos[i].getEquipo(0);
        segundos[i] = grupos[i].getEquipo(1);
        terceros[i] = grupos[i].getEquipo(2);
    }

    ordenarTerceros(terceros, 12);

    for (int i = 0; i < 12; i++) clasificadosR16[cantidadClasificadosR16++] = primeros[i];
    for (int i = 0; i < 12; i++) clasificadosR16[cantidadClasificadosR16++] = segundos[i];
    for (int i = 0; i < 8; i++) clasificadosR16[cantidadClasificadosR16++] = terceros[i];

    delete[] partidosR16;
    partidosR16 = new Partido[16];
    for (int i = 0; i < 16; i++) {
        partidosR16[i].setEtapa("R16");
        partidosR16[i].setFecha(Fecha(10, 7, 2026));
        partidosR16[i].setHora("00:00");
        partidosR16[i].setSede("nombreSede");
        partidosR16[i].configurarArbitrosFijos();
    }

    for (int i = 0; i < 12; i++) {
        partidosR16[i].setEquipo1(primeros[i]);
        partidosR16[i].setEquipo2(terceros[i % 8]);
    }

    Equipo** segundosOrdenados = new Equipo*[12];
    for (int i = 0; i < 12; i++) segundosOrdenados[i] = segundos[i];
    ordenarEquiposPorGrupo(segundosOrdenados, 12);

    for (int i = 0; i < 4; i++) {
        partidosR16[12 + i].setEquipo1(segundosOrdenados[i + 4]);
        partidosR16[12 + i].setEquipo2(segundosOrdenados[i + 8]);
    }

    cout << "Clasificacion a dieciseisavos generada." << endl;
    mostrarTablaDeGrupos();
    cout << "\nPartidos de dieciseisavos:" << endl;
    for (int i = 0; i < 16; i++) {
        cout << partidosR16[i].getEquipo1()->getPais() << " vs " << partidosR16[i].getEquipo2()->getPais() << endl;
    }

    delete[] segundosOrdenados;
    mostrarRecursosBasicos("Clasificacion a R16");
}

Equipo* Torneo::ganadorDePartido(const Partido& partido) const {
    if (partido.getGoles1() > partido.getGoles2()) return partido.getEquipo1();
    return partido.getEquipo2();
}

Equipo* Torneo::perdedorDePartido(const Partido& partido) const {
    if (partido.getGoles1() > partido.getGoles2()) return partido.getEquipo2();
    return partido.getEquipo1();
}

void Torneo::configurarPartidoEliminacion(Partido& partido, Equipo* a, Equipo* b, const char* etapa) {
    partido.setEtapa(etapa);
    partido.setFecha(Fecha(10, 7, 2026));
    partido.setHora("00:00");
    partido.setSede("nombreSede");
    partido.configurarArbitrosFijos();
    partido.setEquipo1(a);
    partido.setEquipo2(b);
}

void Torneo::simularFasesFinales() {
    medidor.reiniciar();
    if (partidosR16 == 0) {
        cout << "Primero clasifique a dieciseisavos." << endl;
        return;
    }

    for (int i = 0; i < 16; i++) {
        medidor.sumarIteracion();
        partidosR16[i].simular(false);
        partidosR16[i].imprimir();
        partidosR16[i].imprimirGoleadores();
        cout << endl;
    }

    delete[] partidosR8;
    partidosR8 = new Partido[8];
    for (int i = 0; i < 8; i++) {
        configurarPartidoEliminacion(partidosR8[i],
                                     ganadorDePartido(partidosR16[2 * i]),
                                     ganadorDePartido(partidosR16[2 * i + 1]),
                                     "R8");
        partidosR8[i].simular(false);
        partidosR8[i].imprimir();
        partidosR8[i].imprimirGoleadores();
        cout << endl;
    }

    delete[] partidosQF;
    partidosQF = new Partido[4];
    for (int i = 0; i < 4; i++) {
        configurarPartidoEliminacion(partidosQF[i],
                                     ganadorDePartido(partidosR8[2 * i]),
                                     ganadorDePartido(partidosR8[2 * i + 1]),
                                     "QF");
        partidosQF[i].simular(false);
        partidosQF[i].imprimir();
        partidosQF[i].imprimirGoleadores();
        cout << endl;
    }

    delete[] partidosSF;
    partidosSF = new Partido[2];
    for (int i = 0; i < 2; i++) {
        configurarPartidoEliminacion(partidosSF[i],
                                     ganadorDePartido(partidosQF[2 * i]),
                                     ganadorDePartido(partidosQF[2 * i + 1]),
                                     "SF");
        partidosSF[i].simular(false);
        partidosSF[i].imprimir();
        partidosSF[i].imprimirGoleadores();
        cout << endl;
    }

    delete[] partidosFinales;
    partidosFinales = new Partido[2];
    configurarPartidoEliminacion(partidosFinales[0],
                                 perdedorDePartido(partidosSF[0]),
                                 perdedorDePartido(partidosSF[1]),
                                 "Tercer Puesto");
    configurarPartidoEliminacion(partidosFinales[1],
                                 ganadorDePartido(partidosSF[0]),
                                 ganadorDePartido(partidosSF[1]),
                                 "Final");

    partidosFinales[0].simular(false);
    partidosFinales[1].simular(false);

    cout << "Etapas finales simuladas correctamente." << endl;
    partidosFinales[0].imprimir();
    partidosFinales[0].imprimirGoleadores();
    cout << endl;
    partidosFinales[1].imprimir();
    partidosFinales[1].imprimirGoleadores();

    top4[0] = ganadorDePartido(partidosFinales[1]);
    top4[1] = perdedorDePartido(partidosFinales[1]);
    top4[2] = ganadorDePartido(partidosFinales[0]);
    top4[3] = perdedorDePartido(partidosFinales[0]);

    guardarHistoricosJugadores("historicos_jugadores.csv");
    mostrarRecursosBasicos("Simulacion fases finales");
}

void Torneo::mostrarEstadisticasFinales() {
    if (top4[0] == 0) {
        cout << "Primero simule las fases finales." << endl;
        return;
    }

    cout << "\n===== ESTADISTICAS FINALES =====" << endl;
    cout << "Top 4 del mundial:" << endl;
    for (int i = 0; i < 4; i++) {
        cout << (i + 1) << ". " << top4[i]->getPais() << endl;
    }

    Jugador* maxCampeon = &top4[0]->getPlantilla()[0];
    for (int i = 1; i < top4[0]->getCantidadJugadores(); i++) {
        if (top4[0]->getPlantilla()[i] > *maxCampeon) {
            maxCampeon = &top4[0]->getPlantilla()[i];
        }
    }

    cout << "Maximo goleador del campeon: " << top4[0]->getPais()
         << " - camiseta " << maxCampeon->getNumeroCamiseta()
         << " con " << maxCampeon->getGoles() << " goles" << endl;

    Jugador* topGoleadores[3];
    for (int i = 0; i < 3; i++) topGoleadores[i] = 0;

    for (int e = 0; e < cantidadEquipos; e++) {
        for (int j = 0; j < equipos[e].getCantidadJugadores(); j++) {
            Jugador* actual = &equipos[e].getPlantilla()[j];
            for (int k = 0; k < 3; k++) {
                if (topGoleadores[k] == 0 || *actual > *topGoleadores[k]) {
                    for (int m = 2; m > k; m--) topGoleadores[m] = topGoleadores[m - 1];
                    topGoleadores[k] = actual;
                    break;
                }
            }
        }
    }

    cout << "Top 3 goleadores del torneo:" << endl;
    for (int i = 0; i < 3; i++) {
        if (topGoleadores[i] != 0) {
            const char* paisJugador = topGoleadores[i]->getEquipo() ? topGoleadores[i]->getEquipo()->getPais() : "Sin equipo";
            cout << (i + 1) << ". " << paisJugador
                 << " - Camiseta " << topGoleadores[i]->getNumeroCamiseta()
                 << " - Goles: " << topGoleadores[i]->getGoles() << endl;
        }
    }

    Equipo* equipoMasGoles = &equipos[0];
    for (int i = 1; i < cantidadEquipos; i++) {
        if (equipos[i].getGolesFavorHistoricos() > equipoMasGoles->getGolesFavorHistoricos()) {
            equipoMasGoles = &equipos[i];
        }
    }

    cout << "Equipo con mas goles historicos: " << equipoMasGoles->getPais()
         << " con " << equipoMasGoles->getGolesFavorHistoricos() << endl;

    int uefaR16 = 0, conmebolR16 = 0, concacafR16 = 0, cafR16 = 0, afcR16 = 0;
    for (int i = 0; i < 32; i++) {
        if (sonIguales(clasificadosR16[i]->getConfederacion(), "UEFA")) uefaR16++;
        else if (sonIguales(clasificadosR16[i]->getConfederacion(), "CONMEBOL")) conmebolR16++;
        else if (sonIguales(clasificadosR16[i]->getConfederacion(), "CONCACAF")) concacafR16++;
        else if (sonIguales(clasificadosR16[i]->getConfederacion(), "CAF")) cafR16++;
        else if (sonIguales(clasificadosR16[i]->getConfederacion(), "AFC")) afcR16++;
    }
    cout << "Presencia por confederacion en R16 -> UEFA:" << uefaR16
         << " CONMEBOL:" << conmebolR16
         << " CONCACAF:" << concacafR16
         << " CAF:" << cafR16
         << " AFC:" << afcR16 << endl;

    int uefaR8 = 0, conmebolR8 = 0, concacafR8 = 0, cafR8 = 0, afcR8 = 0;
    for (int i = 0; i < 8; i++) {
        Equipo* a = ganadorDePartido(partidosR8[i]);
        if (sonIguales(a->getConfederacion(), "UEFA")) uefaR8++;
        else if (sonIguales(a->getConfederacion(), "CONMEBOL")) conmebolR8++;
        else if (sonIguales(a->getConfederacion(), "CONCACAF")) concacafR8++;
        else if (sonIguales(a->getConfederacion(), "CAF")) cafR8++;
        else if (sonIguales(a->getConfederacion(), "AFC")) afcR8++;
    }
    cout << "Presencia por confederacion en R8 -> UEFA:" << uefaR8
         << " CONMEBOL:" << conmebolR8
         << " CONCACAF:" << concacafR8
         << " CAF:" << cafR8
         << " AFC:" << afcR8 << endl;

    int uefaR4 = 0, conmebolR4 = 0, concacafR4 = 0, cafR4 = 0, afcR4 = 0;
    for (int i = 0; i < 4; i++) {
        if (sonIguales(top4[i]->getConfederacion(), "UEFA")) uefaR4++;
        else if (sonIguales(top4[i]->getConfederacion(), "CONMEBOL")) conmebolR4++;
        else if (sonIguales(top4[i]->getConfederacion(), "CONCACAF")) concacafR4++;
        else if (sonIguales(top4[i]->getConfederacion(), "CAF")) cafR4++;
        else if (sonIguales(top4[i]->getConfederacion(), "AFC")) afcR4++;
    }
    cout << "Presencia por confederacion en R4 -> UEFA:" << uefaR4
         << " CONMEBOL:" << conmebolR4
         << " CONCACAF:" << concacafR4
         << " CAF:" << cafR4
         << " AFC:" << afcR4 << endl;
}




void Torneo::guardarHistoricosJugadores(const char* nombreArchivo) const {
    ofstream archivo(nombreArchivo);

    cout << "Guardando en: "
         << std::filesystem::current_path() << endl; // 🔥 ESTA LINEA

    if (!archivo.is_open()) {
        cout << "No se pudo guardar el archivo: " << nombreArchivo << endl;
        return;
    }

    archivo << "Pais,Camiseta,Nombre,Apellido,PJ,Goles,Minutos,Asistencias,Amarillas,Rojas,Faltas\n";

    for (int i = 0; i < cantidadEquipos; i++) {
        for (int j = 0; j < equipos[i].getCantidadJugadores(); j++) {
            Jugador* jug = &equipos[i].getPlantilla()[j];

            archivo << equipos[i].getPais() << ','
                    << jug->getNumeroCamiseta() << ','
                    << jug->getNombre() << ','
                    << jug->getApellido() << ','
                    << jug->getPartidosJugados() << ','
                    << jug->getGoles() << ','
                    << jug->getMinutosJugados() << ','
                    << jug->getAsistencias() << ','
                    << jug->getTarjetasAmarillas() << ','
                    << jug->getTarjetasRojas() << ','
                    << jug->getFaltas() << '\n';
        }
    }

    cout << "Historicos de jugadores guardados en " << nombreArchivo << endl;
}

void Torneo::cargarHistoricosJugadores(const char* nombreArchivo) {
    if (equipos == 0) return;
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        return;
    }

    char linea[512];
    archivo.getline(linea, 512); // encabezado
    while (archivo.getline(linea, 512)) {
        char pais[100] = "";
        char nombre[32] = "";
        char apellido[32] = "";
        int camiseta = 0;
        int pj = 0, goles = 0, minutos = 0, asistencias = 0, amarillas = 0, rojas = 0, faltas = 0;

        if (!parseLineaJugadorHistorico(linea, pais, camiseta, nombre, apellido, pj, goles, minutos, asistencias, amarillas, rojas, faltas)) {
            continue;
        }

        Equipo* equipo = 0;
        for (int i = 0; i < cantidadEquipos; i++) {
            if (sonIguales(equipos[i].getPais(), pais)) {
                equipo = &equipos[i];
                break;
            }
        }
        if (equipo == 0 || equipo->getCantidadJugadores() == 0) continue;

        Jugador* plantilla = equipo->getPlantilla();
        for (int j = 0; j < equipo->getCantidadJugadores(); j++) {
            if (plantilla[j].getNumeroCamiseta() == camiseta) {
                plantilla[j].setNombre(nombre);
                plantilla[j].setApellido(apellido);
                plantilla[j].setPartidosJugados(pj);
                plantilla[j].setGoles(goles);
                plantilla[j].setMinutosJugados(minutos);
                plantilla[j].setAsistencias(asistencias);
                plantilla[j].setTarjetasAmarillas(amarillas);
                plantilla[j].setTarjetasRojas(rojas);
                plantilla[j].setFaltas(faltas);
                break;
            }
        }
    }

    cout << "Historicos de jugadores cargados desde " << nombreArchivo << endl;
}

void Torneo::mostrarRecursosBasicos(const char* funcionalidad) {
    long long memoria = medidor.memoriaTorneoBasica(equipos, cantidadEquipos, grupos, grupos ? cantidadGrupos : 0);
    medidor.imprimirResumen(funcionalidad, memoria);
}

int Torneo::getCantidadEquipos() const {
    return cantidadEquipos;
}

Equipo* Torneo::getEquipo(int i) const {
    if (i < 0 || i >= cantidadEquipos) return 0;
    return &equipos[i];
}
