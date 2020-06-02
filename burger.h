//
//	AUTHOR: Jean Bélanger 
//	DATE: 29 July 2000
//

// STL includes
#include <array>
#include <vector>
#include <valarray>
#include <algorithm>
#include <functional>

// can replace those bu constexpr
#define DIM 100
#define PI 3.1415926535

// type
// typedef struct { double r[DIM+1]; } scalar;
// typedef struct { scalar x; } vector;

//typedef struct { double r[DIM+1]; } vector;
//typedef struct { short x[DIM+1]; } stencil;

using stdvector = std::vector<double>;
using stdvalarray = std::valarray<double>;

// some useful types
/** Data structure used in programming enviromnent.
*/
struct vector {
	//double  r[DIM + 1];
	// instead of using that C-style array, 
	// use the C++11 array which is equivalent of the C-style
	// you all access as c-style array plus some ... new features
	std::array<double, DIM + 1> r{}; // not sure about that ctor initializer list
 
	auto N() const { return r.size(); }

	//template<typename Iter>

	vector& operator+= (const vector& aOther)
	{
		// use std transform (modify object)
		std::transform( r.cbegin(), r.cend(), // source range
			aOther.r.cbegin(), r.begin(),  // begin of second vec and target
			std::plus<double>()); // operation on element

		return *this;
	}

	vector& operator-= (const vector& aOther)
	{
		// use std transform (modify object)
		std::transform(asStdVector().cbegin(), asStdVector().cend(),  // range algorithm
			aOther.asStdVector().cbegin(),
			asStdVector().begin(), std::minus<double>());

		return *this;
	}

	vector& operator*= ( const double aDbl) 
	{
		// use std transform (modify object)
		std::transform(asStdVector().cbegin(), asStdVector().cend(),
			asStdVector().begin(), 
			std::bind( std::multiplies<double>(), std::placeholders::_1, aDbl));

		return *this;
	}

	stdvector asStdVector() const
	{ 
		return stdvector(&r[0],&r[0]+DIM); 
	}
	
	stdvalarray asStdValarray() { return stdvalarray(); }
};

using  stencil = struct { short x[DIM + 1]; };

// functions
int id(char a[], char b[]);
void gen_pts(vector*, float );
vector *initial_cond(vector*);
vector *new_vector(void);
void free_vector(vector*);
vector *reconstr(vector *);
vector *flux(vector *);
vector *time_step(vector *, float );
void test_reconstr(vector *, int);
void test_derivative(void);
void test_stencil(int );
stencil *eno_stencil(vector *);
vector *d1x(vector *, stencil *);
float minmod (float, float);
vector operator+ (const vector& aV1, const vector& aV2);
vector operator- (const vector& aV1, const vector& aV2);
vector operator* (const vector& aV, const double aScalar);