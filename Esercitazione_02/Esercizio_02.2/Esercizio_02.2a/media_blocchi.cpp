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
 
void Reset(int pos[], double r){
   for(int i=0; i<3; i++){
      pos[i] = 0;
   }	
   r = 0; 	  
}	
 
void Blocchi(int L, int N, double mean[], double square[]){

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

   //PARTE 2: GENERO I RANDOM WALK
   int i,j,k,l; //Contatori.
   
   int dir; //Variabile 0, 1 o 2: direzione x, y o z, estratta random.
   int verso; //Variabile 0 o 1: verso avanti o indietro lungo l'asse scelto, estratta random.  
   int pos[3] = {0,0,0}; //Vettore con la posizione. All'inizio è nell'origine, poi sarà nel punto finale.
   
   double r=0; //E' la norma del vettore, la distanza tra l'origine e il punto finale. Variabie da integrare.     
   double sum; //Somma degli elementi in un blocco. Somma le r per fare la media a blocchi. 
	 
   for(i=0; i<N; i++){ //Per ogni blocco
      sum = 0; //Azzero la somma ad ogni ciclo.
	  
      for(j=0; j<L; j++){ //All'interno di ciascun blocco.
	     Reset(pos, r); //Ad ogni ciclo azzero la norma e le componenti del vettore, per generarne delle nuove.
	  
         for(k=0; k<100; k++){ //Genera il Random Walk.
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
			
         }  
		 
   //PARTE 3: VETTORI CON LE MEDIE DEI BLOCCHI E I RISPETTIVI QUADRATI	 	 
         for(l=0; l<3; l++){ //Calcolo la norma: è la variabile r da integrare.
            r += pos[l]*pos[l];	   
         }
		 
         r = sqrt(r);   		   
		 sum += r; //Somma per fare la media a blocchi.
      }	  
	  	  
      mean[i] = sum/double(L); //Questo e' il valor medio del blocco i-esimo. 
	  square[i] = mean[i]*mean[i];
	  //cout << i << "  " << mean[i] << endl; 
   }

   rnd.SaveSeed();   
}

void Media(int L, int M, int N, double mean[], double square[]){
 
   //PARTE 4: MEDIA A BLOCCHI; CALCOLO LA MEDIA DELLE MEDIE DEI VETTORI
   double sum_mean=0; //Somma degli elementi del vettore mean, ossia la somma delle medie.
   double sum_square=0; //Somma degli elementi del vettore square, ossia la somma dei quadrati delle medie. 

   double mean_mean; //Media finale di tutte le medie dei blocchi
   double mean_square; //Media finale dei quadrati delle medie dei blocchi
   
   double varianza; //Varianza sul valore finale della media.
   double stdev; //Deviazione standard sul valore finale della media.
   double incertezza; //Incertezza statistica sul valore finale della media.

   fstream f;
   f.open("posizioni.dat",ios::out);

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
   cout << "E' stato prodotto il file 'posizioni_reticolo.dat'." << endl; 
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
