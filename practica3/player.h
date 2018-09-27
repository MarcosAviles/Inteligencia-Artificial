#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"

class Player{
    public:
      Player(int jug);
      Environment::ActionType Think();
      void Perceive(const Environment &env);

    private:
      int jugador_;
      Environment actual_;
      double Poda_AlfaBeta(const Environment &T, int jugador, int profundidad, int profundidad_alfabeta, Environment::ActionType &accion, double alpha, double beta);
};
#endif
