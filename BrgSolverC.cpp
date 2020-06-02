// BrgSolverC.cpp : Defines the entry point for the console application.
//

// #include "stdafx.h" in the original version
#include "burger.h"
#include  <stdio.h>
#include <string.h>

// AUTHOR: Jean Bélanger 
// DATE August 2000

// int i;
 float pi=(float)PI;
 float t = 0.0, dx;
// vector *x;

//---------------------------------------------------------------------------------------------
//										Parameters of run
//---------------------------------------------------------------------------------------------

int N=DIM;                 // grid size
int Nsu;                   // number of scalars used
int Nts=5;                 // number of time step
float dt=(float)0.01;      // time stepping
float nu=(float)0.05;      // viscosity
int order=2;               // order of spatial scheme
int scheme_type=1;         // 1: upwind one-sided, 2: Roe scheme
int sten_type=0;           // stencil: 0 (centered), 1 (ENO)
int recon_test=0;          // test reconstruction function
int deriv_test=0;          // test first derivative function
int stencil_test=0;        // test ENO stencil
int test0=0;               // test subroutines and exit
int readpar=1;             // readpar = 1 for readings parameters from file

/*
===============================================================

						 MAIN PROGRAM

===============================================================
*/

int BrgSolverC( /*int argc, char* argv[]*/)
{
// 	vector *v, *va;
// 	float val;
// 	int n;
// 	char par[16];

//	FILE *f1,*f2,*f3,*in;
	

//---------------------------------------------------------------------------------------------
//									Readings parameters
//---------------------------------------------------------------------------------------------

// 	FILE* FichierInit=NULL;
// 	char NomFichierInit[256];
// 	::strcpy_s(NomFichierInit, 256, "jbtest.dat");
// 
// 	errno_t w_err;
// 	// Open for write 
// 	w_err = fopen_s( &FichierInit, NomFichierInit, "w" );
// 	
// 	if( w_err == 0 )
// 	{
// 		printf( "The file 'jbtest.dat' was opened\n" );
// 	}
// 	else
// 	{
// 		printf( "The file 'jbtest.dat' was not opened\n" );
// 	}
// 
//     // original version
// 	if((in=fopen("jbtest.dat", "r")) == NULL){
// 		printf("Cannot open file.\n");
// 		printf(" \n");
// 		return 0;
// 	}
// 
// 	// read parameters of run from file
// 	while(readpar){
// 		fscanf(in,"%s%f",par,&val);
// 		if(id(par,"dt"))dt=val;
// 		if(id(par,"nu"))nu=val;
// 		if(id(par,"sten_type"))sten_type=(int)val;
// 		if(id(par,"scheme_type"))scheme_type=(int)val;
// 		if(id(par,"recon_test"))recon_test=(int)val;
// 		if(id(par,"deriv_test"))deriv_test=(int)val;
// 		if(id(par,"stencil_test"))stencil_test=(int)val;
// 		if(id(par,"test0"))test0=(int)val;
// 		if(id(par,"Nts"))Nts=(int)val;
// 		if(id(par,"order"))order=(int)val;
// 		if(id(par,"end"))readpar=(int)val; 
// 	}  
// 	
// 	fclose(in);


//--------------------------------------------------------------------------------------------
//									Printing parameters
//--------------------------------------------------------------------------------------------
	
// 	printf("\n\n ---------------------------------------------------------------------- \n");
// 	printf("                        DEMO BURGER SOLVER 1D                          \n ");
// 	printf(" --------------------------------------------------------------------------\n\n");
// 	printf(" N = %d \n", N);
// 	printf(" Nts = %d \n", Nts);
// 	printf(" Order = %d \n", order);
// 	printf(" nu = %e \n", nu);
// 	printf(" sten_type = %d \n", sten_type);
// 	printf(" scheme_type = %d \n", scheme_type);
// 	printf(" recon_test = %d \n", recon_test);
// 	printf(" deriv_test = %d \n", deriv_test);
// 	printf(" stencil_test %d \n", stencil_test);
// 	printf(" test0 = %d \n", test0);
// 	printf(" dt = %e \n", dt);
// 	printf("------------------------------------------------------------------------------\n\n");

/*
------------------------------------------------------------------------------------------------
											Initialisation
------------------------------------------------------------------------------------------------
*/
// 	f1=fopen("gridmesh", "w");
// 	f2=fopen("initialsolution", "w");
// 
// 	// spatial stepping
// 	dx = (float)(2.0*pi)/(float)N;        
// 	
// 	x = new_vector();
// 	// x coordinates of the grid
// 	gen_pts(x,dx);        
// 	for(i=0;i<=N;i++)
// 		fprintf(f1,"%8.4f \n", x->r[i] );
// 
// 	va = initial_cond(x);   // averaged solution on each cell 
// 	v = reconstr(va);       // reconstruction via primitive variables
// 
// 	printf("-----------------------------------------------\n");
// 	printf(" n");
// 	printf(" Initial Condition \n");
// 	printf(" ----------------------------------------------\n");
// 	for(i=0;i<N;i++)
// 		fprintf(f2,"%8.4f \n", va->r[i]);
// 	
// 	fclose(f2);
	
/*
------------------------------------------------------------------------------------------------
											Testing
------------------------------------------------------------------------------------------------
*/

// test functions used in the program
// 	if(recon_test) test_reconstr(x,N);
// 	if(deriv_test) test_derivative();
// 	if(stencil_test) test_stencil(N);
// 	if(test0) return 0;


/*
------------------------------------------------------------------------------------------------
											Calculation
------------------------------------------------------------------------------------------------
*/

// 	printf("---------------------------------------------------\n");
// 	printf("  \n");
// 	printf(" Calculation \n");
// 	printf("---------------------------------------------------\n");
// 
// 	f3=fopen("finalsolution", "w");
// 
// 	// time stepping and printing data
// 	// for the time integration, a Runge-Kutta of second order is used
// 	for(n=1; n<=Nts;n++){
// 		va = time_step(va,dt);
// 		t=t+dt;
// 		if(n == Nts){
// 			printf(" \n");
// 			printf("------------------------------------\n");
// 			printf(" t = %f \n", t);
// 			printf("------------------------------------\n");
// 			for(i=0;i<N;i++)
// 				fprintf(f3,"%8.4f \n", va->r[i]);
// 		}
// 	}
// 	free_vector(x); free_vector(va);
// 	fclose(f3);

	return 0;
}
