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

using namespace std;
 
int main (int argc, char *argv[]){

   Random rnd;
   int seed[4];
   int p1, p2;
   ifstream Primes("Primes");
   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
	  Primes >> p1 >> p2 ;
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();

   ifstream input("seed.in");
   string property;
   if (input.is_open()){
      while ( !input.eof() ){
         input >> property;
         if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else cerr << "PROBLEM: Unable to open seed.in" << endl;

   int i, j; //Contatori.
   int dir; //Variabile 0, 1 o 2: direzione x, y o z, estratta random.
   int verso; //Variabile 0 o 1: verso avanti o indietro lungo l'asse scelto, estratta random.
   
   int pos[3] = {0,0,0}; //Vettore con la posizione. All'inizio è nell'origine, poi sarà nel punto finale.
   double r; //E' la norma del vettore, la distanza tra l'origine e il punto finale. Variabie da integrare.
   
   fstream f;
   f.open("100_passi.dat",ios::out);
   	  
   for(i=0; i<100; i++){ //Genero 1 random walk di 100 passi.
      /*Estrae un numero decimale in [0,1), lo moltiplica per 3 e ne prende la parte intera: si ottiene così 0, 1 o 2.
	    Questa funzione estrae random un indice del vettore pos[3], ossia una delle tre direzioni.*/
      dir = int(3.0*rnd.Rannyu());  
		 
 	  /*Estrae un numero decimale in [0,1), lo moltiplica per 2 e ne prende la parte intera: si ottiene così o 0 o 1.
	    Questa funzione estrae random un verso sull'asse scelto prima: avanti o indietro.*/
      verso = int(2.0*rnd.Rannyu()); 
	  
	  if(verso == 1){ //Se hai estratto 1, fai un passo in avanti.
         pos[dir]++; //Incrementa di 1 il contentuto nella direzione estratta.
      }
	  else{ //Se hai estratto 0, fai un passo indietro.
	     pos[dir]--; //Dimunisce di 1 il contentuto nella direzione estratta.
	  }
	  
	  r = 0; //Azzero la norma ad ogni passo per calcolare quella nuova.  
      for(j=0; j<3; j++){ //Calcolo la norma ad ogni passo.
	     //cout << pos[j] << endl;
         r += pos[j]*pos[j];	   
      }
	  
      r = sqrt(r);
	  f << i << "  " << r << endl;    
   }
   
   f.close();
   cout << "E' stato prodotto il file '100_passi_reticolo.dat'." << endl; 
   
   rnd.SaveSeed();
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
