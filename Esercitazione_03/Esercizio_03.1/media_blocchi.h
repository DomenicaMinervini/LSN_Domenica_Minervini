/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/

#include <string>
#include <fstream>
#include "random.h"

//Funzione che azzera il contenuto dei vettori.
void Reset(int, double[], double[]);

//Funzione che genera i prezzi S(T).
double Prezzo(double);

//Funzione che calcola il guadagno finale per l'opzione Call.
double Call(double);

//Funzione che calcola il guadagno finale per l'opzione Put.
double Put(double);

//Funzione che alloca i vettori con le medie e i rispettivi quadrati Call.
void Blocchi_C(int, int, double[], double[]);

//Funzione che calcola la media delle medie dei vettori Call.
void Media_C(int, int, int, double[], double[]);

//Funzione che alloca i vettori con le medie e i rispettivi quadrati Put.
void Blocchi_P(int, int, double[], double[]);

//Funzione che calcola la media delle medie dei vettori Put.
void Media_P(int, int, int, double[], double[]);

/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/
