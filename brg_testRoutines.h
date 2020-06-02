#pragma once

#include "brg_UtilityFct.h"

namespace brg 
{
	/** Some test routines for numerical algorithm to test.
	 *
	*/
	class testRoutines 
	{
		// Design Note: 
		//  These tests should be in a separate unit test project (Google Unit test)
	public:
		// test of reconstruction routine ()
		static void test_reconstr( vector* xcoord, int N);

		// test of first derivative calculation
		static void test_derivative();

		//  test computation of stencil (ENO: Essentially Non-Oscillatory)    
		static void test_stencil(int N);
	};
} // end of namespace
