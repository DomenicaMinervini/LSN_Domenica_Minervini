/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/

#ifndef __City__
#define __City__

//Classe che definisce l'elemento città.
class City{

   private:
   int m_label; //Etichetta la città.
   double m_x, m_y; //Coordinate della città.
   
   protected:
   
   public:   
   //Costruttore e distruttore.
   City(int, double, double);
   ~City();
   
   //Funzioni che comunicano l'etichetta e le coordinate.
   int Get_label();
   double Get_x();
   double Get_y();
	
};

#endif // __City__

/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/
