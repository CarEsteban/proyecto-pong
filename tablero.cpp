#include <iostream>
#include <cstdlib>   // For rand() and srand()
#include <ctime>     // For time()
#include <pthread.h> // Libreria de pthread
#include <unistd.h>  // Libreria de sleep
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
int pelotaX = ancho / 2;  // Posición horizontal inicial de la pelota
int pelotaY = alto / 2;   // Posición vertical inicial de la pelota
int velocidadPelotaX = 1; // Velocidad horizontal de la pelota
int velocidadPelotaY = 1; // Velocidad vertical de la pelota

// Variables de puntuación
int scorePlayer1 = 0; // Puntuación del jugador 1 (raqueta izquierda)
int scorePlayer2 = 0; // Puntuación del jugador 2 (raqueta derecha)

pthread_mutex_t mtx;                      // Mutex
pthread_mutex_t mtxRaqueta1, mtxRaqueta2; // Mutex para cada una de las raquetas.

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
        tablero[i][0] = -1;         // Borde izquierdo
        tablero[i][ancho - 1] = -1; // Borde derecho
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
    pthread_mutex_lock(&mtx); // Bloquear el mutex
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

    // Detección de colisión con las raquetas
    if (pelotaX == raqueta1X + 1 && pelotaY >= raqueta1Y && pelotaY <= raqueta1Y + 2)
    {
        velocidadPelotaX *= -1; // Invertir la dirección horizontal al colisionar con la raqueta izquierda
    }
    if (pelotaX == raqueta2X - 1 && pelotaY >= raqueta2Y && pelotaY <= raqueta2Y + 2)
    {
        velocidadPelotaX *= -1; // Invertir la dirección horizontal al colisionar con la raqueta derecha
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

    pthread_mutex_unlock(&mtx); // Desbloquear el mutex
}

void *hiloPelota(void *arg)
{
    while (true)
    {
        actualizarPelota();
        imprimirTablero();
        dormir(100);       // Pausa de 100 milisegundos
        limpiarPantalla(); // Limpiar la pantalla para el siguiente frame

        // Finalizar el juego si alguien alcanza 10 puntos
        if (scorePlayer1 == 10 || scorePlayer2 == 10)
        {
            break;
        }
    }
    return nullptr;
}

void moverRaquetaIA(int raquetaID)
{
    if (raquetaID == 1)
    {
        pthread_mutex_lock(&mtxRaqueta1);
        for (int i = 0; i < 3; ++i)
        {
            if (raqueta1Y + 1 >= 0 && raqueta1Y + 1 < alto)
            {
                tablero[raqueta1Y + i][raqueta1X] = 0;
            }
        }
        if (raqueta1Y < pelotaY && raqueta1Y < alto - 4)
        {
            raqueta1Y++;
        }
        else if (raqueta1Y > pelotaY && raqueta1Y > 1)
        {
            raqueta1Y--;
        }
        for (int i = 0; i < 3; i++)
        {
            if (raqueta1Y + i >= 0 && raqueta1Y + i < alto)
            {
                tablero[raqueta1Y + i][raqueta1X] = 3;
            }
        }
        pthread_mutex_unlock(&mtxRaqueta1);
    }
    else if (raquetaID == 2)
    {
        pthread_mutex_lock(&mtxRaqueta2);
        for (int i = 0; i < 3; ++i)
        {
            if (raqueta2Y + 1 >= 0 && raqueta2Y + 1 < alto)
            {
                tablero[raqueta2Y + i][raqueta2X] = 0;
            }
        }
        if (raqueta2Y < pelotaY && raqueta2Y < alto - 4)
        {
            raqueta2Y++;
        }
        else if (raqueta2Y > pelotaY && raqueta2Y > 1)
        {
            raqueta2Y--;
        }
        for (int i = 0; i < 3; i++)
        {
            if (raqueta2Y + i >= 0 && raqueta2Y + i < alto)
            {
                tablero[raqueta2Y + i][raqueta2X] = 3;
            }
        }
        pthread_mutex_unlock(&mtxRaqueta2);
    }
}

void *hiloJugadorComputadora(void *arg)
{
    int raquetaID = *(int *)arg;
    while (true)
    {
        moverRaquetaIA(raquetaID);
        dormir(100);
        if (scorePlayer1 == 10 || scorePlayer2 == 10)
        {
            break;
        }
    }
    return nullptr;
}

#ifdef _WIN32
#include <conio.h> // Windows-specific header for getch()

char leerTecla()
{
    return getch(); // Función para capturar teclas en Windows
}

#else
#include <termios.h>
#include <unistd.h> // Para usar STDIN_FILENO

char leerTecla()
{
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

void moverRaquetaJugador(char tecla)
{
    pthread_mutex_lock(&mtx);

    // Limpiar la posicion anterior
    for (int i = 0; i < 3; i++)
    {
        if (raqueta1Y + i >= 0 && raqueta1Y + i < alto)
        {
            tablero[raqueta1Y + i][raqueta1X] = 0;
        }
    }

    if (tecla == 'w' && raqueta1Y > 1)
    {
        raqueta1Y--;
    }
    else if (tecla == 's' && raqueta1Y < alto - 4)
    {
        raqueta1Y++;
    }

    // dibujar la nueva posicion
    for (int i = 0; i < 3; i++)
    {
        if (raqueta1Y + i >= 0 && raqueta1Y + i < alto)
        {
            tablero[raqueta1Y + i][raqueta1X] = 2;
        }
    }
    pthread_mutex_unlock(&mtx);
}

void *hiloJugador(void *)
{
    while (true)
    {
        char tecla = leerTecla();
        moverRaquetaJugador(tecla);
        if (scorePlayer1 == 10 || scorePlayer2 == 10)
        {
            break;
        }
    }
    return nullptr;
}

void iniciarComputadoraVSComputadora()
{
    srand(static_cast<unsigned int>(time(0))); // Inicializar el generador de números aleatorios
    iniciarTablero();
    cout << "Iniciando Computadora vs Computadora..." << endl;
    pthread_mutex_init(&mtxRaqueta1, NULL); // Inicializar el mutex de la primera raqueta
    pthread_mutex_init(&mtxRaqueta2, NULL); // Inicializar el mutex de la segunda raqueta

    pthread_t th1, th2, thPelota;
    // Indentificador para poder utilizar correctamente los mtx en moverRaquetaIA
    int raqueta1ID = 1;
    int raqueta2ID = 2;

    pthread_create(&th1, NULL, hiloJugadorComputadora, &raqueta1ID);
    pthread_create(&th2, NULL, hiloJugadorComputadora, &raqueta2ID);
    pthread_create(&thPelota, NULL, hiloPelota, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(thPelota, NULL);

    pthread_mutex_destroy(&mtxRaqueta1); // Destruir el mutex de raqueta 1
    pthread_mutex_destroy(&mtxRaqueta2); // Destruir el mutex de raqueta 1

    cout << "Juego terminado." << endl;
}

void iniciarJugadorVSComputadora()
{
    srand(static_cast<unsigned int>(time(0))); // Inicializar el generador de números aleatorios
    iniciarTablero();
    cout << "Iniciando Jugador vs Computadora..." << endl;
    pthread_mutex_init(&mtx, NULL); // Inicializar el mutex

    pthread_t th1, thPelota;
    pthread_create(&th1, NULL, hiloJugador, NULL);     // Jugador 1 controlado por el usuario
    pthread_create(&thPelota, NULL, hiloPelota, NULL); // Hilo de la pelota

    pthread_t th2;
    pthread_create(&th2, NULL, hiloJugadorComputadora, NULL); // Jugador 2 controlado por la IA

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(thPelota, NULL);

    pthread_mutex_destroy(&mtx); // Destruir el mutex

    cout << "Juego terminado." << endl;
}
