/*
  HEADER FILE FOR INTERACT
  and various other routines of the interact suite
  
  This file sources the following other .h files:
  
  - precision_double/single.h for macros that declare the numerical
    precision

  - macros.h for programming macros such as MIN(x,y)

  - filenames.h for all default filenames

  - structures.h for the fault, icm, and medium structure declarations
     this file is a good place to look for variable definitions

  - solver.h, nnls.h for the matrix solve routines from numerical recipes
    and the Lawson/Hanson routine

  - fortran_proto.h holds other external fortran function declarations

  - auto_proto.h automatically generated C function declarations


  Modify program parameters here on in the other .h files, not in the
  source itself.

  $Id: interact.h,v 2.63 2011/01/09 02:02:43 becker Exp becker $ 
*/
/* 
   system headers 
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>
#ifdef SUN_TRIG_FUNCTIONS
// use this as a test for running on Solaris
#include <ieeefp.h>
#endif
#ifdef USE_GEOPROJECT
#include "gmt.h" 
#endif

/* 

   other headers 

*/
// first floating types
// these are sometimes undefined in <limits.h>
#ifndef FLT_MAX
 #define FLT_MAX 3.40282347E+38F
#endif
#ifndef FLT_MIN
 #define FLT_MIN 1.17549435E-38F
#endif
#ifdef USE_DOUBLE_PRECISION
#include "precision_double.h"
#else
#include "precision_single.h"
#endif
// integer types, we use defines instead of typedef for 
// convenience
#define my_boolean  unsigned short
#define MODE_TYPE unsigned short int  
//
#ifdef USE_PGPLOT
// 
// Logical definition clashes with SuperLU logical definition
//#include "cpgplot.h" 
#endif
#ifdef USE_SLATEC_NNLS
#include "slatec_src.h"
#endif
/* for calls to external FORTRAN routines under SGI and other
 systems where we need an underscore */
#ifdef SGI_SUBROUTINE_CONVENTION
#include "fortran_underscore_defines.h"
#endif
// function definitions and real constants 
#include "macros.h"
// filenames
#include "filenames.h"
#include "geoproject.h"

// which random number generator should we use?
//#define RAND_GEN ran1
#define RAND_GEN ran2

//
// not-nan test function
#ifndef USE_HPUX_FUNCTIONS
#define FINITE_TEST(x) (finite(x))
#else
#define FINITE_TEST(x) (isfinite(x))
#endif

/*
  important constants for program execution 
  you might want to modify
*/
/* 
   set this to the default 
   maximum number of fault patches
   that will result in separate output files 
*/
#define MAX_NR_FLT_FILES_DEF 50
//
// minimum time step if variable time steps are chosen
// make multiple of precision, hopefully the compiler
// will catch that
// 
#define MIN_TIME_STEP (EPS_COMP_PREC/7)
// limit for solution iterations in rupture.c
#define RUPTURE_ITER_LIM 150
// critical cutoff values for coulomb stress (critical when stress > - crit_cutoff)
#define CRITICAL_STRESS_EPS (-EPS_COMP_PREC)
// if exhaustive slipping (not keep_slipping), 
// we could use larger values to allow for inaccuracies
//#define EXHAUSTIVE_CRITICAL_STRESS_EPS (-EPS_COMP_PREC*1000.0)
//
// for testing purposes, keep critical stress the same
#define EXHAUSTIVE_CRITICAL_STRESS_EPS (-EPS_COMP_PREC)

// if spatial correlations of stress field are correlated,
// update them each interval time
#define SPCORR_INTERVAL_DEF 0.5

/* 

   default cutoff value for sparse I matrix

*/
#define I_MAT_CUTOFF_DEF 5.0e-5
/*

 singular value threshold for nearly singular matrices in the SVD
solver.  all entries smaller than this fraction times the maximum
singular value will be set to zero

*/
#define SVD_THRESHOLD EPS_COMP_PREC
/*
  default operational mode switches, can be 
  modified by command line arguments

  note that some of the defaults are set in properties.h

*/
#define READ_FAULT_PROPERTIES_DEF TRUE
#define READ_INITIAL_FAULT_STRESS_DEF TRUE
#define SUPPRESS_INTERACTIONS_DEF FALSE
#define WHOLE_FAULT_MODE_DEF FALSE
#define KEEP_SLIPPING_DEF TRUE
#define NO_OPENING_MODES_DEF FALSE
#define READ_STRESS_RELATION_FACTORS_DEF TRUE
#define PRINT_SLIPLINE_DEF FALSE
#define WHOLE_FAULT_DEACTIVATIONS_DEF FALSE
#define USE_SPARSE_STORAGE_DEF FALSE
#define USE_OLD_IMAT_DEF FALSE
#define SAVE_IMAT_DEF FALSE
#define USE_OLD_AMAT_DEF FALSE
#define SAVE_AMAT_DEF FALSE
#define CHECK_FOR_INTERACTION_FEEDBACK_DEF FALSE 
#define SUPPRESS_NAN_OUTPUT_DEF TRUE // suppress NaNs in output
#define GEOMVIEW_OUTPUT_DEF FALSE // output of files in geomview's OFF format
#define TWO_DIM_APPROX_IS_PLANE_STRESS_DEF FALSE
#define PRINT_PLANE_COORD_DEF FALSE
#define HALF_PLANE_DEF FALSE
#define CONSTANT_TIME_STEP_DEF FALSE
#define DEBUG_DEF FALSE
/* 
   indices used in programs, do not change 
   the ordering here!!!
*/
#define STRIKE 0
#define DIP 1
#define NORMAL 2

#define X 0
#define Y 1
#define Z 2

#define R 0
#define THETA 1
#define PHI 2

#define EIGEN_E1 2      /* the eigenvalues returned by calc_eigensystem_sym
		     are sorted ascendingly, hence if e1 > e2 > e3,
		     those are the indices */
#define EIGEN_E2 1
#define EIGEN_E3 0 

//
// symmetric matrix elements in fortran storage
//
// XX XY XZ   0 3 6
// YX YY YZ   1 4 7
// Zx ZY ZZ   2 5 8
// 
#define XX 0 
#define XY 3
#define XZ 6
#define YY 4
#define YZ 7
#define ZZ 8
#define YX 1 
#define ZX 2
#define ZY 5




/* operational modes for interact */
#define ONE_STEP_CALCULATION 1
#define SIMULATE_LOADING 2
#define SIMULATE_LOADING_AND_PLOT 3
/*
  patch ('element') type

  0: rectangular
  1: point source
  2: triangular (not implemented yet)
  3: 2-D segment, will automatically switch to 2-D mode
  4: like 3, but plane stress instead of plane strain
  5: like 3, but half plane (y<=0)
*/
#define RECTANGULAR_PATCH 0
#define POINT_SOURCE 1
#define TRIANGULAR 2
#define TWO_DIM_SEGMENT_PLANE_STRAIN 3
#define TWO_DIM_SEGMENT_PLANE_STRESS 4
#define TWO_DIM_HALFPLANE_PLANE_STRAIN 5
/* 
   output modes for print_patch_geometry
*/
#define PATCH_OUT_MODE 0
#define GEOMVIEW_MODE 1
#define PSXYZ_MODE 2
#define PSXYZ_SCALAR_MODE 7
#define BC_OUT_MODE 3
#define CORNEROUT_MODE 4
#define VEC_OUT_MODE 5
#define XYZ_AND_VEC_MODE 6
/* 

   activation stress boundary condition 
   modes for the faults 
   make sure to not exceed the MODE_TYPE 
   variable type (255?)

*/
#define INACTIVE 0
#define STRIKE_SLIP 10
#define STRIKE_SLIP_LEFTLATERAL 11
#define STRIKE_SLIP_RIGHTLATERAL 12
#define DIP_SLIP 20
#define DIP_SLIP_UPWARD 21
#define DIP_SLIP_DOWNWARD 22
#define NORMAL_SLIP 30
#define NORMAL_SLIP_OUTWARD 31
#define NORMAL_SLIP_INWARD 32
#define MAXSDIR_SLIP 40

#define SHEAR_STRESS_FREE 50
#define SHEAR_STRESS_FRICTION 51
#define SHEAR_STRESS_FREE_STRIKE_ONLY 52
/* 
   if the above bcs are chosen for a one-step calculation
   this offset in the code will lead to a correction for normal
   stress changes during rupture according to 
   \Delta \sigma_n \times \mu_d
*/
#define OS_C_OFFSET 100
// other codes
#define COULOMB_STRIKE_SLIP (STRIKE_SLIP+OS_C_OFFSET)
#define COULOMB_STRIKE_SLIP_LEFTLATERAL (STRIKE_SLIP_LEFTLATERAL+OS_C_OFFSET)
#define COULOMB_STRIKE_SLIP_RIGHTLATERAL (STRIKE_SLIP_RIGHTLATERAL+OS_C_OFFSET)
#define COULOMB_DIP_SLIP (DIP_SLIP+OS_C_OFFSET) 
#define COULOMB_DIP_SLIP_UPWARD (DIP_SLIP_UPWARD+OS_C_OFFSET) 
#define COULOMB_DIP_SLIP_DOWNWARD (DIP_SLIP_DOWNWARD+OS_C_OFFSET) 
#define COULOMB_MAXSDIR_SLIP  (MAXSDIR_SLIP+OS_C_OFFSET)


#define ACTIVATED 1

/* 
   operational modes for obtaining the interaction
   coefficients 
*/
#define I_MAT_IN_MEMORY 0
#define I_MAT_ON_FILE 1
#define CALC_I_COEFF_NOW 2
#define SPARSE_I_MAT 3
/*
  
  solver modes for the unconstrained 
  A x = b part

*/
#define LU_SOLVER 0
#define SVD_SOLVER 1
#define SPARSE_SOLVER 2

// random modes

#define UNIFORM_DISTRIBUTED 1
#define GAUSS_DISTRIBUTED 2

/* 
   minimum range for xmin/xmax as used to determine the 
   size of the X plotting window
*/
#define MIN_GEOM_RANGE 5.0
/* 
   offset operators for the interaction matrix 
   and 3d fields 
*/
#ifdef NO_OPENING_MODES
// opening modes are suppressed, this assumes that strike and dip are 0 and 1 logically
 #define NRMODE_DEF 2
#else
// all modes are possible
 #define NRMODE_DEF 3
#endif
// reference to I matrix elements
#define ICIM(a,i,j,k,l) ((*(a+i) +j)->s[k][l])
// and if i matrix pointer is passed as ***icm
#define ICIMP(a,i,j,k,l) ((*(*(a)+i) +j)->s[k][l])
// reference in a medium->nmat^2 style
#define POSII(j,k) ((j)*NRMODE_DEF+(k))
#define POSIJ(i,l) ((i)*3+(l))

// references to 3D deformation and stress fields
#define POSS(i, j, k, l) (((k * nxy)+(i * medium->n[Y])+(j))*6+(l))
#define POSU(i, j, k, l) (((k * nxy)+(i * medium->n[Y])+(j))*3+(l))
// the fault and medium structures
#include "structures.h"
// geographic projection routines
#include "myprojectsimple.h"

#ifdef USE_GEOPROJECT 
extern void GMT_end(int, char **);
#endif
//
// FORTRAN (underscore defines have been taken care of above)
//
#include "fortran_proto.h"
#include "numrec_svd_routines.h"
// the C function prototypes, manually 
// #include "manual_proto.h"
// or automatically genrated
#ifdef USE_DOUBLE_PRECISION
#include "auto_proto.h"
#else
#include "auto_proto.sgl.h"
#endif

#ifndef NO_LAPACK3
#define USE_LAPACK_DAC
#endif

/*
  auto proto only finds the last of these functions, 
  list them here
*/
void assemble_a_matrix_1(A_MATRIX_PREC *,int ,my_boolean *,int, int *,struct flt *fault,struct med *);
void assemble_a_matrix_2(A_MATRIX_PREC *,int ,my_boolean *,int, int *,struct flt *fault,struct med *);
void assemble_a_matrix_3(A_MATRIX_PREC *,int ,my_boolean *,int, int *,struct flt *fault,struct med *);
void assemble_a_matrix_4(A_MATRIX_PREC *,int ,my_boolean *,int, int *,struct flt *fault,struct med *);
void assemble_ap_matrix_1(A_MATRIX_PREC *,int ,int ,my_boolean *,my_boolean *,int ,int ,int *,int *_con,struct flt *,struct med *);
void assemble_ap_matrix_2(A_MATRIX_PREC *,int ,int ,my_boolean *,my_boolean *,int ,int ,int *,int *_con,struct flt *,struct med *);
void assemble_ap_matrix_3(A_MATRIX_PREC *,int ,int ,my_boolean *,my_boolean *,int ,int ,int *,int *_con,struct flt *,struct med *);
void assemble_ap_matrix_4(A_MATRIX_PREC *,int ,int ,my_boolean *,my_boolean *,int ,int ,int *,int *_con,struct flt *,struct med *);
void add_quake_stress_1(my_boolean *,COMP_PRECISION *,int ,struct flt *,struct med * );
void add_quake_stress_2(my_boolean *,COMP_PRECISION *,int ,struct flt *,struct med * );
void add_quake_stress_3(my_boolean *,COMP_PRECISION *,int ,struct flt *,struct med * );
void add_quake_stress_4(my_boolean *,COMP_PRECISION *,int ,struct flt *,struct med * );
my_boolean check_coulomb_stress_feedback_1(int, int, struct flt *, struct med *, my_boolean, my_boolean,int*,COMP_PRECISION);
my_boolean check_coulomb_stress_feedback_2(int, int, struct flt *, struct med *, my_boolean, my_boolean,int*,COMP_PRECISION);
my_boolean check_coulomb_stress_feedback_3(int, int, struct flt *, struct med *, my_boolean, my_boolean,int*,COMP_PRECISION);
my_boolean check_coulomb_stress_feedback_4(int, int, struct flt *, struct med *, my_boolean, my_boolean,int*,COMP_PRECISION);




#ifdef DMALLOC
#include "dmalloc.h"
#endif

