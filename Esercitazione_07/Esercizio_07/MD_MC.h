/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#ifndef __fluid__
#define __fluid__

//Random numbers
#include "random.h"
int seed[4];
Random rnd;
const double PI_SF = (4.0/3.0)*acos(-1);

//parameters, observables
const int m_props=1000;
int n_props, iv, ik, it, ie, iw;
double v_tail, p_tail, bin_size, nbins, sd;
double walker[m_props];

//Radial distribution function
double max_dist;
int tot_bin = 100, bin;
int gr[100];
double r, delta_r, gr_norm;
double gr_av[100], gr_av2[100], err_gr[100];

// averages
double blk_av[m_props], blk_norm, accepted, attempted;
double glob_av[m_props], glob_av2[m_props];
double stima_pot, stima_pres, stima_kin, stima_etot, stima_temp;
double err_pot, err_pres, err_kin, err_etot, err_temp;

//configuration
const int m_part=108;
double x[m_part],    y[m_part],    z[m_part];
double xold[m_part], yold[m_part], zold[m_part];
double vx[m_part],  vy[m_part],   vz[m_part];

// thermodynamical state
int npart;
double beta,temp,energy,vol,rho,box,rcut;

// simulation
int iNVET, nstep, nblk, restart, neq;
double delta;

//pigreco
const double pi=3.1415927;

//functions
void Input(void);
void Equilibration(void);
void Reset_gr();
void Reset(int);
void Accumulate(void);
void Averages_test(int);
void Averages_gr(int);
void Averages(int);
void Move(void);
void ConfFinal(void);
void ConfXYZ(int);
void Measure(void);
double Boltzmann(double, double, double, int);
double Pbc(double);
double Error(double,double,int);
double Force(int, int);
double V_tail(void);
double P_tail(void);

#endif

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
