/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/

#include "dices.h"

using namespace std;
 
int main (int argc, char *argv[]){

   int M = 10000; //Numero di cicli: 10000.
   
   cout << "Dado standard: " << endl;
   Standard(M);	
   cout << "------------------" << endl;
   cout << "Dado esponenziale: " << endl;
   Exponent(M);	
   cout << "------------------" << endl;
   cout << "Dado Lorentziano: " << endl;
   Lorentzian(M);	
   cout << "------------------" << endl;
	
   return 0;
	
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