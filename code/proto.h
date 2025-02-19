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

#include "allvars.h"
#include <stdbool.h>

#ifndef MCMC
void SAM(int filenr);
#endif
void construct_galaxies(int filenr, int tree, int halonr);
int join_galaxies_of_progenitors(int halonr, int ngalstart, int *cenngal);
void evolve_galaxies(int halonr, int ngal, int tree, int cenngal);

size_t myfread(void *ptr, size_t size, size_t nmemb, FILE * stream);
size_t myfwrite(void *ptr, size_t size, size_t nmemb, FILE * stream);
int myfseek(FILE * stream, long offset, int whence);
void load_all_auxdata(int filenr);
void load_all_dbids(int filenr);
void load_all_treedata(int filenr);
void write_all_galaxy_data(int filenr);
void write_galaxy_data_snap(int n, int filenr);

void save_galaxy_tree(int filenr, int tree);
int walk(int nr);

int peano_hilbert_key(int x, int y, int z, int bits);
void update_type_two_coordinate_and_velocity(int tree, int i, int centralgal);
void output_galaxy(int treenr, int heap_index);

void close_galaxy_tree_file(void);
void create_galaxy_tree_file(int filenr);
void save_galaxy_tree_append(int i);
void save_galaxy_tree_finalize(int filenr, int tree);
void prepare_galaxy_tree_info_for_output(int filenr, int tree, struct galaxy_tree_data *g,
                                         struct GALAXY_OUTPUT *o);
int walk_galaxy_tree(int nr);
int save_galaxy_tree_compare(const void *a, const void *b);

void save_galaxy_append(int tree, int i, int n);
void close_galaxy_files(void);
void create_galaxy_files(int filenr);

long long calc_big_db_subid_index(int snapnum, int filenr, int subhaloindex);

void dump_memory_table(void);
void *myrealloc_movable_fullinfo(void *p, size_t n, const char *func, const char *file, int line);
void *myrealloc_fullinfo(void *p, size_t n, const char *func, const char *file, int line);
void myfree_movable_fullinfo(void *p, const char *func, const char *file, int line);
void myfree_fullinfo(void *p, const char *func, const char *file, int line);
void *mymalloc_movable_fullinfo(void *ptr, const char *varname, size_t n, const char *func, const char *file,
                                int line);
void *mymalloc_fullinfo(const char *varname, size_t n, const char *func, const char *file, int line);
void report_detailed_memory_usage_of_largest_task(size_t *OldHighMarkBytes, const char *label,
                                                  const char *func, const char *file, int line);
void mymalloc_init(void);

void save_galaxy_tree_reorder_on_disk(void);
int save_galaxy_tree_mp_comp(const void *a, const void *b);

void get_coordinates(float *pos, float *vel, long long ID, int tree, int halonr, int snapnum);


//functions used to scale to a different cosmology
void read_scaling_parameters();
void scale_cosmology(int nhalos);
void un_scale_cosmology(int nhalos);
void read_zlist_original_cosm(void);
void read_zlist_new(void);
double c_correction(float mass, int snapnum);
double find_c(double c_ori, double ratio);
double func_c(double c);
double dgrowth_factor_dt(double a, double omega_m, double omega_l);
double scale_v_cen(int snapnum);

long long calc_big_db_offset(int filenr, int treenr);
void prepare_galaxy_tree_info_for_output(int filenr, int tree, struct galaxy_tree_data *g,
                                         struct GALAXY_OUTPUT *o);

void init(void);
void set_units(void);

void load_tree_table(int filenr);
void load_tree(int nr);
void save_galaxies(int filenr, int tree);
int save_galaxy_tree_compare(const void *a, const void *b);
void prepare_galaxy_for_output(int n, struct GALAXY *g, struct GALAXY_OUTPUT *o);
void fix_units_for_ouput(struct GALAXY_OUTPUT *o);

void free_galaxies_and_tree(void);
void free_tree_table(void);
void endrun(int ierr);

void read_parameter_file(char *fname);
void finalize_galaxy_file(int filenr);

#ifdef SPECIFYFILENR
void read_file_nrs(void);
#endif
float get_nr_files_to_process(int ThisTask);
void assign_files_to_tasks(int *FileToProcess, int *TaskToProcess, int ThisTask, int NTask, int nfiles);

void starformation(int p, int centralgal, double time, double dt, int nstep);
void update_stars_due_to_reheat(int p, int centralgal, double *stars);
void update_from_star_formation(int p, double stars, bool flag_burst, int nstep);
void SN_feedback(int p, int centralgal, double stars, const char feedback_location[]);
void update_from_feedback(int p, int centralgal, double reheated_mass, double ejected_mass);
void update_massweightage(int p, double stars, double time);

void add_galaxies_together(int t, int p);
void init_galaxy(int p, int halonr);
double infall_recipe(int centralgal, int ngal, double Zcurr);
void add_infall_to_hot(int centralgal, double infallingGas);
void compute_cooling(int p, double dt, int ngal);
void do_AGN_heating(double dt, int ngal);
void cool_gas_onto_galaxy(int p, double dt);
void reincorporate_gas(int p, double dt);
void deal_with_galaxy_merger(int p, int merger_centralgal, int centralgal, double time, double deltaT,
                             int nstep);
double do_reionization(float Mvir, double Zcurr);
double NumToTime(int snapnum);



//MATH MISC
void locate(double *xx, int n, double x, int *j);
double integrate(double *flux, int Grid_Length);
void polint(double xa[], double ya[], int n, double x, double *y, double *dy);
void nrerror(const char error_text[]);
double *vector(long nl, long nh);
void free_vector(double *v, long nl, long nh);


//SPECTRO/PHOTOMETRY PROPERTIES
#ifdef COMPUTE_SPECPHOT_PROPERTIES

#ifdef PHOTTABLES_PRECOMPUTED
void setup_LumTables_precomputed(const char SimName[]);
#endif

#ifdef SPEC_PHOTABLES_ON_THE_FLY

void setup_Spec_LumTables_onthefly(void);

//initialize
void setup_RedshiftTab();
void read_vega_spectra(double *LambdaVega, double *FluxVega);
void read_filters(double LambdaFilter[NMAG][MAX_NLambdaFilter], double FluxFilter[NMAG][MAX_NLambdaFilter]);
void read_MetalTab();
void read_InputSSP_spectra(double LambdaInputSSP[SSP_NAGES][SSP_NLambda],
                           double FluxInputSSP[SSP_NAGES][SSP_NLambda], int MetalLoop);
void free_input_spectra(double LambdaInputSSP[SSP_NAGES][SSP_NLambda],
                        double FluxInputSSP[SSP_NAGES][SSP_NLambda]);

//misc
double get_AbsAB_magnitude(double FluxInputSSPInt, double FluxFilterInt, double redshift);
double get_area(double redshift);
double lum_distance(double redshift);

//numerical
double *create_grid(double WaveMin, double WaveMax, int AgeLoop, double redshift,
                    double LambdaInputSSP[SSP_NAGES][SSP_NLambda], int *Min_Wave_Grid, int *Max_Wave_Grid,
                    int *Grid_Length);
void interpolate(double *lgrid, int Grid_Length, double *lambda, int nlambda, double *flux,
                 double *FluxOnGrid);
#endif //SPEC_PHOTABLES_ON_THE_FLY

void find_interpolated_lum(double timenow, double timetarget, double metallicity, int *metindex,
                           int *tabindex, double *f1, double *f2, double *fmet1, double *fmet2);

#ifdef POST_PROCESS_MAGS
void post_process_spec_mags(struct GALAXY_OUTPUT *o);
void compute_post_process_lum(double mass, double age, double metals, int snap, int nlum, double *Lum,
                              double *ObsLum, double *dObsLum, double *dObsLum_forward, double *YLum,
                              double *ObsYLum, double *dObsYLum, double *dObsYLum_forward);
void dust_correction_for_post_processing(int nlum, int snap, double Zg, double ColdGas, double GasDiskRadius,
                                         double CosInclination, double LumDisk, double ObsLumDisk,
                                         double dObsLumDisk, double dObsLumDisk_forward, double YLumDisk,
                                         double ObsYLumDisk, double dObsYLumDisk, double dObsYLumDisk_forward,
                                         double *LumDiskDust, double *ObsLumDiskDust, double *dObsLumDiskDust,
                                         double *dObsLumDiskDust_forward);
#else
void add_to_luminosities(int p, double mstars, double time, double dt, double metallicity);
void dust_model(int p, int snap, int halonr);
#endif

// dust model
void read_dust_tables(void);
double get_extinction(int mag, double Zg, double redshift);

#endif //COMPUTE_SPECPHOT_PROPERTIES






double estimate_merging_time(int halonr, int mostmassive, int p);

double hubble_of_z(int halonr);
double get_virial_velocity(int halonr);
double get_virial_radius(int halonr);
double get_virial_mass(int halonr);
double collisional_starburst_recipe(double mass_ratio, int merger_centralgal, int centralgal, double time,
                                    double deltaT);

void make_bulge_from_burst(int p);
void grow_black_hole(int merger_centralgal, double mass_ratio, double deltaT);
void check_disk_instability(int p);

double lum_to_mag(double lum);


double slab_model(float tau, float teta);
double get_metallicity(double gas, double metals);

double get_disk_radius(int halonr, int p);
void get_gas_disk_radius(int p);
void get_stellar_disk_radius(int p);

void read_output_snaps(void);
void read_zlist(void);


void read_cooling_functions(void);
double get_metaldependent_cooling_rate(double logTemp, double logZ);
double get_rate(int tab, double logTemp);

double time_to_present(double z);
double integrand_time_to_present(double a, void *param);
void find_interpolate_reionization(double zcurr, int *tabindex, double *f1, double *f2);

void init_jump_index(void);
int get_jump_index(double age);

void disrupt(int p);
double peri_radius(int p, int centralgal);


double hot_retain_sat(int i, int centralgal);
void check_options();

double get_initial_disk_radius(int halonr, int p);
void update_bulge_from_disk(int p, double stars);
double bulge_from_disk(double frac);
double func_size(double x, double a);
void bulgesize_from_merger(double mass_ratio, int merger_centralgal, int p, double Mcstar, double Mcbulge,
                           double Mcgas, double Mpstar, double Mpbulge, double Mpgas, double frac);
double isothermal_mass(double Mvir, double Rvir, double dr);
double diskmass(double x);
double bulgemass(double x);
double sat_radius(int p);

void update_type_2(int ngal, int halonr, int prog, int mostmassive);
void update_centralgal(int ngal, int halonr);
void update_hotgas(int ngal, int centralgal);
void update_type_1(int ngal, int halonr, int prog);
void transfer_ICL(int p, int q, double fraction);
double separation_gal(int p, int q);
double separation_halo(int p, int q);


/* check funs */
void update_hot_frac(int p, double reincorporated, float HotGas);
int set_merger_center(int fofhalo);

void transfer_stars(int p, const char cp[], int q, const char cq[], double fraction);
void transfer_gas(int p, const char cp[], int q, const char cq[], double fraction, const char call_function[],
                  int line);
void deal_with_satellites(int centralgal, int ngal);
void mass_checks(const char *string, int igal);

#ifdef STAR_FORMATION_HISTORY
void sfh_initialise(int p);
void sfh_merge(int p, int p1);
void sfh_print(int p);
void sfh_update_bins(int p, int snap, int step, double time);
void create_sfh_bins();
void write_sfh_bins();
#endif //STAR_FORMATION_HISTORY

#ifdef DETAILED_METALS_AND_MASS_RETURN
struct metals metals_add(struct metals m1, struct metals m2, float fraction);
struct metals metals_init();
void metals_print(const char s[], struct metals m);
float metals_total(struct metals m);
#else
float metals_add(float m1, float m2, float fraction);
float metals_init();
void metals_print(const char s[], float m);
float metals_total(float m);
#endif

#ifdef DETAILED_METALS_AND_MASS_RETURN
//in read_yield_tables.c:
void read_yield_tables();
double Chabrier_IMF(double M);

//in calc_SNe_rates.c:
#ifdef INDIVIDUAL_ELEMENTS
void SNe_rates();
#endif

//in yield_integrals.c:
void init_integrated_yields();
void integrate_yields();
int find_initial_metallicity_comp(int Zi, int sfh_bin, int table_type);
int find_initial_mass(double lifetime, int Zi_bin);
int max_Mi_lower(int Mi_lower, int channel_type);
int min_Mi_upper(int Mi_upper, int channel_type);
int find_SNII_mass_bin(double masslimit);
int find_agb_mass_bin(double masslimit);

#ifdef DTD
double DTDcalc(double timevalue);
#endif
#ifdef INDIVIDUAL_ELEMENTS
void find_actual_ejecta_limits(int channel_type, double Mi_lower_actual, double Mi_upper_actual, int Mi_lower,
                               int Mi_upper, int Zi, double *EjectedMasses_lower_actual,
                               double *EjectedMasses_upper_actual, double *TotalMetals_lower_actual,
                               double *TotalMetals_upper_actual, double *Yields_lower_actual,
                               double *Yields_upper_actual);
#else
void find_actual_ejecta_limits(int channel_type, double Mi_lower_actual, double Mi_upper_actual, int Mi_lower,
                               int Mi_upper, int Zi, double *EjectedMasses_lower_actual,
                               double *EjectedMasses_upper_actual, double *TotalMetals_lower_actual,
                               double *TotalMetals_upper_actual);
#endif

void print_galaxy(char string[], int p, int halonr);


//in elements.c:
struct elements elements_add(struct elements ele1, struct elements ele2, float fraction);
struct elements elements_init();
void elements_print(char s[], struct elements ele);
double elements_total(struct elements ele);
double metal_elements_total(struct elements ele);

//in recipe_yields.c:
void update_yields_and_return_mass(int p, int centralgal, double dt, int nstep);
int find_initial_metallicity(int p, int sfh_bin, int table_type, int component_type);

#ifdef INSTANTANEOUS_RECYCLE
void reset_ejection_rates(int i, int sfh_ibin,
                          double *NormSNIIMassEjecRate_actual, double *NormSNIIMetalEjecRate_actual,
                          double *NormSNIaMassEjecRate_actual, double *NormAGBMassEjecRate_actual,
                          double *NormSNIaMetalEjecRate_actual, double *NormAGBMetalEjecRate_actual);
#endif

#endif //DETAILED_METALS_AND_MASS_RETURN

void print_galaxy(char string[], int p, int halonr);
