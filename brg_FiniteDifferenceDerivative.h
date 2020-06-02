#pragma once

// app include
#include "brg_UtilityFct.h"

namespace brg 
{
	class FiniteDifferencederivative 
	{
		// fourth order finite-difference derivative
		// User responsible to delete memory when finish
		static vector* d1x_4th( const vector* v)
		{
			extern int N; // number of grid node
			extern float dx;
			int i;
			float fd = static_cast<float>(1.0)/(12.0*dx);
			vector *vx = nullptr;

			vx = new_vector();
			for( i = 2; i < N - 1; i++)
				vx->r[i] = (v->r[i - 2] - 8.0*v->r[i - 1] + 8.0*v->r[i + 1] - v->r[i + 2])*fd;

			// ----------------------------- end points ----------------------------

			// i=0
			vx->r[0] = (-25.0*v->r[0] + 48.0*v->r[1] - 36.0*v->r[2] + 16 * v->r[3] - 3.0*v->r[4])*fd;
			// i=1
			vx->r[1] = (-3.0*v->r[0] - 10.0*v->r[1] + 18.0*v->r[2] - 6.0*v->r[3] + v->r[4])*fd;
			// i=N
			vx->r[N] = (25.0*v->r[N] - 48.0*v->r[N - 1] + 36.0*v->r[N - 2] - 16 * v->r[N - 3] + 3.0*v->r[N - 4])*fd;
			// i=N-1
			vx->r[N - 1] = (3.0*v->r[N] + 10.0*v->r[N - 1] - 18.0*v->r[N - 2] + 6.0*v->r[N - 3] - v->r[N - 4])*fd;

			return vx;
		}

		// finite - difference derivative using stencil (non-oscillatory)
		// support second and fourth order derivative
		static vector* d1x( vector* a, stencil* s);
	};
} // End of namespace
