//
//**********************************************************************************************
//
//										ENO_DERIVATIVE.CPP
//
//**********************************************************************************************

// #include "stdafx.h"
#include "burger.h"
#include  <cmath>
#include  <cstdlib>
#include  <cstdio>

#define STX s->x[i]

/*
=============================================================================================
											 eno_stencil
=============================================================================================

//	AUTHOR: Jean Bélanger 
//	DATE: 30 July 2000


/**      Computation of stencil for ENO reconstruction.
 *      Returns the CENTRAL index of the stencil
 *
 * 
 *	SECOND ORDER STENCIL
 *	---------------------
 *     Determine the central index of the stencil.We use the divided difference at first
 *	  and second order to obtain information about the behavior of the function.
 *	  
 *		1) first order divided difference: which interval [i-1,i] or [i,i+1] the function 
 *		   is smoother
 *
 *		2) to obtain second order we need to add a grid point. Second order divided difference
 *			are used to determine in which interval the function is smoother
 *  		    For each case two possibility: 
 *				a) [i-1,i] : 
 *					add a point to the right: [i-1,i,i+1] the central index is i
 *					add a point to the left : [i-2,i-1,i] the central index is i-1
 *
 *				b) [i,i+1]
 *					add a point to the right: [i,i+1,i+2] the central index is i+1
 *					add a point to the left : [i-1,i,i+1] the central index is i
 *
 *	FOURTH ORDER STENCIL
 *	--------------------
*/


/*

  TO BE VERIFIED
	NOTA BENE: je ne suis pas certain de la ligne suivante dans 
           periodic_stencil:
           i=N;
           ( a2->r[st] < a2->r[STX] ) STX = st;
           cette ligne est a verifier.                     */
           

stencil * eno_stencil (vector * a)
{
	extern int N, order, sten_type;
	extern float dx;
	int i, periodic_stencil=0;
	short int st;
	vector *a1, *a2, *a3, *a4;
	float dxi = (float)1.0/dx;
	stencil *s;

	s = (stencil *) malloc(sizeof(stencil));

/* nth derivatives ( a1[i] is da/dx at i+1/2,
   a2[i] is d2a/dx2 at i, etc ...).
   Caveat: To reconstruct a function at order n, one needs the
   derivatives of its primitive up to order n+1.               */

	if(s == 0 || sten_type == 0) return s;

	a1 = new_vector ();  
	a2 = new_vector ();
	
	if(order == 4) { 
		a3 = new_vector();  
		a4 = new_vector(); 
	}
	// second order stencil
	if(order == 2){
		for(i=0;i<N;i++)
			a1->r[i] = a->r[i+1] - a->r[i];            // divided difference at first order
		
		a1->r[N] = a->r[1] - a->r[N];                  // extrapolation formula

		for(i=1; i<=N; i++)
			a2->r[i] = a1->r[i] - a1->r[i-1];          // divided difference at second order
		
		a2->r[0] = a1->r[0] - (a->r[0] - a->r[N-1]);   // extrapolation formula

		// this is in case a is not periodic  ?
		for(i=0; i<=N; i++){ 
			a1->r[i] = fabs(a1->r[i]);
			a2->r[i] = fabs(a2->r[i]);
			STX = i;						// initialize:fix the central index to i
		}
		
		// Computation of the central index for a second order stencil. This is done in two
		// steps. First step compute the left index i-1 or right index i+1 according to the  
		// first order divided difference. Second step add a point to the right or left to 
		// obtain the three points stencil with the central index STX. 
		for(i=1; i<N; i++){  
			if( a1->r[i-1] < a1->r[i] ) STX = i-1;       // true: i-1,i, false: i,i+1      
			if( a2->r[STX+1] < a2->r[STX] ) STX += 1;    // add a point (right or left) 
		}
		
		if(periodic_stencil){
			i=0;
			if(a->r[0] - a->r[N-1] < a1->r[0]) STX=N-1;
			if(a2->r[STX+1] < a2->r[STX]) STX = (STX+1)%N;

			i=N;
			if(a1->r[N-1] < a1->r[N]) STX = N-1;
			st = (STX+1 > N) ? STX+1-N : STX+1;
			if( a2->r[st] < a2->r[STX] ) STX = st; 
		}

		else{
			s->x[0] = 1;  s->x[N] = N-1;
			if( s->x[1] == 0 ) s->x[1] = 1;
			if( s->x[N-1] == N ) s->x[N-1] = N-1;
		}
	}

    // fourth order stencil  
	else if(order == 4){
		for(i=0; i<N; i++)
			a1->r[i] = (a->r[i+1] - a->r[i])*dxi;      // divided difference at first order

		for(i=1; i<N; i++)
			a2->r[i] = (a1->r[i] - a1->r[i-1])*dxi;    // divided difference at second order

		for(i=1; i<N-1; i++)
			a3->r[i] = (a2->r[i+1] - a2->r[i])*dxi;    // 

		for(i=2; i<N; i++)
			a4->r[i] = (a3->r[i] - a3->r[i-1])*dxi;

		// .......
		a1->r[0] = 1.e+33;  a2->r[0] = 1.e+33;
		a3->r[0] = 1.e+33;  a4->r[0] = 1.e+33;
		a1->r[N] = 1.e+33;  a2->r[N] = 1.e+33;
		a3->r[N] = 1.e+33;  a4->r[N] = 1.e+33;
		a3->r[1] = 1.e+33;  a4->r[1] = 1.e+33;
		a3->r[N-1] = 1.e+33;  a4->r[N-1] = 1.e+33;

		for(i=0; i<=N; i++){
			a1->r[i] = fabs(a1->r[i]);
			a2->r[i] = fabs(a2->r[i]);
			a3->r[i] = fabs(a3->r[i]);
			a4->r[i] = fabs(a4->r[i]);
		}

		for(i=1; i<N; i++)
			s->x[i] = i;       // central index of the stencil

		for(i=2; i<N-1; i++){
			if(a1->r[i-1] < a1->r[i]) STX = i-1;
			if(a2->r[STX+1] < a2->r[STX]) STX += 1;
			if(a3->r[STX-1] < a3->r[STX]) STX -= 1;
			if(a4->r[STX+1] < a4->r[STX]) STX += 1;
		}

		i = 0;  STX = 2;
		i = 1;  STX = 2;
		if(a4->r[3] < a4->r[2]) STX = 3;
		
		i = N;  STX = N-2;
		i = N-1; STX = N-2;
		if(a4->r[N-3] < a4->r[N-2]) STX = N-3;
	}

	free_vector(a1);  
	free_vector(a2);

	if(order == 4){ 
		free_vector(a3); 
		free_vector(a4); 
	} 

	return s; 
}     

 
/*
==========================================================================================
										d1x
==========================================================================================
*/

//	AUTHOR: Jean Bélanger 
//	DATE: 30 July 2000

//  returns da/dx computed by an ENO method.

/*
	we are at the grid point i, according to the central index computed at this point
		central index = i-1 then the stencil is [i,i-1,i-2] (non-centered formula)
		central index = i then the stencil is [i-1,i,i+1]  (central difference)
		central index = i+1 then the stencil [i+2,i+1,i]  (non-centered formula)
*/

// NOTE: the formula used to compute the derivative are standard finite-difference
//        computed from a Taylor expansion at required order with the truncation error


// I NEED TO CONVERT FROM DOUBLE TO FLOAT
#pragma warning (disable:4244)      

vector* d1x( vector* a, stencil* s)
{
	extern int N, sten_type, order;
	extern float dx;
	int i;
	float f2 = (float)0.5/dx, f12 = (float)1.0/(12.0*dx);
	vector *b;

	b = new_vector();

	// second order derivative (central difference)
	if( order == 2 ) {
		if( s == 0 || sten_type == 0 ){
			for(i=1; i<N; i++)
				b->r[i] = ( a->r[i+1] - a->r[i-1] )*f2;	    // central difference (2nd order) 
			
			// boundary points
			b->r[0] = ( a->r[1] - a->r[N-1] )*f2;	        // centered formula 
			b->r[N] = b->r[0];                            // periodic boundary condition 
		}
		
		// sten_type != 0 then use ENO stencil to evaluate the derivative
		else{
			for(i=2; i<N-1; i++){
				b->r[i] = ( a->r[i+1] - a->r[i-1] )*f2;    // (central index i, centered stencil)
			
			// central index i+1, non-centered stencil
				if( s->x[i] == i+1 )                        
					b->r[i] = ( - a->r[i+2] + 4.0*a->r[i+1] - 3.0*a->r[i] )*f2;
			
			// central index i-1, non-centerd stencil
				else if(s->x[i] == i-1)            
          b->r[i] = ( 3.0*a->r[i] - 4.0*a->r[i-1] + a->r[i-2] )*f2;
			}

	// *********************** end points i=0,1 and N-1,N ************************
		
		// ---------------------------- i=0
      if(s->x[0] == 0)
        b->r[0] = ( a->r[1] - a->r[N-1] )*f2;     // centered formula (extrapolation)
        
	  else if(s->x[0] == 1)
        b->r[0] = ( - a->r[2] + 4.0*a->r[1] - 3.0*a->r[0] )*f2;
        
	  else if(s->x[0] == N-1)
        b->r[0] = ( 3.0*a->r[0] - 4.0*a->r[N-1] + a->r[N-2] )*f2;
	
			// -------------------------- i=1
      if(s->x[1] == 1)
        b->r[1] = ( a->r[2] - a->r[0] )*f2;
        
			else if(s->x[1] == 2)
        b->r[1] = ( - a->r[3] + 4.0*a->r[2] - 3.0*a->r[1] )*f2;
        
			else if(s->x[1] == 0)
        b->r[1] = ( 3.0*a->r[1] - 4.0*a->r[0] + a->r[N-1] )*f2;
	
			// -------------------------- i=N-1
      if(s->x[N-1] == N-1)
        b->r[N-1] = ( a->r[N] - a->r[N-2] ) * f2;
        
			else if(s->x[N-1] == N || s->x[N-1] == 0)      // because of periodicity ?
        b->r[N-1] = ( - a->r[1] + 4.0* a->r[N] - 3.0*a->r[N-1] )*f2;
        
			else if(s->x[N-1] == N-2)                  
        b->r[N-1] = ( 3.0*a->r[N-1] - 4.0*a->r[N-2]  + 3.0*a->r[N-3] )*f2;
		
			// ---------------------------- i=N
      if(s->x[N] == N || s->x[N] == 0)             // because of periodicity ?
          b->r[N] = ( a->r[1] - a->r[N-1] )*f2;
        
			else if(s->x[N] == 1)
				  b->r[N] = ( - a->r[2] + 4.0*a->r[1] - 3.0*a->r[N] )*f2;
        
			else if(s->x[N] == N-1)
          b->r[N] = ( 3.0*a->r[N] - 4.0*a->r[N-1] + a->r[N-2] )*f2;
		}
	}

	else if(order == 4){
			// central difference scheme  
		if(s == 0 || sten_type == 0){
				for(i=2; i<N-1; i++)
					b->r[i] = ( a->r[i-2] - 8.0*a->r[i-1] + 8.0*a->r[i+1] 
                    - a->r[i+2] )*f12;
      
	//-------------------------- end points
				// i=0  non-centerd formula (forward)
				b->r[0] = ( - 25.0*a->r[0] + 48.0*a->r[1] - 36.0*a->r[2] 
                    + 16.0*a->r[3] - 3.0*a->r[4] )*f12;
				
				// i=1  non-centerd formula ?
				b->r[1] = ( - 3.0*a->r[0] - 10.0*a->r[1] + 18.0*a->r[2] 
										- 6.0*a->r[3] + a->r[4] )*f12;
				
				// i=N-1 non-centered formula ?  
				b->r[N-1] = ( 3.0*a->r[N] + 10.0*a->r[N-1] - 18.0*a->r[N-2] 
                    + 6.0*a->r[N-3] - a->r[N-4] )*f12;
				
				// i=N  non-centerd formula (backward)
				b->r[N] = ( 25.0*a->r[N] - 48.0*a->r[N-1] + 36.0*a->r[N-2] 
										- 16.0*a->r[N-3] + 3.0*a->r[N-4] )*f12;
		}

		// sten_type != 0, ENO stencil to evaluate derivative
		else{
			for(i=4; i<N-3; i++){     // grid point i
				// central scheme
				b->r[i] = ( a->r[i-2] - 8.0*a->r[i-1] + 8.0*a->r[i+1] 
					      - a->r[i+2] )*f12;
					
					// non-centerd formula with central index i-2  
				if(s->x[i] == i-2)
					b->r[i] = ( 25.0*a->r[i] - 48.0*a->r[i-1] + 36.0*a->r[i-2] 
							  - 16.0*a->r[i-3] + 3.0*a->r[i-4] )*f12;
          
					// 
				else if(s->x[i] == i-1)
					b->r[i] = ( 3.0*a->r[i+1] + 10.0*a->r[i] - 18.0*a->r[i-1] 
							  + 6.0*a->r[i-2] - a->r[i-3] )*f12;
          
					//
				else if(s->x[i] == i+1)
					b->r[i] = ( - 3.0*a->r[i-1] - 10.0*a->r[i] + 18.0*a->r[i+1] 
                                - 6.0*a->r[i+2] + a->r[i+3] )*f12;
          
					// 
				else if(s->x[i] == i+2)
					b->r[i] = ( - 25.0*a->r[i] + 48.0*a->r[i+1] - 36.0*a->r[i+2] 
                                + 16.0*a->r[i+3] - 3.0*a->r[i+4] )*f12;
			}

	//---------------------------------- end points ----------------------------------
		// the derivative is evaluated with a ENO stencil (using a central index)
		// i=0,1,2,3
			for(i=0; i<4; i++){
				if(s->x[i] == i)  
					b->r[i] = ( a->r[i-2] - 8.0*a->r[i-1] + 8.0*a->r[i+1] 
								    - a->r[i+2] )*f12;
     
				else if(s->x[i] == i-2)
					b->r[i] = ( 25.0*a->r[i] - 48.0*a->r[i-1] + 36.0*a->r[i-2] 
                    - 16.0*a->r[i-3] + 3.0*a->r[i-4] )*f12;
     
				else if(s->x[i] == i-1)
					b->r[i] = ( 3.0*a->r[i+1] + 10.0*a->r[i] - 18.0*a->r[i-1] 
                    + 6.0*a->r[i-2] - a->r[i-3] )*f12;
    
				else if(s->x[i] == i+1)
					b->r[i] = ( - 3.0*a->r[i-1] - 10.0*a->r[i] + 18.0*a->r[i+1] 
										  - 6.0*a->r[i+2] + a->r[i+3] )*f12;
				
				else if(s->x[i] == i+2)
					b->r[i] = ( - 25.0*a->r[i] + 48.0*a->r[i+1] - 36.0*a->r[i+2] 
										  + 16.0*a->r[i+3] -3.0*a->r[i+4] )*f12;
			}

		// i=N, N-1, N-2, N-3
			for(i=N-3; i<=N; i++){
				if(s->x[i] == i)
					b->r[i] = ( a->r[i-2] - 8.0*a->r[i-1] + 8.0*a->r[i+1] 
										- a->r[i+2] )*f12;
    
				else if(s->x[i] == i-2)
					b->r[i] = ( 25.0* a->r[i] - 48.0*a->r[i-1] + 36.0*a->r[i-2] 
							      - 16.0*a->r[i-3] + 3.0*a->r[i-4] )*f12;
    
				else if(s->x[i] == i-1)
					b->r[i] = ( 3.0*a->r[i+1] + 10.0*a->r[i] - 18.0*a->r[i-1] 
							      + 6.0*a->r[i-2] - a->r[i-3] )*f12;
    
				else if(s->x[i] == i+1)
					b->r[i] = (- 3.0*a->r[i-1] - 10.0*a->r[i] + 18.0*a->r[i+1] 
							       - 6.0*a->r[i+2] + a->r[i+3] )*f12;
				
				else if(s->x[i] == i+2)
					b->r[i] = ( -25.0 * a->r[i] + 48.0*a->r[i+1] - 36.0*a->r[i+2] 
							       + 16.0*a->r[i+3] - 3.0*a->r[i+4] )*f12;
			}
		}
  } 

//periodic(b);
// b->r[N] = b->r[0];

return b;

}

