#include <iostream>
#include <chrono>
#include <thread>
#include "tablero.cpp"
#include <locale>
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
        limpiarPantalla(); // Limpiar la pantalla al iniciar el programa
        mostrarMenu();
        cin >> opcion;

        switch (opcion)
        {
        case 1:
            iniciarComputadoraVSComputadora();
            break;
        case 2:
            iniciarJugadorVSComputadora();
            break;
        case 3:
            cout << "Saliendo del juego..." << endl;
            return 0;
        default:
            cout << "Opción no válida, por favor intenta de nuevo." << endl;
            esperarEnter();
        }
    }

    return 0;
}