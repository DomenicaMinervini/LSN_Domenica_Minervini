/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/

#include "city.h"

using namespace std;

//Costruttore e distruttore; pos è la posizione dell'elemento 
//nei tre vettori con cui vorrei costruire la classe city.
City :: City(int label, double x, double y){
   m_label = label;
   m_x = x;
   m_y = y;   	
}

City :: ~City(){}

//Funzioni che comunicano l'etichetta e le coordinate della città.
int City :: Get_label(){
   return m_label;	
}

double City :: Get_x(){
   return m_x;	
}

double City :: Get_y(){
   return m_y;	
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