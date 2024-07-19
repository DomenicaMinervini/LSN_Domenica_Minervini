/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/

#include <iostream>
#include <cmath>
#include <cstdlib>

//Funzione che azzera gli elementi dei vettori.
void Reset(int, double[], double[]);

//Funzione che calcola la distanza di un punto in 3D dall'origine.
double radius(double, double, double);

//Funzione che calcola il rapporto tra la probabilità nuova e vecchia per il ground state.
double rapporto_probabilita_ground_state(double, double);

//Funzione che calcola il rapporto tra la probabilità nuova e vecchia per il primo stato eccitato.
double rapporto_probabilita_stato_eccitato(double, double, double, double);

//Funzione che calcola il minimo tra due valori.
double minimum(double);

/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/