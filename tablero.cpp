#include <iostream>
#include <cstdlib>   // For rand() and srand()
#include <ctime>     // For time()
#include <pthread.h> // Libreria de pthread
#include <unistd.h>  // Libreria de sleep
#include <termios.h> // Libreria de termios
#include <unistd.h>

using namespace std;

// Constantes y variables globales
const int ancho = 80;
const int alto = 20;
int tablero[alto][ancho]; // Primero alto (filas), luego ancho (columnas)

int raqueta1X = 2;            // Posición horizontal de la raqueta izquierda
int raqueta1Y = alto / 2 - 1; // Posición vertical inicial de la raqueta izquierda

int raqueta2X = ancho - 3;    // Posición horizontal de la raqueta derecha
int raqueta2Y = alto / 2 - 1; // Posición vertical inicial de la raqueta derecha

int pelotaX = ancho / 2;      // Posición horizontal inicial de la pelota
int pelotaY = alto / 2;       // Posición vertical inicial de la pelota
int velocidadPelotaX = 1;     // Velocidad horizontal de la pelota
int velocidadPelotaY = 1;     // Velocidad vertical de la pelota

int scorePlayer1 = 0;         // Puntuación del jugador 1 (raqueta izquierda)
int scorePlayer2 = 0;         // Puntuación del jugador 2 (raqueta derecha)

bool salir = false;           // Variable para controlar si se debe salir del juego

// Mutexes
pthread_mutex_t mtx;                      // Mutex para la pelota
pthread_mutex_t mtxRaqueta1, mtxRaqueta2; // Mutex para cada una de las raquetas.
pthread_mutex_t mtxScore;                 // Mutex para el marcador

// Utilidades básicas
// ---------------------------
void limpiarPantalla() {
    system("clear"); // Comando para limpiar en Linux/macOS
}

void dormir(int milisegundos) {
    usleep(milisegundos * 1000); // usleep recibe microsegundos
}

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


    // Si la tecla 'x' es presionada, se debe salir al menú
    if (ch == 'x')
    {
        salir = true; // Cambiar el estado de la variable global
    }

    return ch;
}

void *detectarTeclaX(void *) {
    while (!salir) {
        char tecla = leerTecla();
        if (tecla == 'x') {
            salir = true;
        }
    }
    return nullptr;
}

// Funciones de manejo del tablero y pelota
// ---------------------------
void iniciarTablero() {
    // Limpiar el tablero
    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            tablero[i][j] = 0;
        }
    }

    // Generar los bordes superior, inferior, izquierdo y derecho
    for (int j = 0; j < ancho; j++) {
        tablero[0][j] = 1;        // Borde superior
        tablero[alto - 1][j] = 1; // Borde inferior
    }

    for (int i = 1; i < alto - 1; i++) {
        tablero[i][0] = -1;         // Borde izquierdo
        tablero[i][ancho - 1] = -1; // Borde derecho
    }

    // Inicializar las raquetas
    for (int i = 0; i < 3; ++i) {
        int y1 = raqueta1Y + i;
        int y2 = raqueta2Y + i;
        if (y1 >= 0 && y1 < alto) {
            tablero[y1][raqueta1X] = 2; // Raqueta izquierda
        }
        if (y2 >= 0 && y2 < alto) {
            tablero[y2][raqueta2X] = 3; // Raqueta derecha
        }
    }
}

void resetPelota() {
    pelotaX = ancho / 2;
    pelotaY = alto / 2;
    velocidadPelotaX = (rand() % 2 == 0) ? 1 : -1;
    velocidadPelotaY = (rand() % 2 == 0) ? 1 : -1;
}

void actualizarPelota() {
    pthread_mutex_lock(&mtx); // Bloquear el mutex para la pelota
    pelotaX += velocidadPelotaX;
    pelotaY += velocidadPelotaY;

    // Detección de colisión con la pared superior e inferior
    if (pelotaY <= 1) {
        pelotaY = 1;
        velocidadPelotaY *= -1;
    }
    if (pelotaY >= alto - 2) {
        pelotaY = alto - 2;
        velocidadPelotaY *= -1;
    }

    // Detección de colisión con las raquetas
    if (pelotaX == raqueta1X + 1 && pelotaY >= raqueta1Y && pelotaY <= raqueta1Y + 2) {
        velocidadPelotaX *= -1;
    }
    if (pelotaX == raqueta2X - 1 && pelotaY >= raqueta2Y && pelotaY <= raqueta2Y + 2) {
        velocidadPelotaX *= -1;
    }

    // Detección de colisión con los bordes laterales (marcar punto)
    if (pelotaX <= 1) {
        pthread_mutex_lock(&mtxScore);
        scorePlayer2++;
        pthread_mutex_unlock(&mtxScore);
        resetPelota();
    } else if (pelotaX >= ancho - 2) {
        pthread_mutex_lock(&mtxScore);
        scorePlayer1++;
        pthread_mutex_unlock(&mtxScore);
        resetPelota();
    }

    pthread_mutex_unlock(&mtx); // Desbloquear el mutex de la pelota
}

void imprimirTablero() {
    cout << "Jugador 1: " << scorePlayer1 << " | Jugador 2: " << scorePlayer2 << endl;

    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            if (i == 0 || i == alto - 1) {
                cout << "_"; // Bordes superior e inferior
            } else if (j == 0 || j == ancho - 1) {
                cout << "|"; // Bordes izquierdo y derecho
            } else if (i == pelotaY && j == pelotaX) {
                cout << "O"; // Pelota
            } else if (tablero[i][j] == 2) {
                cout << u8"\u2588"; // Raqueta izquierda
            } else if (tablero[i][j] == 3) {
                cout << u8"\u2588"; // Raqueta derecha
            } else {
                cout << " "; // Espacio vacío
            }
        }
        cout << endl;
    }
}

// Funciones para hilos de ejecución
// ---------------------------
void *hiloPelota(void *arg) {
    while (!salir) {
        actualizarPelota();
        imprimirTablero();
        dormir(100);       // Pausa de 100 milisegundos
        limpiarPantalla(); // Limpiar la pantalla para el siguiente frame
        if (scorePlayer1 == 10 || scorePlayer2 == 10 || salir) {
            break;
        }
    }
    return nullptr;
}


// Funciones para mover las raquetas
// ---------------------------
void moverRaquetaJugador(char tecla) {
    pthread_mutex_lock(&mtx);
    for (int i = 0; i < 3; i++) {
        if (raqueta1Y + i >= 0 && raqueta1Y + i < alto) {
            tablero[raqueta1Y + i][raqueta1X] = 0;
        }
    }

    if (tecla == 'w' && raqueta1Y > 1) {
        raqueta1Y--;
    } else if (tecla == 's' && raqueta1Y < alto - 4) {
        raqueta1Y++;
    }

    for (int i = 0; i < 3; i++) {
        if (raqueta1Y + i >= 0 && raqueta1Y + i < alto) {
            tablero[raqueta1Y + i][raqueta1X] = 2;
        }
    }
    pthread_mutex_unlock(&mtx);
}

void *hiloJugador(void *) {
    while (!salir) {
        char tecla = leerTecla();
        moverRaquetaJugador(tecla);
        if (scorePlayer1 == 10 || scorePlayer2 == 10 || salir) {
            break;
        }
    }
    return nullptr;
}

void moverRaquetaIA(int raquetaID) {
    if (raquetaID == 1) {
        pthread_mutex_lock(&mtxRaqueta1);
        for (int i = 0; i < 3; ++i) {
            if (raqueta1Y + i >= 0 && raqueta1Y + i < alto) {
                tablero[raqueta1Y + i][raqueta1X] = 0;
            }
        }

        int randomFactor = rand() % 10;
        if (randomFactor < 8) {
            if (raqueta1Y < pelotaY && raqueta1Y < alto - 4) {
                raqueta1Y++;
            } else if (raqueta1Y > pelotaY && raqueta1Y > 1) {
                raqueta1Y--;
            }
        } else {
            if (randomFactor % 2 == 0 && raqueta1Y < alto - 4) {
                raqueta1Y++;
            } else if (raqueta1Y > 1) {
                raqueta1Y--;
            }
        }

        for (int i = 0; i < 3; i++) {
            if (raqueta1Y + i >= 0 && raqueta1Y + i < alto) {
                tablero[raqueta1Y + i][raqueta1X] = 2;
            }
        }
        pthread_mutex_unlock(&mtxRaqueta1);
    } else if (raquetaID == 2) {
        pthread_mutex_lock(&mtxRaqueta2);
        for (int i = 0; i < 3; ++i) {
            if (raqueta2Y + i >= 0 && raqueta2Y + i < alto) {
                tablero[raqueta2Y + i][raqueta2X] = 0;
            }
        }

        int randomFactor = rand() % 10;
        if (randomFactor < 8) {
            if (raqueta2Y < pelotaY && raqueta2Y < alto - 4) {
                raqueta2Y++;
            } else if (raqueta2Y > pelotaY && raqueta2Y > 1) {
                raqueta2Y--;
            }
        } else {
            if (randomFactor % 2 == 0 && raqueta2Y < alto - 4) {
                raqueta2Y++;
            } else if (raqueta2Y > 1) {
                raqueta2Y--;
            }
        }

        for (int i = 0; i < 3; i++) {
            if (raqueta2Y + i >= 0 && raqueta2Y + i < alto) {
                tablero[raqueta2Y + i][raqueta2X] = 3;
            }
        }
        pthread_mutex_unlock(&mtxRaqueta2);
    }
}

void *hiloJugadorComputadora(void *arg) {
    int raquetaID = *(int *)arg;
    while (!salir) {
        moverRaquetaIA(raquetaID);
        dormir(100);
        if (scorePlayer1 == 10 || scorePlayer2 == 10 || salir) {
            break;
        }
    }
    return nullptr;
}

// Funciones para iniciar los modos de juego
// ---------------------------
void iniciarComputadoraVSComputadora() {
    srand(static_cast<unsigned int>(time(0)));
    iniciarTablero();
    salir = false;

    pthread_mutex_init(&mtxRaqueta1, NULL);
    pthread_mutex_init(&mtxRaqueta2, NULL);

    pthread_t th1, th2, thPelota, thTeclaX;
    int raqueta1ID = 1;
    int raqueta2ID = 2;

    pthread_create(&th1, NULL, hiloJugadorComputadora, &raqueta1ID);
    pthread_create(&th2, NULL, hiloJugadorComputadora, &raqueta2ID);
    pthread_create(&thPelota, NULL, hiloPelota, NULL);
    pthread_create(&thTeclaX, NULL, detectarTeclaX, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(thPelota, NULL);
    pthread_join(thTeclaX, NULL);

    pthread_mutex_destroy(&mtxRaqueta1);
    pthread_mutex_destroy(&mtxRaqueta2);

    cout << "Juego terminado." << endl;
}

void iniciarJugadorVSComputadora() {
    srand(static_cast<unsigned int>(time(0)));
    iniciarTablero();
    salir = false;

    pthread_mutex_init(&mtxRaqueta2, NULL);

    pthread_t th1, thPelota;
    pthread_create(&th1, NULL, hiloJugador, NULL);
    pthread_create(&thPelota, NULL, hiloPelota, NULL);

    pthread_t th2;
    int raqueta2ID = 2;
    pthread_create(&th2, NULL, hiloJugadorComputadora, &raqueta2ID);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(thPelota, NULL);

    pthread_mutex_destroy(&mtxRaqueta2);

    cout << "Juego terminado." << endl;
}