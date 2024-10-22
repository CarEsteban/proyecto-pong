#include <iostream>
#include "tablero.cpp" // Asegúrate de que la ruta al archivo sea correcta

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
    cout << REDSHINY BOLD << "3." << BLUESHINY << " Configurar velocidades" << RESETCOLOR << endl;
    cout << REDSHINY BOLD << "4." << BLUESHINY << " Salir" << RESETCOLOR << endl;
    cout << REDSHINY << "****************************" << RESETCOLOR << endl;
    cout << WHITESHINY << "Selecciona una opción: ";
}

void configurarVelocidad()
{
    int opcion;
    bool volverAlMenu = false;

    while (!volverAlMenu)
    {
        limpiarPantalla();
        cout << REDSHINY << "****************************" << RESETCOLOR << endl;
        cout << REDSHINY << "*" << BLUESHINY BOLD << "  Configurar Velocidades  " << RESETBOLD REDSHINY << "*" << RESETCOLOR << endl;
        cout << REDSHINY << "****************************" << RESETCOLOR << endl;
        cout << REDSHINY BOLD << "1." << BLUESHINY << " Velocidad lenta (200 ms)" << RESETCOLOR << endl;
        cout << REDSHINY BOLD << "2." << BLUESHINY << " Velocidad media (100 ms)" << RESETCOLOR << endl;
        cout << REDSHINY BOLD << "3." << BLUESHINY << " Velocidad rápida (50 ms)" << RESETCOLOR << endl;
        cout << REDSHINY BOLD << "4." << BLUESHINY << " Ingresar velocidad personalizada" << RESETCOLOR << endl;
        cout << REDSHINY BOLD << "5." << BLUESHINY << " Volver al menú principal" << RESETCOLOR << endl;
        cout << REDSHINY << "****************************" << RESETCOLOR << endl;
        cout << WHITESHINY << "Selecciona una opción: ";
        cin >> opcion;

        switch (opcion)
        {
        case 1:
            velocidad = 200;
            cout << "Velocidad establecida a lenta (200 ms)." << endl;
            dormir(1000);
            volverAlMenu = true;
            break;
        case 2:
            velocidad = 100;
            cout << "Velocidad establecida a media (100 ms)." << endl;
            dormir(1000);
            volverAlMenu = true;
            break;
        case 3:
            velocidad = 50;
            cout << "Velocidad establecida a rápida (50 ms)." << endl;
            dormir(1000);
            volverAlMenu = true;
            break;
        case 4:
            int nuevaVelocidad;
            cout << "Ingresa la velocidad en milisegundos (entre 10 y 500): ";
            cin >> nuevaVelocidad;
            if (nuevaVelocidad >= 10 && nuevaVelocidad <= 500)
            {
                velocidad = nuevaVelocidad;
                cout << "Velocidad establecida a " << velocidad << " ms." << endl;
            }
            else
            {
                cout << "Valor inválido. La velocidad debe estar entre 10 y 500 ms." << endl;
            }
            dormir(1000);
            volverAlMenu = true;
            break;
        case 5:
            volverAlMenu = true;
            break;
        default:
            cout << "Opción no válida, por favor intenta de nuevo." << endl;
            dormir(1000);
            break;
        }
    }
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
            iniciarJugadorVSComputadora(); // Llamada a la función correspondiente
            break;
        case 3:
            configurarVelocidad(); // **Nueva opción para configurar velocidades**
            break;
        case 4:
            cout << "Saliendo del juego..." << endl;
            // Destruye los mutexes antes de salir
            pthread_mutex_destroy(&mtxScore);    // Destruir el mutex del marcador
            pthread_mutex_destroy(&mtxRaqueta1); // Destruir el mutex de la primera raqueta
            pthread_mutex_destroy(&mtxRaqueta2); // Destruir el mutex de la segunda raqueta
            pthread_mutex_destroy(&mtx);         // Destruir el mutex de la pelota
            return 0;
        default:
            cout << "Opción no válida, por favor intenta de nuevo." << endl;
            dormir(1000);
            break;
        }
    }

    return 0;
}