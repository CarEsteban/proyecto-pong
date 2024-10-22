#include <iostream>
#include "tablero.cpp" // Cambia "tablero.cpp" por "tablero.h" si usas un archivo de cabecera

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
    // Inicializa los mutexes que se necesitan
    pthread_mutex_init(&mtxScore, NULL); // Inicializar el mutex del marcador
    pthread_mutex_init(&mtxRaqueta1, NULL); // Inicializar el mutex de la primera raqueta
    pthread_mutex_init(&mtxRaqueta2, NULL); // Inicializar el mutex de la segunda raqueta
    pthread_mutex_init(&mtx, NULL); // Inicializar el mutex de la pelota

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
            pthread_mutex_destroy(&mtxScore); // Destruir el mutex del marcador
            pthread_mutex_destroy(&mtxRaqueta1); // Destruir el mutex de la primera raqueta
            pthread_mutex_destroy(&mtxRaqueta2); // Destruir el mutex de la segunda raqueta
            pthread_mutex_destroy(&mtx); // Destruir el mutex de la pelota
            return 0;
        default:
            cout << "Opción no válida, por favor intenta de nuevo." << endl;
        }
    }

    return 0;
}
