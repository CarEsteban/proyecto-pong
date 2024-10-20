#include <iostream>
#include "tablero.cpp" // Cambia "tablero.cpp" por "tablero.h"

using namespace std;

void mostrarMenu()
{
    cout << "****************************" << endl;
    cout << "*        PONG GAME         *" << endl;
    cout << "****************************" << endl;
    cout << "1. Computadora vs Computadora" << endl;
    cout << "2. Computadora vs Jugador" << endl;
    cout << "3. Salir" << endl;
    cout << "****************************" << endl;
    cout << "Selecciona una opción: ";
}

int main()
{
    int opcion;
    while (true)
    {
        limpiarPantalla(); // Ahora limpiarPantalla() está en el archivo tablero.cpp
        mostrarMenu();
        cin >> opcion;

        switch (opcion)
        {
        case 1:
            iniciarComputadoraVSComputadora(); // Llamada correcta si está definida en tablero.cpp
            break;
        case 2:
            iniciarJugadorVSComputadora(); // Define la función si la necesitas
            break;
        case 3:
            cout << "Saliendo del juego..." << endl;
            return 0;
        default:
            cout << "Opción no válida, por favor intenta de nuevo." << endl;
        }
    }

    return 0;
}
