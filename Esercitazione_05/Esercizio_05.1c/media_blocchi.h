/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/

#include <string>
#include "random.h"
#include "funzioni.h"

//Funzione che alloca i vettori con le medie e i rispettivi quadrati per lo stato fondamentale.
void Blocchi_1s(int, int, int, double[], double[]);

//Funzione che calcola la media delle medie dei vettori per lo stato fondamentale.
void Media_1s(int, int, int, double[], double[]);

//Funzione che alloca i vettori con le medie e i rispettivi quadrati per il primo stato eccitato.
void Blocchi_2p(int, int, int, double[], double[]);

//Funzione che calcola la media delle medie dei vettori per il primo stato eccitato.
void Media_2p(int, int, int, double[], double[]);

/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/
