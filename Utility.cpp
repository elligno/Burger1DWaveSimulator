//
//**********************************************************************************************
//
//										UTILITY.CPP
//
//**********************************************************************************************

// #include "stdafx.h" from original version
#include "burger.h"
#include  <cmath>
#include  <cstdlib>
#include  <cstdio>

// August 22nd, 2014
// NOTE: these variables are a reference count like mechanism 
//        number of scalar used and number of vector used
//        each time a vector is created with the utility 
//        new_vector() thge count is incremented. 
//        I just can't figure out Nst stand for?
//        Number Scalar Total
int Ns=0, Nv=0, Nst=0;
// testing variable for debugging (display numnerical values of the Ns and Nv)
int testing=0; // set to false, 1: true

/*
================================================================================================
											id
================================================================================================
*/

//	AUTHOR: Jean Bélanger 
//	VERSION: August 2000

// comparison of two strings

int id(char a[], char b[])
{
	int i=0,m=1;
	while(a[i] != '\0' || b[i] != '\0') { if(a[i] != b[i]) { m=0; break;} ++i; }

	return m;
}


/*
===============================================================================================
											gen_pts
===============================================================================================
*/

//	AUTHOR: Jean Bélanger 
//	VERSION: August 2000

void gen_pts(vector *x, float dx)
{
	extern int N;
	int i;

	for(i=0; i<=N; i++)
		x->r[i] = (float)i*dx;

}


/*
===============================================================================================
											new_vector
===============================================================================================
*/

//	AUTHOR: Jean Bélanger 
//	VERSION: August 2000

// select one vector field, if any available

vector *new_vector(void)
{
	extern int Nsu;
	vector *a;

	a=(vector *) malloc(sizeof(vector));
	Nv += 1; Nst += 1; if(Nst > Nsu) Nsu = Nst;

	if(testing) printf("  new_vector:  %d %d \n", Ns,Nv,Nst);

	return a;
}


/*
===============================================================================================
											free_vector
===============================================================================================
*/

//	AUTHOR: Jean Bélanger 
//	VERSION: August 2000

// free one vector work field 

void free_vector(vector *a)

{
	free(a);
	Nv -= 1; Nst -= 1;

	if(testing) printf("  free_vector:  %d %d \n", Ns, Nv, Nst);
}


/*================================================================================================
											minmod
==================================================================================================
*/

//	AUTHOR: Jean Bélanger 
//	VERSION: August 2000


// limiter function 


float minmod (float a, float b)
{
	float Resultat;


	if (a*b <= 0.0)
		Resultat = 0.0;
	
	else if ((fabs (a) < fabs (b)) && (a*b > 0.0))
		Resultat = a;
	
	else if ((fabs (b) < fabs (a)) && (a*b > 0.0))
		Resultat = b;

	else if (fabs (b) == fabs (a))		     //	ATTENTION, mis pour vérification !!!!
		Resultat = a;


	else
	{
		printf ("Fonction MINMOD: situation ne correspondant pas a celle attendue\n");
		exit (101);
	}

	return (Resultat);
}

//
// Some vector operations that are quite usefull 
//

vector operator+ (const vector& aV1, const vector& aV2)
{
	stdvector w_res; w_res.reserve(aV1.N());
	std::transform(aV1.asStdVector().cbegin(), aV1.asStdVector().cend(),
		aV2.asStdVector().cbegin(), std::back_inserter(w_res), std::plus<double>());

	vector w_retVec;
	std::copy(w_res.begin(), w_res.end(), &w_retVec.r[0]);

	return w_retVec;
}

vector operator- (const vector& aV1, const vector& aV2)
{
	return vector();
}

vector operator* (const vector& aV, const double aScalar)
{
	return vector();
}





