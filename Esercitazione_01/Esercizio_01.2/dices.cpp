/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/

#include "dices.h"

using namespace std;
 
void Standard(int M){

   //Parte 1: set del seme del generatore.
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

   //Parte 2: generatore di numeri casuali con il dado standard.
   //Procedimento: somma dei valori casuali generati, ne faccio la media, ripeto per i 4 valori di N.
   double sum_std_1; 
   double mean_std_1; 
   double sum_std_2; 
   double mean_std_2; 
   double sum_std_10; 
   double mean_std_10; 
   double sum_std_100; 
   double mean_std_100; 
   
   fstream f;
   f.open("Standard_dice.dat",ios::out);

   for(int i=0; i<M; i++){ //Numero di realizzazioni, ossia i valori da inserire nell'istogramma: M=10000.
      //Ad ogni ciclo azzero i valori.
      sum_std_1 = 0; 
	  mean_std_1 = 0;
      sum_std_2 = 0; 
	  mean_std_2 = 0;
      sum_std_10 = 0; 
	  mean_std_10 = 0;
      sum_std_100 = 0; 
	  mean_std_100 = 0;
	  
      for(int j=0; j<1; j++){ //Sommo i valori generati casualmente in base al dado, per N=1.
	      sum_std_1 += rnd.Rannyu();
      }	  
      for(int j=0; j<2; j++){ //Sommo i valori generati casualmente in base al dado, per N=2.
	      sum_std_2 += rnd.Rannyu();
      }	  
      for(int j=0; j<10; j++){ //Sommo i valori generati casualmente in base al dado, per N=10.
	      sum_std_10 += rnd.Rannyu();
      }	  
      for(int j=0; j<100; j++){ //Sommo i valori generati casualmente in base al dado, per N=100.
	      sum_std_100 += rnd.Rannyu();
      }
	  
      //Calcolo le medie: i valori da inserire nell'istogramma. Divido le somme di prima per gli N corrispondenti. 
      mean_std_1 = sum_std_1/1.0;
      mean_std_2 = sum_std_2/2.0;
      mean_std_10 = sum_std_10/10.0;
      mean_std_100 = sum_std_100/100.0;
	  
	  //Questi sono i valori che compongono i 4 istogrammi. Metto i dati ottenuti su un file esterno.	  
      f << i << "  " << mean_std_1 << "  " << mean_std_2 << "  " << mean_std_10 << "  " << mean_std_100 << endl; 
 
   }
   //Chiudo il file e salvo il seme del generatore.
   f.close();  
   rnd.SaveSeed();
   //Comunico che è stato prodotto il file dei dati e che dati sono.  
   cout << "E' stato prodotto il file 'Standard_dice.dat'. Uso: " << endl;
   cout << "i, mean_std_1, mean_std_2, mean_std_10, mean_std_100." << endl; 
   
}

void Exponent(int M){

   //Parte 1: set del seme del generatore.
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

   //Parte 2: generatore di numeri casuali con il dado esponenziale.
   //Procedimento: somma dei valori casuali generati, ne faccio la media, ripeto per i 4 valori di N.
   double sum_exp_1; 
   double mean_exp_1; 
   double sum_exp_2; 
   double mean_exp_2; 
   double sum_exp_10; 
   double mean_exp_10; 
   double sum_exp_100; 
   double mean_exp_100; 
   
   fstream f;
   f.open("Exponential_dice.dat",ios::out);

   for(int i=0; i<M; i++){ //Numero di realizzazioni, ossia i valori da inserire nell'istogramma: M=10000.
      //Ad ogni ciclo azzero i valori.
      sum_exp_1 = 0; 
	  mean_exp_1 = 0;
      sum_exp_2 = 0; 
	  mean_exp_2 = 0;
      sum_exp_10 = 0; 
	  mean_exp_10 = 0;
      sum_exp_100 = 0; 
	  mean_exp_100 = 0;
	  
	  //Il testo dell'esercizio chiede di passare come parametro di Exponential: decay_rate = 1.0.
      for(int j=0; j<1; j++){ //Sommo i valori generati casualmente in base al dado, per N=1.
	      sum_exp_1 += rnd.Exponential(1.0);
      }	  
      for(int j=0; j<2; j++){ //Sommo i valori generati casualmente in base al dado, per N=2.
	      sum_exp_2 += rnd.Exponential(1.0);
      }	  
      for(int j=0; j<10; j++){ //Sommo i valori generati casualmente in base al dado, per N=10.
	      sum_exp_10 += rnd.Exponential(1.0);
      }	  
      for(int j=0; j<100; j++){ //Sommo i valori generati casualmente in base al dado, per N=100.
	      sum_exp_100 += rnd.Exponential(1.0);
      }
	  
      //Calcolo le medie: i valori da inserire nell'istogramma. Divido le somme di prima per gli N corrispondenti. 
      mean_exp_1 = sum_exp_1/1.0;
      mean_exp_2 = sum_exp_2/2.0;
      mean_exp_10 = sum_exp_10/10.0;
      mean_exp_100 = sum_exp_100/100.0;
	  
	  //Questi sono i valori che compongono i 4 istogrammi. Metto i dati ottenuti su un file esterno.	  
      f << i << "  " << mean_exp_1 << "  " << mean_exp_2 << "  " << mean_exp_10 << "  " << mean_exp_100 << endl; 
 
   }
   //Chiudo il file e salvo il seme del generatore.
   f.close();  
   rnd.SaveSeed();
   //Comunico che è stato prodotto il file dei dati e che dati sono.  
   cout << "E' stato prodotto il file 'Exponential_dice.dat'. Uso: " << endl;
   cout << "i, mean_exp_1, mean_exp_2, mean_exp_10, mean_exp_100." << endl; 

}

void Lorentzian(int M){

   //Parte 1: set del seme del generatore.
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

   //Parte 2: generatore di numeri casuali con il dado Lorentziano.
   //Procedimento: somma dei valori casuali generati, ne faccio la media, ripeto per i 4 valori di N.
   double sum_lor_1; 
   double mean_lor_1; 
   double sum_lor_2; 
   double mean_lor_2; 
   double sum_lor_10; 
   double mean_lor_10; 
   double sum_lor_100; 
   double mean_lor_100; 
   
   fstream f;
   f.open("Lorentian_dice.dat",ios::out);

   for(int i=0; i<M; i++){ //Numero di realizzazioni, ossia i valori da inserire nell'istogramma: M=10000.
      //Ad ogni ciclo azzero i valori.
      sum_lor_1 = 0; 
	  mean_lor_1 = 0;
      sum_lor_2 = 0; 
	  mean_lor_2 = 0;
      sum_lor_10 = 0; 
	  mean_lor_10 = 0;
      sum_lor_100 = 0; 
	  mean_lor_100 = 0;
	 
	  //Il testo dell'esercizio chiede di passare come parametri di Cauchy_Lorentz: mean = 0 e width = 1.0.	 
      for(int j=0; j<1; j++){ //Sommo i valori generati casualmente in base al dado, per N=1.
	      sum_lor_1 += rnd.Cauchy_Lorentz(0,1.0);
      }	  
      for(int j=0; j<2; j++){ //Sommo i valori generati casualmente in base al dado, per N=2.
	      sum_lor_2 += rnd.Cauchy_Lorentz(0,1.0);
      }	  
      for(int j=0; j<10; j++){ //Sommo i valori generati casualmente in base al dado, per N=10.
	      sum_lor_10 += rnd.Cauchy_Lorentz(0,1.0);
      }	  
      for(int j=0; j<100; j++){ //Sommo i valori generati casualmente in base al dado, per N=100.
	      sum_lor_100 += rnd.Cauchy_Lorentz(0,1.0);
      }
	  
      //Calcolo le medie: i valori da inserire nell'istogramma. Divido le somme di prima per gli N corrispondenti. 
      mean_lor_1 = sum_lor_1/1.0;
      mean_lor_2 = sum_lor_2/2.0;
      mean_lor_10 = sum_lor_10/10.0;
      mean_lor_100 = sum_lor_100/100.0;
	  
	  //Questi sono i valori che compongono i 4 istogrammi. Metto i dati ottenuti su un file esterno.	  
      f << i << "  " << mean_lor_1 << "  " << mean_lor_2 << "  " << mean_lor_10 << "  " << mean_lor_100 << endl; 
 
   }
   //Chiudo il file e salvo il seme del generatore.
   f.close();  
   rnd.SaveSeed();
   //Comunico che è stato prodotto il file dei dati e che dati sono.  
   cout << "E' stato prodotto il file 'Lorentian_dice.dat'. Uso: " << endl;
   cout << "i, mean_lor_1, mean_lor_2, mean_lor_10, mean_lor_100." << endl; 

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
