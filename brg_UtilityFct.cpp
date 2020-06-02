
// C++ includes
#include  <cmath>
#include  <cstdio>
#include  <cstdlib>
// app includes
#include "brg_UtilityFct.h"
#include "brg_SimulationParams.h"

using namespace brg;

// some initialization of static 
unsigned UtilityFct::Nst = 0; // not sure yet
unsigned UtilityFct::Nv = 0; 
unsigned UtilityFct::Ns = 0;

int brg::UtilityFct::id(char a[], char b[])
{
		int i = 0, m = 1;
		while (a[i] != '\0' || b[i] != '\0') { if (a[i] != b[i]) { m = 0; break; } ++i; }

		return m;
}

vector * brg::UtilityFct::new_vector(void)
{
	extern int Nsu;
	vector *a;

	a = (vector *)malloc(sizeof(vector));
	Nv += 1; Nst += 1; if (Nst > Nsu) Nsu = Nst;

	// retrieve sole instance of simulation parameters
	const auto& w_simPrms = SimulationParams::getSingleton();

	//if( testing) printf("  new_vector:  %d %d \n", Ns, Nv, Nst);
	if( w_simPrms.testing()) printf("  new_vector:  %d %d \n", Ns, Nv, Nst);

	return a;
}

void brg::UtilityFct::free_vector(vector *a)
{
	free(a);
	Nv -= 1; Nst -= 1;

// retrieve sole instance of simulation parameters
	const auto& w_simPrms = SimulationParams::getSingleton();

	//if( testing) printf("  free_vector:  %d %d \n", Ns, Nv, Nst);
	if( w_simPrms.testing()) printf("  free_vector:  %d %d \n", Ns, Nv, Nst);
}

void brg::UtilityFct::gen_pts(vector* x, float dx)
{
	extern int N;
	int i;

	for (i = 0; i <= N; i++)
		x->r[i] = (float)i*dx;
}