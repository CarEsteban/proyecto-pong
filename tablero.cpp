#include <iostream>

const int ancho = 150;
const int alto = 20;
int tablero[ancho][alto];
const int barPos = alto / 2;

using namespace std;

void esperarEnter()
{
    cout << "Presiona Enter para continuar..." << endl;
    cin.ignore(); // Ignora cualquier carácter residual en el buffer
    cin.get();    // Espera a que el usuario presione Enter
}

void imprimirRaqueta(int x)
{
    // 'x' es la posición horizontal de la raqueta (número de espacios antes del carácter)
    for (int i = 0; i < 3; ++i)
    {
        // Imprimir espacios para posicionar la raqueta horizontalmente
        for (int j = 0; j < x; ++j)
        {
            cout << " ";
        }
        // Imprimir el carácter de la raqueta
        cout << u8"\u2588" << endl;
    }
}

void moverRaqueta()
{
    // logica de mover raqueta con #include <conio.h>
}

void imprimirTablero()
{
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
    for (int i = 0; i < alto; i++) // flas
    {
        for (int j = 0; j < ancho; j++) // columnas
        {
            tablero[i][j] = 0;
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
        tablero[i][0] = -1;        // Borde izquierdo
        tablero[i][alto - 1] = -1; // Borde derecho
    }
    // aqui va el código para iniciar las raquetas y la pelota
}

void iniciarComputadoraVSComputadora()
{
    iniciarTablero();
    cout << "Iniciando Computadora vs Computadora..." << endl;

    imprimirTablero();

    esperarEnter();
}

void iniciarJugadorVSComputadora()
{
    iniciarTablero();
    cout << "Iniciando Computadora vs Jugador..." << endl;

    imprimirTablero();

    esperarEnter();
}