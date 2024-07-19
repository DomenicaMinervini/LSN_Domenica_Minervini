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

void Blocchi(int L, int N, double mean[], double square[]){
	
   //PARTE 1a: GENERATORE DI NUMERI CASUALI
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
   
   
   //PARTE 1b: GENERO LE SBARRETTE E STIMA DI PI GRECO
   int d = 5; //Spessore della striscia di pavimento.
   int l = 4; //Lunghezza dell'ago.
   int n_thr = 100000; //Numero dei lanci.  
   int n_hit; //Numero di aghi che toccano la linea.
   
   double a; //Posizione della cruna relativamente all'inizio della striscia.
   double x; //Coordinata lungo l'asse x della punta dell'ago.
   double y; //Coordinata lungo l'asse y della punta dell'ago.
   double theta; //Angolo sotteso dalla punta dell'ago (x,y).
   
   double b; //Somma della posizione della cruna e della punta.
   double pi = 0; //Pi greco.  
   
   
   //PARTE 2: VETTORI CON LE MEDIE DEI BLOCCHI E I RISPETTIVI QUADRATI
   int i,j,k; //Contatori.
   double r; //Variabile da integrare.
   double sum; //Somma degli elementi in un blocco. 

   /*Passo questi parametri dalla funzione main: 
     int L=100; // L = M/N, numero di elementi per ogni blocco. 
     int M; //Numero totale di lanci: alla fine M = 100000.   
     int N=10; //Numero di blocchi.
	 
     double mean[N]; //Vettore contenente le medie degli elementi di ogni blocco.
     double square[N]; //Vettore contenente i quadrati delle medie degli elementi di ogni blocco.*/

   for(i=0; i<N; i++){ //Per ogni blocco
      sum=0; //Azzero la somma ad ogni ciclo.
	  
      for(j=0; j<L; j++){ //All'interno di ciascun blocco. 
	     n_hit = 0; //Azzero il numero di aghi sulla linea ad ogni ciclo.
	     
         for(k=0; k<n_thr; k++){ //Ogni elemento del blocco è un pavimento cosparso di n_thr aghi!
            //Funzione che calcola pi greco secondo il medoto di Buffon. Riempie il pavimento di n_thr aghi.             		 
	        a = d*rnd.Rannyu(); //Genera la posizione della cruna rispetto al bordo sinistro della striscia.
			
			do{ //Genera le coordinate (x,y) della punta dell'ago. 
	           x = rnd.Rannyu(); //Continua ad estrarre una coppia (x,y)...
		       y = rnd.Rannyu(); //...finché non è soddifratta la condizione. 
			} while(x*x+y*y>1.0); //Tiene solo i casi all'interno della circonferenza.

			theta = acos(x/sqrt(x*x+y*y)); //Angolo formato dalla punta dell'angolo: serve sin(theta),
			                               //la sua proiezione sul segmento ortogonale alla striscia.
	        b = a + l*sin(theta); //Distanza totale della punta dell'ago dal bordo sinistro della striscia
	  
	        if(b > d){ //Se la punta va nella striscia successiva, ossia se tocca la linea,
	           n_hit++; //incrementa di 1 i casi favorevoli.	  
	        }			
         }			 
         pi = 2.0*l*n_thr/(d*n_hit); //Calcola il valore di pi greco per questo pavimento.	
		 
		 //Media a blocchi: funzione da integrare e somma per fare la media.
	     r = pi;
         sum += r;
      }
	  
      mean[i] = sum/double(L); //Questo e' il valor medio del blocco i-esimo. 
	  square[i] = mean[i]*mean[i];
      cout << "Ho calcolato il blocco " << i << endl; 
   }
   
    rnd.SaveSeed(); 
}

void Media(int L, int M, int N, double mean[], double square[]){
 
   //PARTE 3: MEDIA A BLOCCHI; CALCOLO LA MEDIA DELLE MEDIE DEI VETTORI
   double sum_mean=0; //Somma degli elementi del vettore mean, ossia la somma delle medie.
   double sum_square=0; //Somma degli elementi del vettore square, ossia la somma dei quadrati delle medie. 

   double mean_mean; //Media finale di tutte le medie dei blocchi
   double mean_square; //Media finale dei quadrati delle medie dei blocchi
   
   double varianza; //Varianza sul valore finale della media.
   double stdev; //Deviazione standard sul valore finale della media.
   double incertezza; //Incertezza statistica sul valore finale della media.

   fstream f;
   f.open("pi_medio.dat",ios::out);

   for(int k=0;k<N;k++){ //Svolgo l'operazione per 1 blocco, 2 blocchi, ... , N blocchi.
      sum_mean += mean[k]; //Somma degli elementi del vettore medie, dal valore 0 al valore k.
      sum_square += square[k]; //Somma degli elementi del vettore dei quadrati, dal valore 0 al valore k. 

      mean_mean = sum_mean/double(k+1);  //Calcolo la media dei valori medi di ogni blocco aggiungendo ogni volta un blocco.
      mean_square = sum_square/double(k+1); //Faccio lo stesso per i quadrati delle medie. 
	                                     //Divido per k+1 perche' k parte da 0. Cosi' divido per il numero vero di elementi.
	  
      if(k == 0){ 
         incertezza = 0; //Nel caso in cui si abbia un solo blocco, l'incertezza statistica è posta a 0 e gli altri parametri non servono.
      }
      else{
         varianza = mean_square - mean_mean*mean_mean; //Varianza per la media di 1 blocco, 2 blocchi, ... , N blocchi.
         stdev = sqrt(varianza); //Lo stesso per la deviazione standard.
         incertezza = stdev/(sqrt(k)); //Incertezza stastistica del valor medio, se sono stati generati k blocchi.
          /*Nella formula dell'incertezza statistica si divide per un fattore radice di N-1. Dato che il primo elemento del vettore
            è il numero zero, N è già ridotto di 1. Quindi nella formula divido per la radice di N, ossia per K blocchi.*/
      }	  

      M = (k+1)*L; //Numero di lanci a questo punto = numero blocchi *  numero di elementi in un blocco	 
      //cout << "Ho generato " << M << " elementi, con media " << mean_mean << " e incertezza " << incertezza << endl;
      f << M << "  " << mean_mean << "  " << incertezza << endl;	  
   }

   f.close();
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
