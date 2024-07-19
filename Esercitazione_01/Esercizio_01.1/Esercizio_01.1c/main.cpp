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

   int n=10000; //Numero di estrazioni per ogni blocco.   
   int M=100; //Numero di blocchi = numero di elementi del vettore. 

   double ni[M]; //Vettore contenente il numero di estrazioni favorevoli.
   double chi[M]; //Vettore contenente i valori di chi quadro di ogni blocco.
      
   Blocchi(n, M, ni);
   Media(n, M, ni, chi);
   
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
