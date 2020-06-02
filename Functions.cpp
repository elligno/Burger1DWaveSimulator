//
//***********************************************************************
//
//							   FUNCTIONS.CPP
//
//***********************************************************************

// #include "stdafx.h"
#include "burger.h"
#include  <cmath>
#include  <cstdlib>
#include  <cstdio.>

/*
===============================================================
								initial_cond
===============================================================
*/

//	AUTHOR: Jean Bélanger 
//	VERSION: August 2000

// compute intial condition

vector* initial_cond(vector* x)
{
	extern int N;
	extern float pi,dx;
	int i;

	vector* w;

	// v.r[i] = -sin(pi*x->r[i])   on the grid
	// average of v on each cell
	// Here we consider the average of the sine function over a cell of 
	// length Delat x = x(i+1) - x(i)

	w = new_vector ();

	for( i=0;i<N; i++)
		w->r[i] = ( cos(pi*x->r[i+1]) - cos(pi*x->r[i]) )/(pi*dx);
	w->r[N] = 0.0;

	return w;
}


/*
================================================================
							reconstr
================================================================
*/

//	AUTHOR: Jean Bélanger 
//	VERSION: August 2000

// reconstruction of v from its cell-average

vector* reconstr(vector* va)
{
	extern int N;
	extern float dx; 
	int i;
	float fd = (float)1.0/(12.0*dx);
	vector *v, *Primitive;
	stencil *Primitive_sten;

	// V = primitive of v

	Primitive = new_vector();
	Primitive->r[0]=0.0;
	for(i=1;i<=N;i++)
		Primitive->r[i] = Primitive->r[i-1] + va->r[i-1]*dx;
	
	// compute ENO stencil for the primitive function
	Primitive_sten = eno_stencil (Primitive);

	// compute first derivative from ENO stencil
	v = d1x( Primitive, Primitive_sten);

	// v = derivative of V
	//v=d1x(Primitive);

	free_vector (Primitive);
	free (Primitive_sten);

	return v;
}

/*
========================================================================
							       d1x
========================================================================
*/

//	AUTHOR: Jean Bélanger 
//	DATE 29 July 2000

// fourth order finite-difference derivative

/*
vector *d1x(vector *v)
{
	extern int N;
	extern float dx;
	int i;
	float fd = (float)1.0/(12.0*dx);
	vector *vx;

	vx=new_vector();
	for(i=2;i<N-1;i++)
		vx->r[i] = (v->r[i-2] - 8.0*v->r[i-1] + 8.0*v->r[i+1] - v->r[i+2])*fd;

// ----------------------------- end points ----------------------------

	// i=0
	vx->r[0] = (-25.0*v->r[0] + 48.0*v->r[1] - 36.0*v->r[2] + 16*v->r[3] - 3.0*v->r[4])*fd;
	// i=1
	vx->r[1] = (-3.0*v->r[0] - 10.0*v->r[1] + 18.0*v->r[2] - 6.0*v->r[3] + v->r[4])*fd;
	// i=N
	vx->r[N] = (25.0*v->r[N] - 48.0*v->r[N-1] + 36.0*v->r[N-2] - 16*v->r[N-3] + 3.0*v->r[N-4])*fd;
	// i=N-1
	vx->r[N-1] = (3.0*v->r[N] + 10.0*v->r[N-1] - 18.0*v->r[N-2] + 6.0*v->r[N-3] - v->r[N-4])*fd;
	
	return vx;
}
    */


/*=============================================================================
									flux
===============================================================================
*/

//	AUTHOR: Jean Bélanger
//	VERSION: August 2000

// computes fluxes on grid points  


vector* flux( vector* v)
{
	int i;
	extern float nu;
	extern int N;

	vector *f, *vx;
	stencil *s1;

	s1 = 0;   // central stencil

	f = new_vector ();

	// compute the first derivative
	// vx = d1x(v);
	vx = d1x(v,s1);
	
	// compute physical flux 
	for(i=0; i<=N;i++)
		f->r[i] = 0.5*v->r[i]*v->r[i] - nu*vx->r[i];

	free_vector(vx);

	return f;
}


/*
==============================================================================================
											time_step
==============================================================================================
*/

//	AUTHOR: Jean Bélanger 
//	VERSION: August 2000

//  time stepping by 2nd order Runge-Kutta (two steps : predictor-corrector)


vector *time_step( vector* v_averaged, float dt)
{
	extern int N, scheme_type;
	extern float dx;
	int i, sgn_a;
	float c1 = (float)0.5*dt/dx, c2 = (float)dt/dx;
	float a_i12;
	vector *v_reconstruct, *physflux, *nrcflux, *update;

//--------------------------- first step of the Runge-Kutta: predictor ------------------------------ 
	// reconstruction of the function from averaged solution 
	v_reconstruct = reconstr(v_averaged);         

	// Physical fluxes at grid points
	physflux = flux (v_reconstruct);           

	free_vector (v_reconstruct);
	nrcflux = new_vector();

	switch(scheme_type){
		case 1:
        // upwind one-sided scheme (negative propagation speed) ---> f_i+1/2 
			for(i=0;i<N;i++)
				nrcflux->r[i] = (physflux->r[i] - physflux->r[i+1])/dx;

			break;
		
		case 2:
	// Roe Scheme for Burger equation 
			for(i=0;i<N;i++) {
				a_i12 = (v_averaged->r[i+1] - v_averaged->r[i])/2.0;   // wave speed
				if(a_i12 <= 0)
					sgn_a = -1;      // direction of propagation
				else
					sgn_a = 1;

		// numerical flux for Burger equation (upwind based)
				nrcflux->r[i] = 0.5*(physflux->r[i] + physflux->r[i+1]) -
					            0.5*sgn_a*(physflux->r[i+1] - physflux->r[i]);
			}

			break;
	}
	
	free_vector(physflux);
	
	update=new_vector();
	
	// intermediate state	
	for(i=0;i<N;i++)
		update->r[i] = v_averaged->r[i] - c1*nrcflux->r[i];

//----------------------------second step of the Runge-Kutta: corrector ------------------------------
	v_reconstruct = reconstr(update);
	physflux = flux(v_reconstruct);
	
	free_vector(v_reconstruct);
	
	switch(scheme_type){
		case 1:
	// upwind one-sided scheme (negative propagation speed) at i+1/2
			for(i=0;i<N;i++)
				nrcflux->r[i] += (physflux->r[i] - physflux->r[i+1])/dx;

			break;
		case 2:
	// Roe Scheme for Burger equations
			for(i=0;i<N;i++){
				a_i12 = (update->r[i+1] - update->r[i])/2.0;
				if(a_i12 <= 0)
					sgn_a = -1;
				else
					sgn_a = 1;

			// numerical flux
				nrcflux->r[i] += 0.5*(physflux->r[i] + physflux->r[i+1]) -
					        0.5*sgn_a*(physflux->r[i+1] - physflux->r[i]);
			}
			
			break;
	}

	// final state
	for(i=0;i<N;i++)
		v_averaged->r[i] = v_averaged->r[i] - c2*nrcflux->r[i];
	
	free_vector(physflux); 
	free_vector(nrcflux); 
	free_vector(update);

	return v_averaged;
}