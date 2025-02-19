/*  Copyright (C) <2016>  <L-Galaxies>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/> */

/*
 *  Created in: 2008
 *      Author: Bruno Henriques
 */

#include <gsl/gsl_rng.h>
#include <gsl/gsl_sf.h>
#include <gsl/gsl_statistics.h>
#include <gsl/gsl_histogram.h>
#include <gsl/gsl_histogram2d.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_deriv.h>



// Variables for the MCMC sampling

int MCMCNpar;                   //Number of parameters to sample
constexpr auto MCMCNConstraints = 27;   //Nr of Observational Constraints
constexpr auto MCMCMaxObsBins = 100;    //maximum number of bins per observation per redshift

double MCMCConstraintsZZ[NOUT];

//Nr of SAM galaxies used to compare with data
int TotMCMCGals[NOUT];

//To allocate structure with SAM Galaxies
constexpr auto MCMCAllocFactor = 200000;

//constexpr auto MCMCAllocFactor= 1000000;
long MCMCseed;
int Nbins[NOUT][MCMCNConstraints];      //bins on each obs test
double lhood1;

int NFofsInSample[NOUT];
int UsedFofsInSample[NOUT];

#ifdef MR_PLUS_MRII
int NTrees_Switch_MR_MRII;
int Switch_MR_MRII;
#endif
int CurrentMCMCStep;
int GlobalMCMCStep;

time_t global_starting_time;

FILE *FILE_MCMC_LIKELIHOOD;
FILE *FILE_MCMC_PredictionsPerStep[NOUT][MCMCNConstraints];

//READ FROM input.par
char MCMCStartingParFile[512];
char MCMCParPriorsAndSwitchesFile[512];
char MCMCObsConstraints[512];
char MCMCWeightsObsConstraints[512];
char ObsConstraintsDir[512];
char MCMCSampleDir[512];
char MCMCSampleFilePrefix[512];
int MCMCSampleFile;

#ifdef MR_PLUS_MRII
char MCMCSampleFilePrefix_MR[512];
char MCMCSampleFilePrefix_MRII[512];
int MCMCSampleFile_MR;
int MCMCSampleFile_MRII;
#endif
#ifdef HALOMODEL
char MCMCHaloModelDir[512];
#endif
int MCMCTreeSampleFile;
int ChainLength;
int FirstChainNumber;
int Time_Dependent_PhysPar;
int MCMCMode;
double MCMC_LogStep_Size;
double MCMC_Initial_Par_Displacement;
double MCMC_Minimum_Obs_Error;
double AddedErrOnMass;
double MachineTimeOut;
char JobSubmitCommand[512];
char JobSubmitPipe[512];
char JobSubmitFile[512];

//Structures for the MCMC
struct MCMC_OBSCONSTRAINTS
{
  char Name[1000];
  char TestType[1000];
  //for chi-square like tests
  double Bin_low[NOUT][MCMCMaxObsBins];
  double Bin_high[NOUT][MCMCMaxObsBins];
  double Obs[NOUT][MCMCMaxObsBins];
  double Error[NOUT][MCMCMaxObsBins];
  //for binomial like tests
  double ObsUp[NOUT][MCMCMaxObsBins];
  double ObsDown[NOUT][MCMCMaxObsBins];
  int ObsTest_Switch_z[NOUT];
  double ObsTest_Weight_z[NOUT];
} *MCMC_Obs;

struct MCMC_GALAXY
{
  float StellarMass[NOUT];
  float ColdGas[NOUT];
  float BulgeMass[NOUT];
  float BlackHoleMass[NOUT];
  float Sfr[NOUT];
  float MagU[NOUT];
  float MagB[NOUT];
  float MagV[NOUT];
  float MagJ[NOUT];
  float MagK[NOUT];
  float Magu[NOUT];
  float Magg[NOUT];
  float Magr[NOUT];
  float Magi[NOUT];
  float Magz[NOUT];
  float Weight[NOUT];
#ifdef HALOMODEL
  int fofid[NOUT];
  float M_Crit200[NOUT];
  float M_Mean200[NOUT];
  float x[NOUT];
  float y[NOUT];
  float z[NOUT];
  int Type[NOUT];
  int ngal[NOUT];
#endif
} *MCMC_GAL;


struct MCMC_PAR
{
  char Name[1000];
  double Value[NOUT];
  double PropValue[NOUT];
  double PriorMin;
  double PriorMax;
  int Sampling_Switch;
} *MCMC_PAR;

struct MCMC_FOF_struct
{
  //values for the sample of FoF groups
  double Weight[NOUT];
  long long FoFID[NOUT];
#ifdef HALOMODEL
  float M_Crit200[NOUT];
  float M_Mean200[NOUT];
  int NGalsInFoF[NOUT];
  int IndexOfCentralGal[NOUT];
#endif
} *MCMC_FOF, *MCMC_FOF2;



//Variables to construct the halo model and compute
//the correlation function for a sample of galaxies
#ifdef HALOMODEL

int *HashTable;

int NR;
constexpr auto massbins = 65;

constexpr auto minfofmass = 9.5;
constexpr auto maxfofmass = 16.;

int NTasks, ThisTask;
int *numbymass;
int **indexbymass;

int offset, numrad;

double Norm;
double rho_mean, Mstar, Rstar, Delta_invth, rho_c, delta_c;

//constexpr auto rho_c= 2.775e11; //recalculated value (3*H^2/8*PI*G in units of (Msun/h)/(Mpc/h)^3)
constexpr auto Delta = 200.;

//constexpr auto delta_c 1.674 //Millennium cosmology
//double G=4.302e-9;

double ngal_mean;
struct M_params
{
  double k;
  int i;
  int j;
};
struct N_params
{
  int j;
};
struct A_params
{
  double R;
  int i;
  int j;
};
struct mugal_qawo_params
{
  double pa;
  double pb;
  double pc;
};
struct conv_W_P_params
{
  double k;
  double q;
  int censat;
};
struct conv_W_params
{
  double k;
  double R;
  int censat;
};
struct rhalo_params
{
  int censat;
  int norm;
};
struct rsat_params
{
  double lm;
  int extrar;
};
double *cutoff_low, *cutoff_high;
double *kPowerTable, *PowerTable;
char rndsuffix[64];
gsl_interp_accel *SigmaAcc, *PowAcc, *nuAcc;
gsl_spline *SigmaSpline, *PowSpline, *nuSpline;
gsl_interp_accel **NgalAcc;
gsl_spline **NgalSpline;
gsl_interp_accel *ellipAcc;
gsl_spline *ellipSpline;
gsl_interp_accel *alignAcc;
gsl_spline *alignSpline;
gsl_interp_accel *NewpowAcc;
gsl_spline *NewpowSpline;
gsl_interp_accel *CorrAcc;
gsl_spline *CorrSpline;
gsl_interp_accel *Twopow_ccAcc;
gsl_spline *Twopow_ccSpline;
gsl_interp_accel *Twopow_csAcc;
gsl_spline *Twopow_csSpline;
gsl_interp_accel *Twopow_ssAcc;
gsl_spline *Twopow_ssSpline;
gsl_interp_accel *TwopcorrAcc;
gsl_spline *TwopcorrSpline;
double cutoff_fof_low, cutoff_fof_high;
gsl_interp_accel *FofAcc;
gsl_spline *FofSpline;
const gsl_rng_type *T_rng;
gsl_rng *r_rng;
double fitconst;

double parscutoff_low, parscutoff_high;
gsl_interp_accel *paAcc, *pbAcc, *pcAcc;
gsl_spline *paSpline, *pbSpline, *pcSpline;


#endif //END of variables to construct the halo model
