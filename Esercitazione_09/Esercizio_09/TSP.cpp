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

using namespace std;
 
int main (int argc, char *argv[]){

   //Ho risolto il problema con num = 400 e generations = 200.
   //Il codice funziona ponendo almeno num = 200.
   
   int figure; //Seleziona il problema del cerchio o nel quadrato.
   int num; //Numero di elementi della popolazione.
   int generations; //Numero di generazioni. 
   
   if(argc != 4){
	  cout << "Per eseguire il codice bisogna passare i seguenti parametri da terminale: " << endl;
      cout << "argv[0] = nome eseguibile = ./TSP.exe." << endl;
	  cout << "argv[1] = numero intero per scegliere tra circonferenza o quadrato = 0 o altro." << endl;
	  cout << "argv[2] = numero intero per gli elementi della popolazione." << endl;
	  cout << "argv[3] = numero intero per le generazioni della popolazione." << endl;
	  return 1;
   }
   
   //Assegno i valori passati da terminale.
   figure = atoi(argv[1]); 
   num = atoi(argv[2]);   
   generations = atoi(argv[3]);
      
   //------GENERO I PUNTI CHE DESCRIVONO LE CITTA'.  
   Input(); //Funzione che seleziona i numeri random e alloca i vettori
            //con le coordinate dei percorsi sul cerchio e nel quadrato.			
   //Print_video(); //Stampa i dati sul terminale.
   //Print(); //Stampa i dati su due file esterni.

   //------COSTRUISCO LA CLASSE CITY.
   int i, j, k, l; //Contatori.
   vector <City> way; //Genero un vettore di City. A partire da questo, genero le classi Trip.
   
   if(figure == 0){ //Comunica che problema hai scelto.
      cout << "Hai scelto il problema sulla circonerenza." << endl;  
	  
      for(pos=0; pos<dim; pos++){ //Scorro i tre vettori con le caratteristiche della città.
         //Ogni volta alle tre variabili viene associato il contenuto dei vettori alla posizione pos.   
         label = label_circle[pos];
         longitude = longitude_circle[pos];
         latitude = latitude_circle[pos];
	  
         City point(label, longitude, latitude); //Ogni volta la City va costruita perché cambiano i suoi elementi.
         way.push_back(point); //Costruisco il vettore di City aggiungendo in coda l'ultima città costruita.
	  
         //Stampo i valori del vettore way (di ogni città) sul terminale.
         //cout << way[pos].Get_label() << "  " << way[pos].Get_x() << "  " << way[pos].Get_y() << endl;
      }	  
   }
   else{
      cout << "Hai scelto il problema nel quadrato." << endl;
	  
      for(pos=0; pos<dim; pos++){ 
         label = label_square[pos];
         longitude = longitude_square[pos];
         latitude = latitude_square[pos];
	  
         City point(label, longitude, latitude); //Ogni volta la City va costruita perché cambiano i suoi elementi.
         way.push_back(point); //Costruisco il vettore di City aggiungendo in coda l'ultima città costruita.
	  
         //cout << way[pos].Get_label() << "  " << way[pos].Get_x() << "  " << way[pos].Get_y() << endl;
      }	  
   }   


   //------COSTRUISCO LA CLASSE TRIP.
   //Il vettore di City diventa una classe: Trip. In questo modo può effettuare operazioni su se stesso. 
   //Il vettore di città viene promosso a classe, in modo da aggiungergli le funzioni che dovrà svolgere.    
   //Metto insieme tutti i percorsi Trip: questa è la popolazione che dovrà evolvere.

   vector <Trip> population; //E' la popolazione: un vettore di percorsi. 
   vector <Trip> population_new; //E' la copia della popolazione population, dove salvo la nuova popolazione di percorsi.

   ter_dim = int(0.333333*dim); //Le mutazioni all'inizio sono un terzo del numero delle citta'.  
   cout << "Mutazioni all'inizio per ogni percorso = " << ter_dim << endl;
   
   //------GENERO LA GENERAZIONE 0 A PARTIRE DALLA CLASSE TRIP JOURNEY(WAY) UGUALE PER TUTTI E APPLICANDOLE UNA MUTAZIONE 
   //------ESTRATTA A SORTE TRA LE 4 DISPONIBILI. IMPLEMENTO LE MUTAZIONI SUL SINGOLO INDIVIDUO PER TUTTI I NUM INDIVIDUI.
   for(k=0; k<num; k++){
	  Trip journey(way); //Genero un percorso a partire da quello iniziale. Tutti i num percorsi sono uguali. 

      for(l=0; l<ter_dim; l++){ //Numero totale di mutazioni iniziali: il 33% delle città.
         choose = rnd.Dim(5); //Estrae dei numeri interi tra 1 e 4, come le 4 scelte possibili di mutazione.   	  
         Mutation_population(journey, choose); //Applico le mutazioni a journey, creando un elemento della popolazione. 
      }	  
	  
      population.push_back(journey); //Costrusco la popolazione aggiungendo uno alla volta in coda i percorsi mutati.									 
   }

   //Calcolo le frazioni di del numero di elementi della popolazione necessarie per i calcoli.
   Fractions(num);

   //------METTO IN ORDINE CRESCENTE DELLA DISTANZA GLI ELEMENTI DELLA POPOLAZIONE APPENA NATA.
   //La funzione sort è definita nella libreria algorithm e serve ad ordinare gli elementi di un vector.   
   sort(population.begin(), population.end(), Compare_objects());
   //Calcolo la distanza media della meta' migliore della popolazione alla generazione 0.
   best_half_mean = best_half_distance_mean(population);
   
   cout << "Hai inserito numero di generazioni = " << generations << endl;
   gen = 0; //Lo inizializzo a zero: la generazione 0.   
   
   fstream best_distance; 
   best_distance.open("best_distance.dat",ios::out); //Scrivo su un file esterno la norma del best di ogni generazione.
   best_distance << gen << "  " << population[0].distance() << "  " << best_half_mean << endl; //Scrivo il migliore della generazione 0.
  
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

      //Scrivo sul file esterno: numero generazione, distanza del best, distanza media della meta' migliore della generazione. 
      best_distance << gen << "  " << population[0].distance() << "  " << best_half_mean << endl; 

   }

   cout << endl;
   best_half_mean = best_half_distance_mean(population);
   cout << "La media della distanza per la meta' migliore della popolazione " << gen-1 << " e':  " << best_half_mean << endl;

   Print_zero(population); 

   cout << "sono stati prodotti i file 'best_distance.dat' e 'best_path.dat'. " << endl;
   best_distance.close();
      
   rnd.SaveSeed();
   return 0;
   
}


void Input(){

   //GENERATORE DI NUMERI CASUALI
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
   for(j=0; j<4; j++){
	  appo = rnd.Rannyu();
   }
   
   //ALLOCO I VETTORI ETICHETTA E COORDINATE DEI DUE PERCORSI  
   for(i=0; i<dim; i++){
	   
      //Etichette: numeri interi da 1 a 34. Sono i nomi delle città.
      number = i+1;
      label_circle[i] = number;
      label_square[i] = number;

      /*Coordinate su una circonferenza di raggio 1 centrata in (0,0).
      Per ottenerle estraggo un angolo tra 0 e 2 pigrego e ne calcolo cos e sin.
      Prima dell'inizio del ciclo richiamo Rannyu per avere dei dati più casuali.*/
      angle = rnd.Phi();
      longitude_circle[i] = cos(angle);
      latitude_circle[i] = sin(angle);
	  
      /*Coordinate all'interno di un quadrato di lato 2 centrato in (0,0).
      Per ottenerle estraggo la x e la y in una distribuzione [-1,1)
      Tra le due estrazioni ne richiamo un'altra in modo da non avere 
      valori consecutivi estratti con il metodo Rannyu in Phi e in One. 
      Anche i punti sulla circonferenza non saranno estratti consecutivamente.*/
      longitude_square[i] = rnd.One(); 
      appo = rnd.Rannyu();
      latitude_square[i] = rnd.One();	
	  
   }
}

void Print_video(){  
   //Stampo i tre vettori di dati per i tre percorsi. 
   int j; //Contatore.
   
   /*cout << "Percorso di Domenica." << endl;
   for(j=0; j<seven; j++){
      cout << label_dom[j] << "  " << longitude_dom[j] << "  " << latitude_dom[j] << endl;
   }
   cout << "----------------------------" << endl;*/
   
   cout << "Percorso su una circonferenza di raggio 1 centrata in (0.0)." << endl;
   for(j=0; j<dim; j++){
      cout << label_circle[j] << "  " << longitude_circle[j] << "  " << latitude_circle[j] << endl;
   }
   cout << "----------------------------" << endl;
   
   cout << "Percorso dentro ad un quadrato di lato 2 centrato in (0,0)." << endl;
   for(j=0; j<dim; j++){
      cout << label_square[j] << "  " << longitude_square[j] << "  " << latitude_square[j] << endl;
   }  
}

void Print(){ 
   //Stampo le coordinate dei punti dei percorsi su due file esterni.
   fstream circle, square;
   circle.open("Cerchio.dat",ios::out);
   square.open("Quadrato.dat",ios::out);

   for(int j=0; j<dim; j++){
      circle << label_circle[j] << "  " << longitude_circle[j] << "  " << latitude_circle[j] << endl; 	   
      square << label_square[j] << "  " << longitude_square[j] << "  " << latitude_square[j] << endl; 	   
   }

   circle.close();
   square.close();      
}

//Funzione che calcola le frazioni del valore num che servono per calcolare 
//quanti elementi faranno crossover e quanti faranno mutazione.
void Fractions(int num){
   cout << endl << "Frazioni del numero di elementi della popolazione:" << endl;

   if(num % 2 == 1){ //Il numero degli elementi della popolazione deve essere pari.
      num = num-1; //Se non lo e' ci pensa il codice a modificarlo.
   }   
   cout << "Hai inserito numero elementi della popolazione = " << num << endl;
   
   half_num = int(0.5*num); //Calcolo la meta' migliore degli elementi della popolazione.
   cout << "Meta' degli elementi della popolazione = " << half_num << endl;
   
   elite = 2.0*int(0.5*(0.01*num)); //L'1% della popolazione fa elitarismo.  
   //Deve essere un numero pari e deve si applica ad una popolazione di almeno 200 elementi.  
   cout << "Numero di elementi della popolazione che fa elitarismo = " << elite << endl;   

   half_elite = int(0.5*elite); //Serve per assegnare i posti nella nuova popolazione dopo crossover.
   cout << "Meta' degli elementi della popolazione che fa elitarismo = " << half_elite << endl;   
   
   ten_num = int(0.1*num); //Vengono mutati un decimo della popolazione finale.
   cout << "Numero di elementi della popolazione che viene mutato = " << ten_num << endl;	   
}

//Funzione che crea una popolazione a partire da un percorso iniziale e applicandogli una mutazione scelta a sorte.
//Deve restituire un oggetto Trip, ossia il vector mutato. Fatta void non esegue la mutazione.

void Mutation_population(Trip& journey, int choose){
	
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
	  Calcolo la nuova distanza del vettore Trip i-esimo dopo il crossover.  */
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
   best_path.open("best_path.dat",ios::out);
   
   for(int i=0; i<dim; i++){ //Stampo le sue caratteristiche, ossia tutte le componenti di ogni City.
      best_path << population[0].label(i) << "  " << population[0].x(i) << "  " << population[0].y(i) << endl;    
   }  
   //Aggiungo alla fine del percorso la citta' iniziale.
   best_path << population[0].label(0) << "  " << population[0].x(0) << "  " << population[0].y(0) << endl;   
   
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
