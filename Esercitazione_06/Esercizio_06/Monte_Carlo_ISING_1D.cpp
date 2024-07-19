/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <iostream>
#include <fstream>
#include <ostream>
#include <cmath>
#include <iomanip>
#include "Monte_Carlo_ISING_1D.h"

using namespace std;

int main()
{ 
  Input(); //Inizialization
  for(int t=0; t<sim; ++t) //faccio la simulazione per ciascuna delle 31 temperature
  { 
    cout << "Equilibrazione..." << endl; 
    Equilibration(metro,t); //ogni simulazione inizia con l'equilibrazione
    for(int iblk=1; iblk <= nblk; ++iblk) //Simulation
    {
      Reset(iblk);   //Reset block averages
      for(int istep=1; istep <= nstep; ++istep)
      {
        Move(metro,t);
        Measure();
        Accumulate(); //Update block averages
      }
      Averages(iblk,t);   //Print results for current block
    }
    cout << t << "  " << temp[t] << "  " << beta[t] << endl;
  }
  
  ConfFinal(); //Write final configuration
  cout << "There are now some new files '.dat' in the folder." << endl; 
  return 0;
}


void Input(void)
{
  ifstream ReadInput;

  cout << "Classic 1D Ising model             " << endl;
  cout << "Monte Carlo simulation             " << endl << endl;
  cout << "Nearest neighbour interaction      " << endl << endl;
  cout << "Boltzmann weight exp(- beta * H ), beta = 1/T " << endl << endl;
  cout << "The program uses k_B=1 and mu_B=1 units " << endl;

//Read seed for random numbers
   int p1, p2;
   ifstream Primes("Primes");
   Primes >> p1 >> p2;
   Primes >> p1 >> p2;
   Primes.close();

   ifstream input("seed.in");
   input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
   rnd.SetRandom(seed,p1,p2);
   input.close();
  
//Read input informations
  ReadInput.open("input.dat");

  ReadInput >> low_t;

  for(int j=0; j<sim; ++j){
    if(j == 0){
      temp[j] = low_t;
    }
    else{
	  temp[j] = temp[j-1] + 0.05;
    }
    beta[j] = 1.0/temp[j];
  }
  cout << "Lower Temperature, beta " << temp[0] << "  " << beta[0] << endl;
  cout << "Higher Temperature, beta " << temp[sim-1] << "  " << beta[sim-1] << endl;

  ReadInput >> nspin;
  cout << "Number of spins = " << nspin << endl;

  ReadInput >> J;
  cout << "Exchange interaction = " << J << endl;

  ReadInput >> h;
  cout << "External field = " << h << endl << endl;
    
  ReadInput >> metro; // if=1 Metropolis else Gibbs

  ReadInput >> nblk;

  ReadInput >> nstep;

  if(metro==1) cout << "The program perform Metropolis moves" << endl;
  else cout << "The program perform Gibbs moves" << endl;
  cout << "Number of blocks = " << nblk << endl;
  cout << "Number of steps in one block = " << nstep << endl << endl;
  ReadInput.close();


//Prepare arrays for measurements
  iu = 0; //Energy
  ic = 1; //Heat capacity
  im = 2; //Magnetization
  ix = 3; //Magnetic susceptibility
 
  n_props = 4; //Number of observables

//initial configuration
  if(restart==1){
    cout << "The simulation will restart from the configuration saved in 'config.final'." << endl;
    ifstream config("config.final");
    if(config.is_open()==0){
      cout << "Error opening the file 'config.final'." << endl;
    }
	
    for (int i=0; i<nspin; ++i)
	{
      config >> s[i]; //la configurazione è quella salvata in precedenza sul file
    }	
  }
  else //la configuazione è quella generata dal codice
  {
  for (int i=0; i<nspin; ++i)
  {
    if(rnd.Rannyu() >= 0.5) s[i] = 1;
    else s[i] = -1;
  }
  }
  
//Evaluate energy etc. of the initial configuration
  Measure();

//Print initial values for the potential energy and virial
  cout << "Initial energy = " << walker[iu]/(double)nspin << endl;
}

void Equilibration(int metro,int t)
{
   for(int i=0;i<eqstep;i++)
   {
      Move(metro,t);
   }	   	
}


void Move(int metro,int t)
{
  int o;
  double energy_old, energy_new, sm; //Metropolis
  //int sum_spin, bath_spin; //Gibbs

//INCLUDE YOUR CODE HERE
  double p; //rapporto tra le probabilita', Metropolis e Gibbs
  double A; //probabilita' minima tra 1 e rapp
  double value; //valore estratto tra 0 e 1 da confrontare con A

  for(int i=0; i<nspin; ++i)
  {
  //Select randomly a particle (for C++ syntax, 0 <= o <= nspin-1)
    o = (int)(rnd.Rannyu()*nspin);

    if(metro==1) //Metropolis
    {
// INCLUDE YOUR CODE HERE
      //lo spin iniziale e' sm e decidero' se cambiarlo o no; lo spin modificato e' -sm: se uno fa +1, l'altro fa -1
      sm=s[o]; 

      //calcolo le energie nuova e vecchia
      energy_old=Boltzmann(sm,o);
      energy_new=Boltzmann(-sm,o);

      //calcolo il rapporto tra le probabilita' tra mossa nuova e mossa vecchia
      p=exp(-beta[t]*(energy_new-energy_old));

      //accetto il minimo tra rapp e 1
      if(p<1.0)  A=p;
      else  A=1.0;

      //APPLICO L'ALGORITMO DI METROPOLIS
      //estraggo un valore in una distribuzione uniforme tra 0 e 1
      //se A e' maggiore del valore estratto, accetto la mossa nuova
      //altrimenti tengo la configurazione in cui mi trovo
      value=rnd.Rannyu();

      if(value<=A){ //accetto la mossa: la configurazione nuova diventa quella attuale
        s[o]=-sm;
        accepted++; //tentativi accettati
      }
      attempted++; //tentativi totali

    }
    else //Gibbs sampling
    {
// INCLUDE YOUR CODE HERE

      p = 1.0/(1.0+exp(-2.0*beta[t]*(J*(s[Pbc(o-1)]+s[Pbc(o+1)])+h))); //probabilità che lo spin sia verso l'alto, +1
      //adesso è come con l'algoritmo di Metropolis: estraggo un valore in una distribuzione uniforme tra 0 e 1:
      //se p e' maggiore del valore estratto, accetto la mossa descritta da p: +1, 
	  //altrimenti assegno la configurazione opposta: -1
      value=rnd.Rannyu();

      if(value<=p){ //accetto la mossa: la configurazione nuova diventa quella quella con spin up
        s[o]=+1;
	  }
	  else{
	    s[o]=-1; //altrimenti prendo la configuazione opposta, quella con spin down
	  }	  
	  accepted++; //tentativi accettati      	  
      attempted++; //tentativi totali
    }
  }
}

double Boltzmann(int sm, int ip)
{
  double ene = -J * sm * ( s[Pbc(ip-1)] + s[Pbc(ip+1)] ) - h * sm;
  return ene;
}

void Measure()
{
  double u = 0.0, m = 0.0;

//cycle over spin
  for (int i=0; i<nspin; ++i)
  {
     u += -J * s[i] * s[Pbc(i+1)] - 0.5 * h * (s[i] + s[Pbc(i+1)]); //energia
// INCLUDE YOUR CODE HERE
     m += s[i]; //magnetizzazione
  }
// INCLUDE YOUR CODE HERE
  walker[iu] = u;
  walker[ic] = u*u;
  walker[im] = m;
  walker[ix] = m*m;
}


void Reset(int iblk) //Reset block averages
{
   
   if(iblk == 1)
   {
     for(int i=0; i<n_props; ++i)
     {
       glob_av[i] = 0;
       glob_av2[i] = 0;
     }
   }

   for(int i=0; i<n_props; ++i)
   {
     blk_av[i] = 0;
   }
   blk_norm = 0;
   attempted = 0;
   accepted = 0;
}


void Accumulate(void) //Update block averages
{

   for(int i=0; i<n_props; ++i)
   {
     blk_av[i] = blk_av[i] + walker[i];
   }
   blk_norm = blk_norm + 1.0;
}


void Averages(int iblk,int t) //Print results for current block
{
    
   ofstream Acc, Ene, Heat, Mag, Chi; //per evoluzione delle medie a blocchi
   //const int wd=12;
   double stima_u2; //valor medio di u al quadrato
   
   if(h == 0.02){ //calcolo solo la magnetizzazione
     //Mag.open("output.mag.0",ios::app);
     stima_m = blk_av[im]/blk_norm/(double)nspin; //Magnetizzazione
     glob_av[im]  += stima_m;
     glob_av2[im] += stima_m*stima_m;
     err_m=Error(glob_av[im],glob_av2[im],iblk);
     //Mag << setw(wd) << iblk <<  setw(wd) << stima_m << setw(wd) << glob_av[im]/(double)iblk << setw(wd) << err_m << endl;
     //Mag.close();

     if(iblk == nblk){ //nel grafico metto la media del 20-esimo blocco
       cout << "h=0.02. Block number " << iblk << endl; 
       cout << "Sim number " << t << " Temperature " << temp[t] << endl;
       cout << "----------------------------" << endl << endl;

       Mag.open("mag.dat",ios::app);
       Mag << "  " << temp[t] << "  " << glob_av[im]/(double)iblk << "  " << err_m << endl;
       Mag.close();
     }
   }

   else{ //h=0: calcolo le altre tre grandezze
     //Ene.open("output.ene.0",ios::app);
     stima_u = blk_av[iu]/blk_norm/(double)nspin; //Energia per spin
     glob_av[iu]  += stima_u;
     glob_av2[iu] += stima_u*stima_u;
     err_u=Error(glob_av[iu],glob_av2[iu],iblk);
     //Ene << setw(wd) << iblk <<  setw(wd) << stima_u << setw(wd) << glob_av[iu]/(double)iblk << setw(wd) << err_u << endl;
     //Ene.close();

// INCLUDE YOUR CODE HERE
     
     //Heat.open("output.heat.0",ios::app);  //Calore specifico: (beta^2)*((<H^2>)-(<H>^2))
	 stima_u2 = (blk_av[ic]/blk_norm);
	 stima_c = (beta[t])*(beta[t])*(stima_u2-(blk_av[iu]/blk_norm)*(blk_av[iu]/blk_norm))/(double)nspin;	 
     glob_av[ic]  += stima_c;
     glob_av2[ic] += stima_c*stima_c;
     err_c=Error(glob_av[ic],glob_av2[ic],iblk);
     //Heat << setw(wd) << iblk <<  setw(wd) << stima_c << setw(wd) << glob_av[ic]/(double)iblk << setw(wd) << err_c << endl;
     //Heat.close();
	 
     //Chi.open("output.chi.0",ios::app);
     stima_x = (beta[t])*blk_av[ix]/blk_norm/(double)nspin; //Suscettività magnetica
     glob_av[ix]  += stima_x;
     glob_av2[ix] += stima_x*stima_x;
     err_x=Error(glob_av[ix],glob_av2[ix],iblk);
     //Chi << setw(wd) << iblk <<  setw(wd) << stima_x << setw(wd) << glob_av[ix]/(double)iblk << setw(wd) << err_x << endl;
     //Chi.close();


      if(iblk == nblk){ //nel grafico metto la media del 20-esimo blocco
        cout << "h=0. Block number " << iblk << endl; 
        cout << "Simulation number " << t << " Temperature " << temp[t] << " Beta " << beta[t] << endl;
        cout << "----------------------------" << endl << endl;

        Acc.open("acc.dat",ios::app);
        Acc << temp[t] << "  " << 100.0*accepted/attempted << endl;
        Acc.close();
  
        Ene.open("ene.dat",ios::app);
        Ene << "  " << temp[t] << "  " << glob_av[iu]/(double)iblk << "  " << err_u << endl;
        Ene.close();

        Heat.open("heat.dat",ios::app);
        Heat << "  " << temp[t] << "  " << glob_av[ic]/(double)iblk << "  " << err_c << endl;
        Heat.close();

        Chi.open("chi.dat",ios::app);
        Chi << "  " << temp[t] << "  " << glob_av[ix]/(double)iblk << "  " << err_x << endl;
        Chi.close();
      }
   }

}


void ConfFinal(void)
{
  ofstream WriteConf;

  cout << "Print final configuration to file config.final " << endl << endl;
  WriteConf.open("config.final");
  for (int i=0; i<nspin; ++i)
  {
    WriteConf << s[i] << endl;
  }
  WriteConf.close();

  rnd.SaveSeed();
}

int Pbc(int i)  //Algorithm for periodic boundary conditions
{
    if(i >= nspin) i = i - nspin;
    else if(i < 0) i = i + nspin;
    return i;
}

double Error(double sum, double sum2, int iblk)
{
    if(iblk==1) return 0.0;
    else return sqrt((sum2/(double)iblk - pow(sum/(double)iblk,2))/(double)(iblk-1));
}

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
