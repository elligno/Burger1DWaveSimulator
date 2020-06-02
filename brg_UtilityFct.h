#pragma once

// app include
#include "burger.h"

namespace brg 
{
	/** Static class that provides services for simulation.
	 *
	*/
	class UtilityFct 
	{
	public:
		// comparison of two strings
		static int id(char a[], char b[]);

		// select one vector field, if any available
		static vector* new_vector();

		// free one vector work field 
		static void free_vector(vector* aVec);

		// generate x-coord of the one-dimensional grid
		void gen_pts(vector* x, float dx);

		// reference counting (to be completed)
		static unsigned Nst;
		static int Nsu;
		static unsigned Ns;
		static unsigned Nv;
	};
} // End of namespace
