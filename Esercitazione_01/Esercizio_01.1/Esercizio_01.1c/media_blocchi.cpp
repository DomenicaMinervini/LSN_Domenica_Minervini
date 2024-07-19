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

double Value(double x){
   return x;
}
 
void Blocchi(int n, int M, double ni[]){

   //PARTE 1: GENERATORE DI NUMERI CASUALI
   Random rnd;
   int seed[4];
   int p1, p2;
   ifstream Primes("Primes");
   if (Primes.is_open()){
      Primes >> p1 >> p2;
	  Primes >> p1 >> p2;
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

   //PARTE 2: ALLOCO IL VETTORE CON I VALORI DI CHI QUADRO 
   int i,j; //Contatori.
   double r; //Variabile da inserire nel bin se soddisfa la condizione richiesta.
   double min, max; //Estremi dell'intervallo da cui estraggo gli n valori ad ogni ciclo.
     
   for(i=0; i<M; i++){ //Divido l'intervallo [0,1] in 100 sottointervalli identici.
      ni[i] = 0; //All'inizio del ciclo il bin e' vuoto.
	  min = double(i)/double(M); //Estremo inferiore dell'intervallo in questo ciclo.
	  max = double(i+1)/double(M); //Estremo superiore dell'intervallo in questo ciclo.	
	  
      for(j=0; j<n; j++){ //All'interno di ciascun esperimento genera 10000 valori.
	     r = Value(rnd.Rannyu()); //Divido l'intervallo [0,1] in M intervalli identici.
		 if(r>=min and r<=max){ //Se r si trova nell'intervallino, 
		    ni[i] = ni[i] + 1; //allora aumenta il bin dei casi favorevoli di uno.
	     }		 		 
      }	  	   
   }

   rnd.SaveSeed();     
}

void Media(int n, int M, double ni[], double chi[]){  
 
   //PARTE 3: CALCOLO CHI QUADRO
   int k; //Contatore.
   double sum=0; //Somma degli elementi in un blocco: chi quadro totale.  
   double E; //Valore di aspettazione.
   
   E = double(n)/double(M);
   
   fstream f; 
   f.open("chi_quadro.dat",ios::out);
  
   for(k=0; k<M; k++){ //A partire dagli hit (n_i), calcolo i chi quadro parziali.
      chi[k] = (ni[k]-E)*(ni[k]-E)/E; //E' il vettore con i valori dei 100 chi quadro parziali.
	  sum += chi[k]; //Sommo i chi quadro parziali e ottengo chi quadro. 
	  f << k+1 << "   " << chi[k] << endl;
	  //cout << k+1 << "   " << ni[k] << "   " << chi[k] << endl;
   } 
   
   f.close();
   cout << "Il valore finale di chi quadro e': " << sum << endl;    
   
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
