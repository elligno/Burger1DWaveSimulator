// C++ includes
#include<cstdio>
#include <cmath>
// app includes
#include "brg_testRoutines.h"

using namespace brg;

void brg::testRoutines::test_reconstr(vector *xcoord, int N)
{
	int i;
	extern float pi;
	vector *v_reconstruct, *v_averaged, *v_exact;

	FILE *out;

	out = fopen("testreconstr", "w");

	v_exact = new_vector();
	v_averaged = new_vector();

	for (i = 0; i < N; i++) {
		v_exact->r[i] = -sin(pi*xcoord->r[i]);
		v_averaged->r[i] = (cos(pi*xcoord->r[i + 1]) - cos(pi*xcoord->r[i])) /
			(pi*(xcoord->r[i + 1] - xcoord->r[i]));
	}
	// end point
	v_exact->r[N] = 0.0;

	// reconstruction of the function sinus from its mean value
	v_reconstruct = reconstr(v_averaged);

	// print output in a file for comparison
	fprintf(out, "  v_exact    v_reconstruct \n");

	for (i = 0; i <= N; i++)
		fprintf(out, " %8.4f      %8.4f \n", v_exact->r[i], v_reconstruct->r[i]);

	fclose(out);

	free_vector(v_exact);
	free_vector(v_averaged);
	free_vector(v_reconstruct);

	return;
}

void brg::testRoutines::test_derivative()
{
	int i;
	extern int N;
	extern float pi, dx;
	float x;
	vector *v_exact, *vx_calculate, *vx_exact;
	vector *fct, *f_x_ex, *f_x;
	stencil *s, *s1;

	FILE *out;

	out = fopen("testderivativ", "w");

	v_exact = new_vector();
	vx_exact = new_vector();

	for (i = 0; i <= N; i++) {
		x = dx*(float)i;
		v_exact->r[i] = sin(pi*x);
		vx_exact->r[i] = pi*cos(pi*x);
	}

	// compute the first derivative
	s1 = 0;
	vx_calculate = d1x( v_exact, s1);

	// print output in a file for comparison
	fprintf(out, "  vx_exact    vx_calculate \n");

	for (i = 0; i <= N; i++)
		fprintf(out, " %8.4f      %8.4f \n", vx_exact->r[i], vx_calculate->r[i]);

	free_vector(v_exact);
	free_vector(vx_exact);
	free_vector(vx_calculate);

	fct = new_vector();
	f_x_ex = new_vector();

	fprintf(out, "\n test x derivative : \n");

	// compute the function
	for (i = 0; i <= N; i++) {
		x = dx*(float)i;
		if (i <= N / 4) fct->r[i] = sin(x);
		if (N / 4 < i && i <= N / 2) fct->r[i] = cos(x);
		if (N / 2 < i && i <= N) fct->r[i] = sin(x);

		// exact derivative of the function
		if (i <= N / 4) f_x_ex->r[i] = cos(x);
		if (N / 4 < i && i <= N / 2) f_x_ex->r[i] = -sin(x);
		if (N / 2 < i && i <= N)   f_x_ex->r[i] = cos(x);
	}

	// compute the ENO stencil
	s = eno_stencil(fct);

	fprintf(out, "\n  i      function   stencil   \n");
	for (i = 0; i <= N; i++)
		fprintf(out, " %d   %E    %d  \n", i, fct->r[i], s->x[i]);

	// compute the first derivative according to the ENO stencil
	f_x = d1x(fct, s);
	fprintf(out, "\n i      f_x                f_x_ex          ecart    \n");

	for (i = 0; i <= N; i++)
		fprintf(out, " %d    %E    %E      %E  \n",
			i, f_x->r[i], f_x_ex->r[i], fabs(f_x->r[i] - f_x_ex->r[i]));

	//		free_vector(f_x);  free(s);

	/*		printf("\n test derivatives with exponential function:  \n\n");
	printf("\n test x derivative :   \n");

	// compute exponential function
	c = new_vector();
	for(i=0; i<=N; i++){
	x = dx*(float)(i - N/2);          // [-N/2,N/2]
	r2 = x*x;                         // modulus

	fct->r[i] = exp(-r2);             // Gaussian function
	f_x_ex->r[i] = -2.0*x*exp(-r2);   // derivative of the function

	c->r[i] = 0.0;          //
	if(r2 < 0.25)
	c->r[i] = 1.0;
	}

	// compute ENO stencil
	s = eno_stencil(c);

	// print result in file
	fprintf(out,"\n i        f         stencil    \n");
	for(i=0; i<=N; i++)
	fprintf(out," %d %E      %d \n", i, fct->r[i], s->x[i]);

	// compute the derivative according to ENO stencil
	f_x = new_d1x(fct,s);

	fprintf(out,"\n i           fx           fx ex  \n");
	for(i=0; i<=N; i++)
	fprintf(out," %d  %E     %E \n", i, f_x->r[i], f_x_ex->r[i]);

	*/
	free_vector(f_x);
	free(s);
	free_vector(fct);
	free_vector(f_x_ex);
	//		free_vector(c);  

//	}

	return;
}

void brg::testRoutines::test_stencil(int N)
{
	int i;
	vector *a;
	stencil *s;

	FILE *out;

	out = fopen("teststencil", "w");
	s = (stencil *)malloc(sizeof(stencil));

	a = new_vector();

	// compute step function for testing stencil 
	for (i = 0; i <= N; i++)
		a->r[i] = (N / 4 < i && i < 3 * N / 4) ? 1.0 : 0.0;

	// compute ENO stencil
	s = eno_stencil(a);

	fprintf(out, " \n Test stencil : \n");
	fprintf(out, " i     function    stencil \n");

	for (i = 0; i <= N; i++)
		fprintf(out, " %d     %f      %d  \n",
			i, a->r[i], s->x[i]);

	free(s);

	// 
	a->r[3 * N / 4] = 0.8;
	s = eno_stencil(a);

	fprintf(out, " \n  Test stencil : \n");
	fprintf(out, " i     function    stencil \n");

	for (i = 0; i <= N; i++)
		fprintf(out, " %d     %f     %d  \n",
			i, a->r[i], s->x[i]);

	fclose(out);
	free(s);

	return;
}
