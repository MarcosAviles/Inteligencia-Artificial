#include "belkan.h"
#include "conexion.h"
#include "environment.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>
#include <cstring>

using namespace std;



// -----------------------------------------------------------
void PasarVectoraMapaCaracteres(int fila, int columna, char m[200][200], char *v, int brujula){
  m[fila][columna]=v[0];

    switch(brujula){
        case 0: // Orientacion Norte

		m[fila-1][columna]=v[1];
		for (int i=0; i<3; i++){
		  m[fila-2][columna+i-1]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila-3][columna+j-2]=v[5+j];
		}
	        break;
	case 1: // Orientacion Este
		m[fila][columna+1]=v[1];
		for (int i=0; i<3; i++){
		  m[fila+i-1][columna+2]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila+j-2][columna+3]=v[5+j];
		}
	        break;
        case 2: // Orientacion Sur
		m[fila+1][columna]=v[1];
		for (int i=0; i<3; i++){
		  m[fila+2][columna+1-i]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila+3][columna+2-j]=v[5+j];
		}
		                break;
        case 3: // Orientacion Oeste
		m[fila][columna-1]=v[1];
		for (int i=0; i<3; i++){
		  m[fila+1-i][columna-2]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila+2-j][columna-3]=v[5+j];
		}

                break;
    }

}

void PasarVectoraMapaCaracteres100(int fila, int columna, char m[100][100], char *v, int brujula){
  m[fila][columna]=v[0];

    switch(brujula){
        case 0: // Orientacion Norte

		m[fila-1][columna]=v[1];
		for (int i=0; i<3; i++){
		  m[fila-2][columna+i-1]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila-3][columna+j-2]=v[5+j];
		}
	        break;
	case 1: // Orientacion Este
		m[fila][columna+1]=v[1];
		for (int i=0; i<3; i++){
		  m[fila+i-1][columna+2]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila+j-2][columna+3]=v[5+j];
		}
	        break;
        case 2: // Orientacion Sur
		m[fila+1][columna]=v[1];
		for (int i=0; i<3; i++){
		  m[fila+2][columna+1-i]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila+3][columna+2-j]=v[5+j];
		}
		                break;
        case 3: // Orientacion Oeste
		m[fila][columna-1]=v[1];
		for (int i=0; i<3; i++){
		  m[fila+1-i][columna-2]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila+2-j][columna-3]=v[5+j];
		}

                break;
    }

}

// -----------------------------------------------------------
void Agent::Perceive(Environment &env)
{
	env.SenSorStatus(VISTA_, SURFACE_, MENSAJE_, REINICIADO_, EN_USO_, MOCHILLA_, PUNTUACION_, FIN_JUEGO_, SALUD_, false);

}


bool Agent::Perceive_Remote(conexion_client &Cliente, Environment &env)
{
	bool actualizado=false;


	actualizado = env.Perceive_Remote(Cliente);
	if (actualizado)
		env.SenSorStatus(VISTA_, SURFACE_, MENSAJE_, REINICIADO_, EN_USO_, MOCHILLA_, PUNTUACION_, FIN_JUEGO_, SALUD_, true);

    return actualizado;
}


// -----------------------------------------------------------
string ActionStr(Agent::ActionType accion)
{
	switch (accion)
	{
	case Agent::actFORWARD: return "FORWARD";
	case Agent::actTURN_L: return "TURN LEFT";
	case Agent::actTURN_R: return "TURN RIGHT";
	case Agent::actIDLE: return "IDLE";
	case Agent::actPICKUP: return "PICK UP";
	case Agent::actPUTDOWN: return "PUT DOWN";
	case Agent::actPUSH: return "PUSH";
	case Agent::actPOP: return "POP";
	case Agent::actGIVE: return "GIVE";
	case Agent::actTHROW: return "THROW";
	default: return "????";
	}
}
void CapturaFilaColumnaPK (string mensaje, int &fila, int &columna){
	if (mensaje.substr(0,8)=="PK fila:"){
		int pos = mensaje.find('c');
		string valor = mensaje.substr(9,pos-8);
		fila = atoi(valor.c_str());

		int pos2 = mensaje.find('.');
		pos = pos+8;
		valor = mensaje.substr(pos,pos2-1);
		columna = atoi(valor.c_str());
	}
}
// -----------------------------------------------------------
void Agent::ActualizarInformacion(Environment *env){
	// Actualizar mi informacion interna
	if (REINICIADO_){
        guardarPartida();
		// Lo que tengas que hacer si eres reposicionado en el juego
		for (int i=0; i<200; i++){
			for(int j=0; j<200; j++){
				mapa_entorno_[i][j]='?';
				mapa_objetos_[i][j]='?';
			}
		}
		x_=99;
		y_=99;
        n_gps=0;
		posicionf=0;
		posicionc=0;
		for(int i=0; i<2; i++){
            for(int j=0; j<2; j++){
                posicion_[i][j]=0;
            }
        }
        pos_encontrada=false;

        movimiento_obligatorio=false;
        n_movimientos=0;
        indice=0;

        // para coger objetos y controlar la mochila
        n_objetos=0;
        guardar_mochila=false;
	}

	switch(last_accion_){
	  case 0: //avanar
	      if(MENSAJE_!="Has chocado con un obstaculo movil. "){
            switch(orientacion_){
	    case 0: // norte
		    y_--;
		    break;
	    case 1: // este
		    x_++;
		    break;
	    case 2: // sur
		    y_++;
		    break;
	    case 3: // oeste
		    x_--;
		    break;
	  }
	      }
	  break;
	  case 1: // girar izq
		  orientacion_=(orientacion_+3)%4;
		  break;
	  case 2: // girar dch
		  orientacion_=(orientacion_+1)%4;
		  break;
	}

	// Comprobacion para no salirme del rango del mapa
	bool algo_va_mal=false;
	if (y_<0){
		y_=0;
		algo_va_mal=true;
	}
	else if (y_>199){
		y_=199;
		algo_va_mal=true;
	}
	if (x_<0){
		x_=0;
		algo_va_mal=true;
	}
	else if (x_>199){
		x_=199;
		algo_va_mal=true;
	}

	if (algo_va_mal){
		cout << "CUIDADO: NO ESTAS CONTROLANDO BIEN LA UBICACION DE TU AGENTE\n";
	}

	// contador que utiliza para la matriz de tiempos y voy incrementando el valor por donde paso
	ciclo++;
	tiempo[y_][x_]=ciclo;

	// Si no estoy orientado y no encontrado puntos de referencia, busco los puntos de referencia y guarda la posicion original y local
	if(n_gps<2 && !pos_encontrada){
        if(VISTA_[0]=='K'){
                CapturaFilaColumnaPK(MENSAJE_, posicion_[posicionf][posicionc], posicion_[posicionf][posicionc+1]);
                posicion_l[posicionf][posicionc]=y_;
                posicion_l[posicionf][posicionc+1]=x_;
                n_gps++;
                posicionf++;

                if(posicion_l[0][0]==posicion_l[1][0] && posicion_l[0][1]==posicion_l[1][1]){
                    n_gps--;
                    posicionf--;
                    posicion_[posicionf][posicionc]=0;
                    posicion_[posicionf][posicionc+1]=0;
                    posicion_l[posicionf][posicionc]=0;
                    posicion_l[posicionf][posicionc+1]=0;

                }
        }
	}

	// si ya encontrado dos puntos de referencia y no estoy orientado, evito guardar el mismo punto de referencia
	// si los dos puntos de referencia que tengo almacenados no son iguales me puedo orientar y me oriento
	if(n_gps==2 && !pos_encontrada){

            // Este caso estoy orientado hacia el sur
            if((posicion_[0][0]-posicion_[1][0])==-(posicion_l[0][0]-posicion_l[1][0])){
                    rotar90();
                    rotar90();
                    posicion_l[1][0]=199-posicion_l[1][0];
                    posicion_l[1][1]=199-posicion_l[1][1];
                    transladarPrincipioMatrizEntorno();
                    guardarPartida();
            }

            // En esta caso estoy orientado hacia el oeste
            else if((posicion_[0][0]-posicion_[1][0])==(posicion_l[0][1]-posicion_l[1][1])){
                rotar90();
                swap(posicion_l[1][0], posicion_l[1][1]);
                posicion_l[1][1]=199-posicion_l[1][1];
                transladarPrincipioMatrizEntorno();
                guardarPartida();
            }

            // En este caso estoy orientado al este
            else if((posicion_[0][0]-posicion_[1][0])==-(posicion_l[0][1]-posicion_l[1][1])){
                rotar90();
                rotar90();
                rotar90();
                swap(posicion_l[1][0], posicion_l[1][1]);
                posicion_l[1][0]=199-posicion_l[1][0];
                transladarPrincipioMatrizEntorno();
                guardarPartida();

            }

            // En este caso estoy orientado al norte y no tengo que orientarme
            else{
                transladarPrincipioMatrizEntorno();
                guardarPartida();
            }
            pos_encontrada=true;
            recuperarPartida();
            x_=posicion_[1][1];
            y_=posicion_[1][0];
    }
	PasarVectoraMapaCaracteres(y_,x_,mapa_entorno_,VISTA_,orientacion_);
	PasarVectoraMapaCaracteres(y_,x_,mapa_objetos_,SURFACE_,orientacion_);
    if(pos_encontrada){
        PasarVectoraMapaCaracteres100(y_,x_,mapa_solucion_,VISTA_,orientacion_);
    }
	env->ActualizarMatrizUsuario(mapa_entorno_);

}

// Función para rotar la matriz entorno 90 grados a la derecha
void Agent::rotar90(){
        char aux[200][200];
        int tiempo2[200][200];
        for(int i=0; i<200; i++){
            for(int j=0; j<200; j++){
                aux[j][200-i-1]=mapa_entorno_[i][j];
                tiempo2[j][200-i-1]=tiempo[i][j];
            }
        }
        for(int i=0; i<200; i++){
            for(int j=0; j<200; j++){
                mapa_entorno_[i][j]=aux[i][j];
                tiempo[i][j]=tiempo2[i][j];
            }
        }
        orientacion_ =  (orientacion_+1)%4;
}

// Esta función la utiliza para cuando ya estoy orientado translado la matriz entorno a la posicion 0,0 de la misma matriz
void Agent::transladarPrincipioMatrizEntorno(){
    /*int ini, fin, fila, columna;
    bool encontrado=false;
    fila=0;
    columna=0;
    for(int i=0; i<200 && !encontrado; i++){
        for(int j=0; j<200 && !encontrado; j++){
            if(mapa_entorno_[i][j]=='K'){
                ini=i;
                fin=j;
                encontrado=true;
            }
        }
    }
    if(posicion_[0][0]<posicion_[1][0]){
        ini=ini-posicion_[0][0];
        int ayuda=100-posicion_[0][1];
        fin=fin+ayuda;
    }
    else{
        ini=ini-posicion_[1][0];
        int ayuda=100-posicion_[1][1];
        fin=fin+ayuda;
    }
    char aux[100][100];
    int aux2[100][100];
    for(int i=ini; i<ini+100; i++){
        for(int j=fin-100; j<fin; j++){
            aux[fila][columna]=mapa_entorno_[i][j];
            aux2[fila][columna]=tiempo[i][j];
            columna++;
        }
        columna=0;
        fila++;
    }

    for(int i=0; i<200; i++){
        for(int j=0; j<200; j++){
            mapa_entorno_[i][j]='?';
            tiempo[i][j]=0;
        }
    }

    for(int i=0; i<100; i++){
        for(int j=0; j<100; j++){
            mapa_entorno_[i][j]=aux[i][j];
            tiempo[i][j]=aux2[i][j];
        }
    }
    y_=posicion_[1][0];
    x_=posicion_[1][1];
    */

    char aux[100][100];
    int tiempo2[100][100];
    for(int i=0; i<100; i++){
        for(int j=0; j<100; j++){
            aux[i][j]=mapa_entorno_[i+posicion_l[1][0]-posicion_[1][0]][j+posicion_l[1][1]-posicion_[1][1]];
            tiempo2[i][j]=tiempo[i+posicion_l[1][0]-posicion_[1][0]][j+posicion_l[1][1]-posicion_[1][1]];
        }
    }
    for(int i=0; i<200; i++){
        for(int j=0; j<200; j++){
            mapa_entorno_[i][j]='?';
            tiempo[i][j]=0;
        }
    }
    for(int i=0; i<100; i++){
        for(int j=0; j<100; j++){
            mapa_entorno_[i][j]=aux[i][j];
            tiempo[i][j]=tiempo2[i][j];
        }
    }
    retintarBordes();

}

// Esto es un poco fulleria, retindo los bordes tanto de la matriz entorno como la matriz solucion, asi gano algo de puntos
void Agent::retintarBordes(){
     //margen superior e inferior

    for (int col=0; col<100; col++){
        mapa_entorno_[0][col]='P';
        mapa_entorno_[1][col]='P';
        mapa_entorno_[2][col]='P';
        mapa_entorno_[99][col]='P';
        mapa_entorno_[98][col]='P';
        mapa_entorno_[97][col]='P';
        tiempo[0][col]=20000;
        tiempo[1][col]=20000;
        tiempo[2][col]=20000;
        tiempo[99][col]=20000;
        tiempo[98][col]=20000;
        tiempo[97][col]=20000;
    }

    for (int fil=0; fil<100; fil++){
        mapa_entorno_[fil][0]='P';
        mapa_entorno_[fil][1]='P';
        mapa_entorno_[fil][2]='P';
        mapa_entorno_[fil][99]='P';
        mapa_entorno_[fil][98]='P';
        mapa_entorno_[fil][97]='P';
        tiempo[fil][0]=20000;
        tiempo[fil][1]=20000;
        tiempo[fil][2]=20000;
        tiempo[fil][99]=20000;
        tiempo[fil][98]=20000;
        tiempo[fil][97]=20000;
    }

    for(int i=0; i<100; i++){
        for(int j=0; j<100; j++){
            if(mapa_entorno_[i][j]=='?'){
                mapa_entorno_[i][j]='S';
            }
        }
    }
    /*for(int i=0; i<3; i++){
        int aux=99;
        for(int j=0; j<100; j++){
            mapa_solucion_[i][j]='P';
            mapa_solucion_[aux][j]='P';
            mapa_entorno_[i][j]='P';
            mapa_entorno_[aux][j]='P';
        }
        aux--;
    }
    //margen izquierdo y derecheo
    for(int i=0; i<100; i++){
        for(int j=0; j<3; j++){
            int aux=99;
            mapa_solucion_[i][j]='P';
            mapa_solucion_[i][aux]='P';
            mapa_entorno_[i][j]='P';
            mapa_entorno_[i][aux]='P';
            aux--;
        }
    }*/
}

// En el caso de que se muera el personaje salvo lo que tengo explorado en el mapa solución
void Agent::guardarPartida(){
    for(int i=0; i<100; i++){
        for(int j=0; j<100; j++){
            if(mapa_solucion_[i][j]=='?'){
                mapa_solucion_[i][j]=mapa_entorno_[i][j];
            }
        }
    }
}


// Caso contrario anterior, cuando resucitas y vuelves a orientart recupero lo que ya antes habia explorado
void Agent::recuperarPartida(){
    for(int i=0; i<100; i++){
        for(int j=0; j<100; j++){
            mapa_entorno_[i][j]=mapa_solucion_[i][j];
        }
    }
}

/*void Agent::actualizarMapaSol(){
    if(orientacion_==0){
        mapa_solucion_[y_][x_]=VISTA_[0];
        mapa_solucion_[y_-1][x_]=VISTA_[1];
        mapa_solucion_[y_-2][x_-1]=VISTA_[2];
        mapa_solucion_[y_-2][x_]=VISTA_[3];
        mapa_solucion_[y_-2][x_+1]=VISTA_[4];
        mapa_solucion_[y_-3][x_-2]=VISTA_[5];
        mapa_solucion_[y_-3][x_-1]=VISTA_[6];
        mapa_solucion_[y_-3][x_]=VISTA_[7];
        mapa_solucion_[y_-3][x_+1]=VISTA_[8];
        mapa_solucion_[y_-3][x_+2]=VISTA_[9];
    }
    if(orientacion_==1){
        mapa_solucion_[y_][x_]=VISTA_[0];
        mapa_solucion_[y_][x_+1]=VISTA_[1];
        mapa_solucion_[y_-1][x_+2]=VISTA_[2];
        mapa_solucion_[y_][x_+2]=VISTA_[3];
        mapa_solucion_[y_+1][x_+2]=VISTA_[4];
        mapa_solucion_[y_-2][x_+3]=VISTA_[5];
        mapa_solucion_[y_-1][x_+3]=VISTA_[6];
        mapa_solucion_[y_][x_+3]=VISTA_[7];
        mapa_solucion_[y_+1][x_+3]=VISTA_[8];
        mapa_solucion_[y_+2][x_+3]=VISTA_[9];
    }
    if(orientacion_==2){
        mapa_solucion_[y_][x_]=VISTA_[0];
        mapa_solucion_[y_+1][x_]=VISTA_[1];
        mapa_solucion_[y_+2][x_+1]=VISTA_[2];
        mapa_solucion_[y_+2][x_]=VISTA_[3];
        mapa_solucion_[y_+2][x_-1]=VISTA_[4];
        mapa_solucion_[y_+3][x_+2]=VISTA_[5];
        mapa_solucion_[y_+3][x_+1]=VISTA_[6];
        mapa_solucion_[y_+3][x_]=VISTA_[7];
        mapa_solucion_[y_+3][x_-1]=VISTA_[8];
        mapa_solucion_[y_+3][x_-2]=VISTA_[9];
    }
    if(orientacion_==3){
        mapa_solucion_[y_][x_]=VISTA_[0];
        mapa_solucion_[y_][x_-1]=VISTA_[1];
        mapa_solucion_[y_+1][x_-2]=VISTA_[2];
        mapa_solucion_[y_][x_-2]=VISTA_[3];
        mapa_solucion_[y_-1][x_-2]=VISTA_[4];
        mapa_solucion_[y_+2][x_-3]=VISTA_[5];
        mapa_solucion_[y_+1][x_-3]=VISTA_[6];
        mapa_solucion_[y_][x_-3]=VISTA_[7];
        mapa_solucion_[y_-1][x_-3]=VISTA_[8];
        mapa_solucion_[y_-2][x_-3]=VISTA_[9];
    }
}*/


// Camino de Pulgarcito, calculo el movimiento siguiente según mi orientación y los valores de mi matriz tiempo.
Agent::ActionType Agent::calcularMovimiento(){
    Agent::ActionType movimiento=actFORWARD;
    if(orientacion_==0){
        if(tiempo[y_][x_-1]<tiempo[y_-1][x_] && tiempo[y_][x_-1]<=tiempo[y_][x_+1]){
            movimiento=actTURN_L;
        }
        else if(tiempo[y_][x_+1]<tiempo[y_-1][x_] && tiempo[y_][x_+1]<=tiempo[y_][x_-1]){
            movimiento=actTURN_R;
        }
        else if(tiempo[y_][x_+1]==20000 && tiempo[y_-1][x_]==20000 && tiempo[y_][x_-1]==20000){
            movimiento=actTURN_L;
        }
    }
    if(orientacion_==1){
        if(tiempo[y_-1][x_]<tiempo[y_][x_+1] && tiempo[y_-1][x_]<=tiempo[y_+1][x_]){
            movimiento=actTURN_L;
        }
        else if(tiempo[y_+1][x_]<tiempo[y_][x_+1] && tiempo[y_+1][x_]<=tiempo[y_-1][x_]){
            movimiento=actTURN_R;
        }
        else if(tiempo[y_-1][x_]==20000 && tiempo[y_][x_+1]==20000 && tiempo[y_+1][x_]==20000){
            movimiento=actTURN_L;
        }
    }

    if(orientacion_==2){
        if(tiempo[y_][x_+1]<tiempo[y_+1][x_] && tiempo[y_][x_+1]<=tiempo[y_][x_-1]){
            movimiento=actTURN_L;
        }
        else if(tiempo[y_][x_-1]<tiempo[y_+1][x_] && tiempo[y_][x_-1]<=tiempo[y_][x_+1]){
            movimiento=actTURN_R;
        }
        else if(tiempo[y_][x_+1]==20000 && tiempo[y_+1][x_]==20000 && tiempo[y_][x_-1]==20000){
            movimiento=actTURN_L;
        }
    }

    if(orientacion_==3){
        if(tiempo[y_+1][x_]<tiempo[y_][x_-1] && tiempo[y_+1][x_]<=tiempo[y_-1][x_]){
            movimiento=actTURN_L;
        }
        else if(tiempo[y_-1][x_]<tiempo[y_][x_-1] && tiempo[y_-1][x_]<=tiempo[y_+1][x_]){
            movimiento=actTURN_R;
        }
        else if(tiempo[y_+1][x_]==20000 && tiempo[y_][x_-1]==20000 && tiempo[y_+1][x_]==20000){
            movimiento=actTURN_L;
        }
    }
    return movimiento;
}

// Cuando me encuentro un obstaculo, en mi matriz tiempo le asigno un valor alto para evitarlo y no volver a pasar por ahi
void Agent::actualizarObstaculo(){
    switch(orientacion_){
        case 0: tiempo[y_-1][x_]=20000;
            break;
        case 1: tiempo[y_][x_+1]=20000;
            break;
        case 2: tiempo[y_+1][x_]=20000;
            break;
        case 3: tiempo[y_][x_-1]=20000;
            break;
    }
    /*if(orientacion_==1){
        tiempo[y_][x_+1]=20000;
    }
    if(orientacion_==0){
        tiempo[y_-1][x_]=20000;
    }
    if(orientacion_==2){
        tiempo[y_+1][x_]=20000;
    }
    if(orientacion_==3){
        tiempo[y_][x_-1]=20000;
    }*/
}

Agent::ActionType Agent::darObjetos(){
    Agent::ActionType movimiento;
    switch(n_objetos){
                    case 0: break;
                    case 1: if(EN_USO_!='-'){
                                movimiento=actGIVE;
                                n_objetos--;
                            }
                            else{
                                movimiento_obligatorio=true;
                                movimiento=actPOP;
                                recorrido[0]=actGIVE;
                                n_movimientos=1;
                                n_objetos--;
                            }
                            break;
                    case 2: if(EN_USO_!='-'){
                                movimiento_obligatorio=true;
                                movimiento=actGIVE;
                                recorrido[0]=actPOP;
                                recorrido[1]=actGIVE;
                                n_movimientos=2;
                                n_objetos=n_objetos-2;
                            }
                            else{
                                movimiento_obligatorio=true;
                                movimiento=actPOP;
                                recorrido[0]=actGIVE;
                                recorrido[1]=actPOP;
                                recorrido[2]=actGIVE;
                                n_movimientos=3;
                                n_objetos=n_objetos-2;
                            }
                            break;
                    case 3: if(EN_USO_!='-'){
                                movimiento_obligatorio=true;
                                movimiento=actGIVE;
                                recorrido[0]=actPOP;
                                recorrido[1]=actGIVE;
                                recorrido[2]=actPOP;
                                recorrido[3]=actGIVE;
                                n_movimientos=4;
                                n_objetos=n_objetos-3;
                            }
                            else{
                                movimiento_obligatorio=true;
                                movimiento=actPOP;
                                recorrido[0]=actGIVE;
                                recorrido[1]=actPOP;
                                recorrido[2]=actGIVE;
                                recorrido[3]=actPOP;
                                recorrido[4]=actGIVE;
                                n_movimientos=5;
                                n_objetos=n_objetos-3;
                            }
                            break;
                    case 4: if(EN_USO_!='-'){
                                movimiento_obligatorio=true;
                                movimiento=actGIVE;
                                recorrido[0]=actPOP;
                                recorrido[1]=actGIVE;
                                recorrido[2]=actPOP;
                                recorrido[3]=actGIVE;
                                recorrido[4]=actPOP;
                                recorrido[5]=actGIVE;
                                n_movimientos=6;
                                n_objetos=n_objetos-4;
                            }
                            else{
                                movimiento_obligatorio=true;
                                movimiento=actPOP;
                                recorrido[0]=actGIVE;
                                recorrido[1]=actPOP;
                                recorrido[2]=actGIVE;
                                recorrido[3]=actPOP;
                                recorrido[4]=actGIVE;
                                recorrido[5]=actPOP;
                                recorrido[6]=actGIVE;
                                n_movimientos=7;
                                n_objetos=n_objetos-4;
                            }
                            break;
                    case 5: if(EN_USO_!='-'){
                                movimiento_obligatorio=true;
                                movimiento=actGIVE;
                                recorrido[0]=actPOP;
                                recorrido[1]=actGIVE;
                                recorrido[2]=actPOP;
                                recorrido[3]=actGIVE;
                                recorrido[4]=actPOP;
                                recorrido[5]=actGIVE;
                                recorrido[6]=actPOP;
                                recorrido[7]=actGIVE;
                                n_movimientos=8;
                                n_objetos=n_objetos-5;
                            }
                            else{
                                movimiento_obligatorio=true;
                                movimiento=actPOP;
                                recorrido[0]=actGIVE;
                                recorrido[1]=actPOP;
                                recorrido[2]=actGIVE;
                                recorrido[3]=actPOP;
                                recorrido[4]=actGIVE;
                                recorrido[5]=actPOP;
                                recorrido[6]=actGIVE;
                                recorrido[7]=actPOP;
                                recorrido[8]=actGIVE;
                                n_movimientos=9;
                                n_objetos=n_objetos-5;
                            }
                            break;
                }
    return movimiento;
}


// Este método devuelve un recorrido si encuentra un punto de referencia en su vision
void Agent::buscarPK(){
if(!movimiento_obligatorio && n_gps<2){
        for(int i=0; i<9; i++){
            if(VISTA_[i]=='K'){
                switch(i){
                    case 0:
                    break;
                    case 1:
                        recorrido[0]= actFORWARD;
                        movimiento_obligatorio=true;
                        n_movimientos=1;
                    break;
                    case 2:
                        // terminar movimiento obligatorio
                        if(VISTA_[1]=='B' or VISTA_[1]=='A' or VISTA_[1]=='P' or VISTA_[1]=='M' or (SURFACE_[1]>=101 && SURFACE_[1]<=121)){
                            recorrido[0]=actTURN_L;
                            movimiento_obligatorio=true;
                            n_movimientos=1;
                        }
                        else if(VISTA_[3]=='B' or VISTA_[3]=='A' or VISTA_[3]=='P' or VISTA_[3]=='M' or (SURFACE_[3]>=101 && SURFACE_[3]<=121)){
                            recorrido[0]= actFORWARD;
                            recorrido[1]= actTURN_L;
                            recorrido[2]=actTURN_R;
                            movimiento_obligatorio=true;
                            n_movimientos=3;
                        }
                        else{
                            recorrido[0]= actFORWARD;
                            recorrido[1]= actFORWARD;
                            recorrido[2]= actTURN_L;
                            recorrido[3]= actFORWARD;
                            movimiento_obligatorio=true;
                            n_movimientos=4;
                        }
                    break;
                    case 3:
                        if(VISTA_[1]=='B' or VISTA_[1]=='A' or VISTA_[1]=='P' or VISTA_[1]=='M' or (SURFACE_[1]>=101 && SURFACE_[1]<=121)){
                            if(VISTA_[4]=='B' or VISTA_[4]=='A' or VISTA_[4]=='P' or VISTA_[4]=='M' or (SURFACE_[4]>=101 && SURFACE_[4]<=121)){
                                if(VISTA_[2]=='B' or VISTA_[2]=='A' or VISTA_[2]=='P' or VISTA_[2]=='M' or (SURFACE_[2]>=101 && SURFACE_[2]<=121)){
                                    movimiento_obligatorio=false;
                                }
                                else{
                                    recorrido[0]=actTURN_L;
                                    recorrido[1]=actFORWARD;
                                    recorrido[2]=actTURN_R;
                                    movimiento_obligatorio=true;
                                    n_movimientos=3;
                                }

                            }
                            else{
                                recorrido[0]=actTURN_R;
                                recorrido[1]=actFORWARD;
                                recorrido[2]=actTURN_L;
                                movimiento_obligatorio=true;
                                n_movimientos=3;
                            }
                        }else{
                            recorrido[0]= actFORWARD;
                            recorrido[1]= actFORWARD;
                            movimiento_obligatorio=true;
                            n_movimientos=2;
                        }
                    break;
                    case 4:
                        recorrido[0]= actFORWARD;
                        recorrido[1]= actFORWARD;
                        recorrido[2]= actTURN_R;
                        recorrido[3]= actFORWARD;
                        movimiento_obligatorio=true;
                        n_movimientos=4;
                    break;
                    case 5:
                        if(VISTA_[6]=='B' or VISTA_[6]=='A' or VISTA_[6]=='P' or VISTA_[6]=='M'){
                            if(VISTA_[2]=='B' or VISTA_[2]=='A' or VISTA_[2]=='P' or VISTA_[2]=='M'){
                                break;
                            }
                        }
                        else{
                            recorrido[0]= actFORWARD;
                            recorrido[1]= actFORWARD;
                            recorrido[2]= actFORWARD;
                            recorrido[3]= actTURN_L;
                            recorrido[4]= actFORWARD;
                            recorrido[5]= actFORWARD;
                            movimiento_obligatorio=true;
                            n_movimientos=6;
                        }
                    break;
                    case 6:
                        recorrido[0]= actFORWARD;
                        recorrido[1]= actFORWARD;
                        recorrido[2]= actFORWARD;
                        recorrido[3]= actTURN_L;
                        recorrido[4]= actFORWARD;
                        movimiento_obligatorio=true;
                        n_movimientos=5;
                    break;
                    case 7:
                        recorrido[0]= actFORWARD;
                        recorrido[1]= actFORWARD;
                        recorrido[2]= actFORWARD;
                        movimiento_obligatorio=true;
                        n_movimientos=3;
                    break;
                    case 8:
                        if(VISTA_[3]=='B' or VISTA_[3]=='A' or VISTA_[3]=='P' or VISTA_[3]=='M' or (SURFACE_[3]>=101 && SURFACE_[3]<=121)){
                            recorrido[0]=actFORWARD;
                            recorrido[1]=actTURN_R;
                            recorrido[2]=actFORWARD;
                            recorrido[3]=actTURN_L;
                            recorrido[4]=actFORWARD;
                            recorrido[5]=actFORWARD;
                            movimiento_obligatorio=true;
                            n_movimientos=6;
                        }
                        else{
                            recorrido[0]= actFORWARD;
                            recorrido[1]= actFORWARD;
                            recorrido[2]= actFORWARD;
                            recorrido[3]= actTURN_R;
                            recorrido[4]= actFORWARD;
                            movimiento_obligatorio=true;
                            n_movimientos=5;
                        }
                    break;
                    case 9:
                        recorrido[0]= actFORWARD;
                        recorrido[1]= actFORWARD;
                        recorrido[2]= actFORWARD;
                        recorrido[3]= actTURN_R;
                        recorrido[4]= actFORWARD;
                        recorrido[5]= actFORWARD;
                        movimiento_obligatorio=true;
                        n_movimientos=6;
                    break;
                }
            }
        }
	}
}
// -----------------------------------------------------------
Agent::ActionType Agent::Think()
{
	Agent::ActionType accion = actFORWARD; // Por defecto avanza


	// moverse en busca de un PK
	if(!movimiento_obligatorio && n_gps<2){
        buscarPK();
	}
	if(movimiento_obligatorio){
            accion = recorrido[indice];
            indice++;
            n_movimientos--;
            if(n_movimientos==0){
                movimiento_obligatorio=false;
                indice=0;
            }
    }
    else if(guardar_mochila){
        //cout<< "Guardo el objeto encontrado en la mochila\n";
        accion = actPUSH;
        guardar_mochila=false;
    }
    else if((SURFACE_[0]>=48 && SURFACE_[0]<=57) && n_objetos<5){
        //cout<< "Cojo objeto del mundo\n";
        accion = actPICKUP;
        //tengo_objeto=true;
        guardar_mochila=true;
        n_objetos++;
    }
    else if(VISTA_[1]=='B' or VISTA_[1]=='A' or VISTA_[1]=='M' or VISTA_[1]=='D' or VISTA_[1]=='P' or (SURFACE_[1]>=101 && SURFACE_[1]<=113)){
        //cout << "Regla1: Evitar Chocar\n";
            //accion=actTURN_L;
            if(n_objetos>0 && (SURFACE_[1]>=101 && SURFACE_[1]<=113 && SURFACE_[1]!='q' && SURFACE_[1]!='e' && SURFACE_[1]!='f')){
                accion=darObjetos();
            }
            else{
                actualizarObstaculo();
                accion=calcularMovimiento();
            }
    }
    else {
        //cout<<"Regla pulgarcito"<<endl;
        accion=calcularMovimiento();

    }

    // recuerdo la ultima accion realizada
	last_accion_ = accion;
	return accion;
}
