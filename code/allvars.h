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

/** @file allvars.h
  */
#ifndef ALLVARS_H
#define ALLVARS_H

#include <cstdio>
#include <vector>
#include <gsl/gsl_rng.h>
#include <ctime>

constexpr auto MIN_ALLOC_NUMBER = 1000;
constexpr auto ALLOC_INCREASE_FACTOR = 1.1;
constexpr auto ALLOC_DECREASE_FACTOR = 0.7;

constexpr auto PRECISION_LIMIT = 1.e-7;

template < typename T, typename U > static auto wrap(T x, U y)
{
  return x > (y) / 2 ? ((x) - (y)) : ((x) < (-(y) / 2.) ? ((x) + (y)) : (x));
}

template < typename T > static auto pow2(T x)
{
  return x * x;
}

template < typename T > static auto pow3(T x)
{
  return x * x * x;
}

#define  terminate(x) {char termbuf[5000]; sprintf(termbuf, "code termination on task=%d, function %s(), file %s, line %d: %s\n", ThisTask, __FUNCTION__, __FILE__, __LINE__, x); printf("%s", termbuf); fflush(stdout); endrun(1);}


#define  mymalloc(x, y)            mymalloc_fullinfo(x, y, __FUNCTION__, __FILE__, __LINE__)
#define  mymalloc_movable(x, y, z) mymalloc_movable_fullinfo(x, y, z, __FUNCTION__, __FILE__, __LINE__)

#define  myrealloc(x, y)           myrealloc_fullinfo(x, y, __FUNCTION__, __FILE__, __LINE__)
#define  myrealloc_movable(x, y)   myrealloc_movable_fullinfo(x, y, __FUNCTION__, __FILE__, __LINE__)

#define  myfree(x)                 myfree_fullinfo(x, __FUNCTION__, __FILE__, __LINE__)
#define  myfree_movable(x)         myfree_movable_fullinfo(x, __FUNCTION__, __FILE__, __LINE__)

#define  report_memory_usage(x, y) report_detailed_memory_usage_of_largest_task(x, y, __FUNCTION__, __FILE__, __LINE__)


#ifdef GALAXYTREE
#define  CORRECTDBFLOAT(x)  ((fabs(x)<(1.e-30) || isnan(x)) ?(0.0):(x))
#else
#define  CORRECTDBFLOAT(x) x
#endif


//WATCH OUT! In the case of MCMC running both MR and MRII the larger value is used to "allocate" all the arrays
//inside the code its LastDarkMatterSnapShot+1 that defines the extent of the loops
//(in MCMC MR_plus_MRII mode this are not always identical)

constexpr auto MAXSNAPS =
#ifdef MRII
  68                            /* Number of snapshots in the dark matter simulation */
#else
#ifdef PHOENIX
  72
#else
#ifdef CATERPILLAR
  256
#else
  64                            //NORMAL MILLENNIUM
#endif //CATERPILLAR
#endif //PHOENIX
#endif //MRII
  ;                             //do not miss this ;!!!

constexpr auto MAXGALFAC = 2.3; /*1.5/2.3 - maximum fraction of satellite without a halo (for memory allocation) */

constexpr auto STEPS = 20;      /* Number of integration intervals between two snapshots */

constexpr auto ALLOCPARAMETER = 50.;    /* new definition !!! THIS HAS TO BE 50 !!! DONT EVER EVER EVER CHANGE !!! */

//To understand the units in the code read through set_units in init.c!!!
constexpr auto GRAVITY = 6.672e-8;
constexpr auto SOLAR_MASS = 1.989e33;
constexpr auto SOLAR_LUM = 3.826e33;
constexpr auto RAD_CONST = 7.565e-15;
constexpr auto AVOGADRO = 6.0222e23;
constexpr auto BOLTZMANN = 1.3806e-16;
constexpr auto GAS_CONST = 8.31425e7;
constexpr auto C = 2.9979e10;
constexpr auto PLANCK = 6.6262e-27;
constexpr auto PROTONMASS = 1.6726e-24;
constexpr auto HUBBLE = 3.2407789e-18;  /* in h/sec */

//To understand the units in the code read through set_units in init.c!!!

constexpr auto UNITLENGTH_IN_CM = 3.08568e+24;  // Mpc - WATCH OUT, distances in the code are in Mpc/h
constexpr auto UNITMASS_IN_G = 1.989e+43;       // 10^10Msun - WATCH OUT, masses in the code are in 10^10Msun/h
constexpr auto UNITVELOCITY_IN_CM_PER_S = 100000;       // Km/s - WATCH OUT, this are the correct units in the code km/s
constexpr auto SEC_PER_MEGAYEAR = 3.155e13;
constexpr auto SEC_PER_YEAR = 3.155e7;

#ifdef GALAXYTREE
#undef  NOUT
#define NOUT MAXSNAPS
#endif


#ifdef STAR_FORMATION_HISTORY
constexpr auto SFH_NMERGE = 3;  //  SFH_NMERGE=Nmax+1 (Nmax used in Shamshiri2014)

#ifdef CATERPILLAR
constexpr auto SFH_NBIN = 24;   //  CATERPILLAR - 256 snapshots
#else
constexpr auto SFH_NBIN = 20;
#endif //CATERPILLAR

#endif //STAR_FORMATION_HISTORY

#ifdef DETAILED_METALS_AND_MASS_RETURN
struct metals
{
  float type1a;
  float type2;
  float agb;
};

#ifdef INDIVIDUAL_ELEMENTS
//Individual element histories:
struct elements
{
  float H;
  float He;
#ifndef MAINELEMENTS
  float Cb;                     //NOTE: Carbon (C) is stored as Cb here
  float N;
#endif
  float O;
#ifndef MAINELEMENTS
  float Ne;
#endif
  float Mg;
#ifndef MAINELEMENTS
  float Si;
  float S;
  float Ca;
#endif
  float Fe;
};

//Number of chemical elements tracked:
#ifndef MAINELEMENTS
constexpr auto NUM_ELEMENTS = 11;       //All: [H][He][C][N][O][Ne][Mg][Si][S][Ca][Fe]
#else
constexpr auto NUM_ELEMENTS = 5;        //Only [H][He][O][Mg][Fe]
#endif

#endif //INDIVIDUAL_ELEMENTS
#endif //DETAILED_METALS_AND_MASS_RETURN

/**
 * Galaxy structure for output
 */
#ifdef LIGHT_OUTPUT
struct GALAXY_OUTPUT
{
  int Type;                     // Galaxy type: 0 for central galaxies of a main halo, 1 for central galaxies in sub-halos, 2 for satellites without halo.
  int SnapNum;                  // The snapshot number where this galaxy was identified.
  float CentralMvir;            // 10^10/h Msun virial mass of background (FOF) halo containing this galaxy
  float CentralRvir;            // 10^10/h Msun virial mass of background (FOF) halo containing this galaxy
  float Pos[3];                 // 1/h Mpc - Galaxy Positions
  float Mvir;                   // 10^10/h Msun - Virial mass of the subhalo the galaxy is/was the center of.
  float Rvir;                   // Mpc/h - Virial radius of the subhalo the galaxy is/was the center of.
  float Vvir;                   // km/s - Virial velocity of the subhalo the galaxy is/was the center of.
  float DistanceToCentralGal[3];

  /* baryonic reservoirs */
  float ColdGas;                // 10^10/h Msun - Mass in cold gas.
  float BulgeMass;              // 10^10/h Msun - Mass in the bulge
  float DiskMass;
  float HotGas;                 // 10^10/h Msun - Mass in hot gas
  float BlackHoleMass;          // 10^10/h Msun - Mass in black hole

  /* magnitudes in various bands */
#ifdef COMPUTE_SPECPHOT_PROPERTIES
#ifdef OUTPUT_OBS_MAGS
  float ObsMagDust[NMAG];       // dust corrected, rest-frame absolute mags
#endif
#ifdef OUTPUT_REST_MAGS
  float MagDust[NMAG];          // dust corrected, rest-frame absolute mags
#endif
#endif
};
#else
#pragma pack(1)                 //structure alignment for 1 Byte.
struct GALAXY_OUTPUT
{
#ifdef GALAXYTREE
  long long GalID; /** ID of galaxy, unique within simulation and SAM run.*/
  long long HaloID;             // Unique ID of MPA halo containing this galaxy
#endif
#ifdef MBPID
  long long MostBoundID;        // Most bound particle at centre of subhalo last associated with this galaxy.  Put here as want all 8-byte blocks together at top of output record.
#endif
#ifdef GALAXYTREE
  long long FirstProgGal;       // Main progenitor of this galaxy. Also the first progenitor in a linked list representation of the merger tree.
  long long NextProgGal;        // Next progenitor of this galaxy in linked list representation of merger tree
  long long LastProgGal;        // Galaxies with id between this galaxyId and this lastProgenitorId form the merger tree rooted in this galaxy.
  long long FOFCentralGal;
  long long FileTreeNr;
  long long DescendantGal;      // Pointer to the descendant of this galaxy in its merger tree; -1 if there is no descendant
  long long MainLeafId;
  long long TreeRootId;
  long long SubID;
  long long MMSubID;            // fofId, the subhaloid of the subhalo at the center of the fof group
  int PeanoKey;                 // Peano-Hilbert key, (bits=8), for position in 500/h Mpc box
  float Redshift;               // redshift of the snapshot where this galaxy resides
#endif
  int Type;                     // Galaxy type: 0 for central galaxies of a main halo, 1 for central galaxies in sub-halos, 2 for satellites without halo.
#ifndef GALAXYTREE
  int HaloIndex;
  // long long SubID;
  // long long FirstHaloInFOFgroup;
#endif
#ifdef HALOPROPERTIES
  float HaloM_Mean200, HaloM_Crit200, HaloM_TopHat;
  float HaloPos[3];
  float HaloVel[3];
  float HaloVelDisp;
  float HaloVmax;
  float HaloSpin[3];
#endif
  int SnapNum;                  // The snapshot number where this galaxy was identified.
  float LookBackTimeToSnap;     //The time from a given snapshot to z=0, in years
  float CentralMvir;            // 10^10/h Msun virial mass of background (FOF) halo containing this galaxy
  float CentralRvir;            // Rvir of background (FOF) halo containing this galaxy
  float DistanceToCentralGal[3];
  /* properties of subhalo at the last time this galaxy was a central galaxy */
  float Pos[3];                 // 1/h Mpc - Galaxy Positions
  float Vel[3];                 // km/s - Galaxy Velocities
  int Len;                      //Number of particles in the associated subhalo  
  float Mvir;                   // 10^10/h Msun - Virial mass of the subhalo the galaxy is/was the center of.
  float Rvir;                   // Mpc/h - Virial radius of the subhalo the galaxy is/was the center of.
  float Vvir;                   // km/s - Virial velocity of the subhalo the galaxy is/was the center of.
  float Vmax;                   // km/s - Maximum rotational velocity of the subhalo, or the last value for type 2's galaxies.
  float GasSpin[3];             // Gas Spin
  float StellarSpin[3];         // Stellar Spin
  float InfallVmax;             // km/s - Vmax at infall
  float InfallVmaxPeak;         // km/s - Max previous Vmax at infall
  int InfallSnap;               // Snapnum at infall
  float InfallHotGas;
  float HotRadius;              //Mpc/h - Radius of the hot gas
  /*dynamical friction merger time */
  float OriMergTime;
  float MergTime;
  /* baryonic reservoirs */
  float ColdGas;                // 10^10/h Msun - Mass in cold gas.
  float StellarMass;            // 10^10/h Msun - Disk+Bulge
  float BulgeMass;              // 10^10/h Msun - Mass in the bulge
  float DiskMass;
  float HotGas;                 // 10^10/h Msun - Mass in hot gas
  float EjectedMass;            // 10^10/h Msun - Mass in ejected gas
  float BlackHoleMass;          // 10^10/h Msun - Mass in black hole
  /* ICL magnitude and mass */
  float ICM;                    // mass in intra-cluster stars, for type 0,1
#ifdef DETAILED_METALS_AND_MASS_RETURN
  struct metals MetalsColdGas;  // 10^10/h Msun -        Mass in metals in cold gas.
  struct metals MetalsBulgeMass;        // 10^10/h Msun -      Mass in metals in the bulge
  struct metals MetalsDiskMass; // 10^10/h Msun -       Mass in metals in the disk
  struct metals MetalsHotGas;   // 10^10/h Msun - Mass in metals in the hot gas
  struct metals MetalsEjectedMass;      // 10^10/h Msun -    Mass in metals in the ejected gas
  struct metals MetalsICM;      // total mass in metals in intra-cluster stars, for type 0,1
#ifdef METALS_SELF
  struct metals MetalsHotGasSelf;       // hot gas metals that come from self
#endif
#else
  float MetalsColdGas;          // 10^10/h Msun -        Mass in metals in cold gas.
  float MetalsStellarMass;      // 10^10/h Msun -    Mass in metals in the bulge+disk
  float MetalsBulgeMass;        // 10^10/h Msun -      Mass in metals in the bulge
  float MetalsDiskMass;         // 10^10/h Msun -       Mass in metals in the disk
  float MetalsHotGas;           // 10^10/h Msun - Mass in metals in the hot gas
  float MetalsEjectedMass;      // 10^10/h Msun -    Mass in metals in the ejected gas
  float MetalsICM;              // total mass in metals in intra-cluster stars, for type 0,1
#ifdef METALS_SELF
  float MetalsHotGasSelf;       // hot gas metals that come from self
#endif
#endif                          //DETAILED_METALS_AND_MASS_RETURN
#ifdef TRACK_BURST
  float BurstMass;              // Mass formed in starbursts
#endif
  /* misc */
  float PrimordialAccretionRate;
  float CoolingRadius;          // Q: store this ? (was stored in Delucia20006a)
  float CoolingRate;
  float CoolingRate_beforeAGN;
  float QuasarAccretionRate;
  float RadioAccretionRate;
  float Sfr;
  float SfrBulge;
  float XrayLum;
  float BulgeSize;
  float StellarDiskRadius;
  float GasDiskRadius;
  float CosInclination;         // cos(angle) between galaxy spin and the z-axis
  int DisruptOn;                // 0: galaxy merged onto merger center; 1: galaxy was disrupted before merging onto its descendant, matter went into ICM of merger center
  int MergeOn;                  // 0: standard delucia-like merger behaviour for type 1 galaxy; 1: galaxy mass > halo mass, separate dynamical friction time calculated ....
  /* magnitudes in various bands */
#ifdef COMPUTE_SPECPHOT_PROPERTIES
#ifdef OUTPUT_REST_MAGS
  float MagDust[NMAG];          // dust corrected, rest-frame absolute mags
  float Mag[NMAG];              // rest-frame absolute mags
  float MagBulge[NMAG];         // rest-frame absolute mags for the bulge
#ifdef ICL
  float MagICL[NMAG];           // rest-frame absolute mags of ICL
#endif
#endif

#ifdef OUTPUT_OBS_MAGS
  float ObsMagDust[NMAG];       // dust-corrected, obs-frame absolute mags
  float ObsMag[NMAG];           // obs-frame absolute mags
  float ObsMagBulge[NMAG];      // obs-frame absolute mags for the bulge
#ifdef ICL
  float ObsMagICL[NMAG];        // observer-frame absolute mags for intra-cluster light
#endif
#ifdef OUTPUT_MOMAF_INPUTS
  // define luminosities as if the galaxy were one snapshot earlier, i.e. higher redshift, than its actual snapshot
  float dObsMagDust[NMAG];
  float dObsMag[NMAG];
  float dObsMagBulge[NMAG];
#ifdef ICL
  float dObsMagICL[NMAG];
#endif                          //
#ifdef KITZBICHLER
  // define luminosities as if the galaxy were one snapshot later, i.e. lower redshift, than its actual snapshot
  float dObsMagDust_forward[NMAG];
  float dObsMag_forward[NMAG];
  float dObsMagBulge_forward[NMAG];
#ifdef ICL
  float dObsMagICL_forward[NMAG];
#endif                          //
#endif                          //KITZBICHLER
#endif                          //OUTPUT_MOMAF_INPUTS
#endif                          //OUTPUT_OBS_MAGS

#endif                          //COMPUTE_SPECPHOT_PROPERTIES

  float MassWeightAge;
#ifdef  POST_PROCESS_MAGS
  float rbandWeightAge;
#endif

#ifdef STAR_FORMATION_HISTORY
  int sfh_ibin;                 //Index of highest bin currently in use
  int sfh_numbins;              // number of non empty bins
  float sfh_DiskMass[SFH_NBIN];
  float sfh_BulgeMass[SFH_NBIN];
  float sfh_ICM[SFH_NBIN];
#ifdef DETAILED_METALS_AND_MASS_RETURN
  struct metals sfh_MetalsDiskMass[SFH_NBIN];   // Metals locked up in stars in disk.
  struct metals sfh_MetalsBulgeMass[SFH_NBIN];  // Metals locked up in stars in bulge.
  struct metals sfh_MetalsICM[SFH_NBIN];        // Metals locked up in stars in ICM.
#else
  float sfh_MetalsDiskMass[SFH_NBIN];   // Metals locked up in stars in disk.
  float sfh_MetalsBulgeMass[SFH_NBIN];  //Metals locked up in stars in bulge.
  float sfh_MetalsICM[SFH_NBIN];        // Metals locked up in stars in ICM.
#endif
#ifdef TRACK_BURST
  float sfh_BurstMass[SFH_NBIN];        // Mass formed in starbursts
#endif
#endif                          //STAR_FORMATION_HISTORY

#ifdef INDIVIDUAL_ELEMENTS
  struct elements sfh_ElementsDiskMass[SFH_NBIN];
  struct elements sfh_ElementsBulgeMass[SFH_NBIN];
  struct elements sfh_ElementsICM[SFH_NBIN];

  //float DiskMass_elements[ELEMENT_NUM];
  struct elements DiskMass_elements;
  struct elements BulgeMass_elements;
  struct elements ColdGas_elements;
  struct elements HotGas_elements;
  struct elements ICM_elements;
  struct elements EjectedMass_elements;
#endif                          //INDIVIDUAL_ELEMENTS
};

// next only of interest to DB output, which generally requires complete tree
#ifdef STAR_FORMATION_HISTORY
struct SFH_BIN
{
  long long GalID;              // ID of the galaxy
  short snapnum;                // snapnum of the galaxy, repeated here for faster lookups of times etc
  short sfh_ibin;               //Index of highest bin currently in use
//    float sfh_time; //time to present at the middle of bin in years.
//    float sfh_dt; //time width of bin in years.
  float sfh_DiskMass;
  float sfh_BulgeMass;
  float sfh_ICM;
#ifdef DETAILED_METALS_AND_MASS_RETURN
  struct metals sfh_MetalsDiskMass;     // Metals locked up in stars in disk.
  struct metals sfh_MetalsBulgeMass;    // Metals locked up in stars in bulge.
  struct metals sfh_MetalsICM;  // Metals locked up in stars in ICM.
#else
  float sfh_MetalsDiskMass;     // Metals locked up in stars in disk.
  float sfh_MetalsBulgeMass;    //Metals locked up in stars in bulge.
  float sfh_MetalsICM;          // Metals locked up in stars in ICM.
#endif
#ifdef TRACK_BURST
  float sfh_BurstMass;          // Mass formed in starbursts
#endif

#ifdef INDIVIDUAL_ELEMENTS
  struct elements sfh_ElementsDiskMass;
  struct elements sfh_ElementsBulgeMass;
  struct elements sfh_ElementsICM;

  //float DiskMass_elements[ELEMENT_NUM];
  struct elements DiskMass_elements;
  struct elements BulgeMass_elements;
  struct elements ColdGas_elements;
  struct elements HotGas_elements;
  struct elements ICM_elements;
  struct elements EjectedMass_elements;
#endif                          //INDIVIDUAL_ELEMENTS
};

struct SFH_Time
{
  int snapnum;                  // snapnum
  int bin;                      // index of current bin
  double lookbacktime;          // lookback time in years (???) to center of current bin
  // proposal: in output write the start of the bin and its end, rather than center and dt
  double dt;                    // width of the current bin in years (???)
  int nbins;                    // # of highest resolution bins used to create current bin
};
#endif //STAR_FORMATION_HISTORY
#pragma pack()                  //structure alignment ends.
#endif //When LIGHT_OUTPUT is not defined

extern struct galaxy_tree_data
{
  int HaloGalIndex;
  int IndexStored;
  int SnapNum;
  int GalID;
  int FirstProgGal;
  int NextProgGal;
  int LastProgGal;
  int DescendantGal;
  int MainLeaf;
  int TreeRoot;
  int FOFCentralGal;
  int Done;
}
 *GalTree;

/*Structure with all the data associated with galaxies (this is not the same as the output!)*/
extern struct GALAXY            /* Galaxy data */
{
  int HeapIndex;
  int GalTreeIndex;
  int NextGalaxy;
#ifdef GALAXYTREE
  int FirstProgGal;
#endif
  int Type;
  int HaloNr;
  long long MostBoundID;
  int SnapNum;
  int CentralGal;               //own ID for types 0 and 1, unless 1's have little dark matter and already merging to type 0. For 2's its the merger centre
  float CentralMvir;
  /* properties of subhalo at the last time this galaxy was a central galaxy */
  float Pos[3];
  float MergCentralPos[3];
  float Vel[3];
  float Pos_notupdated[3];
  float Vel_notupdated[3];
#ifdef HALOPROPERTIES
  float HaloM_Mean200, HaloM_Crit200, HaloM_TopHat;
  float HaloPos[3];
  float HaloVel[3];
  float HaloVelDisp;
  float HaloVmax;
#endif
  float HaloSpin[3];
  float GasSpin[3];
  float StellarSpin[3];
  int Len;
  float Mvir;
  float Rvir;
  float Vvir;
  float Vmax;
  float InfallVmax;
  float InfallVmaxPeak;         // km/s - Max previous Vmax at infall
  int InfallSnap;
  float InfallHotGas;
  float InfallHotGasRadius;
  float HotRadius;
  /* baryonic reservoirs */
  float ColdGas;
  float BulgeMass;
  float DiskMass;
  float HotGas;
  float EjectedMass;
  float BlackHoleMass;
#ifdef DETAILED_METALS_AND_MASS_RETURN
  struct metals MetalsColdGas;
  struct metals MetalsBulgeMass;
  struct metals MetalsDiskMass;
  struct metals MetalsHotGas;
  struct metals MetalsEjectedMass;
#ifdef METALS_SELF
  struct metals MetalsHotGasSelf;
#endif
#else
  float MetalsColdGas;
  float MetalsBulgeMass;
  float MetalsDiskMass;
  float MetalsHotGas;
  float MetalsEjectedMass;
#ifdef METALS_SELF
  float MetalsHotGasSelf;
#endif
#endif                          //DETAILED_METALS_AND_MASS_RETURN
#ifdef TRACK_BURST
  float BurstMass;
#endif

  /* misc */
  float PrimordialAccretionRate;
  float CoolingRate;
  float CoolingRate_beforeAGN;
  float CoolingRadius;
  float CoolingGas;
  float QuasarAccretionRate;
  float RadioAccretionRate;
  float AGNheatingFromCentral;
  float Sfr;
  float SfrBulge;
  float StarMerge;
  float XrayLum;
  float BulgeSize;
  float StellarDiskRadius;
  float GasDiskRadius;
#ifdef GALAXYTREE
  int DisruptOn;
#endif
  // float halfradius;
  //float periradius;
  float CosInclination;         //angle between galaxy spin and the z-axis
  float OriMergTime;
  float MergTime;
  float OriMvir;
  float OriRvir;
  float MergeSat;
  float DistanceToCentralGal[3];
  int MergeOn;
  float ICM;
#ifdef DETAILED_METALS_AND_MASS_RETURN
  struct metals MetalsICM;
#else
  float MetalsICM;
#endif

  /* luminosities in various bands */
#ifdef COMPUTE_SPECPHOT_PROPERTIES
#ifndef POST_PROCESS_MAGS
#ifdef OUTPUT_REST_MAGS
  float Lum[NMAG][NOUT];
  float YLum[NMAG][NOUT];
  float LumBulge[NMAG][NOUT];
  float YLumBulge[NMAG][NOUT];
  float LumDust[NMAG][NOUT];
#ifdef ICL
  float ICLLum[NMAG][NOUT];
#endif
#endif                          //OUTPUT_REST_MAGS

#ifdef COMPUTE_OBS_MAGS
  float ObsLum[NMAG][NOUT];
  float ObsYLum[NMAG][NOUT];
  float ObsLumBulge[NMAG][NOUT];
  float ObsYLumBulge[NMAG][NOUT];
  float ObsLumDust[NMAG][NOUT];
#ifdef ICL
  float ObsICL[NMAG][NOUT];
#endif

#ifdef OUTPUT_MOMAF_INPUTS
  float dObsLum[NMAG][NOUT];
  float dObsYLum[NMAG][NOUT];
  float dObsLumBulge[NMAG][NOUT];
  float dObsYLumBulge[NMAG][NOUT];
  float dObsLumDust[NMAG][NOUT];
#ifdef ICL
  float dObsICL[NMAG][NOUT];
#endif
#endif
#endif                          //COMPUTE_OBS_MAGS

#endif                          //ndef POST_PROCESS_MAGS
#endif                          //COMPUTE_SPECPHOT_PROPERTIES

  float MassWeightAge[NOUT];
#ifdef STAR_FORMATION_HISTORY
  int sfh_ibin;                 //Index of highest bin currently in use
  double sfh_age;               //Time in years of last call to sph_update_bins
  int sfh_flag[SFH_NBIN];
  float sfh_dt[SFH_NBIN];       //Size of time interval in units of years
  float sfh_t[SFH_NBIN];        //Time at low-redshift edge of bin in same units
  int sfh_Nbins[SFH_NBIN];      //Number of bins on the time interval
  float sfh_DiskMass[SFH_NBIN]; //Stellar mass in disk, in bin in standard units
  float sfh_BulgeMass[SFH_NBIN];        //Stellar mass in bulge, in bin in standard units
  float sfh_ICM[SFH_NBIN];      //Stellar mass in ICM, in bin in standard units
#ifdef DETAILED_METALS_AND_MASS_RETURN
  struct metals sfh_MetalsDiskMass[SFH_NBIN];   //Metals locked up in stars in disk.
  struct metals sfh_MetalsBulgeMass[SFH_NBIN];  //Metals locked up in stars in bulge.
  struct metals sfh_MetalsICM[SFH_NBIN];        //Metals locked up in stars in ICM.
#else
  float sfh_MetalsDiskMass[SFH_NBIN];   //Metals locked up in stars in disk.
  float sfh_MetalsBulgeMass[SFH_NBIN];  //Metals locked up in stars in bulge.
  float sfh_MetalsICM[SFH_NBIN];        //Metals locked up in stars in ICM.
#endif
#ifdef TRACK_BURST
  float sfh_BurstMass[SFH_NBIN];        //Stellar mass formed in bursts, in standard units.
#endif
#endif                          //STAR_FORMATION_HISTORY

#ifdef INDIVIDUAL_ELEMENTS
  struct elements sfh_ElementsDiskMass[SFH_NBIN];
  struct elements sfh_ElementsBulgeMass[SFH_NBIN];
  struct elements sfh_ElementsICM[SFH_NBIN];

  struct elements DiskMass_elements;
  struct elements BulgeMass_elements;
  struct elements ColdGas_elements;
  struct elements HotGas_elements;
  struct elements ICM_elements;
  struct elements EjectedMass_elements;
#endif                          //INDIVIDUAL_ELEMENTS
} *Gal, *HaloGal;


// Documentation can be found in the database
extern struct halo_data
{
  /* merger tree pointers */
  int Descendant;
  int FirstProgenitor;
  int NextProgenitor;
  int FirstHaloInFOFgroup;
  int NextHaloInFOFgroup;

  /* properties of halo */
  int Len;
  float M_Mean200, M_Crit200, M_TopHat;
  float Pos[3];
  float Vel[3];
  float VelDisp;
  float Vmax;
  float Spin[3];
  long long MostBoundID;

  /* original position in subfind output */
  int SnapNum;
  int FileNr;
  int SubhaloIndex;
  float SubHalfMass;
} *Halo, *Halo_Data;


// Documentation can be found in the database
#ifndef MCMC
extern struct halo_ids_data
{
  long long HaloID;
  long long FileTreeNr;
  long long FirstProgenitor;
  long long LastProgenitor;
  long long NextProgenitor;
  long long Descendant;
  long long FirstHaloInFOFgroup;
  long long NextHaloInFOFgroup;
#ifdef MRII
  long long MainLeafID;
#endif
  double Redshift;
  int PeanoKey;
  int dummy;                    /* need to use this padding for 64bit alignment */
} *HaloIDs, *HaloIDs_Data;
#else
extern struct halo_ids_data
{
  long long FirstHaloInFOFgroup;
} *HaloIDs, *HaloIDs_Data;
#endif


// Documentation can be found in the database
extern struct halo_aux_data     /* auxiliary halo data */
{
  int DoneFlag;
  int HaloFlag;
  int NGalaxies;
  int FirstGalaxy;
  float M_Mean200_Unscaled;
  float M_Crit200_Unscaled;
  float Pos_Unscaled[3];
  float Vel_Unscaled[3];
  float Vmax_Unscaled;
  float Spin_Unscaled[3];
} *HaloAux;


extern int FirstFile;           /* first and last file for processing */
extern int LastFile;

extern int Ntrees;              /* number of trees in current file */
extern double AllocValue_MaxHaloGal;
extern double AllocValue_MaxGal;
extern double AllocValue_MaxGalTree;

extern int MaxGal;              /* Maximum number of galaxies allowed for Gal[] array */
extern int NHaloGal, MaxHaloGal;
extern int NGalTree, MaxGalTree;
extern int *HaloGalHeap;
extern int IndexStored;

extern int LastSnapShotNr;

extern int LastDarkMatterSnapShot;

#ifdef MR_PLUS_MRII             //OPTION for MCMC
extern int LastDarkMatterSnapShot_MR;
extern int LastDarkMatterSnapShot_MRII;
#endif


extern char SpecPhotDir[512];
extern char PhotPrefix[50];
extern char SpecPhotIMF[50];
extern char McFile[512];
extern char FileWithFilterNames[512];
extern char CoolFunctionsDir[512];
extern char OutputDir[512];

/* in case a second parameter is given as argument to the code, this will be taken as a
 * temporary outputdir to allow fast I/O. OutputDir will be replaced by this directory
 * and in the end everything will be moved to the FinalOutputDir (original OutputDir
 * given in input.par )*/
extern char FinalOutputDir[512];
extern char FileNameGalaxies[512];
extern char SimulationDir[512];
extern char FileWithOutputRedshifts[512];

extern char FileWithZList[512];

//variable used to scale to a different cosmology
extern char FileWithZList_OriginalCosm[512];

#ifdef MR_PLUS_MRII             //OPTION for MCMC
extern char FileWithZList_MR[512];
extern char FileWithZList_OriginalCosm_MR[512];
extern char FileWithZList_MRII[512];
extern char FileWithZList_OriginalCosm_MRII[512];
#endif

extern double ScalePos;
extern double ScaleMass;

#ifdef SPECIFYFILENR
extern char FileNrDir[512];
extern int ListInputFilrNr[111];
#endif

extern int TotHalos;
extern int TotGalaxies[NOUT];
extern int *TreeNgals[NOUT];

extern int *FirstHaloInSnap;

extern int *TreeNHalos;
extern int *TreeFirstHalo;

extern void *TreeAuxData;


extern double MaxMemSize;

extern size_t AllocatedBytes;
extern size_t HighMarkBytes;
extern size_t FreeBytes;

extern int ThisTask, NTask;

#ifdef GALAXYTREE
extern int GalCount;
extern int TotGalCount;
#endif

/* Cosmological parameters */
extern double BaryonFrac;
extern double Sigma8;
extern double Omega;
extern double OmegaLambda;
extern double Hubble_h;
extern double Omega_OriginalCosm;
extern double OmegaLambda_OriginalCosm;
extern double Hubble_h_OriginalCosm;

//SIMULATION RELATED
extern double PartMass;
extern double BoxSize;
extern double PartMass_OriginalCosm;
extern double BoxSize_OriginalCosm;

#ifdef MR_PLUS_MRII             //OPTION for MCMC
extern double PartMass_MR;
extern double BoxSize_MR;
extern double PartMass_OriginalCosm_MR;
extern double BoxSize_OriginalCosm_MR;
extern double PartMass_MRII;
extern double BoxSize_MRII;
extern double PartMass_OriginalCosm_MRII;
extern double BoxSize_OriginalCosm_MRII;
#endif


/* flags */
extern int ReionizationModel;
extern int DiskRadiusModel;
extern int StarFormationModel;
extern int FeedbackReheatingModel;
extern int FeedbackEjectionModel;
extern int FateOfSatellitesGas;
extern int ReIncorporationModel;
extern int AGNRadioModeModel;
extern int DiskInstabilityModel;
extern int BHGrowthInDiskInstabilityModel;
extern int HotGasStripingModel;
extern int DisruptionModel;
extern int StarBurstModel;
extern int BulgeFormationInMinorMergersOn;
extern int MetallicityOption;

/* parameters */
extern double Reionization_z0;
extern double Reionization_zr;
extern double Yield;
extern double RecycleFraction;
extern double ThreshMajorMerger;
extern double MergerTimeMultiplier;
extern double RamPressureStrip_CutOffMass;
extern double SfrEfficiency;
extern double SfrColdCrit;
extern double SfrBurstEfficiency;
extern double SfrBurstSlope;
extern double AgnEfficiency;
extern double BlackHoleGrowthRate;
extern double BlackHoleSeedMass;
extern double BlackHoleCutoffVelocity;
extern double FeedbackReheatingEpsilon;
extern double ReheatPreVelocity;
extern double ReheatSlope;
extern double FeedbackEjectionEfficiency;
extern double EjectPreVelocity;
extern double EjectSlope;
extern double ReIncorporationFactor;
extern double EnergySNcode, EnergySN;
extern double EtaSNcode, EtaSN;

extern double UnitTime_in_s, UnitPressure_in_cgs, UnitDensity_in_cgs, UnitCoolingRate_in_cgs, UnitEnergy_in_cgs, UnitTime_in_Megayears, //Using time as stored in the code, this gives Myr/h
  UnitTime_in_years,
#ifdef HALOMODEL
  RhoCrit,
#endif
  G, Hubble, a0, ar;

extern int ListOutputSnaps[NOUT];
extern float ListOutputRedshifts[NOUT];

extern double ZZ[MAXSNAPS];
extern double AA[MAXSNAPS];

//variable used to scale to a different cosmology
extern double AA_OriginalCosm[MAXSNAPS];

extern double Age[MAXSNAPS];

extern int Zlistlen;

extern gsl_rng *random_generator;


extern int NumMergers;


/*  tabulated stuff */

#ifdef STAR_FORMATION_HISTORY
/* SFH_ is the reference structure for storing the star formation histories in
 * logarithmic bins. It is computed in init.c generating a binning structure for
 * each snapshot/time step. In the code galaxy structures are adjusted with respect
 * to this structure at each step. */
//extern double SFH_t[MAXSNAPS][STEPS][SFH_NBIN]; //Time to present at the lower edge of the bin (code units)
extern std::vector < std::vector < std::vector < double >>>SFH_t;

//extern double SFH_dt[MAXSNAPS][STEPS][SFH_NBIN]; //Time width of the bin (code units)
extern std::vector < std::vector < std::vector < double >>>SFH_dt;

//extern int SFH_Nbins[MAXSNAPS][STEPS][SFH_NBIN]; //Number of bins merged in each bin (only useful for the merging algorithm)
extern std::vector < std::vector < std::vector < int >>>SFH_Nbins;

//extern int SFH_ibin[MAXSNAPS][STEPS]; //Last active bin
extern std::vector < std::vector < int >>SFH_ibin;

#ifdef DETAILED_METALS_AND_MASS_RETURN
extern double tau_t[STEPS * MAXSNAPS];  //Time-to-z=0 of every timestep in the code. (Used for SNe rates in yield_integrals.c)
extern double tau_dt[STEPS * MAXSNAPS]; //Width of every timestep in the code. (Used for SNe rates in yield_integrals.c)
#endif
#endif //STAR_FORMATION_HISTORY


#ifdef DETAILED_METALS_AND_MASS_RETURN

//Number of interpolated points within the mass ranges for the four types of yield table:
constexpr auto LIFETIME_MASS_NUM = 150;
constexpr auto LIFETIME_Z_NUM = 6;
constexpr auto AGB_MASS_NUM = 59;       //55 //ROB: 59, when going from 0.85 to 7 Msun
constexpr auto AGB_Z_NUM = 3;

#ifdef PORTINARI
constexpr auto SNII_MASS_NUM = 85;      //ROB: 85, from 6 <= M[Msun] <= 120. Change SNII_MIN_MASS and SNII_MAX_MASS for shorter ranges.
constexpr auto SNII_Z_NUM = 5;
#endif
#ifdef CHIEFFI
constexpr auto SNII_MASS_NUM = 81;      //ROB: 56 if 7 <= M[Msun] <= 50. 81 if 7 <= M[Msun] <= 120. (NB: You can set SNII_MASS_NUM 81, and SNII_MAX_MASS 50. But DON"T put SNII_MASS_NUM > 81 ever!)
constexpr auto SNII_Z_NUM = 6;
#endif
constexpr auto SNIA_MASS_NUM = 83;      //48 //Number increased after extending range to cover M2 masses (07-02-12)

//Mass ranges for the different modes of ejection:
constexpr auto AGB_MIN_MASS = 0.85;
constexpr auto AGB_MAX_MASS = 7.0;      //6.0
constexpr auto SNIA_MIN_MASS = 3.0;
constexpr auto SNIA_MAX_MASS = 16.0;

#ifdef PORTINARI
constexpr auto SNII_MIN_MASS = 7.0;     //6.0
constexpr auto SNII_MAX_MASS = 120.0;
#endif
#ifdef CHIEFFI
constexpr auto SNII_MIN_MASS = 7.0;
constexpr auto SNII_MAX_MASS = 120.0;   //50.0
#endif

int ELETOBIGCOUNTA;
int FRACCOUNTA;

//Arrays that yield tables are written to:
float lifetimeMasses[LIFETIME_MASS_NUM];
float lifetimeMetallicities[LIFETIME_Z_NUM];
float lifetimes[LIFETIME_Z_NUM][LIFETIME_MASS_NUM];
float AGBMasses[AGB_MASS_NUM];  //Initial star masses [Msun]
float AGBMetallicities[AGB_Z_NUM];      //Initial star metallicities [Msun]
float AGBEjectedMasses[AGB_Z_NUM][AGB_MASS_NUM];        //Total mass ejected [Msun]
float AGBTotalMetals[AGB_Z_NUM][AGB_MASS_NUM];  //Total metal YIELD ejected [Msun]
float AGBYields[AGB_Z_NUM][11][AGB_MASS_NUM];   //YIELD ejected, for each element [Msun]
float SNIIMasses[SNII_MASS_NUM];
float SNIIMetallicities[SNII_Z_NUM];
float SNIIEjectedMasses[SNII_Z_NUM][SNII_MASS_NUM];
float SNIITotalMetals[SNII_Z_NUM][SNII_MASS_NUM];
float SNIIYields[SNII_Z_NUM][11][SNII_MASS_NUM];

#ifndef DTD
float SNIaMasses[SNIA_MASS_NUM];
float SNIaEjectedMasses[SNIA_MASS_NUM];
float SNIaTotalMetals[SNIA_MASS_NUM];
float SNIaYields[42][SNIA_MASS_NUM];
#else
float SNIaYields[42];
#endif

//Integrated yields arrays:
float NormSNIIMassEjecRate[STEPS * MAXSNAPS][SFH_NBIN][LIFETIME_Z_NUM];
float NormSNIIMetalEjecRate[STEPS * MAXSNAPS][SFH_NBIN][LIFETIME_Z_NUM];

#ifdef INDIVIDUAL_ELEMENTS
float NormSNIIYieldRate[STEPS * MAXSNAPS][SFH_NBIN][LIFETIME_Z_NUM][NUM_ELEMENTS];
#endif
float NormAGBMassEjecRate[STEPS * MAXSNAPS][SFH_NBIN][LIFETIME_Z_NUM];
float NormAGBMetalEjecRate[STEPS * MAXSNAPS][SFH_NBIN][LIFETIME_Z_NUM];

#ifdef INDIVIDUAL_ELEMENTS
float NormAGBYieldRate[STEPS * MAXSNAPS][SFH_NBIN][LIFETIME_Z_NUM][NUM_ELEMENTS];
#endif
float NormSNIaMassEjecRate[STEPS * MAXSNAPS][SFH_NBIN][LIFETIME_Z_NUM];
float NormSNIaMetalEjecRate[STEPS * MAXSNAPS][SFH_NBIN][LIFETIME_Z_NUM];

#ifdef INDIVIDUAL_ELEMENTS
float NormSNIaYieldRate[STEPS * MAXSNAPS][SFH_NBIN][LIFETIME_Z_NUM][NUM_ELEMENTS];
#endif

//Arrays used to plot SNe rates from SFH bins (yield_integrals.c):
float TheSFH[SFH_NBIN];
float SNIIRate[STEPS * MAXSNAPS][LIFETIME_Z_NUM];
float SNIaRate[STEPS * MAXSNAPS][LIFETIME_Z_NUM];
float AGBRate[STEPS * MAXSNAPS][LIFETIME_Z_NUM];

//Arrays used to plot SNe rates from SFH-timesteps (calc_SNe_rates.c):
float TheSFH2[STEPS * MAXSNAPS];
float SNIIRate2[STEPS * MAXSNAPS][LIFETIME_Z_NUM];
float SNIaRate2[STEPS * MAXSNAPS][LIFETIME_Z_NUM];
float AGBRate2[STEPS * MAXSNAPS][LIFETIME_Z_NUM];

//SNIa parameters:
constexpr auto A_FACTOR = 0.04; //Fraction of mass from all objects between SNIa_MIN_MASS and SNIA_MAX_MASS that comes from SN-Ia.

//constexpr auto FRAC2HOT 0.9 //Fraction of material released by disk stars that goes straight into the HotGas. Res goes in ColdGas.
#ifdef DTD
//constexpr auto KALPHA 1.4765 //1.59203 //Now set in yield_integrals.c
//constexpr auto        F316 0.0384 //Integral of the IMF (by number) from 3.0 - 16.0 Msun //Now set in yield_integrals.c
constexpr auto SNIAEJECMASS = 1.2300971;        //Total mass (and total metals) ejected by a SNIa explosion in Msun //Value form original yield table (42 elements): 1.3740855. //Value when only considering 11 elements: 1.2300971

#ifdef BIMODALDTD
constexpr auto DTD_NORM = 0.903206;     //For P98 Z=0.02 lifetimes (26Myrs - 21Gyrs)
#endif
#ifdef CUSTOMDTD
constexpr auto DTD_NORM = 0.524836;     //For P98 Z=0.02 lifetimes (26Myrs - 21Gyrs)
#endif
#ifdef GAUSSIANDTD
constexpr auto DTD_NORM = 1.0;
constexpr auto TAUCHARAC = 1.0; //Characteristic delay time for SNe-Ia (i.e. peak of Gaussian distribution) in Gyrs //default: 2.0
constexpr auto SIGMA_TD = 0.2 * TAUCHARAC;      //0.2 for narrow-DTD, 0.5 for wide_DTD
#endif
#ifdef POWERLAWDTD
constexpr auto DTD_NORM = 7.21863;      //For P98 Z=0.02 lifetimes (26Myrs - 21Gyrs)
constexpr auto DTD_SLOPE = -1.12;       //Slope of power law, according to Maoz et al. (2012)
#endif
#ifdef RUITERDTD
constexpr auto DTD_NORM = 1.09545;      //For P98 Z=0.02 lifetimes (26Myrs - 21Gyrs)
constexpr auto TAUCHARAC = 0.5; //Peak of Gaussian (prompt) component [in Gyrs]
constexpr auto SIGMA_TD = 0.2 * TAUCHARAC;      //Width of Gaussian (prompt) component
constexpr auto DTD_SLOPE = -2.0;        //Slope of power law (delayed) component (see Ruiter et al. 2012)
#endif
#endif

#endif //DETAILED_METALS_AND_MASS_RETURN


#ifdef COMPUTE_SPECPHOT_PROPERTIES
// SSP PHOT_TABLES - magnitues of starburst population as a function of age

#ifdef M05
constexpr auto SSP_NAGES = 220; // Age grid of the SSP tables
constexpr auto SSP_NMETALLICITES = 4;   // Number of Metalicities used

#ifdef SPEC_PHOTABLES_ON_THE_FLY
constexpr auto SSP_NLambda = 1221;
#endif
#endif

#ifdef BC03
constexpr auto SSP_NAGES = 221; // Age grid of the SSP tables
constexpr auto SSP_NMETALLICITES = 6;   // Number of Metalicities used

#ifdef SPEC_PHOTABLES_ON_THE_FLY
constexpr auto SSP_NLambda = 1221;
#endif
#endif

#ifdef CB07
constexpr auto SSP_NAGES = 221; // Age grid of the SSP tables
constexpr auto SSP_NMETALLICITES = 6;   // Number of Metalicities used

#ifdef SPEC_PHOTABLES_ON_THE_FLY
constexpr auto SSP_NLambda = 1238;
#endif
#endif

//table containing the Metallicity grid of the SSP tables (converted to log10)
extern float SSP_logMetalTab[SSP_NMETALLICITES];

//table containing the Age grid of the SSP tables (originally in years, converted to log10(internal time units 1e12 Yrs/h))
extern float SSP_logAgeTab[SSP_NAGES];

//table containing redshift (different from the one in the code when scaling to future times)
extern float RedshiftTab[MAXSNAPS];
extern float LumTables[NMAG][SSP_NMETALLICITES][MAXSNAPS][SSP_NAGES];
extern float FilterLambda[NMAG + 1];    //wavelength of each filter + 1 for V-band

#ifdef SPEC_PHOTABLES_ON_THE_FLY
constexpr auto MAX_NLambdaFilter = 1000;
extern int NLambdaFilter[NMAG];

//VEGA
constexpr auto NLambdaVega = 3303;
#endif

//DUST EXTINCTION
constexpr auto ExpTauBCBulge = 0.5;     // constant extinction for young stars in bulges.
constexpr auto MUWIDTH = 0.2;
constexpr auto MUCENTER = 0.3;
extern long mu_seed;

#endif //COMPUTE_SPECPHOT_PROPERTIES

/*For H2 formation recipe - Not Supported*/
constexpr auto RHO_LEN = 101;
constexpr auto Z_LEN = 13;

extern size_t HighMark;

#ifdef UPDATETYPETWO
extern int NtotHalos, TotIds, Nids, TotSnaps, OffsetIDs;
extern int *CountIDs_halo, *OffsetIDs_halo, *CountIDs_snaptree, *OffsetIDs_snaptree;
extern long long *IdList;
extern float *PosList, *VelList;
#endif


extern int Hashbits;
extern int NumWrittenInParallel;
extern double ScaleFactor;      // factor by which to multiply a position to get its ph index (after floring)


#ifdef USE_MEMORY_TO_MINIMIZE_IO
extern char *ptr_auxdata, *ptr_treedata, *ptr_dbids, *ptr_galaxydata, *ptr_galsnapdata[NOUT];
extern size_t offset_auxdata, offset_treedata, offset_dbids;
extern size_t offset_galaxydata, maxstorage_galaxydata, filled_galaxydata;
extern size_t offset_galsnapdata[NOUT], maxstorage_galsnapdata[NOUT], filled_galsnapdata[NOUT];
#endif

#endif

extern float Reion_z[46], Reion_Mc[46];

extern FILE *tree_file;
extern FILE *treeaux_file;
extern FILE *treedbids_file;
extern FILE *FdGalTree;
extern FILE *FdGalTreeSFH;
extern FILE *FdGalDumps[NOUT];
