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
 
void Blocchi_1s(int E, int L, int N, double mean[], double square[]){

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


   //PARTE 2: EQUILIBRAZIONE
   /*Consiste nel fare all'inizio alcuni cicli di algoritmo di Metropolis senza considerarli nella media a blocchi.
   Ho scritto la funzione di equilibrazione nella funzione che produce i blocchi e non come una funzione a parte 
   perchè il codice lavora con variabili locali e non globali. Di conseguenza, se richiamo il generatore di numeri 
   casuali due volte in due ambienti separati, ricomincia a generare i valori dall'inizio e non da dove è arrivato 
   grazie al lavoro di selezione svolto dall'algoritmo di Metropolis: sarebbe come non fare l'equilibrazione.*/
   
   int i,j,l; //Contatori.
   
   double delta_x, delta_y, delta_z; //spostamento infinitesimo dalla posizione attuale
   double x_old, y_old, z_old; //coodinate in cui mi trovo adesso, attuali
   double x_new, y_new, z_new; //coordinate successive

   double r_old, r_new; //posizione attuale e successiva; r_old è la variabile da integrare
   double rapp, alpha, value; //rapporto probabilita', probabilita' scelta, valore tra 0 e 1
   double sum; //somma degli elementi in un blocco
   
   //posizione iniziale
   x_old=0.866;
   y_old=0.866;
   z_old=0.866;

   cout << "Coordinate iniziali 1s:  " << x_old << "  " << y_old << "  " << z_old << endl;
   
   for(l=0;l<E;l++){ //equilibrazione per E cicli di algoritmo di Metropolis
      //mi sposto in un intorno tra la mia posizione attuale e il range:
      delta_x=rnd.Gauss(0,1);
      delta_y=rnd.Gauss(0,1);
      delta_z=rnd.Gauss(0,1);

      //assegno le nuove coodinate
      x_new=x_old+delta_x;
      y_new=y_old+delta_y;
      z_new=z_old+delta_z;

      //calcolo i raggi: sono in tre dimensioni
      r_old=radius(x_old,y_old,z_old);
      r_new=radius(x_new,y_new,z_new); 

      //calcolo il rapporto tra la probabilita nuova e la vecchia
      rapp=rapporto_probabilita_ground_state(r_old,r_new);

      //scelgo il minimo tra 1 e il rapporto tra le probabilita'
      alpha=minimum(rapp);

      //decido se accettare o meno la nuova mossa
      value=rnd.Rannyu();

      //se accetto la mossa la posizione nuova diventa quella attuale
      if(value<=alpha){ 
         x_old=x_new;
         y_old=y_new;
         z_old=z_new; 
      }		 
	  
   }	

   cout << "Ho eseguito l'equilbrazione per lo stato 1s." << endl;
   cout << "Coordinate 1s dopo l'equilibrazione:  " << x_old << "  " << y_old << "  " << z_old << endl;
   //La simulazione comincia dalle coordinate con cui finisce l'equilibrazione.   
    
   //PARTE 3: ALGORITMO DI METROPOLIS; SIMULAZIONE VERA E PROPRIA 
   fstream g;
   g.open("coordinate_1s_nor.dat",ios::out);
   
   for(i=0; i<N; i++){ //per ogni blocco
      sum=0; //azzero la somma ad ogni ciclo
	  
      for(j=0; j<L; j++){ //all'interno di ciascun blocco
	  	  
         //mi sposto in un intorno tra la mia posizione attuale e il range:
         //e' il raggio che da come probabilita' di accettazione circa 0.5
         delta_x=rnd.Gauss(0,1);
         delta_y=rnd.Gauss(0,1);
         delta_z=rnd.Gauss(0,1);

         //assegno le nuove coodinate
         x_new=x_old+delta_x;
         y_new=y_old+delta_y;
         z_new=z_old+delta_z;

         //calcolo i raggi
         r_old=radius(x_old,y_old,z_old);
         r_new=radius(x_new,y_new,z_new);

         //calcolo il rapporto tra la probabilita nuova e la vecchia
         rapp=rapporto_probabilita_ground_state(r_old,r_new);

         //scelgo il minimo tra 1 e il rapporto tra le probabilita': in questo modo 
         //ho accettato con probabilita' alpha la mossa da r_old a r_new
         alpha=minimum(rapp);

         //APPLICO L'ALGORITMO DI METROPOLIS
         //estraggo un valore in una distribuzione uniforme tra 0 e 1
         //se alpha e' maggiore del valore estratto, accetto la mossa nuova
         //altrimenti tengo la posizione in cui mi trovo
         value=rnd.Rannyu();

         if(value<=alpha){ //accetto la mossa: la posizione nuova diventa quella attuale
            x_old=x_new;
            y_old=y_new;
            z_old=z_new;  
			
			   if(i == N-1){ 			   
			      //metto le coordinate dei raggi calcolati nell'ultimo ciclo: faro' un grafico
				  //solo se accetto la mossa, altrimenti metterei nel plot lo stesso punto più volte
                  g << x_old << "  " << y_old << "  " << z_old << endl;
               } 
         }			 
         sum += r_old;
      }

   //PARTE 4: VETTORI CON LE MEDIE DEI BLOCCHI E I RISPETTIVI QUADRATI	  
      mean[i] = sum/double(L); //Questo e' il valor medio del blocco i-esimo. 
	  square[i] = mean[i]*mean[i];	  
   }
   
   g.close();
   cout << "E' stato prodotto il file 'coordinate_1s_nor.dat'." << endl;
   rnd.SaveSeed();   
}

void Media_1s(int L, int M, int N, double mean[], double square[]){
 
   //PARTE 5: MEDIA A BLOCCHI; CALCOLO LA MEDIA DELLE MEDIE DEI VETTORI
   double sum_mean=0; //Somma degli elementi del vettore mean, ossia la somma delle medie.
   double sum_square=0; //Somma degli elementi del vettore square, ossia la somma dei quadrati delle medie. 

   double mean_mean; //Media finale di tutte le medie dei blocchi
   double mean_square; //Media finale dei quadrati delle medie dei blocchi
   
   double varianza; //Varianza sul valore finale della media.
   double stdev; //Deviazione standard sul valore finale della media.
   double incertezza; //Incertezza statistica sul valore finale della media.

   fstream f;
   f.open("raggi_1s_nor.dat",ios::out);

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

      //M = (k+1)*L; //Numero di lanci a questo punto = numero blocchi *  numero di elementi in un blocco	 
      //cout << "Ho generato " << M << " elementi, con media " << mean_mean << " e incertezza " << incertezza << endl;
      f << k+1 << "  " << mean_mean << "  " << incertezza << endl;	  
   }

   f.close();
   cout << "E' stato prodotto il file 'raggi_1s_nor.dat'." << endl;
   cout << "---------------------------------------------" << endl;
  
}


void Blocchi_2p(int E, int L, int N, double mean[], double square[]){

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


   //PARTE 2: EQUILIBRAZIONE

   int i,j,l; //Contatori.

   double delta_x, delta_y, delta_z; //spostamento infinitesimo dalla posizione attuale
   double x_old, y_old, z_old; //coodinate in cui mi trovo adesso, attuali
   double x_new, y_new, z_new; //coordinate successive

   double r_old, r_new; //posizione attuale e successiva; r_old è la variabile da integrare
   double rapp, alpha, value; //rapporto probabilita', probabilita' scelta, valore tra 0 e 1
   double sum; //somma degli elementi in un blocco
   
   //posizione iniziale
   x_old=2.887;
   y_old=2.887;
   z_old=2.887;

   cout << "Coordinate iniziali 2p:  " << x_old << "  " << y_old << "  " << z_old << endl;
   
   for(l=0;l<E;l++){ //equilibrazione per E cicli di algoritmo di Metropolis
      //mi sposto in un intorno tra la mia posizione attuale e il range:
      delta_x=rnd.Gauss(0,1);
      delta_y=rnd.Gauss(0,1);
      delta_z=rnd.Gauss(0,1);

      //assegno le nuove coodinate
      x_new=x_old+delta_x;
      y_new=y_old+delta_y;
      z_new=z_old+delta_z;

      //calcolo i raggi: sono in tre dimensioni
      r_old=radius(x_old,y_old,z_old);
      r_new=radius(x_new,y_new,z_new); 

      //calcolo il rapporto tra la probabilita nuova e la vecchia
      rapp=rapporto_probabilita_ground_state(r_old,r_new);

      //scelgo il minimo tra 1 e il rapporto tra le probabilita'
      alpha=minimum(rapp);

      //decido se accettare o meno la nuova mossa
      value=rnd.Rannyu();

      //se accetto la mossa la posizione nuova diventa quella attuale
      if(value<=alpha){ 
         x_old=x_new;
         y_old=y_new;
         z_old=z_new; 
      }		 
	  
   }	

   cout << "Ho eseguito l'equilbrazione per lo stato 2p." << endl;
   cout << "Coordinate 2p dopo l'equilibrazione:  " << x_old << "  " << y_old << "  " << z_old << endl;
   //La simulazione comincia dalle coordinate con cui finisce l'equilibrazione.   

   //PARTE 3: ALGORITMO DI METROPOLIS; SIMULAZIONE VERA E PROPRIA
   fstream g;
   g.open("coordinate_2p_nor.dat",ios::out);
   
   for(i=0; i<N; i++){ //per ogni blocco
      sum=0; //azzero la somma ad ogni ciclo
	  
      for(j=0; j<L; j++){ //all'interno di ciascun blocco
	  	  
         //mi sposto in un intorno tra la mia posizione attuale e il range:
         //e' il raggio che da come probabilita' di accettazione circa 0.5
         delta_x=rnd.Gauss(0,1);
         delta_y=rnd.Gauss(0,1);
         delta_z=rnd.Gauss(0,1);

         //assegno le nuove coodinate
         x_new=x_old+delta_x;
         y_new=y_old+delta_y;
         z_new=z_old+delta_z;

         //calcolo i raggi: sono in tre dimensioni
         r_old=radius(x_old,y_old,z_old);
         r_new=radius(x_new,y_new,z_new);

         //calcolo il rapporto tra la probabilita nuova e la vecchia
         rapp=rapporto_probabilita_stato_eccitato(r_old, r_new, z_old, z_new);

         //scelgo il minimo tra 1 e il rapporto tra le probabilita': in questo modo 
         //ho accettato con probabilita' alpha la mossa da r_old a r_new
         alpha=minimum(rapp);

         //APPLICO L'ALGORITMO DI METROPOLIS
         //estraggo un valore in una distribuzione uniforme tra 0 e 1
         //se alpha e' maggiore del valore estratto, accetto la mossa nuova
         //altrimenti tengo la posizione in cui mi trovo
         value=rnd.Rannyu();

         if(value<=alpha){ //accetto la mossa: la posizione nuova diventa quella attuale
            x_old=x_new;
            y_old=y_new;
            z_old=z_new;  
			
			   if(i == N-1){ 			   
			      //metto le coordinate dei raggi calcolati nell'ultimo ciclo: faro' un grafico
				  //solo se accetto la mossa, altrimenti metterei nel plot lo stesso punto più volte
                  g << x_old << "  " << y_old << "  " << z_old << endl;
               } 
         }			 
         sum += r_old;
      }

   //PARTE 4: VETTORI CON LE MEDIE DEI BLOCCHI E I RISPETTIVI QUADRATI	  
      mean[i] = sum/double(L); //Questo e' il valor medio del blocco i-esimo. 
	  square[i] = mean[i]*mean[i];	  
   }
   
   g.close();
   cout << "E' stato prodotto il file 'coordinate_2p_nor.dat'." << endl;
   rnd.SaveSeed();   
}

void Media_2p(int L, int M, int N, double mean[], double square[]){
 
   //PARTE 5: MEDIA A BLOCCHI; CALCOLO LA MEDIA DELLE MEDIE DEI VETTORI
   double sum_mean=0; //Somma degli elementi del vettore mean, ossia la somma delle medie.
   double sum_square=0; //Somma degli elementi del vettore square, ossia la somma dei quadrati delle medie. 

   double mean_mean; //Media finale di tutte le medie dei blocchi
   double mean_square; //Media finale dei quadrati delle medie dei blocchi
   
   double varianza; //Varianza sul valore finale della media.
   double stdev; //Deviazione standard sul valore finale della media.
   double incertezza; //Incertezza statistica sul valore finale della media.

   fstream f;
   f.open("raggi_2p_nor.dat",ios::out);

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

      //M = (k+1)*L; //Numero di lanci a questo punto = numero blocchi *  numero di elementi in un blocco	 
      //cout << "Ho generato " << M << " elementi, con media " << mean_mean << " e incertezza " << incertezza << endl;
      f << k+1 << "  " << mean_mean << "  " << incertezza << endl;	  
   }

   f.close();
   cout << "E' stato prodotto il file 'raggi_2p_nor.dat'." << endl;
   cout << "---------------------------------------------" << endl;
   
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
