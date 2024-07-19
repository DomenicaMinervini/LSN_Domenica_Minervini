/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/

#include "media_blocchi.h"

using namespace std;
 
int main(int argc, char *argv[]){

   int E=20; //Numero di cicli per l'equilibrazione.
   int L=10000; //L = M/N, numero di elementi per ogni blocco.   
   int M=0; //Numero totale di simulazioni di random walk; in tutto sara' 1000000
   int N=120; //Numero di blocchi = numero di elementi del vettore.

   double mean[N]; //Vettore contenente le medie degli elementi di ogni blocco.
   double square[N]; //Vettore contenente i quadrati delle medie degli elementi di ogni blocco.

   //Svolgo l'esercizio per lo stato fondamentale.
   Reset(N, mean, square);   
   Blocchi_1s(E, L, N, mean, square);
   Media_1s(L, M, N, mean, square);
      
   //Svolgo l'esercizio per il primo stato eccitato.
   Reset(N, mean, square);   
   Blocchi_2p(E, L, N, mean, square);
   Media_2p(L, M, N, mean, square);

   return 0;
   
}

/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/
