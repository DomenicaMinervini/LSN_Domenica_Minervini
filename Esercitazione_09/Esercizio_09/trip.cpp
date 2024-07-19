/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/

#include "trip.h"

//using namespace std è nel file trip.h.

/*Ho definito un vettore di elementi City. Ogni casella del vettore è un vettore, costituito dagli 
elementi della classe City. Ad ogni casella-elemento si possono applicare le funzioni della classe City.   
Con la funzione push_back della classe vector aggiungo un elemento alla volta al vettore.
Ogni elemento viene aggiunto alla fine del vettore. Gli elementi del vettore sono le classi City.*/

//Costruttore e distruttore.
Trip :: Trip(vector <City> way){
   //DATA MEMBER 1: IL VETTORE DI CITTA'.
   m_trip = way; //Passo il vettore che viene generato nella funzione main.
	
   //DATA MEMBER 2: LA DIMENSIONE DEL VETTORE, QUANTE CITTA' CI SONO.
   m_dim = m_trip.size(); 
	
   /*Dimensione del vettore m_trip: size() è una funzione di vector.
   Ho reso m_dim un elemento della classe trip per calcolarlo una volta per tutte, 
   visto che sarà richiamato in quasi tutte le altre funzioni della classe.
   Calcolarlo una volta sola anziché migliaia di volte, aumenta l'efficienza del codice.*/
	
   //DATA MEMBER 3: LA DISTANZA CHE FORMANO LE CITTA' ESSENDO PROPRIO IN QUESTO ORDINE.
   m_dist = norm();  		
}

Trip :: ~Trip(){}

//Funzione che restituisce il vettore di City m_trip, data member della classe.
vector <City> Trip :: mtrip(){
   return m_trip;	
}

//Se si vuole conoscere la dimensione del vettore m_trip. Comunica il data member m_dim.
int Trip :: dimension(){
   return m_dim;	
}

//Calcolo della norma del vettore m_trip. 
double Trip :: norm(){	
   int i; //Contatore.
   double L; //Norma del vettore = distanza del percorso.

    L = sqrt(pow(m_trip[0].Get_x()-m_trip[m_dim-1].Get_x(), 2) + pow(m_trip[0].Get_y()-m_trip[m_dim-1].Get_y(), 2));
   
    /*L è la distanza del vettore m_trip. La inizializzo ad un valore perché è data dalla somma di tutti i segmentini 
    che compongono il percorso. Con questo pretesto aggiungo il pezzetto finale, ossia la norma del segmento  
    che collega il penultimo punto del percorso (m_dim-1) al punto finale = punto iniziale = 0.
    La lunghezza di ogni segmentino si calcola facendo la norma (qui il teorema di Pitagora) del vettore compreso 
    tra due punti successivi di m_trip. L è la funzione costo da minimizzare: stiamo cercando il percorso più breve.*/  
   	  
    for(i=1; i<m_dim; i++){
       L += sqrt(pow(m_trip[i].Get_x()-m_trip[i-1].Get_x(), 2) + pow(m_trip[i].Get_y()-m_trip[i-1].Get_y(), 2));
    }  	 

   return L;
}

//Distanza del vettore m_trip. Comunica il data member m_dist.
double Trip :: distance(){	
   return m_dist;
}

/*Il vettore m_trip è fatto di tante caselline di tipo City. 
Ciascuna di loro ha tre componenti: label, x e y. Queste tre funzioni
chiedono al vettore m_trip di comunicarci, dato un intero che indica 
la casellina, quale label, x o y hanno gli elementi nella casellina.*/

int Trip :: label(int pos){
   return m_trip[pos].Get_label();
}

double Trip :: x(int pos){
   return m_trip[pos].Get_x();
}

double Trip :: y(int pos){
   return m_trip[pos].Get_y();
}

//Funzione che controlla che il primo elemento del vettore sia sempre il primo. 
void Trip :: check_first(){
   //Stampo l'etichetta del primo elemento: deve essere 1, per costruzione.
   //cout << "Label del primo elemento del vettore = " << m_trip[0].Get_label() << endl;  

   if(m_trip[0].Get_label() != 1){
      cout << "Il primo elemento del vettore e' stato spostato. Non va bene." << endl;
      m_trip.clear(); //Cancella tutto il vettore m_trip perché non rispetta le condizioni iniziali.
   }
}

//Funzione che controlla che tutti gli elementi del vettore siano diversi tra loro.
void Trip :: check_equal(){
   int i, j; //Contatori.

   for(i=0; i<m_dim; i++){ //Fissa un elemento del vettore e scorri sui rimanenti.
      for(j=i+1; j<m_dim; j++){ //Confronta i con tutti gli elementi successivi.
	     if(m_trip[i].Get_label() == m_trip[j].Get_label()){
		    cout << "Ci sono alcuni elementi uguali nel vettore. Non va bene." << endl;
			//cout << m_trip[i].Get_label() << "  " << m_trip[j].Get_label() << endl;
            m_trip.clear(); //clear() è una funzione di vector e cancella tutto il vettore.	
         }
      }		  
   }
}

//Funzione che dopo ogni mutazione, controlla che il nuovo vettore prodotto
//corrisponda agli standard richiesti e ne calcola la nuova distanza.
void Trip :: check_after_mutation(){
   //check_first(); //Controlla che il primo elemento del percorso resti invariato.
   //check_equal(); //Controlla che non ci siano elementi uguali nel percorso.
   
   m_dist = norm();	//Applicata la mutazione al percorso, è cambiata la sua distanza.
}

//Funzione che scambia due componenti del vettore a scelta dell'utente.
void Trip :: exchange_user(int one, int two){
   City temp(0,0,0); //Variabile d'appoggio in cui conservo il primo elemento da scambiare.

   if(one==0 or two==0 or one>m_dim-1 or two>m_dim-1){
      /*Queste condizioni fanno sì che non si possa spostare il primo elemento del vettore.
      Chiedono anche che l'utente inserisca due numeri inferiori alla lunghezza del vettore. 
      Questo if è vuoto: se non sono rispettate le condizioni, questa funzione non fa nulla:
      lo scambio degli elementi non avviene e il vettore resta sempre lo stesso.*/
	  
      cout << "Impossibile effettuare lo scambio." << endl;
      cout << "Inserire valori diversi da 0 e minori di " << m_dim << "." << endl;
   }   
   else{
      temp = m_trip[one]; //Lo metto al sicuro. 
      m_trip[one] = m_trip[two]; //Lo posso sovrascrivere.
      m_trip[two] = temp; //Ricompare il valore iniziale.	  
      //cout << "Ho scambiato " << one << " con " << two << "." << endl; 
   }	

   check_after_mutation(); //Controlla che il vettore mutato rispetti le condizioni richieste.   
}

//Funzione che scambia due componenti del vettore scelte random.
void Trip :: exchange_couple(int one, int two){
   //Le condizioni di controllo sono dettate nel main dall'intervallo in cui si estrae random.
   City temp(0,0,0); //Variabile d'appoggio in cui conservo il primo elemento da scambiare.

   temp = m_trip[one]; //Lo metto al sicuro.
   m_trip[one] = m_trip[two]; //Lo posso sovrascrivere.
   m_trip[two] = temp; //Ricompare il valore iniziale.
   
   check_after_mutation(); //Controlla che il vettore mutato rispetti le condizioni richieste.   
}

//Funzione che fa slittare in avanti di una posizione tutte le componenti del vettore, tranne la prima.
void Trip :: shift_one(){
   City temp(0,0,0); //Variabile di appoggio in cui conservo il secondo elemento del vettore, al numero 1
                     //(il primo è lo zero). Lo rimetterò nel vettore all'ultima posizione (la m_dim-1).
					 
   temp = m_trip[1]; //Metto al sicuro il secondo elemento del vettore. Tutti gli elementi del vettore 
                     //saranno sovrascritti l'uno sull'altro da destra a sinistra: 1<-2, 2<-3, ...
   for(int i=1; i<m_dim-1; i++){ //Parto da i=1 perché l'elemento 0-esimo non si tocca.
      m_trip[i] = m_trip[i+1]; //Sovrascrivo l'elemento i+1-esimo sull'i-esimo.   
   }
   
   m_trip[m_dim-1] = temp; //Ora anche l'ultimo elemento del vettore è stato spostato nella casella accanto: 
                         //nella sua casellina posso sovrascrivere il secondo elemento del vettore.
						 
   check_after_mutation(); //Controlla che il vettore mutato rispetti le condizioni richieste.   
}

//Funzione che fa slittare in avanti di un certo numero di posizioni tutte le componenti del vettore, tranne la prima. 
void Trip :: shift_enne(int enne){
   int i, j; //Contatori.
   City temp(0,0,0); //Variabile di appoggio in cui conservo il secondo elemento del vettore, al numero 1
                     //(il primo è lo zero). Lo rimetterò nel vettore all'ultima posizione (la m_dim-1).

   for(j=0; j<enne; j++){ //Fare uno slittamento di enne posizioni in avanti è come fare enne slittamenti da 1.
                          //quindi ripeto lo slittamento da 1 per j volte, fino alla quantità richiesta, enne.
					 
      temp = m_trip[1]; //Metto al sicuro il secondo elemento del vettore. Tutti gli elementi del vettore 
                        //saranno sovrascritti l'uno sull'altro da destra a sinistra: 1<-2, 2<-3, ...
      for(i=1; i<m_dim-1; i++){ //Parto da i=1 perché l'elemento 0-esimo non si tocca.
         m_trip[i] = m_trip[i+1]; //Sovrascrivo l'elemento i+1-esimo sull'i-esimo.   
      }
   
      m_trip[m_dim-1] = temp; //Ora anche l'ultimo elemento del vettore è stato spostato nella casella accanto: 
                             //nella sua casellina posso sovrascrivere il secondo elemento del vettore. 				 						 
   }
   
   //cout << "Ho fatto uno shift di " << enne << " posizioni." << endl;   
   check_after_mutation(); //Controlla che il vettore mutato rispetti le condizioni richieste.   
}

//Funzione che scambia due blocchi di elementi del vettore, fissati il punto iniziale del primo blocco e la lunghezza. 
void Trip :: exchange_block(int half, int start, int jump){
   //Le condizioni di controllo sono dettate nel main dall'intervallo in cui si estrae random.
   City temp(0,0,0); //Variabile d'appoggio in cui conservo il primo da scambiare ad ogni ciclo.

   for(int i=start; i<start+jump+1; i++){ //Scambio tutti gli elementi fissati gli intervalli ideali.	   
      temp = m_trip[i]; //Lo metto al sicuro.
      m_trip[i] = m_trip[i+half]; //Lo posso sovrascrivere.
      m_trip[i+half] = temp; //Ricompare il valore iniziale.   
   } 

   //cout << "Il primo elemento del blocco da scambiare: " << start << endl;
   //cout << "Il numero di elementi nei blocchi da scambiare: " << jump << endl;  
   check_after_mutation(); //Controlla che il vettore mutato rispetti le condizioni richieste.      
}

//Funzione che inverte gli elementi in un blocco del vettore, dati i due estremi.
void Trip :: inversion(int first, int last){
   int i; //Contatore.
   int appo; //Variabile d'appoggio per l'eventuale scambio tra first e last.
   int dist, half; //Lunghezza dell'intervallo e la sua metà.
   City temp(0,0,0); //Variabile d'appoggio in cui conservo il primo da scambiare ad ogni ciclo.
   
   //Mi assicuro che il primo elemento dell'intervallo sia minore del secondo, altrimenti li inverto.
   if(first > last){
      appo = first;
      first = last;
      last = appo;
   }
   
   dist = last-first+1; //Senza il +1 il primo elemento sarebbe escluso. 
   half = int(0.5*dist); //Lo rendo un intero. Se dist è pari, half cade a metà. Se dist è dispari, half 
                           //cade poco prima di metà e l'elemento centrale del blocco non viene scambiato.	
						   
   for(i=0; i<half; i++){ //Lo scambio è simmetrico rispetto alla metà del blocco.
      temp = m_trip[first+i]; //Metto al sicuro il primo elemento del blocco.
      m_trip[first+i] = m_trip[last-i]; //Al posto del primo elemento metto l'ultimo.
      m_trip[last-i] = temp; //Nell'ultimo elemento del blocco metto il primo.
	  
      /*Ad ogni ciclo il primo estremo del blocco aumenta di uno e l'ultimo estremo diminuisce di uno.
        Così sono sempre il primo e l'ultimo a scabiarsi. Il centrale si scambia con se stesso. L'intervallo si 
        accorcia progressivamente dei due estremi in modo che alla fine del ciclo, questi si incontrino al centro.*/  
   }
   
   //cout << "Estremi del blocco da invertire: " << first << "  " << last << endl; 
   check_after_mutation(); //Controlla che il vettore mutato rispetti le condizioni richieste.   
}

/*Funzione che, considerato un elemento pos del vettore, dice se l'elemento successivo, pos+1, 
  sia più piccolo o più grande di pos. La funzione lo comunica restituendo un operatore  
  di tipo boolean: se il primo elemento, pos, è più grande del secondo, pos+1, restituisce 
  il valore true = 1, in caso contrario restituisce il valore false = 0.
  Le quantità che vengono confrontate della classe City sono le label.*/
bool Trip :: give(int pos){
   int next; //Posizione successiva a pos.
   bool answer; //Valore restituito = risposta alla domanda.
   
   if(pos == m_dim-1){ //L'elemento successivo all'ultimo elemento del vettore è il secondo,
      next = 1; //ossia il numero 1, in modo che tutti abbiano un vicino successivo.
   }
   else{ //Normalmente il vicino successivo è quello la cui posizione è aumentata di 1.
      next = pos+1; //Il primo elemento, ossia il numero 0, non si può spostare.
   }
   
   //cout << "Confronto tra le posizioni " << pos << " e " << next << endl; 
   //Se è più grande il primo dei due restituisci true, altrimenti restituisci false.
   if(m_trip[pos].Get_label() > m_trip[next].Get_label()){
      answer = true;
   }
   else{
      answer = false;
   }	
   
   return answer;  
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
