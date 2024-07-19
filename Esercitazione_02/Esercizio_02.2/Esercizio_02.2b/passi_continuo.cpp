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

   /*for(int i=0; i<20; i++){
      cout << rnd.Phi() << "  " << rnd.Theta() << endl;
   }*/
  
   int i, j; //Contatori.  
   double phi, theta; //Angolo solido.
   double pos[3] = {0.0,0.0,0.0}; //Vettore con la posizione. All'inizio è nell'origine, poi sarà nel punto finale.
   double r; //E' la norma del vettore, la distanza tra l'origine e il punto finale. Variabie da integrare.

   fstream f;
   f.open("100_passi_continuo.dat",ios::out);  
   
   for(i=0; i<100; i++){ //Genero 1 random walk di 100 passi.
      //Estraggo l'angolo solido, che identifica direzione e verso dello spostamento.
	  phi = rnd.Phi();
	  theta = rnd.Theta();
	  
      //Incrementa le tre coordinate cartesiane ad ogni passo. Dovrei moltiplicare tutte e tre per a = 1.
      pos[0] += sin(theta)*cos(phi);
      pos[1] += sin(theta)*sin(phi);
      pos[2] += cos(theta);

	  r = 0; //Azzero la norma ad ogni passo per calcolare quella nuova.  
      for(j=0; j<3; j++){ //Calcolo la norma ad ogni passo.
	     //cout << pos[j] << endl;
         r += pos[j]*pos[j];	   
      }  
	  
	  r = sqrt(r);
	  f << i << "  " << r << endl;    
   }
   
   f.close();
   cout << "E' stato prodotto il file '100_passi_continuo.dat'." << endl; 

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
