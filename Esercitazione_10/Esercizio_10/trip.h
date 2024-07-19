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
#include <vector>
#include "random.h"
#include "city.h"

#ifndef __Trip__
#define __Trip__

using namespace std;

//Classe che definisce l'elemento città.
class Trip{

   private:
   vector <City> m_trip; //Vettore di elementi City. Ha delle funzioni.
   int m_dim; //Dimensione del vettore = quanti elementi ci sono.
   double m_dist; //Norma del vettore m_trip, ossia la distanza, la lunghezza del percorso.
   
   protected:
   
   public:
   Trip(vector <City>);
   ~Trip(); //Costruttore e distruttore. 

   vector <City> mtrip(); //Funzione che restituisce il vettore di City m_trip, data member della classe.   
   int dimension(); //Dimensione del vettore m_trip, data member della classe;
   double norm(); //Funzione che calcola la norma del vettore m_trip, ossia la distanza del percorso. 
   double distance(); //Funzione che restituisce la distanza del vettore, ossia il data member m_dist.
   
   int label(int); //label dell'elemento City data la sua posizione nel vettore.
   double x(int); //x dell'elemento City data la sua posizione nel vettore.
   double y(int); //y dell'elemento City data la sua posizione nel vettore.
   
   void check_first(); //Funzione che controlla che il primo elemento del vettore sia sempre il primo.
   void check_equal(); //Funzione che controlla che tutti gli elementi del vettore siano diversi tra loro.
   void check_after_mutation(); //Funzione che verifica le due condizioni sopra e calcola la nuova distanza.
   
   void exchange_user(int, int); //Scambia due componenti del vettore scelte dall'utente.
   void exchange_couple(int, int); //Scambia due componenti del vettore scelte random.
   void shift_one(); //Sposta in avanti di una posizione tutte le componenti del vettore tranne la prima.
   void shift_enne(int); //Sposta in avanti di enne posizioni le componenti del vettore tranne la prima.
   void exchange_block(int, int, int); //Scambia due blocchi di elementi del vettore, fissato l'inizio e il salto.
   void inversion(int, int); //Inverte gli elementi in un blocco del vettore, dati i due estremi.
   
   bool give(int); //Comunica se un elemento del vettore sia più grande o più piccolo del suo successivo.
		
};

#endif // __Trip__

/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/