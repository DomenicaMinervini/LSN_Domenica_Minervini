/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/

#include "funzioni.h"

using namespace std;

void Reset(int N, double mean[], double square[]){
   for(int i=0; i<N; i++){
      mean[i] = 0;
	  square[i] = 0;
   }	   	
}

double radius(double x, double y, double z){
   return sqrt(x*x+y*y+z*z);
}

double rapporto_probabilita_ground_state(double r_old, double r_new){
   return exp(2.0*(r_old-r_new));
}

double rapporto_probabilita_stato_eccitato(double r_old, double r_new, double z_old, double z_new){
   return (exp(r_old-r_new))*((z_new*z_new)/(z_old*z_old));
}

double minimum(double rapp){
   if(rapp<1.0) return rapp;
   else return 1.0;
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