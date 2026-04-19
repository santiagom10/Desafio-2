#include <iostream>
#include "DesafioN2Libreria.h"

using namespace std;
int main() {
    DesafioN2Libreria torneo;
    int opcion = 0;
    do {
        cout << "\n UdeAWorldCup " << endl;
        cout << "1. Cargar equipos " << endl;
        cout << "2. Mostrar equipos cargados" << endl;
        cout << "3. Sortear grupos" << endl;
        cout << "4. Mostrar grupos" << endl;
        cout << "0. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        switch(opcion) {
        case 1:
            break;
        case 2:
            torneo.mostrarEquipos();
            break;
        case 3:
            torneo.sortearGrupos();
            break;
        case 4:
            torneo.mostrarGrupos();
            break;
        case 0:
            cout << "Saliendo..." << endl;
            break;
        default:
            cout << "Opcion invalida." << endl;
        }
    } while(opcion != 0);
    return 0;
}
