#pragma once

// C++ include 
#include <memory> // shared ptr
// Qt include
#include <QtWidgets/QMainWindow>
//app includes
#include "ui_QtGuiBrg1Demo.h"
#include "burger.h"

// forward declaration
class BurgerSolver;

/** GUI application to manage simulation of the Burger Wave Equation.
  *
	*  Simulate the propagation of a non-linear wave developing a shock.
  *  solving burger equation using a numerical method.
*/
class QtGuiBrg1Demo : public QMainWindow
{
	Q_OBJECT

public:
	/**
	* A constructor.
	*/
	explicit QtGuiBrg1Demo( QWidget *parent = Q_NULLPTR);

	/**
	* return type of scheme supported by simulation
	*/
	int getschemeType() const { return m_schemeType; }

private:
	Ui::QtGuiBrg1DemoClass ui;

	private slots:
	/**
	* stepButtonClicked
	*  This method handle button when clicked (SLOT).
	*  Run the simulation step-by-step in a debug mode.
	*
	* \return bool
	* \param
	*/
	bool runButtonClicked();    

	/**
	 * save simulation result to file
   */
	bool oneStepButtonClicked();   

	/**
	* read simulation params from file
	*/
	void readButtonClicked();

	/**
	* print simulation params
	*/
	void printRunParamsClicked();

protected:
	/**
	*  GUI win extent
	*/
	//void setWindowSize();

 /**
	* not sure about this one
  */
	//void browse();              

	/**
  *  Read parameters from file (input)
  */
	virtual void readParams();          

	/**
	  *  print params for current sim.
    */
	//virtual void runSimulation(); 
	
	/**
	*  print params for current sim.
	*/
	virtual void printRunParams();
	/**
	 * ... to be completed
	 */
	//virtual void runLoop();
/**
	*grid, cnd. and ...
 */
	void Initialization();     
/**
	* Perform test of numerical operator
  */
	bool Testing();     
/**
  * Calculation of what???
  */
	void Calculation();         
															
	BurgerSolver* m_brgSolver;  /**< ODE solver*/

	//
	// GUI parameters (called parameters of run in the original program)
	// managed by GUI and set by user before simulation start
	//
	int Nts;           /**< number of time step*/
	float m_dt;          /**< time stepping*/
	float m_nu;          /**< viscosity*/
	int m_order;         /**< order of spatial scheme*/
	int m_schemeType;    /**< 1: upwind one-sided, 2: Roe scheme*/
	int m_stenType;      /**< stencil: 0 (centered), 1 (ENO)*/
	bool reconTest;      /**< test reconstruction function*/
	bool derivTest;    /**< test first derivative function*/
	bool stencilTest;  /**< test ENO stencil*/
	bool test0;        /**< test subroutines and exit*/
	char par[16];      /**< reading input file*/
	bool readpar;      /**< ...*/    
	float val;         /**< ...*/
										 /**<	float pi;          ... extern*/
	float t;           /**< ...*/
										 /**<	float dx;          ... extern*/
	vector *x;         /**< shall be implemented with shared_ptr*/
	vector *v, *va;    /**< shall be implemented with shared_ptr*/

	// not sure yet
	std::shared_ptr<vector> m_x; /**< next version*/
	std::shared_ptr<vector> m_v;
	std::shared_ptr<vector> m_va;

	//	unsigned N;        array and vector dimension
	QString m_propfilename;  /**< property/config filename*/
	//std::ofstream m_logFile;  logger of events
	// some utility from boost file system
	// 	bool FindFile( const boost::filesystem::path& directory,                                       
	// 		boost::filesystem::path& path,  
	// 		const std::string& filename );
};