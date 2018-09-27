
#ifndef AGENT__
#define AGENT__

#include <string>
#include <iostream>
#include <cstdlib>
#include "conexion.h"
using namespace std;

// -----------------------------------------------------------
//				class Agent
// -----------------------------------------------------------
class Environment;
class Agent
{
public:
	Agent(){
		x_= 99;
		y_= 99;
		orientacion_=3;
		role_="PlYR";
		last_accion_=3;
		REINICIADO_=false;
		size_=200;
		for (int i=0;i<200;i++)
		    for(int j=0;j<200;j++){
		      mapa_entorno_[i][j]='?';
		      mapa_objetos_[i][j]='?';
		      tiempo[i][j]=0;
		    }

		for (int i=0;i<100;i++)
		    for(int j=0;j<100;j++)
		      mapa_solucion_[i][j]='?';

		//para encontrar dos posiciones de referencia
		n_gps=0;
		posicionf=0;
		posicionc=0;
		for(int i=0; i<2; i++){
            for(int j=0; j<2; j++){
                posicion_[i][j]=0;
                posicion_l[i][j]=0;
            }
        }
        pos_encontrada=false;

        movimiento_obligatorio=false;
        n_movimientos=0;
        indice=0;

        // para coger objetos y controlar la mochila
        n_objetos=0;
        guardar_mochila=false;
        ciclo=1;
        tengo_objeto=false;
	}

	enum ActionType
	{
	    actFORWARD, // avanzar
	    actTURN_L,  // Girar Izquierda
	    actTURN_R,  // Girar Derecha
	    actIDLE,    // No hacer nada
	    actPICKUP,  // Recoger un objeto
	    actPUTDOWN, // Soltar un objeto
	    actPUSH,    // Meter en la mochila
	    actPOP,     // Sacar de la mochila
	    actGIVE,	// Dar un objeto a un personaje
	    actTHROW	// Lanzar un objeto

	};

	void Perceive(Environment &env);
	bool Perceive_Remote(conexion_client &Cliente, Environment &env);
	void ActualizarInformacion(Environment *env);
	ActionType Think();
	void FixLastAction(Agent::ActionType x){last_accion_=x;};

	char mapa_entorno_[200][200]; // mapa que muestra el tipo de terreno
	char mapa_objetos_[200][200]; // mapa que muestra los objetos que estan encima del terreno
	char mapa_solucion_[100][100]; // Mapa que almacena la solucion que el alumno propone
	// Funciones de acceso a los datos
	void GetCoord(int &fila, int &columna, int &brujula){fila=y_;columna=x_;brujula=orientacion_;};
    void rotar90();
    void transladarPrincipioMatrizEntorno();
    void retintarBordes();
    void guardarPartida();
    ActionType calcularMovimiento();
    void recuperarPartida();
    void actualizarObstaculo();
    ActionType darObjetos();
    void buscarPK();

private:
	//Variables de interaccion con el entorno grafico
	int size_;

	//SENSORES
	char VISTA_[10];
	char SURFACE_[10];
	bool REINICIADO_;
	string MENSAJE_;
	char EN_USO_;
	char MOCHILLA_[5];
	char PUNTUACION_[9];
	bool FIN_JUEGO_;
	char SALUD_;

	//Variables de estado
	int x_,y_, orientacion_;
	int last_accion_;
	string role_;

	//para encontrar la orientacion ---
	int n_gps;
	int posicion_[2][2], posicion_l[2][2];
	int posicionf, posicionc;
	bool pos_encontrada;

    //Para ir directo a por un punto de referencia
    Agent::ActionType recorrido[10];
    bool movimiento_obligatorio;
    int n_movimientos, indice;
    int tiempo[200][200];
    int ciclo;

    //para controlar la mochila y coger objetos
    int n_objetos;
    bool guardar_mochila;
    bool tengo_objeto;
};

string ActionStr(Agent::ActionType);

#endif
