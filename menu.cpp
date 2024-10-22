#include <iostream>
#include "tablero.cpp" // Cambia "tablero.cpp" por "tablero.h" si usas un archivo de cabecera

// Definiendo colores para tablero
#define RESETCOLOR "\033[0m"
#define REDSHINY "\033[91m"
#define BLUESHINY "\033[94m"
#define YELLOWSHINY "\033[93m"
#define WHITESHINY "\033[97m"
#define PURPLESHINY "\033[95m"
#define BOLD "\033[1m"
#define RESETBOLD "\033[22m"

using namespace std;

void mostrarMenu()
{
    cout << REDSHINY << "****************************" << RESETCOLOR << endl;
    cout << REDSHINY << "*" << BLUESHINY BOLD << "       PONG GAME          " << RESETBOLD REDSHINY << "*" << RESETCOLOR << endl;
    cout << REDSHINY << "****************************" << RESETCOLOR << endl;
    cout << REDSHINY BOLD << "1." << BLUESHINY << " Computadora vs Computadora" << RESETCOLOR << endl;
    cout << REDSHINY BOLD << "2." << BLUESHINY << " Computadora vs Jugador" << RESETCOLOR << endl;
    cout << REDSHINY BOLD << "3." << BLUESHINY << " Salir" << RESETCOLOR << endl;
    cout << REDSHINY << "****************************" << RESETCOLOR << endl;
    cout << WHITESHINY << "Selecciona una opción: ";
}

int main()
{
    // Inicializa los mutexes que se necesitan
    pthread_mutex_init(&mtxScore, NULL);    // Inicializar el mutex del marcador
    pthread_mutex_init(&mtxRaqueta1, NULL); // Inicializar el mutex de la primera raqueta
    pthread_mutex_init(&mtxRaqueta2, NULL); // Inicializar el mutex de la segunda raqueta
    pthread_mutex_init(&mtx, NULL);         // Inicializar el mutex de la pelota

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
            // Destruye los mutexes antes de salir
            pthread_mutex_destroy(&mtxScore);    // Destruir el mutex del marcador
            pthread_mutex_destroy(&mtxRaqueta1); // Destruir el mutex de la primera raqueta
            pthread_mutex_destroy(&mtxRaqueta2); // Destruir el mutex de la segunda raqueta
            pthread_mutex_destroy(&mtx);         // Destruir el mutex de la pelota
            return 0;
        default:
            cout << "Opción no válida, por favor intenta de nuevo." << endl;
        }
    }

    return 0;
}
