#pragma once

#include "burger.h"

namespace brg 
{
	/** Finite difference model for the Burger equation based on conservative form.
	*/
	class FiniteDifferenceModel 
	{
	public:

		// time step in time 
		void advance( const float from, const float to, const float dt) {/*Not implememnted yet*/}

		// computes fluxes on grid points  
		vector* flux( const vector* v)
		{
			int i;
			extern float nu;
			extern int N;

			vector *f, *vx;
			stencil *s1=nullptr; // central stencil

			//s1 = 0;    central stencil

			f = new_vector();

			// compute the first derivative
			// vx = d1x(v);
			vx = d1x(v, s1);

			// compute physical flux 
			for (i = 0; i <= N; i++)
				f->r[i] = 0.5*v->r[i] * v->r[i] - nu*vx->r[i];

			free_vector(vx);

			return f;
		}

		// return solution
		double* getSolution() { return nullptr; } // debugging purpose
	private:
		double* m_sln;

		// Time stepper
	};
} // End of namespace
