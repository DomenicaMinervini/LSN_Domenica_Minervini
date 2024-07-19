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

//------VARIABILI

//Modificare il commento a seconda del problema che si vuole risolvere: sono le dimensioni dei vettori.
//Non si puo' fare altrimenti perché devono essere const int e bisogna dichiarare subito quanto valgono.
//DIM = numero di città da percorrere, GENERATIONS = numero di generazioni,
//NUM = numero di percorsi generati = numero di elementi della popolazione.
//N_MIGR = numero di generazioni ogni cui fare migrazione del best.

//------SCEGLI IL PROBLEMA CHE VUOI RISOLVERE. 

//1. Test delle sette citta'.
/*const int dim = 7;
const int generations = 10
int num = 200;
int n_migr = 2;*/

//2. Problema sulla circonferenza.
/*const int dim = 34;
const int generations = 200;
int num = 400;
int n_migr = 10;*/

//3. Problema nel quadrato.
/*const int dim = 34;
const int generations = 500;
int num = 400;
int n_migr = 10;*/

//4. Capitali d'America.
/*const int dim = 50;
const int generations = 500;
int num = 700;
int n_migr = 20;*/

//5. Capoluoghi d'Italia.
const int dim = 110;
const int generations = 1050;
int num = 1300;
int n_migr = 15;


//------VARIABILI COMUNI A TUTTI I PROBLEMI.

//Numeri random.
Random rnd;
int seed[4];

//Numeri per costruire i percorsi, per ogni problema.
double equi; //Variabile d'appoggio in cui inserisco i valori estratti all'inizio per fare equilibrazione.
int prob; //Seleziona il problema: test, cerchio, quadrato, America, Italia.
int pos; //Indice della posizione nel vector way i cui elementi costruisco la classe City. 
int label; //Etichetta che viene assegnata ogni volta che si costruisce una classe City.
double longitude, latitude; //Lo stesso per coordinate x e y della City.

//Numero di elementi della popolazione e sue frazioni che servono per costruire la generazione successiva.
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

//Numero di generazioni, indici per fare mutazioni/crossover.
int gen; //Indice delle generazioni.
int icr; //Indici nel vector della popolazione che faranno crossover.
int imt; //Indici nel vector della popolazione che faranno mutazione.
double best_half_mean; //Valor medio della distanza della meta' migliore della popolazione.

//Vettori in cui scompatto l'informazione portata dal vector <city>, data member di Trip.
//Poiché non posso inviare con mpi un elemento Trip ma solo int o double, divido 
//il vector <city> nelle sue tre componenti, int, double, double in tre vettori separati.
int label_migrator[dim];
double longitude_migrator[dim];
double latitude_migrator[dim];


//------FUNZIONI.

//GENERATORE DI NUMERI CASUALI PER COSTRUIRE LE CITTA'.
//Funzione che da il set al seme del generatore di numeri casuali
//e che genera i punti per i percorsi sulla circonferenza e nel quadrato.
void Input(int);
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
