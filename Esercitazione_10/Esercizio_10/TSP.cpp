/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/

#include "TSP.h"
#include "mpi.h"

using namespace std;
 
int main (int argc, char *argv[]){
   //Il codice funziona per un numero di elementi della popolazione NUM almeno = 200!
   //Togliere il commento al valori relativi al problema desiderato nel file TSP.h.
   //Togliere il commento ai file di input relativi al problema desiderato nel file TSP.cpp.

   //------DICHIARAZIONE DELLE VARIABILI CHE PARTECIPANO AL PROCESSO IN PARALLELO.
   //Dichiarare chi sono gli elementi che parteciperanno al processo prima di iniziare. 
   int size; //Numero di nodi che prendono parte al processo.
   int rank; //Che nodo sono io?

   const int root = 0; //Nodo radice, funzioni:
   //1. comunica le estrazioni dei nodi sender e receiver a tutti gli altri;
   //2. comunica i risultati della sua ricerca indipendente in parallelo.
 
   int sender_rank; //Numero estratto a sorte. E' il nodo che invia e riceve la comunicazione.
   int receiver_rank; //Numero estratto a sorte. E' il nodo che riceve e invia la comunicazione.

   //------INIZIALIZZAZIONE DEL PROCESSO IN PARALLELO.
   MPI_Init(&argc, &argv); //Inizializzo con le referenze ai parametri passati da terminale.
   
   //MPI_COMM_WORLD è il gruppo dei nodi che partecipano al lavoro in parallelo. 
   
   MPI_Comm_size(MPI_COMM_WORLD, &size); //Chiamata comune per dire quanti nodi partecipano al lavoro in parallelo.   
   MPI_Comm_rank(MPI_COMM_WORLD, &rank); //Chiamata comune per dire che nodo sono io nel lavoro in parallelo.   

   MPI_Status stat; //Variabile che dice lo stato.
   //cout << "Sono il nodo " << rank << " dei " << size << " che hai utilizzato." << endl;
     
   //------GENERO I PUNTI RANDOM CHE DESCRIVONO LE CITTA'.  
   Input(rank); //Funzione che seleziona i numeri random. 
   
   //------CARICO LE COORDINATE DEL PROBLEMA DA STUDIARE DA FILE ESTERNO.
   int i, j, k, l; //Contatori.
   fstream file_label, file_coord; //File esterni di dati del problema.

   //------SCEGLI IL PROBLEMA CHE VUOI RISOLVERE. 

   //Comunica i dati del problema selezionato solo il nodo radice.
   if(rank==root){   
      cout << "Numero di citta' nel percorso = " << dim << endl;  	  
      cout << "Numero di elementi della popolazione = " << num << endl;  
      cout << "Numero di generazioni  = " << generations << endl;
      cout << "Numero di generazioni ogni cui scambiare il best = " << n_migr << endl;	  
   }

   //Conosco già il numero delle città, degli elementi della popolazione e delle generazioni.
   //Apro i file di dati con le label e le coordinate corrispondenti al problema scelto.
   //Modificare i commenti in base al problema scelto.

   //1. Test delle sette città.
   //file_label.open("Label_Seven.dat", ios::in);	  
   //file_coord.open("Coord_Seven.dat", ios::in);

   //2. Sulla circonferenza.	  
   //file_label.open("Label_Circle.dat", ios::in);	  
   //file_coord.open("Coord_Circle.dat", ios::in);

   //3. Nel quadrato.
   //file_label.open("Label_Square.dat", ios::in);	  
   //file_coord.open("Coord_Square.dat", ios::in);

   //4. Capitali d'America.
   //file_label.open("Label_America.dat", ios::in);	  
   //file_coord.open("Coord_America.dat", ios::in);

   //5. Capoluoghi d'Italia.
   file_label.open("Label_Italy.dat", ios::in);	  
   file_coord.open("Coord_Italy.dat", ios::in);

   if(!file_label or !file_coord){ 
      //Se manca almeno uno dei due file di dati nella cartella di lavoro il processo si blocca.
      cerr << "Problema: i file di dati di input non sono presenti in questa cartella!" << endl; 
      return 1;
   } 
 

   //------COSTRUISCO LA CLASSE CITY.
   vector <City> way; //Genero un vettore di City. A partire da questo, genero le classi Trip.
   	
   for(pos=0; pos<dim; pos++){ //Inserisco per una city alla volta i dati di input.	  
      //Se ci sono i file inserisco i dati uno alla volta nelle rispettive variabili d'appoggio.   	  
      //Ogni volta alle tre variabili viene associato il contenuto dei vettori alla posizione pos.   	  
      file_label >> label;
      file_coord >> longitude;
      file_coord >> latitude;

      //Caricata la terna, con essa costruisco la città e la aggiungo in coda al vettore del percorso.	  
      City point(label, longitude, latitude); //Ogni volta la City va costruita perché cambiano i suoi elementi.
      way.push_back(point); //Costruisco il vettore di City aggiungendo in coda l'ultima città costruita.
	  
      //Stampo i valori del vettore way (di ogni città) sul terminale.
      //cout << way[pos].Get_label() << "  " << way[pos].Get_x() << "  " << way[pos].Get_y() << endl;
   }

   //Chiudi i file di dati di input.	  
   file_label.close();	
   file_coord.close();  

   //------COSTRUISCO LA CLASSE TRIP.
   //Il vettore di City diventa una classe: Trip. In questo modo può effettuare operazioni su se stesso. 
   //Il vettore di città viene promosso a classe, in modo da aggiungergli le funzioni che dovrà svolgere.    
   //Metto insieme tutti i percorsi Trip: questa è la popolazione che dovrà evolvere.

   vector <Trip> population; //E' la popolazione: un vettore di percorsi. 
   vector <Trip> population_new; //E' la copia della popolazione population, dove salvo la nuova popolazione di percorsi.

   ter_dim = int(0.333333*dim); //Le mutazioni all'inizio sono un terzo del numero delle citta'.  
   //cout << "Mutazioni all'inizio per ogni percorso = " << ter_dim << endl;
   
   //------GENERO LA GENERAZIONE 0 A PARTIRE DALLA CLASSE TRIP JOURNEY(WAY) UGUALE PER TUTTI E APPLICANDOLE UNA MUTAZIONE 
   //------ESTRATTA A SORTE TRA LE 4 DISPONIBILI. IMPLEMENTO LE MUTAZIONI SUL SINGOLO INDIVIDUO PER TUTTI I NUM INDIVIDUI.
   for(k=0; k<num; k++){
	  Trip journey(way); //Genero un percorso a partire da quello iniziale. Tutti i num percorsi sono uguali. 

      for(l=0; l<ter_dim; l++){ //Numero totale di mutazioni iniziali: il 33% delle città.
         choose = rnd.Dim(5); //Estrae dei numeri interi tra 1 e 4, come le 4 scelte possibili di mutazione. 
         Mutation_population(journey, choose); //Applico le mutazioni a journey, creando un elemento della popolazione. 
      }
	  
	  //cout << k << "  " << journey.distance() << endl;
      population.push_back(journey); //Costrusco la popolazione aggiungendo uno alla volta in coda i percorsi mutati.				  
   }

   //Calcolo le frazioni di del numero di elementi della popolazione necessarie per i calcoli.
   Fractions(num);

   fstream best_distance; //Scrivo su un file esterno i risultati per ogni generazione:
                          //1. il migliore, 2. la media della meta' dei migliori.
                          //Scrivera' i risultati il nodo che ha trovato il best assoluto.

   //Tutti i nodi hanno due vettori personali in cui scrivono, per ogni generazione:
   double step_best[generations]; //il best di quella generazione;
   double step_mean[generations]; //la media della meta' dei migliori.

   //------METTO IN ORDINE CRESCENTE DELLA DISTANZA GLI ELEMENTI DELLA POPOLAZIONE APPENA NATA.
   //La funzione sort è definita nella libreria algorithm e serve ad ordinare gli elementi di un vector.   
   sort(population.begin(), population.end(), Compare_objects());
   
   //cout << "-------------------------" << endl;  
   //Print_population(population); //Stampo la popolazione.
    
   //Calcolo la distanza media della meta' migliore della popolazione alla generazione 0.
   best_half_mean = best_half_distance_mean(population);
   
   gen = 0; //Lo inizializzo a zero: la generazione 0. 
   step_best[gen] = population[0].distance();
   step_mean[gen] = best_half_mean;
  
   for(gen=1; gen<generations; gen++){ //gen = numero della generazione.
   
      //------CREO UNA COPIA DELLA POPOLAZIONE SULLA QUALE DOVRO' SOVRASCRIVERE LA NUOVA GENERAZIONE.
      population_new = population;

      //------HO OTTENUTO LA POPOLAZIONE INIZIALE: LA GENERAZIONE 0. COSTRUISCO COSI' LA GENERAZIONE SUCCESSIVA:
      //------1) L'1% DEI MIGLIORI CI VA DI DIRITTO.
      //------2) IL 99% ELEMENTI SI OTTENGONO TRAMITE CROSSOVER.
      //------3) OTTENUTA LA NUOVA POPOLAZIONE, SI SELEZIONANO IL 10% DEGLI ELEMENTI A SORTE PER FARE MUTAZIONE.

      //------IMPLEMENTO IL CROSSOVER. SELEZIONO I GENITORI NELLA POPULATION CON LA ROULETTE TRUCCATA.
      for(j=elite; j<half_num+1; j++){ //Faccio half_num tentativi di crossover. I primi elite elementi sono assegnati con elitarismo.
	  
         //------1) SELEZIONO I DUE ELEMENTI DELLA POPOLAZIONE CHE FARANNO CROSSOVER E IL PUNTO DEL TAGLIO.
         pos_one = 0; //Lo inizializzo.
         pos_two = pos_one; //Li uguaglio per entrare nel ciclo while.
         //Non ha senso che un cromosoma faccia crossover su se stesso: vado avanti a selezionarli finchè non sono diversi.	  
         //Seleziono i due elementi della popolazione che faranno crossover: vanno da 0 a dimensione del vettore-1.	  	  
         while(pos_two == pos_one){ 
            pos_one = int(num*rnd.Fake_roulette());
            pos_two = int(num*rnd.Fake_roulette());  	  
         }
	  
         //Seleziono la posizione nel cromosoma in cui faro' il taglio. Ha senso eseguire il taglio 
         //da 2 a dimensione vettore-3, ossia dal secondo elemento (0 e 1 esclusi) al terzultimo (due a destra).
         cut = 1+rnd.Dim(dim-3);
	  
         //------2) IMPLEMENTO IL CROSSOVER TRA DUE INDIVIDUI PER GENERARE LA POPOLAZIONE.	  
         //Assegno i risultati del crossover agli elementi in posizione 3 e 4 di popul_appo.
         Crossover_population(way, population[pos_one], population[pos_two], population_new, j);
		 
         //cout << "Per crossover ho selezionato:  " << pos_one << " e " << pos_two << " e cut " << cut << endl;
      } 

      //------3) IMPLEMENTO LA MUTAZIONE: SELEZIONA IL 10% DEGLI INDIVIDUI, A SORTE NELLA POPOLAZIONE NUOVA.
      for(i=0; i<ten_num; i++){
        choose = rnd.Dim(5); //Estrae dei numeri interi tra 1 e 4, come le 4 scelte possibili di mutazione.
        imt = rnd.Dim(num);  //Estrae dei numeri interi tra 1 e num-1, come le scelte possibili degli elementi della popolazione. 
	    Mutation_population(population_new[imt], choose); //Applico 1 mutazione a population[imt], portando varietà nella popolazione.
		
	    //cout << "Per mutazione ho selezionato:  " << imt << endl;
      }

      //------4) RIMPIAZZO LA VECCHIA GENERAZIONE CON QUELLA NUOVA.
      population = population_new;
   
      //------5) METTO IN ORDINE CRESCENTE DELLA DISTANZA GLI ELEMENTI DELLA POPOLAZIONE APPENA NATA DA CROSSOVER.
      sort(population.begin(), population.end(), Compare_objects());

      //------6) CALCOLO LA MEDIA DELLA DISTANZA PER LA META' MIGLIORE DELLA POPOLAZIONE.
      best_half_mean = best_half_distance_mean(population);

      //------7) SCRIVO I RISULTATI NEL VETTORE PERSONALE DI OGNI NODO.
      //Scrivo: numero generazione, distanza del best, distanza media della meta' migliore della generazione. 
      //best_distance << gen << "  " << population[0].distance() << "  " << best_half_mean << endl; 
      step_best[gen] = population[0].distance();
      step_mean[gen] = best_half_mean;

      //------8) SCAMBIO DEL BEST TRA I CONTINENTI.
      if(gen % n_migr == 0){ //Lo scambio avviene ogni n_migr generazioni.
	  
	     /*if(rank==root){ //Informazione aggiuntiva.
		    cout << "Generazione " << gen << " su " << generations << endl;
		 }*/

         //------A) ESTRAZIONE DEI NODI SENDER E RECEIVER.
         if(rank==root){ //Solo il nodo 0 fa le estrazioni. I nodi sono estratti con la stessa probabilita'.
            receiver_rank = int(size*rnd.Rannyu()); 
            sender_rank = int(size*rnd.Rannyu()); 

            while(sender_rank == receiver_rank){ //Vado avanti ad estrarre finche' non trovo due nodi diversi.
               sender_rank = int(size*rnd.Rannyu()); //Estraggo un numero tra 0 e rank-1.
            }

            cout << "Alla generazione " << gen << " avviene lo scambio del best tra i continenti." << endl;
            cout << "Sono il nodo " << rank << " ho estratto " << sender_rank << " e " << receiver_rank << endl;
            //Una volta che il nodo 0 ha deciso chi saranno sender e receiver, lo comunica al gruppo dei nodi.
         }

         //------B) COMUNICAZIONE DI CHI SARA' SENDER E RECEIVER.
         //Tutti i nodi partecipano alla comunicazione da 0 a tutti gli altri.
         //Indirizzo della variabile che mando, quante sono, tipo, chi lo invia, gruppo dei destinatari.
         MPI_Bcast(&sender_rank, 1, MPI_INTEGER, root, MPI_COMM_WORLD);  
         MPI_Bcast(&receiver_rank, 1, MPI_INTEGER, root, MPI_COMM_WORLD);  

         int itag = 1; //Etichetta del messaggio.

         //------C) SENDER SCOMPATTA L'INFORMAZIONE DEL BEST MIGRATOR E LA INVIA AL RECEIVER.
         if(rank==sender_rank){ //Se io sono stato scelto come il nodo che invia.
            //Scompatto l'informazione del best che ho trovato nelle sue tre componenti, int, double, double,
            //in modo da poterle inviare al receiver rank. Mpi non consente l'invio di elementi Trip. 

            for(i=0; i<dim; i++){
               label_migrator[i] = population[0].label(i);
               longitude_migrator[i] = population[0].x(i);
               latitude_migrator[i] = population[0].y(i);
            }

            //Invio i tre vettori delle caratteristiche del best che migra, label, x, y, al receiver rank.
            //Send: Indirizzo della variabile che mando, quante sono, tipo, destinatario, etichetta, gruppo dei nodi.
            MPI_Send(&label_migrator, dim, MPI_INTEGER, receiver_rank, itag, MPI_COMM_WORLD);
            MPI_Send(&longitude_migrator, dim, MPI_DOUBLE, receiver_rank, itag, MPI_COMM_WORLD);
            MPI_Send(&latitude_migrator, dim, MPI_DOUBLE, receiver_rank, itag, MPI_COMM_WORLD);

            cout << "Sono " << rank << " e ho inviato il best a " << receiver_rank << endl;
         }

         //------D) RECEIVER RICEVE L'INFORMAZIONE DEL BEST MIGRATOR DA SENDER E LA RICOMPONE.
         else if(rank==receiver_rank){ //Se io sono stato scelto come il nodo che riceve.
            //Ricevo da parte di sender rank i tre vettori con le caratteristiche del best che migra.
            //Recv: Indirizzo della variabile che ricevo, quante sono, tipo, mittente, etichetta, gruppo dei nodi, stato.
            MPI_Recv(&label_migrator, dim, MPI_INTEGER, sender_rank, itag, MPI_COMM_WORLD, &stat);
            MPI_Recv(&longitude_migrator, dim, MPI_DOUBLE, sender_rank, itag, MPI_COMM_WORLD, &stat);
            MPI_Recv(&latitude_migrator, dim, MPI_DOUBLE, sender_rank, itag, MPI_COMM_WORLD, &stat);

            cout << "Sono " << rank << " e ho ricevuto il best da " << sender_rank << endl;

           //------E) COSTRUISCO LA CLASSE CITY: E' ARRIVATO MIGRATOR!
           vector <City> way; //Genero un vettore di City. A partire da questo, genero le classi Trip.
   	
           for(pos=0; pos<dim; pos++){ //Inserisco per una city alla volta i dati di input.	  
               //Inserisco i dati delle city uno alla volta nelle rispettive variabili d'appoggio.
               label = label_migrator[pos];
               longitude = longitude_migrator[pos];
               latitude = latitude_migrator[pos];

               //Caricata la terna, con essa costruisco la città e la aggiungo in coda al vettore del percorso.	 
               //Le citta' vengono aggiunte una alla volta nell'ordine in cui sono state inviate. 
               City point(label, longitude, latitude); //Ogni volta la City va costruita perché cambiano i suoi elementi.
               way.push_back(point); //Costruisco il vettore di City aggiungendo in coda l'ultima città costruita.
	  
               //Stampo i valori del vettore way (di ogni città) sul terminale.
               //cout << way[pos].Get_label() << "  " << way[pos].Get_x() << "  " << way[pos].Get_y() << endl;
            }

            //------F) COSTRUISCO L'OGGETTO TRIP CON WAY NELL'ORDINE DEL BEST MIGRATOR.
            Trip migrator(way); //Rivesto le citta' in quel dato ordine di tutte le funzioni della classe Trip.

            //Calcolo la distanza del best migratore e la confronto con la distanza del best della popolazione.
            cout << "Best a confronto migrante e indigeno: " << migrator.distance() << "  " << population[0].distance() << endl;
	        cout << "---------------------------" << endl; 

            //------G) RIMPIAZZO IL BEST INDIGENO CON IL BEST MIGRANTE.
            //Il best indigeno viene cancellato e al suo posto entra il migrante.
            population[0] = migrator;

            //Ordino la popolazione in ordine crescente rispetto alla distanza: non sappiamo
            //se il nuovo best sia migliore o peggiore del best di prima o degli altri elementi.
            sort(population.begin(), population.end(), Compare_objects());

         } //Parentesi che chiude l'operato di receiver rank.
      } //Parentesi che chiude lo scambio del best tra i continenti.
   } //Parentesi che chiude l'evoluzione delle generazioni.

   //------PROCESSO COLLETTIVO: TUTTI COMUNICANO SUL TERMINALE IL RISULTATO DELLA LORO RICERCA.

   double my_best; //Risultato della ricerca di ciascun nodo.
   my_best = population[0].distance();

   double irecv; //Buffer in cui ogni nodo comunica il suo minimo. Qui il nodo radice svolgera' il processo di riduzione
                 //che in questo caso consiste nel trovare il minimo tra tutti quelli ricevuti.

   cout << endl;
   cout << "La distanza migliore finale per il nodo " << rank << " e': " << my_best << endl;
   cout << "La media finale della distanza per la meta' migliore della popolazione per il nodo " << rank << " e': " << best_half_mean << endl;

   //------TUTTI COMUNICANO IL BEST CHE HANNO TROVATO AL NODO RADICE.

   //La funzione Reduce svolge delle operazioni, come somma, prodotto o minimo, e invia i risultati da un nodo agli altri.
   //Buffer dei dati che vuoi ridurre, buffer dei dati ridotti, numero di elementi nel buffer per nodo, tipo di dato, 
   //operazione che vuoi svolgere per ridurre, nodo che riceve il risultato ridotto, gruppo dei nodi coinvolti nel processo.   
   MPI_Reduce(&my_best, &irecv, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD); 

   if(rank==0){ //Stampa il risultato della riduzione solo chi la svolge: il nodo 0.
      cout << endl;
      cout << "Minimo dei rank: " << irecv << endl;   
   } 

   //Il nodo 0 comunica a tutti gli altri nodi chi e' il minimo della riduzione.
   //Indirizzo della variabile che mando, quante sono, tipo, chi lo invia, gruppo dei destinatari.
   MPI_Bcast(&irecv, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD); 

   if(my_best==irecv){ //Se tu sei nodo il cui best personale e' uguale al minimo inviato a tutti, dillo.
      cout << "Sono il nodo " << rank << " e ho trovato il best assoluto!" << endl;

      //Scrive i risultati su file esterno solo il nodo che ha trovato il best assoluto.

      //1. Scrivo su un file esterno la distanza del best e della meta' dei migliori di ogni generazione.
      best_distance.open("best_distance.dat",ios::out); 
      for(gen=0; gen<generations; gen++){
         //step_best[gen] = population[0].distance();
         //step_mean[gen] = best_half_mean;
         best_distance << gen << "  " << step_best[gen] << "  " << step_mean[gen] << endl; 
      }
      best_distance.close();

      Print_zero(population); //2. Scrivo su file esterno le caratteristiche del best assoluto.
      cout << "sono stati prodotti i file 'best_distance.dat' e 'best_path.dat'. " << endl;
   }

   rnd.SaveSeed();

   MPI_Finalize(); //Funzione che specifica che il lavoro in parallelo è finito.   

   return 0;
   
}


void Input(int rank){

   //GENERATORE DI NUMERI CASUALI
   int i, j; //Contatori.
   int p1, p2;
   ifstream Primes("Primes");
   if (Primes.is_open()){
	 for(i=0; i<rank+2; i++){
		//Legge una riga diversa nel file di input del seme del generatore,
		//in modo che ogni nodo produca una sequenza random diversa. 
        Primes >> p1 >> p2 ;
	 }
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

   //Il primo elemento estratto è uguale per tutti i nodi.
   //Faccio dei tiri preliminari anche per l'equilibrazione.
   for(j=0; j<140; j++){
	  equi = rnd.Rannyu();
   }  
}

//Funzione che calcola le frazioni del valore num che servono per calcolare 
//quanti elementi faranno crossover e quanti faranno mutazione.
void Fractions(int num){
   //cout << endl << "Frazioni del numero di elementi della popolazione:" << endl;

   if(num % 2 == 1){ //Il numero degli elementi della popolazione deve essere pari.
      num = num-1; //Se non lo e' ci pensa il codice a modificarlo.
   }   
   //cout << "Hai inserito numero elementi della popolazione = " << num << endl;
   
   half_num = int(0.5*num); //Calcolo la meta' migliore degli elementi della popolazione.
   //cout << "Meta' degli elementi della popolazione = " << half_num << endl;
   
   elite = 2.0*int(0.5*(0.01*num)); //L'1% della popolazione fa elitarismo.  
   //Deve essere un numero pari e deve si applica ad una popolazione di almeno 200 elementi.  
   //cout << "Numero di elementi della popolazione che fa elitarismo = " << elite << endl;   

   half_elite = int(0.5*elite); //Serve per assegnare i posti nella nuova popolazione dopo crossover.
   //cout << "Meta' degli elementi della popolazione che fa elitarismo = " << half_elite << endl;   
   
   ten_num = int(0.1*num); //Vengono mutati un decimo della popolazione finale.
   //cout << "Numero di elementi della popolazione che viene mutato = " << ten_num << endl;	   
}

//Funzione che crea una popolazione a partire da un percorso iniziale e applicandogli una mutazione scelta a sorte.
//Deve restituire un oggetto Trip, ossia il vector mutato. Fatta void non esegue la mutazione.

void Mutation_population(Trip& journey, int choose){
   //cout << "Ho selezionato: " << choose << endl;
   
   //------MUTAZIONE 1: SCAMBIO DUE ELEMENTI DEL VETTORE.
   if(choose == 1){	   
      one = rnd.Dim(dim);
      two = rnd.Dim(dim);  
      journey.exchange_couple(one,two); 
   }

   //------MUTAZIONE 2: SHIFT DI ENNE ELEMENTI DEL VETTORE.
   else if(choose == 2){	   
      enne = rnd.Dim(dim);
      journey.shift_enne(enne);
   }

   //------MUTAZIONE 3: SCAMBIO DI DUE BLOCCHI DI ELEMENTI DEL VETTORE.
   else if(choose == 3){	   
      half = int(0.5*(dim-1)); //Prima meta' del vettore: qui estraggo start.
      start = rnd.Dim(half+1); //Estraggo l'inizio del primo blocco da scambiare.
      jump = int((half-start+1)*rnd.Rannyu()); //Lunghezza del salto dal punto start = lunghezza del blocco da scambiare.
	  
      //L'altro blocco da scambiare viene calcolato dalla funzione exchange_block nella seconda meta' del vettore.
      journey.exchange_block(half, start, jump);

   }

   //------MUTAZIONE 4: INVERSIONE DEGLI ELEMENTI DI IN BLOCCO DEL VETTORE.
   else if(choose == 4){
      first = rnd.Dim(dim);
      last = rnd.Dim(dim);
      journey.inversion(first, last);
   }
   
   //------CASO DI DEFAULT, SE LA SCELTA NON CORRISPONDE AI REQUISITI: NON FACCIO NULLA.
   else{}  
}

/*Funzione che fa il crossover tra due cromosomi, ossia tra due possibili percorsi della popolazione.
Entrambi i vector vengono tagliati in cut. La parte a sinistra di cut resta invariata, quella a destra
viene scambiata tra i due cromosomi, scartando chi è già presente in questa parte e tenendo chi manca.
Passo vector <City> way per costruire le classi Trip firstborn e secondborn, che devono essere Trip come i genitori.*/

void Crossover_population(vector <City> way, Trip father, Trip mother, vector <Trip>& population_new, int icr){
   
//------2) DEFINISCO I PROTAGONISTI DEL CROSSOVER: PADRE, MADRE E DUE FIGLI.
   //population[pos_one] = padre, population[pos_two] e appo_pos_two = madre.
   //Salvo le componenti della madre, visto che saranno cancellate e che serviranno.   
   Trip appo_mother(way); 
   appo_mother = mother;
   
   //Figli che nasceranno dall'unione dei genitori.
   Trip firstborn(way);
   Trip secondborn(way);  

//------3) CROSSOVER: i figli sono il risultato del crossover tra padre e madre.
   //Eseguo due volte il crossover con gli stessi genitori ma nell'ordine invertito.
   //Così si ottengono due figli diversi. Comunico anche dove fare il taglio nei genitori.
   
   //cout << "------CROSSOVER ONE.------" << endl;
   firstborn = Crossover(father, mother, cut);  
   
   //cout << "------CROSSOVER TWO.------" << endl;
   secondborn = Crossover(appo_mother, father, cut);   

//------4) LA NUOVA GENERAZIONE. 
   //ora che sono nati i figli, li posso sostituire ai genitori, passando il testimone alla nuova generazione.
   population_new[icr] = firstborn;             
   population_new[icr+half_num-half_elite] = secondborn;
   
//------5) CONTROLLO CHE LA NUOVA POPOLAZIONE SIA CONFORME AI REQUISITI.
   for(int i=0; i<num; i++){
	  /*Passo in rassegna tutti gli elementi della popolazione.
	  Controllo che tutti gli elementi City di ogni vettore Trip siano diversi tra loro.
	  Controllo che il primo elemento del vettore Trip sia la City con label = 1.
	  Se non soddisfa ai requisiti il vettore Trip rimane, ma è vuoto e ha dim = 0.
	  Calcolo la nuova distanza del vettore Trip i-esimo dopo il crossover.*/
      population_new[i].check_after_mutation();
   }
   
}

/*Funzione che fa il crossover tra due genitori. All'inizio son è un guscio vuoto.
Compiuto il crossover tra i cromosomi/genitori, la funzione restituisce un figlio. 
L'ordine in cui vengono passati i genitori alla funzione, determina i ruoli che 
hanno i genitori nel crossover e quindi come sarà il figlio.
parent_one: passa al figlio la parte prima del taglio, nell'ordine in cui si trova nel genitore.
parent_two: passa al figlio tutte le altre componenti, che non sono presenti nella parte prima 
del taglio, nell'ordine in cui si trovano nel genitore.*/

Trip Crossover(Trip father, Trip mother, int cut){
   int i, j, k; //Contatori.
   int dim_par_one; //Dimensione del vettore parent_one.
   //int dim_par_two, dim_son; //Dimensione dei vettori. Togliere il commento per stampare i risultati.
   
   /*Vector di City father e mother. Trasformo le classi Trip in vector <City>, prendendo il data member m_trip 
   da ciascuno di loro, per poter sfruttare le funzioni della libreria vector. Il data member m_trip 
   della classe Trip viene così spogliato da tutte le sue funzioni e rimane un semplice vector di City. 
   Questo vettore non gode più delle funzioni di Trip ma essendo un vector, per lui varranno le funzioni
   di vector. Essendo inoltre una lista di City, ciascuna di loro gode delle funzioni della classe City.*/
     
   vector <City> parent_one;   
   vector <City> parent_two;   
   vector <City> son; //Figlio della coppia parent_one e parent_two. 

   parent_one = father.mtrip();  
   parent_two = mother.mtrip(); 
   
   dim_par_one = parent_one.size();

//------PROCESSO DI CROSSOVER DAL QUALE NASCE IL FIGLIO.  
   //1) La parte prima del taglio è la stessa di quella del primo genitore.
   for(k=0; k<cut+1; k++){  
      son.push_back(parent_one[k]);
   }
  
   //2) Cancello nel secondo genitore gli elementi che sono già presenti nel figlio. 
   for(j=0; j<cut+1; j++){ //Per tutti gli elementi presenti per ora nel figlio.
      for(i=0; i<dim_par_one; i++){ //Per tutti gli elementi presenti nel secondo genitore.
	  
	     //Il confronto avviene tra le label degli elementi City. parent_two[i]: è una City, parent_two è un vector <City>. 
         if(son[j].Get_label() == parent_two[i].Get_label()){ //Quando trovo l'elemento presente sia nel secondo genitore sia nel figlio
		    parent_two.erase(parent_two.begin()+i); //lo elimino dal vettore secondo genitore, altrimenti non fa nulla.
		    break; //Appena trovi l'elemento da eliminare esci dal ciclo: non serve controllare anche quelli dopo.
		 }
      }		  
   }
   
   //3) Gli elementi mancanti nel figlio sono quelli che ha adesso il secondo genitore, proprio in quell'ordine.
   for(k=cut+1; k<dim_par_one; k++){
      son.push_back(parent_two[0]);     //Aggiungo in coda al figlio il primo elemento del secondo genitore.
	  parent_two.erase(parent_two.begin()); //Al secondo genitore cancello il primo elemento, così al prossimo ciclo aggiungo in coda al figlio 
	                                //sempre il primo elemento del secondo genitore, in modo che si mantenga l'ordine del vettore secondo genitore.
									
									//parent_two.begin(): è il primo elemento del vettore e bisogna specificarlo sempre quando si usa
									//la funzione erase() di vector, per specificare quale posizione cancellare.
									//Esempio: se vuoi cancellare la posizione i del vector v, devi dire a erase v.begin()+i.
   }   

   //son è un vector <City> ma gli elementi di population sono Trip. Allora promuovo son 
   //a elemento della classe Trip, rivestendolo così di tutte le funzioni di questa classe.
   Trip child(son);
   
   parent_one.clear(); //Posso liberare i vector dei genitori e del figlio.
   parent_two.clear(); //Saranno riempiti dalla prossima generazione.
   son.clear(); //Le informazioni del data member son sono state passate a child.
   
   return child;
   
}

//Funzione che calcola la media della distanza per la meta' migliore della popolazione.
double best_half_distance_mean(vector <Trip> population){
   double sum = 0; //Somma delle distanze;
   double best_mean; //Media delle distanze della meta' dei migliori.

   for(int k=0; k<half_num; k++){
      sum += population[k].distance();
   }

   best_mean = sum/half_num;
   return best_mean;
}

//Funzione che stampa gli elementi della popolazione.
void Print_population(vector <Trip> population){
   //int i, j; //Contatori.
   
   for(int j=0; j<num; j++){ //Per ogni elemento della popolazione, ossia per ogni vettore di Trip,
      /*for(i=0; i<dim; i++){ //stampo le sue caratteristiche, ossia tutte le componenti di ogni City.
         cout << population[j].label(i) << "  " << population[j].x(i) << "  " << population[j].y(i) << endl;    
      }*/
      cout << j << "  Norma = " << population[j].distance() << endl; 
      cout << "-------------------" << endl;  
   }
}

//Funzione che stampa l'elemento 0-esimo della popolazione, ossia l'elemento 0-esimo del vector Trip.
//Se la popolazione e' ordinata in ordine crescente della dimensione, stampa il best della popolazione.
void Print_zero(vector <Trip> population){
   fstream best_path;
   best_path.open("best_path.dat",ios::app);
   
   for(int i=0; i<dim; i++){ //Stampo le sue caratteristiche, ossia tutte le componenti di ogni City.
      best_path << population[0].label(i) << "  " << population[0].x(i) << "  " << population[0].y(i) << endl;    
   }  
   //Aggiungo alla fine del percorso la citta' iniziale.
   best_path << population[0].label(0) << "  " << population[0].x(0) << "  " << population[0].y(0) << endl << endl;   
   
   //cout << "Norma del best = " << population[0].distance() << endl; 
   best_path.close();
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
