
#pragma once

#include "burger.h"

namespace brg 
{
	class Burger1DEquations 
	{
	public:
		Burger1DEquations(double aNu):m_nu(aNu) {}
		// no copy or assignment allowed 
		Burger1DEquations(const Burger1DEquations&) = delete;
		Burger1DEquations& operator= (const Burger1DEquations&) = delete;

		// computes fluxes on grid points  
		vector* flux( vector* v)
		{
			int i;
			extern float nu;
			extern int N;

			vector *f, *vx;
			stencil *s1;

			s1 = nullptr;   // central stencil

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
	private:
		double m_nu;
	};
} // end of namespace
