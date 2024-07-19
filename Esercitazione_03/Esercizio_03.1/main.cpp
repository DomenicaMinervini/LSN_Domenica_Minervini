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

   int L=1000; //L = M/N, numero di elementi per ogni blocco.   
   int M=0; //Numero totale di simulazioni di prezzi; in tutto sara' 100000
   int N=100; //Numero di blocchi = numero di elementi del vettore.

   double mean[N]; //Vettore contenente le medie degli elementi di ogni blocco.
   double square[N]; //Vettore contenente i quadrati delle medie degli elementi di ogni blocco.

   //Calcolo il guadagno per l'opzione Call.   
   Reset(N, mean, square);
   Blocchi_C(L, N, mean, square);
   Media_C(L, M, N, mean, square);
   
   //Calcolo il guadagno per l'opzione Put.   
   Reset(N, mean, square);
   Blocchi_P(L, N, mean, square);
   Media_P(L, M, N, mean, square);
      
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
