
#ifndef burgersolver_H
#define burgersolver_H

// C++ include
#include <memory>
// #include "Handle.h"
// #include "TimePrm.h"
//#include "FieldLattice.h"

class BurgerSolver 
{
public:
	// ctor
	BurgerSolver()  {}
	~BurgerSolver() {}
	// some methods to solve Burger equations (no implementation provided yet)
	void setIC()        {}
	void solveProblem() {}
	void plotCurve()    {}
	void dumpSolution() {}
protected:
private:
// |	Handle<GridLattice>  m_grid; // the lattice grid
// 	Handle<FieldLattice> m_U;    // solution U at time level n+1
// 	Handle<FieldLattice> m_Up;   // solution U at time level n+1/2
// 	Handle<TimePrm>      m_tip;  // 
};

#endif // burgersolver_H