
#ifndef BURG1DQTAPP_H
#define BURG1DQTAPP_H

// C++ includes
#include <memory>
#include <fstream>
// Qt include
#include <QtWidgets/QMainWindow>
// boost include
//#include <boost/filesystem.hpp>      // boost file system utility
// project includes
#include "ui_burg1dqtapp.h"
#include "burger.h"

// Qt forward declaration
QT_BEGIN_INCLUDE_NAMESPACE
//class QMainWindow;
class QComboBox;
class QGroupBox;
class QPushButton;
class QSpinBox;
class QTextEdit;
class QLineEdit;
class QMenuBar;
class QLabel;
class QHBoxLayout;
class QDialogButtonBox;
QT_END_INCLUDE_NAMESPACE

// forward declaration
class BurgerSolver;

/** Burger1D Application (simulate propagation of a non-linear wave).
 *  Solve One-Dimensional Burger equation by using a finite difference
 *  numerical scheme. The physical phenomena associate to it is a 
 *  breaking wave (develop a shock). 
 * 
 *   To be completed
*/
class Burg1DQtApp : public QMainWindow
{
	Q_OBJECT

public:
	Burg1DQtApp(QWidget *parent = 0);
	~Burg1DQtApp();

private:
    Ui::Burg1DQtAppClass ui;

	private slots:
	/**
  	 * stepButtonClicked
	 *  This method handle button when clicked (SLOT). 
	 *  Run the simulation step-by-step in a debug mode.
	 *
	 * \return bool
	 * \param  
	 */
	bool plotButtonClicked();   // plot simulation result
	bool runButtonClicked();    // run the simulation
	bool saveButtonClicked();   // stop simulation
	void accept();
protected:
	void setWindowSize();       // GUI win extent
//	void browse();              // don't know yet
	void readParams();          // read config fr om file
	void printRunParams();      // print params for current sim.
	void Initialization();      // grid, cnd. and ...
	bool Testing();             // test our algorithm
	void Calculation();         // call inside button clicked

private:

	// Qt layout
	QComboBox*   m_comboBox;     // ...
	QTextEdit*   m_textEdit;     // ...
	QSpinBox*    m_spinBox;      // ...
	QPushButton* m_browseButton; // ...
	QPushButton* m_runButton;    // run the simulation
	QPushButton* m_saveButton;   // save data to file
	QLabel*      m_label;        // ...
	QHBoxLayout* m_hlayout;      // 

	// =================================
	void createMenu();
	void createHorizontalGroupBox();
	void createGridGroupBox();
	void createFormGroupBox();
	// separation ...
	enum { NumGridRows = 3, NumButtons = 4 };

	QMenuBar *menuBar;
	// group box lines up horizontally and vertically
	QGroupBox *horizontalGroupBox;
	// divide into rows and columns
	QGroupBox *gridGroupBox;
	// ...
	QGroupBox *formGroupBox;
	// area where you can write text
	QTextEdit        *smallEditor;
	QTextEdit        *bigEditor;
	QLabel           *labels[NumGridRows];
	QLineEdit        *lineEdits[NumGridRows];
	QPushButton      *buttons[NumButtons];
	QDialogButtonBox *buttonBox;
	// ...
	QMenu *fileMenu;
	QAction *exitAction;
	// ================================

  // solving burger equation using a numerical of type finite difference 
  // using the conservative form of the equation.  
	BurgerSolver* m_brgSolver;    // ODE solver

	// GUI parameters (called parameters of run in the original program)
	int Nts;          // number of time step
	float dt;         // time stepping
	float nu;         // viscosity
	int order;        // order of spatial scheme
	int scheme_type;  // 1: upwind one-sided, 2: Roe scheme
	int sten_type;    // stencil: 0 (centered), 1 (ENO)
	bool recon_test;   // test reconstruction function
	bool deriv_test;   // test first derivative function
	bool stencil_test; // test ENO stencil
	bool test0;        // test subroutines and exit
	char par[16];     // reading input file
	bool readpar;      // ...    
	float val;        // ...
//	float pi;          ... extern
	float t;          // ...
//	float dx;          ... extern
	vector *x;        // shall be implemented with shared_ptr
	vector *v, *va;   // shall be implemented with shared_ptr
	std::shared_ptr<vector> m_x; // next version
	std::shared_ptr<vector> m_v;
	std::shared_ptr<vector> m_va;
//	unsigned N;        array and vector dimension
	QString m_propfilename;  // property/config filename
	//std::ofstream m_logFile;  logger of events
	// some utility from boost file system
// 	bool FindFile( const boost::filesystem::path& directory,                                       
// 		boost::filesystem::path& path,  
// 		const std::string& filename );
	
	void plot();
	void plotAndSave();
};
#endif // BURG1DQTAPP_H
