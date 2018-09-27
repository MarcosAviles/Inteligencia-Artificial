#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"

using namespace std;

const double masinf=9999999999.0, menosinf=-9999999999.0;


// Constructor
Player::Player(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
    actual_=env;
}

double Puntuacion(int jugador, const Environment &estado){
    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;
}

// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Valoracion(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else{
    double valoracion=0;
    int columna=0;
    for (int i=0; i<7; i++){
        for (int j=0; j<7; j++){
            //ficha=estado.See_Casilla(i,j)+0;
            //if(ficha==0){ // esta vacia
            columna=estado.Get_Ocupacion_Columna(j);
            if(jugador==1){ // Si comienzo la partida como jugador 1
                if(columna<7){
                    if(j<6 && i>0){// evitar el caso de la ultima columna y la primera fila
                        if(estado.See_Casilla(i,j+1)+0==2 || estado.See_Casilla(i,j+1)+0==5){ // a la derecha hay una ficha del otro jugador
                            valoracion+=10;
                        }
                        if(j>0 && estado.See_Casilla(i,j-1)+0==2 || estado.See_Casilla(i,j-1)+0==5){ // a la izquierda hay una ficha del otro jugador
                            valoracion+=10;
                        }
                        if(estado.See_Casilla(i-1,j)+0==2){ // si encima hay una ficha del jugador
                            valoracion+=20;
                        }
                        if(estado.See_Casilla(i-1,j)+0==5){ // si justo encima hay una bomba del otro jugador
                            valoracion-=30;
                        }
                    }

                    if(j<5){
                        if((estado.See_Casilla(i,j+1)+0==2 || estado.See_Casilla(i,j+1)+0==5) && (estado.See_Casilla(i,j+2)+0==2 || estado.See_Casilla(i,j+2)+0==5)){
                            valoracion-=30;
                        }
                        if((estado.See_Casilla(i,j)+0==1 || estado.See_Casilla(i,j)+0==4) && (estado.See_Casilla(i,j+1)+0==1 || estado.See_Casilla(i,j+1)+0==4) && (estado.See_Casilla(i,j+2)+0==1 || estado.See_Casilla(i,j+2)+0==4)){
                            valoracion-=50;
                        }
                    }
                    if(j<4){
                        if((estado.See_Casilla(i,j)+0==1 || estado.See_Casilla(i,j)+0==4) && (estado.See_Casilla(i,j+1)+0==1 || estado.See_Casilla(i,j+1)+0==4) && (estado.See_Casilla(i,j+3)+0==1 || estado.See_Casilla(i,j+3)+0==4)){
                            valoracion-=50;
                        }
                        if((estado.See_Casilla(i,j)+0==1 || estado.See_Casilla(i,j)+0==4) && (estado.See_Casilla(i,j+2)+0==1 || estado.See_Casilla(i,j+2)+0==4) && (estado.See_Casilla(i,j+3)+0==1 || estado.See_Casilla(i,j+3)+0==4)){
                            valoracion-=50;
                        }
                        if((estado.See_Casilla(i,j+1)+0==1 || estado.See_Casilla(i,j+1)+0==4) && (estado.See_Casilla(i,j+2)+0==1 || estado.See_Casilla(i,j+2)+0==4) && (estado.See_Casilla(i,j+3)+0==1 || estado.See_Casilla(i,j+3)+0==4)){
                            valoracion-=50;
                        }
                        if((estado.See_Casilla(i,j)+0==1 || estado.See_Casilla(i,j)+0==4) && (estado.See_Casilla(i,j+1)+0==1 || estado.See_Casilla(i,j+1)+0==4) && (estado.See_Casilla(i,j+2)+0==1 || estado.See_Casilla(i,j+2)+0==4)){
                            valoracion-=50;
                        }
                    }
                    //diagonales
                    if(i>1){
                        if(j<5){
                            if(estado.See_Casilla(i,j)+0==1 && estado.See_Casilla(i-1,j+1)+0==1 && estado.See_Casilla(i-2,j+2)+0==1){
                                valoracion-=30;
                            }
                        }
                        if(j>1){
                            if(estado.See_Casilla(i,j-1)+0==1 && estado.See_Casilla(i-1,j-1)+0==1 && estado.See_Casilla(i-2,j-2)+0==1){
                                valoracion-=30;
                            }
                        }
                        if((estado.See_Casilla(i,j)+0==1 || estado.See_Casilla(i,j)+0==4) && (estado.See_Casilla(i-1,j)+0==1 || estado.See_Casilla(i-1,j)+0==4) && (estado.See_Casilla(i-2,j)+0==1 || estado.See_Casilla(i-2,j)+0==4)){
                            valoracion-=30;
                        }
                    }
                    if(i<5){
                        if(j<5){
                            if((estado.See_Casilla(i,j)+0==1 || estado.See_Casilla(i,j)+0==4) && (estado.See_Casilla(i+1,j+1)+0==1 || estado.See_Casilla(i+1,j+1)+0==4) && (estado.See_Casilla(i+2,j+2)+0==1 || estado.See_Casilla(i+2,j+2)+0==4)){
                                valoracion-=30;
                            }
                        }
                        if((estado.See_Casilla(i,j)+0==1 || estado.See_Casilla(i,j)+0==4) && (estado.See_Casilla(i+1,j)+0==1 || estado.See_Casilla(i+1,j)+0==4) && (estado.See_Casilla(i+2,j)+0==1 || estado.See_Casilla(i+2,j)+0==4)){
                            valoracion-=60;
                        }
                        if((estado.See_Casilla(i,j)+0==1 || estado.See_Casilla(i,j)+0==4) && (estado.See_Casilla(i,j+1)+0==1 || estado.See_Casilla(i,j+1)+0==4) && (estado.See_Casilla(i,j+2)+0==1 || estado.See_Casilla(i,j+2)+0==4)){
                            valoracion-=30;
                        }
                    }
                    if(j>2){
                        if(i<5){
                            if((estado.See_Casilla(i,j)+0==1 || estado.See_Casilla(i,j)+0==4) && (estado.See_Casilla(i+1,j-1)+0==1 || estado.See_Casilla(i+1,j-1)+0==4) && (estado.See_Casilla(i+2,j-2)+0==1 || estado.See_Casilla(i+2,j-2)+0==4)){
                                valoracion-=30;
                            }
                        }
                        if(i<4){
                            if((estado.See_Casilla(i,j)+0==1 || estado.See_Casilla(i,j)+0==4) && (estado.See_Casilla(i+1,j-1)+0==1 || estado.See_Casilla(i+1,j-1)+0==4) && (estado.See_Casilla(i+3,j-3)+0==1 || estado.See_Casilla(i+3,j-3)+0==4)){
                                valoracion-=30;
                            }
                        }
                    }
                }
            }
            else{ // Si comiento la partida como jugador 2
                if(columna<7){
                    if(j<6 && i>0){// evitar el caso de la ultima columna y la primera fila
                        if(estado.See_Casilla(i,j+1)+0==1 || estado.See_Casilla(i,j+1)+0==4){ // a la derecha hay una ficha del otro jugador
                            valoracion+=10;
                        }
                        if(j>0 && estado.See_Casilla(i,j-1)+0==1 || estado.See_Casilla(i,j-1)+0==4){ // a la izquierda hay una ficha del otro jugador
                            valoracion+=10;
                        }
                        if(estado.See_Casilla(i-1,j)+0==1){ // si encima hay una ficha del jugador
                            valoracion+=20;
                        }
                        if(estado.See_Casilla(i-1,j)+0==4){ // si justo encima hay una bomba del otro jugador
                            valoracion-=30;
                        }
                    }

                    if(j<5){
                        if((estado.See_Casilla(i,j+1)+0==1 || estado.See_Casilla(i,j+1)+0==4) && (estado.See_Casilla(i,j+2)+0==1 || estado.See_Casilla(i,j+2)+0==4)){
                            valoracion-=30;
                        }
                        if((estado.See_Casilla(i,j)+0==2 || estado.See_Casilla(i,j)+0==5) && (estado.See_Casilla(i,j+1)+0==2 || estado.See_Casilla(i,j+1)+0==5) && (estado.See_Casilla(i,j+2)+0==2 || estado.See_Casilla(i,j+2)+0==5)){
                            valoracion-=50;
                        }
                    }
                    if(j<4){
                        if((estado.See_Casilla(i,j)+0==2 || estado.See_Casilla(i,j)+0==5) && (estado.See_Casilla(i,j+1)+0==2 || estado.See_Casilla(i,j+1)+0==5) && (estado.See_Casilla(i,j+3)+0==2 || estado.See_Casilla(i,j+3)+0==5)){
                            valoracion-=50;
                        }
                        if((estado.See_Casilla(i,j)+0==2 || estado.See_Casilla(i,j)+0==5) && (estado.See_Casilla(i,j+2)+0==2 || estado.See_Casilla(i,j+2)+0==5) && (estado.See_Casilla(i,j+3)+0==2 || estado.See_Casilla(i,j+3)+0==5)){
                            valoracion-=50;
                        }
                        if((estado.See_Casilla(i,j+1)+0==2 || estado.See_Casilla(i,j+1)+0==5) && (estado.See_Casilla(i,j+2)+0==2 || estado.See_Casilla(i,j+2)+0==5) && (estado.See_Casilla(i,j+3)+0==2 || estado.See_Casilla(i,j+3)+0==5)){
                            valoracion-=50;
                        }
                        if((estado.See_Casilla(i,j)+0==2 || estado.See_Casilla(i,j)+0==5) && (estado.See_Casilla(i,j+1)+0==2 || estado.See_Casilla(i,j+1)+0==5) && (estado.See_Casilla(i,j+2)+0==2 || estado.See_Casilla(i,j+2)+0==5)){
                            valoracion-=50;
                        }
                    }
                    //diagonales
                    if(i>1){
                        if(j<5){
                            if(estado.See_Casilla(i,j)+0==2 && estado.See_Casilla(i-1,j+1)+0==2 && estado.See_Casilla(i-2,j+2)+0==2){
                                valoracion-=30;
                            }
                        }
                        if(j>1){
                            if(estado.See_Casilla(i,j-1)+0==2 && estado.See_Casilla(i-1,j-1)+0==2 && estado.See_Casilla(i-2,j-2)+0==2){
                                valoracion-=30;
                            }
                        }
                        if((estado.See_Casilla(i,j)+0==2 || estado.See_Casilla(i,j)+0==5) && (estado.See_Casilla(i-1,j)+0==2 || estado.See_Casilla(i-1,j)+0==5) && (estado.See_Casilla(i-2,j)+0==2 || estado.See_Casilla(i-2,j)+0==5)){
                            valoracion-=30;
                        }
                    }
                    if(i<5){
                        if(j<5){
                            if((estado.See_Casilla(i,j)+0==2 || estado.See_Casilla(i,j)+0==5) && (estado.See_Casilla(i+1,j+1)+0==2 || estado.See_Casilla(i+1,j+1)+0==5) && (estado.See_Casilla(i+2,j+2)+0==2 || estado.See_Casilla(i+2,j+2)+0==5)){
                                valoracion-=30;
                            }
                        }
                        if((estado.See_Casilla(i,j)+0==2 || estado.See_Casilla(i,j)+0==5) && (estado.See_Casilla(i+1,j)+0==2 || estado.See_Casilla(i+1,j)+0==5) && (estado.See_Casilla(i+2,j)+0==2 || estado.See_Casilla(i+2,j)+0==5)){
                            valoracion-=60;
                        }
                        if((estado.See_Casilla(i,j)+0==2 || estado.See_Casilla(i,j)+0==5) && (estado.See_Casilla(i,j+1)+0==2 || estado.See_Casilla(i,j+1)+0==5) && (estado.See_Casilla(i,j+2)+0==2 || estado.See_Casilla(i,j+2)+0==5)){
                            valoracion-=30;
                        }
                    }
                    if(j>2){
                        if(i<5){
                            if((estado.See_Casilla(i,j)+0==2 || estado.See_Casilla(i,j)+0==5) && (estado.See_Casilla(i+1,j-1)+0==2 || estado.See_Casilla(i+1,j-1)+0==5) && (estado.See_Casilla(i+2,j-2)+0==2 || estado.See_Casilla(i+2,j-2)+0==5)){
                                valoracion-=30;
                            }
                        }
                        if(i<4){
                            if((estado.See_Casilla(i,j)+0==2 || estado.See_Casilla(i,j)+0==5) && (estado.See_Casilla(i+1,j-1)+0==2 || estado.See_Casilla(i+1,j-1)+0==5) && (estado.See_Casilla(i+3,j-3)+0==2 || estado.See_Casilla(i+3,j-3)+0==5)){
                                valoracion-=30;
                            }
                        }
                    }
                }
            }
        }
    }
    return valoracion;
    }

}

// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
          return Puntuacion(jugador,estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar









// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){
    j=0;
    for (int i=0; i<8; i++){
        if (aplicables[i]){
           opciones[j]=i;
           j++;
        }
    }
}

double Player::Poda_AlfaBeta(const Environment & T, int jugador, int profundidad, int profundidad_alfabeta, Environment::ActionType &accion, double alpha, double beta){

	Environment::ActionType accion_anterior;
	double aux;

	bool opciones[7];
	int n_act = T.possible_actions(opciones);

	if(profundidad==profundidad_alfabeta || T.JuegoTerminado()){
        return Valoracion(T, jugador);
	}
	else{
        int ult_act=-1;
        Environment hijo = T.GenerateNextMove(ult_act);
        if (profundidad%2==0) {
            while(ult_act<8 && beta>alpha){
                aux = Poda_AlfaBeta(hijo, jugador, profundidad+1, profundidad_alfabeta, accion_anterior, alpha, beta);
                if(aux>alpha){
                    alpha=aux;
                    accion = static_cast <Environment::ActionType > (ult_act);
                }
                hijo = T.GenerateNextMove(ult_act);
            }
            return alpha;
        }
        else{
            while (ult_act<8 && beta>alpha){
                aux = Poda_AlfaBeta(hijo, jugador, profundidad+1, profundidad_alfabeta, accion_anterior, alpha, beta);
                if (aux<beta){
                    beta=aux;
                    accion = static_cast <Environment::ActionType > (ult_act);
                }
                hijo = T.GenerateNextMove(ult_act);
            }
            return beta;
        }

	}

}




// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){
    const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

    Environment::ActionType accion; // acción que se va a devolver
    bool aplicables[8]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si PUT1 es aplicable
                        // aplicables[1]==true si PUT2 es aplicable
                        // aplicables[2]==true si PUT3 es aplicable
                        // aplicables[3]==true si PUT4 es aplicable
                        // aplicables[4]==true si PUT5 es aplicable
                        // aplicables[5]==true si PUT6 es aplicable
                        // aplicables[6]==true si PUT7 es aplicable
                        // aplicables[7]==true si BOOM es aplicable



    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha, beta; // Cotas de la poda AlfaBeta

    int n_act; //Acciones posibles en el estado actual


    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    //actual_.PintaTablero();
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<8; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;


    //--------------------- COMENTAR Desde aqui
    /*cout << "\n\t";
    int n_opciones=0;
    JuegoAleatorio(aplicables, opciones, n_opciones);

    if (n_act==0){
      (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
      cout << " No puede realizar ninguna accion!!!\n";
      //accion = Environment::actIDLE;
    }
    else if (n_act==1){
           (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
            cout << " Solo se puede realizar la accion "
                 << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[0])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[0]);

         }
         else { // Hay que elegir entre varias posibles acciones
            int aleatorio = rand()%n_opciones;
            cout << " -> " << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[aleatorio])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[aleatorio]);
         }
    */
    //--------------------- COMENTAR Hasta aqui


    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------


    // Opcion: Poda AlfaBeta
    // NOTA: La parametrizacion es solo orientativa
    alpha=menosinf;
    beta=masinf;
    valor = Poda_AlfaBeta(actual_, jugador_, 0, PROFUNDIDAD_ALFABETA, accion, alpha, beta);
    cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;
    /*if(actual_.See_Casilla(0,2)==0){
        accion=actual_.PUT2;
    }*/
    return accion;
}

