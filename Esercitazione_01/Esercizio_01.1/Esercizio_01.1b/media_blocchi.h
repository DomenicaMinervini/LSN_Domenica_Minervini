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

/*Funzione che esegue la media a blocchi per il calcolo dell'integrale.
void Media_Blocchi(int, double[], double[]); */

//Funzione che restituisce il valore di sigma quadro.
double Sigma_Square(double);

//Funzione che alloca i vettori con le medie e i rispettivi quadrati.
void Blocchi(int, int, double[], double[]);

//Funzione che calcola la media delle medie dei vettori.
void Media(int, int, int, double[], double[]);

/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/
