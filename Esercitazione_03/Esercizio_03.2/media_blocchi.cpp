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

void Reset(int N, double mean[], double square[]){
   for(int i=0; i<N; i++){
      mean[i] = 0;
	  square[i] = 0;
   }	   	
}

double Prezzo(double Z){ //Funzione che genera i prezzi S(t).
   double t=0.01; //Tempo finale.
   double rate=0.1; //Interesse.
   double vol=0.25; //Volatilità.
   
   double coef; //Lo calcolo una volta sola.    
   coef = rate-0.5*vol*vol;
   
   return exp((coef)*t+Z*vol*sqrt(t));     
}

double Call(double S){ //Calcola il guadagno finale per la Call.
   double K=100.0; //Prezzo a cui vorrei comprare.     
   double T=1.0; //Tempo finale.
   double rate=0.1; //Interesse.
   
   double coef; //Lo calcolo una volta sola.    
   coef = exp(-rate*T);

   if(S>K) return coef*(S-K);
   else return 0;
}

double Put(double S){ //Calcola il guadagno finale per la Put.
   double K=100.0; //Prezzo a cui vorrei comprare.     
   double T=1.0; //Tempo finale.
   double rate=0.1; //Interesse.
   
   double coef; //Lo calcolo una volta sola.    
   coef = exp(-rate*T);

   if(K>S) return coef*(K-S);
   else return 0;
}
 
void Blocchi_C(int L, int N, double mean[], double square[]){

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

   //PARTE 2: VETTORI CON LE MEDIE DEI BLOCCHI E I RISPETTIVI QUADRATI
   int i,j,l; //Contatori.
   double Z; //Variabile estratta in N(0,1).   
   double S; //Prezzo che vorrei calcolare.
   
   double C; //Guadagno per l'opzione Call. Variabile da integrare.
   double sum; //Somma degli elementi in un blocco. 

   for(i=0; i<N; i++){ //Per ogni blocco
      sum=0; //Azzero la somma ad ogni ciclo.
	  
      for(j=0; j<L; j++){ //All'interno di ciascun blocco.	  
         S = 100.0; //Ad ogni ciclo imposto il prezzo iniziale.
	                //Sarà modificato intervallo dopo intervallo, per 100 volte.
					
         for(l=0; l<100; l++){ //Divido l'intervallo in 100 intervallini.
		 
	        Z = rnd.Gauss(0,1); //Estraggo la variabile Z nell'intervallo i-esimo.
	        S = S*Prezzo(Z); //Calcolo il prezzo nell'intervallo i-esimo e assegno
                             //ogni volta il valore del prezzo dello step precedente.
							 
		    //Si sommano così tutti i prezzi e si ottiene il prezzo al tempo finale T=1.	
		 }		 
         C = Call(S); //Calcolo il guadagno Call.			 
         sum += C;	//Sommo i guadagni per fare la media a blocchi.	 		 
      }
	  
      mean[i] = sum/double(L); //Questo e' il valor medio del blocco i-esimo. 
	  square[i] = mean[i]*mean[i];
	  //cout << i << "  " << mean[i] << endl;  
   }
       
   rnd.SaveSeed();   
}

void Media_C(int L, int M, int N, double mean[], double square[]){
 
   //PARTE 3: MEDIA A BLOCCHI; CALCOLO LA MEDIA DELLE MEDIE DEI VETTORI
   double sum_mean=0; //Somma degli elementi del vettore mean, ossia la somma delle medie.
   double sum_square=0; //Somma degli elementi del vettore square, ossia la somma dei quadrati delle medie. 

   double mean_mean; //Media finale di tutte le medie dei blocchi
   double mean_square; //Media finale dei quadrati delle medie dei blocchi
   
   double varianza; //Varianza sul valore finale della media.
   double stdev; //Deviazione standard sul valore finale della media.
   double incertezza; //Incertezza statistica sul valore finale della media.

   fstream f;
   f.open("intervalli_call.dat",ios::out);

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
      f << M << "  " << mean_mean << "  " << incertezza << endl;	  
   }

   f.close();
   cout << "E' stato prodotto il file 'intervalli_call.dat'." << endl;
}

void Blocchi_P(int L, int N, double mean[], double square[]){

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

   //PARTE 2: VETTORI CON LE MEDIE DEI BLOCCHI E I RISPETTIVI QUADRATI
   int i,j,l; //Contatori.
   double Z; //Variabile estratta in N(0,1).   
   double S; //Prezzo che vorrei calcolare.
   
   double P; //Guadagno per l'opzione Put. Variabile da integrare.
   double sum; //Somma degli elementi in un blocco. 

   for(i=0; i<N; i++){ //Per ogni blocco
      sum=0; //Azzero la somma ad ogni ciclo.
	  
      for(j=0; j<L; j++){ //All'interno di ciascun blocco.	  
         S = 100.0; //Ad ogni ciclo imposto il prezzo iniziale.
	                //Sarà modificato intervallo dopo intervallo, per 100 volte.
					
         for(l=0; l<100; l++){ //Divido l'intervallo in 100 intervallini.
		 
	        Z = rnd.Gauss(0,1); //Estraggo la variabile Z nell'intervallo i-esimo.
	        S = S*Prezzo(Z); //Calcolo il prezzo nell'intervallo i-esimo e assegno
                             //ogni volta il valore del prezzo dello step precedente.
							 
		    //Si sommano così tutti i prezzi e si ottiene il prezzo al tempo finale T=1.	
		 }		 
         P = Put(S); //Calcolo il guadagno Put.			 
         sum += P;	//Sommo i guadagni per fare la media a blocchi.	 		 
      }
	  
      mean[i] = sum/double(L); //Questo e' il valor medio del blocco i-esimo. 
	  square[i] = mean[i]*mean[i];
	  //cout << i << "  " << mean[i] << endl;  
   }
       
   rnd.SaveSeed();   
}

void Media_P(int L, int M, int N, double mean[], double square[]){
 
   //PARTE 3: MEDIA A BLOCCHI; CALCOLO LA MEDIA DELLE MEDIE DEI VETTORI
   double sum_mean=0; //Somma degli elementi del vettore mean, ossia la somma delle medie.
   double sum_square=0; //Somma degli elementi del vettore square, ossia la somma dei quadrati delle medie. 

   double mean_mean; //Media finale di tutte le medie dei blocchi
   double mean_square; //Media finale dei quadrati delle medie dei blocchi
   
   double varianza; //Varianza sul valore finale della media.
   double stdev; //Deviazione standard sul valore finale della media.
   double incertezza; //Incertezza statistica sul valore finale della media.

   fstream f;
   f.open("intervalli_put.dat",ios::out);

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
      f << M << "  " << mean_mean << "  " << incertezza << endl;	  
   }

   f.close();
   cout << "E' stato prodotto il file 'intervalli_put.dat'." << endl;
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
