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

double Cosinus(double x){
   //double PI = acos(-1.);
   double PIM = 0.5*acos(-1.);
   return PIM*cos(PIM*x)/(2.0*(1.0-x));	
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

   //PARTE 2: VETTORI CON LE MEDIE DEI BLOCCHI E I RISPETTIVI QUADRATI
   double r; //Variabile da integrare.
   double sum; //Somma degli elementi in un blocco. 
   
   int gen; //Numero di valori generati da cui ne seleziono L.
   int hit; //Numero di valori accettati: ne servono L per ogni blocco.
   double perc; //Percentuale di valori accettati.
   
   double x,y,l; //Valori random x e y e delimitatore l.
   
   fstream g;   
   g.open("accettati.dat",ios::out);

   /*Passo questi parametri dalla funzione main: 
     int L=100; // L = M/N, numero di elementi per ogni blocco. 
     int M; //Numero totale di lanci: alla fine M = 100000.   
     int N=10; //Numero di blocchi.
	 
     double mean[N]; //Vettore contenente le medie degli elementi di ogni blocco.
     double square[N]; //Vettore contenente i quadrati delle medie degli elementi di ogni blocco.*/

   for(int i=0; i<N; i++){ //Per ogni blocco
      //ad ogni ciclo di blocco azzero i valori di: somme, valori generati, valori accettati, percentuale.
      sum=0;
      gen=0;
      hit=0;
      perc=0;

      /*Nel blocco di codice successivo, accetto solo quei valori tra [0,1] che soddisfano ad un requisito.
      In questo caso, vado avanti a generare valori finché il codice non ne abbia accettati L=1000.*/

      while(hit<L){ //Finché non hai generato L valori buoni genera coppie x,y.
            x = rnd.Rannyu();
            y = 2.0*rnd.Rannyu();
            l = 2.0*(1.0-x);

            if(y<l){ //Svolgo l'operazione solo per le x idonee, all'interno di ciascun blocco.
                  r = Cosinus(x); //Funzione da integrare del valore generato idoneo.
                  sum += r; //Somma di tutti i valori generati.
                  hit++; //Incrementa i casi favorevoli.
            }

            gen++; //Incrementa il numero di coppie generate.
      }

      perc = 100.0*double(hit)/double(gen); //Calcolo la percentuale di coppie accettate
      g << i+1 << "  " << perc << endl; //e le metto su un file esterno.
	  
      mean[i] = sum/double(L); //Questo e' il valor medio del blocco i-esimo. 
	  square[i] = mean[i]*mean[i];
   }
   
   g.close();   
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
   f.open("cosinus_two.dat",ios::out);

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
