#include <iostream>
#include "torneo.h"

using namespace std;

int main() {
    Torneo torneo;
    int opcion = 0;

    do {
        cout << "\n========== UdeAWorldCup ==========" << endl;
        cout << "1. Cargar equipos desde CSV" << endl;
        cout << "2. Crear jugadores artificiales" << endl;
        cout << "3. Mostrar equipos cargados" << endl;
        cout << "4. Formar bombos" << endl;
        cout << "5. Sortear grupos" << endl;
        cout << "6. Mostrar grupos" << endl;
        cout << "7. Generar partidos de grupos" << endl;
        cout << "8. Simular fase de grupos" << endl;
        cout << "9. Mostrar tablas de grupos" << endl;
        cout << "10. Clasificar a dieciseisavos" << endl;
        cout << "11. Simular fases finales" << endl;
        cout << "12. Mostrar estadisticas finales" << endl;
        cout << "13. Guardar historicos de jugadores" << endl;
        cout << "0. Salir" << endl;
        cout << "Seleccione una opcion: ";

        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Entrada invalida." << endl;
            continue;
        }

        switch (opcion) {
        case 1:
            torneo.cargarEquiposDesdeCSV("selecciones_clasificadas_mundial.csv");
            break;
        case 2:
            torneo.crearJugadoresDeTodosLosEquipos();
            torneo.cargarHistoricosJugadores("historicos_jugadores.csv"); // 🔥 AQUÍ
            break;
        case 3:
            torneo.mostrarEquipos();
            break;
        case 4:
            torneo.formarBombos();
            break;
        case 5:
            torneo.sortearGrupos();
            break;
        case 6:
            torneo.mostrarGrupos();
            break;
        case 7:
            torneo.generarPartidosDeGrupos();
            break;
        case 8:
            torneo.simularFaseDeGrupos();
            break;
        case 9:
            torneo.mostrarTablaDeGrupos();
            break;
        case 10:
            torneo.clasificarADieciseisavos();
            break;
        case 11:
            torneo.simularFasesFinales();
            break;
        case 12:
            torneo.mostrarEstadisticasFinales();
            break;
        case 13:
            torneo.guardarHistoricosJugadores("historicos_jugadores.csv");
            break;
        case 0:
            cout << "Saliendo..." << endl;
            break;
        default:
            cout << "Opcion invalida." << endl;
        }
    } while (opcion != 0);

    return 0;
}
