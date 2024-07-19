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

   /*1.Genero n_thr aghi con la funzione pi();
     2.Calcolo L volte pi greco in un blocco;
	 3.Genero N blocchi di L elementi.*/

   int L=1000; //L = M/N, numero di elementi per ogni blocco:
               //genero 1000 valori di pi greco in ogni blocco.   
   int M=0;   //Numero totale di lanci; in tutto sara' 100000
   int N=100; //Numero di blocchi = numero di elementi del vettore:
              //numero di volte che ripeto l'esperimento. 100

   double mean[N]; //Vettore contenente le medie degli elementi di ogni blocco.
   double square[N]; //Vettore contenente i quadrati delle medie degli elementi di ogni blocco.
   
   Blocchi(L, N, mean, square);
   Media(L, M, N, mean, square);
   
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
