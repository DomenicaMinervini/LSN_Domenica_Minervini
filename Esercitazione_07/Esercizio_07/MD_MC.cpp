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
#include "MD_MC.h"

using namespace std;

int main()
{  
  Reset_gr(); //Vector g(r) is initialized at zero 
	
  Input(); //Inizialization
  int nconf = 1;
  
  Equilibration(); //Equilibration before simulation
  cout << "Equilibration done." << endl << endl;
  cout << "---------------------------" << endl << endl; 
  
  Reset_gr(); //Reset g(r) after equilibration

  for(int iblk=1; iblk <= nblk; iblk++) //Simulation
  {
    Reset(iblk);   //Reset block averages
    for(int istep=1; istep <= nstep; istep++)
    {
      Move();
      Measure();
      Accumulate(); //Update block averages
      if(istep%10 == 0){
//        ConfXYZ(nconf);//Write actual configuration in XYZ format //Commented to avoid "filesystem full"! 
        nconf += 1;
      }
    }

    //Averages_test(iblk); //Print 500000 values of U/N
    //Averages_gr(iblk); //Compute g(r)
    Averages(iblk);   //Print results for current block
	
  }
  
  ConfFinal(); //Write final configuration

  return 0;
}


void Input(void)
{
  ifstream ReadInput, ReadConf, ReadVelocity, Primes, Seed;

  cout << "Classic Lennard-Jones fluid        " << endl;
  cout << "MD(NVE) / MC(NVT) simulation       " << endl << endl;
  cout << "Interatomic potential v(r) = 4 * [(1/r)^12 - (1/r)^6]" << endl << endl;
  cout << "Boltzmann weight exp(- beta * sum_{i<j} v(r_ij) ), beta = 1/T " << endl << endl;
  cout << "The program uses Lennard-Jones units " << endl;

//Read seed for random numbers
  int p1, p2;
  Primes.open("Primes");
  Primes >> p1 >> p2 ;
  Primes.close();

//Read input informations
  ReadInput.open("input.solid");
  //ReadInput.open("input.liquid");
  //ReadInput.open("input.gas");

  ReadInput >> iNVET;

  ReadInput >> restart;

  if(restart) Seed.open("seed.out");
  else Seed.open("seed.in");
  Seed >> seed[0] >> seed[1] >> seed[2] >> seed[3];
  rnd.SetRandom(seed,p1,p2);
  Seed.close();

  ReadInput >> temp;
  beta = 1.0/temp;
  cout << "Temperature = " << temp << endl;

  ReadInput >> npart;
  cout << "Number of particles = " << npart << endl;

  ReadInput >> rho;
  cout << "Density of particles = " << rho << endl;
  
  vol = (double)npart/rho;
  box = pow(vol,1.0/3.0);
  cout << "Volume of the simulation box = " << vol << endl;
  cout << "Edge of the simulation box = " << box << endl;

  ReadInput >> rcut;
  cout << "Cutoff of the interatomic potential = " << rcut << endl << endl;
    
  ReadInput >> delta;

  ReadInput >> nblk;

  ReadInput >> nstep;

  ReadInput >> neq;

  cout << "The program perform Metropolis moves with uniform translations" << endl;
  cout << "Moves parameter = " << delta << endl;
  cout << "Number of blocks = " << nblk << endl;
  cout << "Number of steps in one block = " << nstep << endl;
  cout << "Number of steps for equilibration = " << neq << endl << endl;
  ReadInput.close();
  
//Tail corrections
  v_tail = V_tail();
  p_tail = P_tail();
  cout << " V Tail =  " << v_tail << ", P Tail =  " << p_tail << endl << endl;   

//Prepare arrays for measurements
  iv = 0; //Potential energy
  it = 1; //Temperature
  ik = 2; //Kinetic energy
  ie = 3; //Total energy
  iw = 4; //Pressure
  n_props = 5; //Number of observables

//Read initial configuration
  cout << "Read initial configuration" << endl << endl;
  if(restart)
  {
    ReadConf.open("config.out");
    ReadVelocity.open("velocity.out");
    for (int i=0; i<npart; ++i) ReadVelocity >> vx[i] >> vy[i] >> vz[i];
  }
  else 
  {
    ReadConf.open("config.in");
    cout << "Prepare velocities with center of mass velocity equal to zero " << endl;
    double sumv[3] = {0.0, 0.0, 0.0};
    for (int i=0; i<npart; ++i)
    {
      vx[i] = rnd.Gauss(0.,sqrt(temp));
      vy[i] = rnd.Gauss(0.,sqrt(temp));
      vz[i] = rnd.Gauss(0.,sqrt(temp));
      sumv[0] += vx[i];
      sumv[1] += vy[i];
      sumv[2] += vz[i];
    }
    for (int idim=0; idim<3; ++idim) sumv[idim] /= (double)npart;
    double sumv2 = 0.0, fs;
    for (int i=0; i<npart; ++i)
    {
      vx[i] = vx[i] - sumv[0];
      vy[i] = vy[i] - sumv[1];
      vz[i] = vz[i] - sumv[2];
      sumv2 += vx[i]*vx[i] + vy[i]*vy[i] + vz[i]*vz[i];
    }
    sumv2 /= (double)npart;
    fs = sqrt(3 * temp / sumv2);   // fs = velocity scale factor 
    cout << "velocity scale factor: " << fs << endl << endl;
    for (int i=0; i<npart; ++i)
    {
      vx[i] *= fs;
      vy[i] *= fs;
      vz[i] *= fs;
    }
  }

  for (int i=0; i<npart; ++i)
  {
    ReadConf >> x[i] >> y[i] >> z[i];
    x[i] = Pbc( x[i] * box );
    y[i] = Pbc( y[i] * box );
    z[i] = Pbc( z[i] * box );
  }
  ReadConf.close();

  for (int i=0; i<npart; ++i)
  {
    if(iNVET)
    {
      xold[i] = x[i];
      yold[i] = y[i];
      zold[i] = z[i];
    }
    else
    {
      xold[i] = Pbc(x[i] - vx[i] * delta);
      yold[i] = Pbc(y[i] - vy[i] * delta);
      zold[i] = Pbc(z[i] - vz[i] * delta);
    }
  }
  
//Evaluate properties of the initial configuration
  Measure();

//Print initial values for measured properties
  cout << "Initial potential energy = " << walker[iv]/(double)npart << endl;
  /*cout << "Initial temperature      = " << walker[it] << endl;
  cout << "Initial kinetic energy   = " << walker[ik]/(double)npart << endl;
  cout << "Initial total energy     = " << walker[ie]/(double)npart << endl;*/
  cout << "Initial pressure         = " << walker[iw] << endl;
  cout << "---------------------------" << endl << endl; 

  return;
}

//INCLUDE YOUR CODE HERE

void Reset_gr(){
  int k;
  for(k=0; k<tot_bin; k++){
    gr[k] = 0;  
	gr_av[k] = 0;
  }	
}

void Equilibration()
{
  int i, j; //contatori
  for(i=0; i<neq; i++) 
  { //i cicli che portano il sistema all'equilibrio sono neq
    for(j=0; j<nstep; j++)
    { //per tutti i valori del blocco
      Move();
    }
  }
}

void Move()
{
  int o;
  double p, energy_old, energy_new;
  double xnew, ynew, znew;

  if(iNVET) // Monte Carlo (NVT) move
  {
    for(int i=0; i<npart; ++i)
    {
    //Select randomly a particle (for C++ syntax, 0 <= o <= npart-1)
      o = (int)(rnd.Rannyu()*npart);

    //Old
      energy_old = Boltzmann(x[o],y[o],z[o],o);

    //New
      x[o] = Pbc( x[o] + delta*(rnd.Rannyu() - 0.5) );
      y[o] = Pbc( y[o] + delta*(rnd.Rannyu() - 0.5) );
      z[o] = Pbc( z[o] + delta*(rnd.Rannyu() - 0.5) );

      energy_new = Boltzmann(x[o],y[o],z[o],o);

    //Metropolis test
      p = exp(beta*(energy_old-energy_new));
      if(p >= rnd.Rannyu())  
      {
      //Update
        xold[o] = x[o];
        yold[o] = y[o];
        zold[o] = z[o];
        accepted = accepted + 1.0;
      } else {
        x[o] = xold[o];
        y[o] = yold[o];
        z[o] = zold[o];
      }
      attempted = attempted + 1.0;
    }
  } else // Molecular Dynamics (NVE) move
  {
    double fx[m_part], fy[m_part], fz[m_part];

    for(int i=0; i<npart; ++i){ //Force acting on particle i
      fx[i] = Force(i,0);
      fy[i] = Force(i,1);
      fz[i] = Force(i,2);
    }

    for(int i=0; i<npart; ++i){ //Verlet integration scheme

      xnew = Pbc( 2.0 * x[i] - xold[i] + fx[i] * pow(delta,2) );
      ynew = Pbc( 2.0 * y[i] - yold[i] + fy[i] * pow(delta,2) );
      znew = Pbc( 2.0 * z[i] - zold[i] + fz[i] * pow(delta,2) );

      vx[i] = Pbc(xnew - xold[i])/(2.0 * delta);
      vy[i] = Pbc(ynew - yold[i])/(2.0 * delta);
      vz[i] = Pbc(znew - zold[i])/(2.0 * delta);

      xold[i] = x[i];
      yold[i] = y[i];
      zold[i] = z[i];

      x[i] = xnew;
      y[i] = ynew;
      z[i] = znew;

      accepted = accepted + 1.0;
      attempted = attempted + 1.0;
    }
  }
  return;
}

double Boltzmann(double xx, double yy, double zz, int ip)
{
  double ene=0.0;
  double dx, dy, dz, dr;

  for (int i=0; i<npart; ++i)
  {
    if(i != ip)
    {
// distance ip-i in pbc
      dx = Pbc(xx - x[i]);
      dy = Pbc(yy - y[i]);
      dz = Pbc(zz - z[i]);

      dr = dx*dx + dy*dy + dz*dz;
      dr = sqrt(dr);

      if(dr < rcut)
      {
        ene += 1.0/pow(dr,12) - 1.0/pow(dr,6);
      }
    }
  }

  return 4.0*ene;
}

double Force(int ip, int idir){ //Compute forces as -Grad_ip V(r)
  double f=0.0;
  double dvec[3], dr;

  for (int i=0; i<npart; ++i){
    if(i != ip){
      dvec[0] = Pbc( x[ip] - x[i] );  // distance ip-i in pbc
      dvec[1] = Pbc( y[ip] - y[i] );
      dvec[2] = Pbc( z[ip] - z[i] );

      dr = dvec[0]*dvec[0] + dvec[1]*dvec[1] + dvec[2]*dvec[2];
      dr = sqrt(dr);

      if(dr < rcut){
        f += dvec[idir] * (48.0/pow(dr,14) - 24.0/pow(dr,8)); // -Grad_ip V(r)
      }
    }
  }
 
  return f;
}

void Measure() //Properties measurement
{
//INCLUDE YOUR CODE HERE
  double dx, dy, dz, dr; //coordinate spaziali e distanza dall'origine
  double kin = 0.0; //energia cinetica
  double v=0.0, vij=0.0; //energia potenziale come Lennard-Jones: totale e per la coppia i-j
  double w=0.0, wij=0.0; //pressione come Lennard-Jones: totale e per la coppia i-j
  
  max_dist = box/2.0; //Distanza massima a cui si possono trovare le particelle i-j.

//cycle over pairs of particles
  for (int i=0; i<npart-1; ++i)
  {
    for (int j=i+1; j<npart; ++j)
    {
// distance i-j in pbc
      dx = Pbc(x[i] - x[j]);
      dy = Pbc(y[i] - y[j]);
      dz = Pbc(z[i] - z[j]);

      dr = dx*dx + dy*dy + dz*dz;
      dr = sqrt(dr);
	  
//INCLUDE YOUR CODE HERE
/*Codice per il calcolo della funzione di distrubuzione radiale g(r).
Sopra è calcolata dr, la distanza tra le due particelle i-j: i è al centro della scatola.
Questa distanza è compresa in un cerchio di raggio box/2, ossia metà lato della scatola: dr è compreso in (0,box/2). 
L'intervallo è diviso in 100 bin di uguale lunghezza: box/200. Il numero totale di bin è 100.*/

/*Per calcolare in quale bin cade la particella j-esima applico la relazione di proporzionalità:
NUMERO DEL BIN DELLA PARTICELLA j-ESIMA : NUMERO TOTALE DI BIN = LUNGHEZZA dr TRA i E j : LUNGHEZZA TOTALE IN CUI CERCO j.
Quando il bin viene estratto incremento il conteggio di g(r) per quella posizione di 2 perché il codice, per efficienza, 
calcola solo il termine i-j. Moltiplicando per 2 considero anche il termine j-i che è simmetrico. Considero la coppia.*/

  if(dr < max_dist){ //Se dr è nel limite stabilito, puoi calcolare g(r).

      bin = int(tot_bin*dr/max_dist); //Numero del bin estratto
      gr[bin] += 2; //Incremento di due il bin estratto
  }

     if(dr < rcut)
      { 
        vij = 1.0/pow(dr,12) - 1.0/pow(dr,6); //energia potenziale per la coppia i-j
        v += vij; //l'energia potenziale totale è data dalla somma di tutte le energie di coppia

//INCLUDE YOUR CODE HERE
        wij = 48.0/pow(dr,12) - 24.0/pow(dr,6); //pressione per la coppia i-j
        w += wij; //la pressione totale è data dalla somma di tutte le pressioni di coppia
      }
    }          
  }

  for (int i=0; i<npart; ++i) kin += 0.5 * (vx[i]*vx[i] + vy[i]*vy[i] + vz[i]*vz[i]);

  walker[iv] = 4.0 * v; //Potential energy
  /*walker[ik] = kin; //Kinetic energy
  walker[it] = (2.0/3.0) * kin/(double)npart; //Temperature
  walker[ie] = 4.0 * v + kin;  //Total energy*/
  walker[iw] = rho*walker[it] + (1.0/(3.0*vol))*(w/(double)npart); //Pressure

  return;
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

//INCLUDE YOUR CODE HERE 
double V_tail(){ //Correzione di coda per il potenziale
   return 8.0*pi*rho*rho*((1.0/(9.0*pow(rcut,9)))-(1.0/(3.0*pow(rcut,3))));
}

double P_tail(){ //Correzione di coda per la pressione
   return 32.0*pi*rho*rho*((1.0/(9.0*pow(rcut,9)))-(1.0/(3.0*pow(rcut,3))));
}

//INCLUDE YOUR CODE HERE
void Averages_test(int iblk){ 
    //Stampa i 500000 valori di U/N per fare il test della lunghezza ideale del blocco.
	//La simulazione dura 45 minuti. Commentare nella funzione main se non serve. 
	
    ofstream Test;
    Test.open("test_epot_solid_MC.dat",ios::app);
    //Test.open("test_epot_liquid_MC.dat",ios::app);
    //Test.open("test_epot_gas_MC.dat",ios::app);

	
	//Energia potenziale per particella.
    stima_pot = blk_av[iv]/blk_norm/(double)npart + v_tail; 
	
	//File output << Blocco numero: " << "  " << media di blocco.
	//Siccome il blocco è costituito da un solo elemento, la media coincide con l'elemento stesso.
    Test << iblk <<  "  " << stima_pot << endl;

    Test.close();	
	
    return;	
}

void Averages_gr(int iblk){ 
    /*Calcola la g(r) media. g(r) è un vettore di 100 elementi.
	Il risultato al blocco i-esimo è il valor medio su i cicli di ciascuno dei 100 elementi
    Calcolo g(r) normalizzata per tutti i blocchi, per tutti i valori dell'intervallo.*/   

    delta_r = max_dist/tot_bin; //Risoluzione dell'esperimento, ossia la larghezza del bin.
	
	ofstream Gr;
	Gr.open("gr_solid_MC.dat",ios::app);
	//Gr.open("gr_liquid_MC.dat",ios::app);
	//Gr.open("gr_gas_MC.dat",ios::app);
	
	for(int i=0; i<tot_bin; i++){
	  r = i*delta_r; //E' il numero di bin che precedono il bin in cui sto calcolando g(r).
	  gr_norm = rho*npart*PI_SF*(pow(r+delta_r,3) - pow(r,3)); //Normalizzazione di g(r) per il blocco i-esimo.
	  
	  if(gr_norm == 0){ //Se la norma fa 0, per costruzione la faccio diventare 1.
        gr_norm = 1;    //Così evito la divisione per 0 e il valore medio gr_av[0] è gr[0].
	  }
	  
      gr_av[i]  = gr[i]/gr_norm/blk_norm; //Vettore con le medie.
      gr_av2[i] = gr_av[i]*gr_av[i]; //Vettore con i quadrati delle medie.
	  err_gr[i] = Error(gr_av[i],gr_av[i],iblk); //Vettore con gli errori.*/
	  
	  /*gr[i]: numero di conteggi del bin i-esimo per la funzione di distribuzione radiale g(r).
	  gr_av[i]: numero di conteggi del bin i-esimo per la funzione g(r), dopo aver applicato la normalizzazione.	
      Il valore i-esimo del vettore è dato dalla media dei valori i-esimi del vettore calcolati in tutti i cicli precedenti.
	  err_gr[i]: incertezza statistica per il bin i-esimo del vettore gr_av[i]; incertezza su g(r).*/

      if(iblk == 20){ //Stampo i valori di g(r) solo per l'ultimo blocco.  
	     Gr << r << "  " << gr_av[i]/(double)iblk << "  " << err_gr[i] << endl;
      }	  
    }
	
    Gr.close();
	
	return;

}


void Averages(int iblk) //Print results for current block
{
    //ofstream Epot, Ekin, Etot, Temp, Pres;
    //const int wd=12;
    ofstream Epot, Pres;

    cout << "Block number " << iblk << endl;
    cout << "Acceptance rate " << accepted/attempted << endl << endl;

    Epot.open("output_epot_solid_MC.dat",ios::app);
    /*Ekin.open("output_ekin.dat",ios::app);
    Etot.open("output_etot.dat",ios::app);
    Temp.open("output_temp.dat",ios::app);*/
    Pres.open("output_pres_solid_MC.dat",ios::app);
    
    stima_pot = blk_av[iv]/blk_norm/(double)npart + v_tail; //Potential energy
    glob_av[iv] += stima_pot;
    glob_av2[iv] += stima_pot*stima_pot;
    err_pot=Error(glob_av[iv],glob_av2[iv],iblk);
/*    
    stima_kin = blk_av[ik]/blk_norm/(double)npart; //Kinetic energy
    glob_av[ik] += stima_kin;
    glob_av2[ik] += stima_kin*stima_kin;
    err_kin=Error(glob_av[ik],glob_av2[ik],iblk);

    stima_etot = blk_av[ie]/blk_norm/(double)npart + v_tail; //Total energy
    glob_av[ie] += stima_etot;
    glob_av2[ie] += stima_etot*stima_etot;
    err_etot=Error(glob_av[ie],glob_av2[ie],iblk);

    stima_temp = blk_av[it]/blk_norm; //Temperature
    glob_av[it] += stima_temp;
    glob_av2[it] += stima_temp*stima_temp;
    err_temp=Error(glob_av[it],glob_av2[it],iblk);*/

//INCLUDE YOUR CODE HERE 
    stima_pres = blk_av[iw]/blk_norm + p_tail; //Pressure
    glob_av[iw] += stima_pres;
    glob_av2[iw] += stima_pres*stima_pres;
    err_pres=Error(glob_av[iw],glob_av2[iw],iblk);


//Potential energy per particle
    Epot << "  " << iblk <<  "  " << stima_pot << "  " << glob_av[iv]/(double)iblk << "  " << err_pot << endl;
//Kinetic energy per particle
/*    Ekin << "  " << iblk <<  "  " << stima_kin << "  " << glob_av[ik]/(double)iblk << "  " << err_kin << endl;
//Total energy per particle
    Etot << "  " << iblk <<  "  " << stima_etot << "  " << glob_av[ie]/(double)iblk << "  " << err_etot << endl;
//Temperature
    Temp << "  " << iblk <<  "  " << stima_temp << "  " << glob_av[it]/(double)iblk << "  " << err_temp << endl;*/
//Pressure
    Pres << "  " << iblk <<  "  " << stima_pres << "  " << glob_av[iw]/(double)iblk << "  " << err_pres << endl;

    cout << "---------------------------" << endl << endl; 

    Epot.close();
    /*Ekin.close();
    Etot.close();
    Temp.close();*/
    Pres.close();
	
	return;

}


void ConfFinal(void)
{
  ofstream WriteConf, WriteVelocity, WriteSeed;

  cout << "Print final configuration to file config.out" << endl << endl;
  WriteConf.open("config.out");
  WriteVelocity.open("velocity.out");
  for (int i=0; i<npart; ++i)
  {
    WriteConf << x[i]/box << "   " <<  y[i]/box << "   " << z[i]/box << endl;
    WriteVelocity << vx[i] << "   " <<  vy[i] << "   " << vz[i] << endl;
  }
  WriteConf.close();
  WriteVelocity.close();

  rnd.SaveSeed();
}

void ConfXYZ(int nconf){ //Write configuration in .xyz format
  ofstream WriteXYZ;

  WriteXYZ.open("frames/config_" + to_string(nconf) + ".xyz");
  WriteXYZ << npart << endl;
  WriteXYZ << "This is only a comment!" << endl;
  for (int i=0; i<npart; ++i){
    WriteXYZ << "LJ  " << Pbc(x[i]) << "   " <<  Pbc(y[i]) << "   " << Pbc(z[i]) << endl;
  }
  WriteXYZ.close();
}

double Pbc(double r)  //Algorithm for periodic boundary conditions with side L=box
{
    return r - box * rint(r/box);
}

double Error(double sum, double sum2, int iblk)
{
    return sqrt(fabs(sum2/(double)iblk - pow(sum/(double)iblk,2))/(double)iblk);
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
