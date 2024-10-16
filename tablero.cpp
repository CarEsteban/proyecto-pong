#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <pthread.h> // Libreria de pthread
#include <unistd.h> // Libreria de sleep
#include <termios.h> // Libreria de termios

// Incluir las bibliotecas necesarias dependiendo del sistema operativo
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

using namespace std;

const int ancho = 80;
const int alto = 20;
int tablero[alto][ancho]; // Primero alto (filas), luego ancho (columnas)
const int barPos = alto / 2;

int raqueta1X = 2;            // Posición horizontal de la raqueta izquierda
int raqueta1Y = alto / 2 - 1; // Posición vertical inicial de la raqueta izquierda

int raqueta2X = ancho - 3;    // Posición horizontal de la raqueta derecha
int raqueta2Y = alto / 2 - 1; // Posición vertical inicial de la raqueta derecha

// Variables de la pelota
int pelotaX = ancho / 2;      // Posición horizontal inicial de la pelota
int pelotaY = alto / 2;       // Posición vertical inicial de la pelota
int velocidadPelotaX = 1;     // Velocidad horizontal de la pelota
int velocidadPelotaY = 1;     // Velocidad vertical de la pelota

// Variables de puntuación
int scorePlayer1 = 0;         // Puntuación del jugador 1 (raqueta izquierda)
int scorePlayer2 = 0;         // Puntuación del jugador 2 (raqueta derecha)

pthread_mutex_t mtx; //Mutex

void limpiarPantalla()
{
#ifdef _WIN32
    system("cls"); // Comando para limpiar en Windows
#else
    system("clear"); // Comando para limpiar en Linux/macOS
#endif
}

void dormir(int milisegundos)
{
#ifdef _WIN32
    Sleep(milisegundos);
#else
    usleep(milisegundos * 1000); // usleep recibe microsegundos
#endif
}

void imprimirTablero()
{
    cout << "Jugador 1: " << scorePlayer1 << " | Jugador 2: " << scorePlayer2 << endl;

    for (int i = 0; i < alto; i++)
    {
        for (int j = 0; j < ancho; j++)
        {
            if (i == 0 || i == alto - 1)
            {
                cout << "_"; // Bordes superior e inferior como línea continua
            }
            else if (j == 0 || j == ancho - 1)
            {
                cout << "|"; // Bordes izquierdo y derecho como líneas verticales
            }
            else if (i == pelotaY && j == pelotaX)
            {
                cout << "O"; // Representación de la pelota
            }
            else if (tablero[i][j] == 2)
            {
                cout << u8"\u2588"; // Carácter de la raqueta izquierda
            }
            else if (tablero[i][j] == 3)
            {
                cout << u8"\u2588"; // Carácter de la raqueta derecha
            }
            else
            {
                cout << " "; // Espacio vacío en el interior
            }
        }
        cout << endl; // Nueva línea al final de cada fila
    }
}

void iniciarTablero()
{
    // Limpiar el tablero
    for (int i = 0; i < alto; i++) // filas
    {
        for (int j = 0; j < ancho; j++) // columnas
        {
            tablero[i][j] = 0; // Limpiar el tablero
        }
    }

    // Generar el borde superior e inferior
    for (int j = 0; j < ancho; j++)
    {
        tablero[0][j] = 1;        // Borde superior
        tablero[alto - 1][j] = 1; // Borde inferior
    }

    // Generar los bordes izquierdo y derecho
    for (int i = 1; i < alto - 1; i++)
    {
        tablero[i][0] = -1;           // Borde izquierdo
        tablero[i][ancho - 1] = -1;   // Borde derecho
    }

    // Inicializar las raquetas
    for (int i = 0; i < 3; ++i)
    {
        int y1 = raqueta1Y + i;
        int y2 = raqueta2Y + i;
        if (y1 >= 0 && y1 < alto)
        {
            tablero[y1][raqueta1X] = 2; // Dibujar raqueta izquierda
        }
        if (y2 >= 0 && y2 < alto)
        {
            tablero[y2][raqueta2X] = 3; // Dibujar raqueta derecha
        }
    }
}

void resetPelota()
{
    pelotaX = ancho / 2;
    pelotaY = alto / 2;
    // Asignar una dirección aleatoria inicial a la pelota
    velocidadPelotaX = (rand() % 2 == 0) ? 1 : -1;
    velocidadPelotaY = (rand() % 2 == 0) ? 1 : -1;
}

void actualizarPelota()
{
    pthread_mutex_lock(&mtx); //Bloquear el mutex
    // Actualizar la posición de la pelota
    pelotaX += velocidadPelotaX;
    pelotaY += velocidadPelotaY;

    // Detección de colisión con la pared superior
    if (pelotaY <= 1)
    {
        pelotaY = 1;
        velocidadPelotaY *= -1; // Invertir la dirección vertical
    }

    // Detección de colisión con la pared inferior
    if (pelotaY >= alto - 2)
    {
        pelotaY = alto - 2;
        velocidadPelotaY *= -1; // Invertir la dirección vertical
    }

    // Detección de colisión con los bordes laterales (marcar punto)
    if (pelotaX <= 1)
    {
        scorePlayer2++; // Jugador 2 marca un punto
        resetPelota();
    }
    else if (pelotaX >= ancho - 2)
    {
        scorePlayer1++; // Jugador 1 marca un punto
        resetPelota();
    }
    pthread_mutex_unlock(&mtx); //Desbloquear el mutex
}
void*hiloPelota(void*arg){
    while (true)
    {
        actualizarPelota();
        imprimirTablero();
        dormir(100); // Pausa de 100 milisegundos
        limpiarPantalla(); // Limpiar la pantalla para el siguiente frame
    }
}

void moverRaquetaIA() {
    pthread_mutex_lock(&mtx);
    if (raqueta2Y < pelotaY) {
        raqueta2Y++;
    } else if (raqueta2Y > pelotaY) {
        raqueta2Y--;
    }
    pthread_mutex_unlock(&mtx);
}

void* hiloJugador2(void*) {
    while (true) {
        moverRaquetaIA();
        dormir(100);
    }
}
/*NECESARIO AGREGAR LA FUNCION PARA LEER LAS TECLAS USANDO LIBRERIA TERMIOS Y LA FUNCION DEL MOVIMIENTO
DEL JUGADOR CON SU MUTEX LOCK Y UNLOCK */
void* hiloJugador1(void*) {
    while (true) {
        char tecla = leerTecla();
        moverRaqueta1(tecla);
    }
}
/*Podria ser que haya que cambiar esa parte no estoy seguro Ian*/
void iniciarComputadoraVSComputadora()
{
    srand(static_cast<unsigned int>(time(0))); // Inicializar el generador de números aleatorios
    iniciarTablero();
    cout << "Iniciando Computadora vs Computadora..." << endl;
    pthread_mutex_init(&mtx, NULL); // Inicializar el mutex

    pthread_t th1, th2, thPelota;
    pthread_create(&th1, NULL, hiloJugador1, NULL);
    pthread_create(&th2, NULL, hiloJugador2, NULL);
    pthread_create(&thPelota, NULL, hiloPelota, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(thPelota, NULL);

    pthread_mutex_destroy(&mtx); // Destruir el mutex


    while (true)
    {
        actualizarPelota();
        imprimirTablero();
        dormir(100); // Pausa de 100 milisegundos
        limpiarPantalla(); // Limpiar la pantalla para el siguiente frame
    }
}

int main()
{
    iniciarComputadoraVSComputadora();
    return 0;
}
