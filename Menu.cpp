#include <iostream>
using namespace std;

void mostrarMenu() {
   cout << "****************************" << endl;
   cout << "*        PONG GAME         *" << endl;
   cout << "****************************" << endl;
   cout << "1. Computadora vs Computadora" << endl;
   cout << "2. Computadora vs Jugador" << endl;
   cout << "3. Salir" << endl;
   cout << "****************************" << endl;
   cout << "Selecciona una opción: ";
}

int main() {
   int opcion;

   while (true) {
      mostrarMenu();
      cin >> opcion;

      switch (opcion) {
         case 1:
               cout << "Modo Computadora vs Computadora seleccionado." << endl;
               //Codigo 
               break;
         case 2:
               cout << "Modo Computadora vs Jugador seleccionado." << endl;
               //Codigo
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
