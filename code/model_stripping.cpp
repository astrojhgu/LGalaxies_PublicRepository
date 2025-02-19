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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <ctime>

using namespace std;

#include "allvars.h"
#include "proto.h"


/**@file recipe_stripping.c
 * @brief deal_with_satellites.c 
 *
 *       This is where the gas and ICM components of newly accreted satellites
 *       are treated.
 *
 *       There are basically 2 options for the way satellite components are
 *       added into centrals:
 *
 *         HotGasStripingModel ==1
 *         If inside Rvir, hot and ejected gas from satellites of both types 1 and 2
 *         is instantaneously striped and added to type 0.
 *
 *         HotGasStripingModel ==0
 *         Type 1's keep an ejected component.
 *         Type 1's are stripped of hot and ejected gas gradually and later in the code.
 *         A fraction of the hot and ejected gas in the type 2's is
 *         added to the type 1 and the rest to the type 0.
 *         If satellites are outside Rvir, type 1 keeps all its components and receives
 *         everything from type 2's.
 *
 *         In these routines, centralgal is the type 0 at the centre of the halo;
 *         Gal[i].CentralGal is the galaxy around which each satellite orbits;
 *         For simplicity of reference in the comments in the code below, the latter
 *         will be called the type 1, even though it may be the same galaxy as the type 0.
 *
 *         */

void deal_with_satellites(int centralgal, int ngal)
{
  int i, merger_centre;
  double dis, gasfraction_intotype1, stripped_fraction;

  for(i = 0; i < ngal; i++)     /* Loop over all galaxies in the FoF-halo */
    {
      mass_checks("Top of deal_with_satellites a", i);
      mass_checks("Top of deal_with_satellites b", centralgal);
      mass_checks("Top of deal_with_satellites c", Gal[i].CentralGal);

      /* dis is the separation of the type 1 from the type 0 */
      dis = separation_gal(centralgal, Gal[i].CentralGal) / (1 + ZZ[Halo[Gal[centralgal].HaloNr].SnapNum]);


      /* HotGasStripingModel ==  0=> Guo2010 non instantaneous treatment of gas stripping in type 1's
       *
       * if the galaxy is a type 2 and still has hot and ejected gas it is removed at this point
       * (meaning that the halo was fully stripped in previous step) and split between type 0 and type 1
       *
       * If type 2 is orbiting a type 1
       * if the type 2 is inside Rvir of the type 0  (dis < Gal[centralgal].Rvir) the gas is split between 0 and 1
       * if type 2 is outside Rvir of type 0, all the gas goes to type 1
       *
       * If the type 2 is orbiting a type 0 centralgal and Gal[i].CentralGal both refer to the type 0*/

      if(HotGasStripingModel == 0)
        {
          /* All gas Stripped from Type 2 galaxies */
          if(Gal[i].Type == 2)
            {
              //if type 2 is inside Rvir of type 0 split between type 0 and type 1
              if(dis < Gal[centralgal].Rvir)
                gasfraction_intotype1 = Gal[Gal[i].CentralGal].HotRadius / Gal[Gal[i].CentralGal].Rvir;
              //if type 2 is outside Rvir of type 0, all goes to type 1
              else
                gasfraction_intotype1 = 1.;

              Gal[i].HotRadius = 0.0;
              if(Gal[i].HotGas > 0.0)
                transfer_gas(Gal[i].CentralGal, "Hot", i, "Hot", gasfraction_intotype1,
                             "deal_with_satellites", __LINE__);
              if(Gal[i].EjectedMass > 0.0)
                transfer_gas(Gal[i].CentralGal, "Ejected", i, "Ejected", gasfraction_intotype1,
                             "deal_with_satellites", __LINE__);

              mass_checks("deal_with_satellites i #0", i);
              mass_checks("deal_with_satellites Gal[i].CentraGal #0", Gal[i].CentralGal);
#ifdef TRACK_BURST
              /* Transfer burst component first */
              transfer_stars(Gal[i].CentralGal, "Burst", i, "Burst",
                             GasFraction_intotype1 * Gal[i].ICM / (Gal[i].DiskMass + Gal[i].BulgeMass +
                                                                   Gal[i].ICM));
#endif
              transfer_stars(Gal[i].CentralGal, "ICM", i, "ICM", gasfraction_intotype1);
              mass_checks("deal_with_satellites i #1", i);
              mass_checks("deal_with_satellites Gal[i].CentraGal #1", Gal[i].CentralGal);
#ifndef POST_PROCESS_MAGS
#ifdef ICL
              transfer_ICL(Gal[i].CentralGal, i, gasfraction_intotype1);
#endif
#endif
              //All the gas not moved to the type 1 yet goes to the type 0
              if(gasfraction_intotype1 < 1.)
                {
                  if(Gal[i].HotGas > 0.0)
                    transfer_gas(centralgal, "Hot", i, "Hot", 1., "deal_with_satellites", __LINE__);
                  if(Gal[i].EjectedMass > 0.0)
                    transfer_gas(centralgal, "Ejected", i, "Ejected", 1., "deal_with_satellites", __LINE__);

#ifdef TRACK_BURST
                  /* Transfer burst component first */
                  transfer_stars(centralgal, "Burst", i, "Burst",
                                 Gal[i].ICM / (Gal[i].DiskMass + Gal[i].BulgeMass + Gal[i].ICM));
#endif
                  transfer_stars(centralgal, "ICM", i, "ICM", 1.);
                  mass_checks("deal_with_satellites #2", i);
                  mass_checks("deal_with_satellites #2", centralgal);
#ifndef POST_PROCESS_MAGS
#ifdef ICL
                  transfer_ICL(centralgal, i, 1.);
#endif
#endif
                }
            }
          //Type 1 galaxies (or type 2's for the modified stripping) - stripping if galaxy inside Rvir of merger_centre
          else if(Gal[i].Type == 1 && dis < Gal[centralgal].Rvir && Gal[i].HotGas > 0.0)
            {
              merger_centre = centralgal;

              //hot_retain_sat also re-evaluates HotRadius
              stripped_fraction = 1. - (float) (hot_retain_sat(i, merger_centre)) / Gal[i].HotGas;
              if(stripped_fraction < 0.)
                {
                  printf("***Error in hot_retain_sat - returns value larger than HotGas***\n");
                  exit(1);
                }

              transfer_gas(merger_centre, "Hot", i, "Hot", stripped_fraction, "deal_with_satellites",
                           __LINE__);
              transfer_gas(merger_centre, "Ejected", i, "Ejected", stripped_fraction, "deal_with_satellites",
                           __LINE__);
              mass_checks("deal_with_satellites #3", i);
              mass_checks("deal_with_satellites #3", merger_centre);
#ifdef TRACK_BURST
              /* Transfer burst component first */
              transfer_stars(merger_centre, "Burst", i, "Burst",
                             stripped_fraction * Gal[i].ICM / (Gal[i].DiskMass + Gal[i].BulgeMass +
                                                               Gal[i].ICM));
#endif
              transfer_stars(merger_centre, "ICM", i, "ICM", stripped_fraction);
              mass_checks("deal_with_satellites #4", i);
              mass_checks("deal_with_satellites #4", merger_centre);
#ifndef POST_PROCESS_MAGS
#ifdef ICL
              transfer_ICL(merger_centre, i, stripped_fraction);
#endif
#endif
            }

          mass_checks("deal_with_satellites #5", i);
          mass_checks("deal_with_satellites #5", centralgal);

        }

      /* Instantaneous stripping of gas from satellites and no ejection of type 2 into type 1,
       * still there is the condition on Rvir that determines that if a galaxy is a newly
       * accreted type 2 outside Rvir of type 0, its gas will go into the type 1. If it's
       * a type 1 outside Rvir of type 0, it retains all its gas. -> DeLucia2007*/
      else if(HotGasStripingModel == 1)
        {
          /* If galaxy is a satellite inside Rvir it will lose its hot and
           * ejected gas into the hot gas component of the centralgal.
           * Only galaxies within Rvir contribute to the central halo.*/
          if(dis < Gal[centralgal].Rvir && i != centralgal)
            {
              transfer_gas(centralgal, "Hot", i, "Hot", 1., "deal_with_satellites", __LINE__);
              transfer_gas(centralgal, "Ejected", i, "Ejected", 1., "deal_with_satellites", __LINE__);
#ifdef TRACK_BURST
              /* Transfer burst component first */
              transfer_stars(centralgal, "Burst", i, "Burst",
                             Gal[i].ICM / (Gal[i].DiskMass + Gal[i].BulgeMass + Gal[i].ICM));
#endif
              transfer_stars(centralgal, "ICM", i, "ICM", 1.);
#ifndef POST_PROCESS_MAGS
#ifdef ICL
              transfer_ICL(centralgal, i, 1.);
#endif
#endif
              Gal[i].HotRadius = 0.;
            }
          /* If its a type 1 outside Rvir it retains all its gas components, so do nothing
           * else if (Gal[i].Type ==1) {}
           * If galaxy is a type 2 outside Rvir of type 0, then all its gas components
           * will be added to the type 1. */
          else if(Gal[i].Type == 2)
            {
              transfer_gas(Gal[i].CentralGal, "Hot", i, "Hot", 1., "deal_with_satellites", __LINE__);
              transfer_gas(Gal[i].CentralGal, "Ejected", i, "Ejected", 1., "deal_with_satellites", __LINE__);
#ifdef TRACK_BURST
              /* Transfer burst component first */
              transfer_stars(Gal[i].CentralGal, "Burst", i, "Burst",
                             Gal[i].ICM / (Gal[i].DiskMass + Gal[i].BulgeMass + Gal[i].ICM));
#endif
              transfer_stars(Gal[i].CentralGal, "ICM", i, "ICM", 1.);
#ifndef POST_PROCESS_MAGS
#ifdef ICL
              transfer_ICL(Gal[i].CentralGal, i, 1.);
#endif
#endif
              Gal[i].HotRadius = 0.;
            }
        }                       //end of HotGasStripingModel == 1

      mass_checks("Bottom of deal_with_satellites i", i);
      mass_checks("Bottom of deal_with_satellites centralgal", centralgal);

    }                           /* End of HotGasStripingModel choice */

  return;

}





/** Gradual stripping of hot and ejected gas from type 1 satellites. 
 *  This is caused both by tidal and ram-pressure stripping.
 *  This function returns the actual mass of hot gas that the
 *  type 1 retains.
 *
 *  TIDAL STRIPPING
 *  Hot gas is tidally stripped at the same rate at which dark matter is
 *  stripped:
 *
 * \f$ \frac{M_{\rm{hot}}(R_{\rm{tidal}})}{M_{\rm{hot,infall}}}=
 *  \frac{M_{\rm{DM}}}{M_{\rm{DM,infall}}}\f$
 *
 *  Since the hot gas distribution is assumed to be \f$ \rho \propto r^{-2}\f$
 *  this means \f$ M_{\rm{hot}}(r) \propto r.\f$ Therefore, the tidal
 *  radius beyond gas is stripped is given by:
 *
 *  \f$ R_{\rm{tidal}}=
 *  \left(\frac{M_{\rm{DM}}}{M_{\rm{DM,infall}}}\right)R_{\rm{DM,infall}}\f$
 *
 *  RAM PRESSURE STRIPING
 *  Let \f$R_{r.p.}\f$ represent the distance from the centre of the satellite
 *  at which ram pressure striping equals its self-gravity. Then:
 *
 *  \f$ \rho_{\rm{sat}}(R_{\rm{r.p.}})V^2_{\rm{sat}}=
 *      \rho_{\rm{par}}(R_{\rm{orbit}})V^2_{\rm{orbit}}\f$
 *  Where the four terms represent respectively the density of the satellite
 *  at \f$R_{\rm{r.p.}}\f$, the virial velocity of the satellite at infall,
 *  the density of the parent halo at the radius of the satellite and the
 *  orbit velocity of the satellite (given by \f$V_{\rm{c}} of the parent halo\f$)
 *
 *  The stripping radius is given by
 *
 *  \f$R_{\rm{strip}}=min(R_{\rm{tidal}},R_{\rm{r.p.}})\f$
 *
 * */
double hot_retain_sat(int i, int centralgal)
{
  double hotremain;
  double R_Stripping, R_Tidal, R_RamPressure, R_Orbit, TotalMass_sat, Vorbit;

  if(Gal[centralgal].Type != 0)
    exit(0);

  /*Calculate tidal stripping radius */
  R_Tidal = Gal[i].Len * PartMass / Gal[i].Mvir * Gal[i].Rvir;

  /*Ram pressure stripping radius calculation */

  /*First calculate the orbital radius of the satellite R_orbit */
  R_Orbit = separation_gal(centralgal, i) / (1 + ZZ[Halo[Gal[centralgal].HaloNr].SnapNum]);


  /*If the central galaxy has no hot gas, it exerts no ram pressure stripping on the
   * satellite. */
  if(Gal[centralgal].HotGas < 1.e-6 || Gal[centralgal].Mvir < RamPressureStrip_CutOffMass)
    R_RamPressure = Gal[i].HotRadius;
  else
    {
      TotalMass_sat = Gal[i].Mvir;
      Vorbit = sqrt((G * Gal[centralgal].Mvir) / Gal[centralgal].Rvir);
      R_RamPressure = sqrt(Gal[i].HotGas / Gal[i].HotRadius) * sqrt(G * TotalMass_sat / Gal[i].Rvir) *
        sqrt(Gal[centralgal].Rvir / Gal[centralgal].HotGas) * R_Orbit * 1. / Vorbit;
    }

  /*Get the smaller of tidal and ram pressure stripping radii. */
  R_Stripping = min(R_Tidal, R_RamPressure);

  /*if the stripping radius is larger then hot radius there is
   * no stripping*/
  if(R_Stripping > Gal[i].HotRadius || Gal[i].HotGas < 1.e-8)
    hotremain = Gal[i].HotGas;
  // If stripping radius is smaller than the hot radius
  else
    {
      //Assuming M_hot(r) proportional to r, the remaining hot gas is given by:
      hotremain = Gal[i].HotGas * R_Stripping / Gal[i].HotRadius;
      // hot radius is updated to the stripping radius
      Gal[i].HotRadius = R_Stripping;

      // Check that HotRadius has sensible values
      if(Gal[i].HotRadius < 1.e-8)
        Gal[i].HotRadius = Gal[i].Len * PartMass / Gal[i].Mvir * Gal[i].Rvir;
      if(Gal[i].HotRadius > Gal[i].Rvir)
        Gal[i].HotRadius = Gal[i].Rvir;

    }

  if(hotremain > Gal[i].HotGas)
    hotremain = Gal[i].HotGas;

  return hotremain;
}
