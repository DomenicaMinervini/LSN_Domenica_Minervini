/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/

#include <algorithm>
#include "trip.h"

#ifndef __TSP__
#define __TSP__

//VARIABILI

//Numeri random.
Random rnd;
int seed[4];

//Numeri per costruire i tre percorsi.
int number; //Nome della città. 
double appo; //Variabile di appoggio per valore random.
double angle; //Numero estratto da una distribuzione [0, 2 pigreco).
//Se lo moltiplico per raggio=1, ottengo i punti su una circonferenza di raggio=1.
//Estraggo i punti su una circonferenza di raggio 1 e centrata in (0,0) per il percorso a.
//Estraggo i punti in un quadrato di lato 2 centrato in (0,0) per il percorso b.

//I vettori di dati. Ci sono tre tipi di percorso: vita di Domenica ...
/*const int seven = 7; //Dimensione del vettore = numero di città da percorrere.
int label_dom[seven]={1,2,3,4,5,6,7}; //Vettore con le etichette delle città.
double longitude_dom[seven]={0,-1,2,0,2,4,4}; //Vettore longitudine = coordinate x delle città.
double latitude_dom[seven]={0,0,0,-2.5,1,1,3.5}; //Vettore latitudine = coordinate y delle città.*/

//... su una circonferenza e dentro ad un quadrato.
int figure; //Seleziona il problema del cerchio o nel quadrato.
const int dim = 34; //Dimensione dei vettori = numero di città da percorrere.
int label_circle[dim], label_square[dim];
double longitude_circle[dim], longitude_square[dim];
double latitude_circle[dim], latitude_square[dim];

int pos; //Posizione nei vettori con i cui elementi costruisco la classe City. 
int label; //Etichetta che viene assegnata ogni volta che si costruisce una classe City.
double longitude, latitude; //Lo stesso per coordinate x e y della City.

//Numero di elementi della popolazione e sue frazioni che servono per costruire la generazione successiva.
int num; //Numero di percorsi generati = numero di elementi della popolazione.
int half_num; //Meta' degli elementi della popolazione.
int ten_num; //Numero di elementi della popolazione che viene mutato dopo crossover.
int ter_dim; //Un terzo di dim.
int elite; //Numero di elementi della popolazioni che fanno elitarismo.
int half_elite; //Meta' degli elementi che fanno elitarismo. 

//Posizioni del vettore di città da scambiare, o estremi dei blocchi da invertire per le mutazioni.
int choose; //Metodo di mutazione scelto a sorte. Assume valori tra 1 e 4, come le mutazioni possibili.
int one, two; //Posizioni del vettore da scambiare estratte random. Mutazione 1.
int enne; //Di quante posizioni sposto in avanti gli elementi del vettore. Mutazione 2.
int half, start, jump; //Meta' vettore, inizio del primo bloccco, lunghezza del blocco. Mutazione 3.
int first, last; //Primo ed ultimo elemento del blocco che subira' l'inversione. Mutazione 4.
int pos_one, pos_two, cut; //I due elementi della popolazione che faranno crossover e punto del taglio.

//Numero di generazioni, il suo contatore, indici per fare mutazioni/crossover.
int generations; //Numero di generazioni.
int gen; //Indice delle generazioni.
int icr; //Indici nel vector della popolazione che faranno crossover.
int imt; //Indici nel vector della popolazione che faranno mutazione.
double best_half_mean; //Valor medio della distanza della meta' migliore della popolazione.

/*Le città di Domenica. Non è una richiesta d'esame. Le ho usate come test durante 
lo svolgimento del codice, per verificarne ad ogni passo il corretto funzionamento.
City myhome(1,0,0);
City market(2,-1.0,0);
City lounge(3,2.0,0);
City cheese(4,0,-2.5);
City bakery(5,2.0,1.0);
City temple(6,4.0,1.0);
City newsta(7,4.0,3.5);*/

//FUNZIONI

//GENERATORE DI NUMERI CASUALI PER COSTRUIRE LE CITTA'.
//Funzione che da il set al seme del generatore di numeri casuali
//e che genera i punti per i percorsi sulla circonferenza e nel quadrato.
void Input();
//Funzione che stampa i tre vettori di dati sul terminale.
void Print_video();
//Funzione che stampa le coordinate di cerchio e quadrato su due file esterni.
void Print();
//Funzione che calcola le frazioni di num, numero di elementi della popolazione, per fare mutazione e crossover.
void Fractions(int);

//GENERATORI DELLA POPOLAZIONE: MUTAZIONI E CROSSOVER.
//Funzione che genera una popolazione a partire da un percorso iniziale e applicando le mutazioni.
void Mutation_population(Trip&, int);
//Funzione che fa il crossover tra due cromosomi, ossia tra due possibili percorsi della popolazione.
void Crossover_population(vector <City>, Trip, Trip, vector <Trip>&, int);
//Funzione che fa il crossover tra due genitori: è richiamata in Crossover_population.
Trip Crossover(Trip, Trip, int);
//Funzione che calcola la media della distanza per la meta' migliore della popolazione.
double best_half_distance_mean(vector <Trip>);

//STAMPA DEI RISULTATI RELATIVI ALLA POPOLAZIONE.
//Funzione che stampa gli elementi della popolazione.
void Print_population(vector <Trip>);
//Funzione che stampa l'elemento 0-esimo della popolazione.
void Print_zero(vector <Trip>);

//ORDINAMENTO DEGLI ELEMENTI DELLA POPOLAZIONE.
//Funzione che confronta le distanze di due elementi del vector <Trip> population.
//Compare nella funzione sort che scambia due componenti Trip del vector population.
class Compare_objects{
public:
    bool operator()(Trip& alfa, Trip& beta){
        return alfa.distance() < beta.distance();
    }
};

#endif //__TSP__

/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/
