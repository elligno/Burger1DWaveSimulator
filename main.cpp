#include "QtGuiBrg1Demo.h"
#include <QtWidgets/QApplication>

// deprecated
float pi = (float)PI;
float t = 0.0, dx;
// vector *x;

//----------------------------------------------------------------------
//										Parameters of run
//----------------------------------------------------------------------

int N = DIM;             // grid size
int Nsu;                 // number of scalars used
int Nts = 5;             // number of time step
float dt = (float)0.01;  // time stepping
float nu = (float)0.05;  // viscosity
int order = 2;           // order of spatial scheme
int scheme_type = 1;     // 1: upwind one-sided, 2: Roe scheme
int sten_type = 0;       // stencil: 0 (centered), 1 (ENO)
int recon_test = 0;      // test reconstruction function
int deriv_test = 0;      // test first derivative function
int stencil_test = 0;    // test ENO stencil
int test0 = 0;           // test subroutines and exit
int readpar = 1;         // readpar = 1 for readings parameters from file

int main( int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtGuiBrg1Demo w;

	// testing************************

	// just testing some qt features
	QCoreApplication* w_appInst = QGuiApplication::instance();
	// sanity check
	if( nullptr != w_appInst)
	{
		const QString& w_appName = w_appInst->applicationName();
	}

	const QString& w_guiAppName = QGuiApplication::applicationName();

	// can i cast on the derived app to retrieve my stuff?
	QtGuiBrg1Demo* w_testcast =
		dynamic_cast<QtGuiBrg1Demo*>(QGuiApplication::instance());

	if( nullptr != w_testcast)
	{
		// not sure what to do yet, just testing
		auto w_schemeType = w_testcast->getschemeType();
	}

	// testing ******************************
	
	w.show();
	return a.exec();
}
