#include <iostream>

// Qt includes
#include <QFile>
#include <QIODevice>
#include <QTextEdit>
#include <QTextStream>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
// App demo include
#include "QtGuiBrg1Demo.h"

QtGuiBrg1Demo::QtGuiBrg1Demo(QWidget *parent)
	: QMainWindow(parent),
// 	  m_runButton(nullptr),
// 	  m_hlayout(nullptr),
// 	  menuBar(nullptr),
// 	  horizontalGroupBox(nullptr),
// 	  gridGroupBox(nullptr),
// 	  formGroupBox(nullptr),
// 	  smallEditor(nullptr),
// 	  bigEditor(nullptr),
	  //  labels(nullptr),
	  Nts(5),       // number of time step
	  m_dt(0.01f),            // time stepping
	  m_nu(0.05f),            // viscosity
				 								// pi((float)PI),    
	  t(0.0),          // 
									  // dx(0.0),         
	  x(nullptr),      //
	  m_order(2),        // order of spatial scheme
	  m_schemeType(1),  // 1: upwind one-sided, 2: Roe scheme
	  m_stenType(0),    // stencil: 0 (centered), 1 (ENO)
	  reconTest(false),   // test reconstruction function
	  derivTest(false),   // test first derivative function
	  stencilTest(false), // test ENO stencil
	  test0(false),        // test subroutines and exit
	  readpar(true)       // readpar = 1 for readings parameters from file
{
	qApp->aboutQt(); // print some info about Qt 
	qApp->setApplicationName( QString("Burger1D Wave Simulator"));
	setWindowTitle( tr("Burger1D Simulator"));
  
	// ready to launch the app
	ui.setupUi(this);

	//
	// Prepare layout of the central window 
	// 

	QTextEdit* bigEditor = new QTextEdit(this);
	bigEditor->setPlainText( tr("This widget takes up all the remaining space "
		"in the top-level layout."));

	// standard button at the bottom of the window
	QDialogButtonBox* buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok
		                              | QDialogButtonBox::Cancel);

	// set button signal and slot when push event occur
	// NOTE: i think i have to re-write the accept and reject method 
	QObject::connect( buttonBox, SIGNAL(accepted()), this, SLOT(accept())); //OK 
	QObject::connect( buttonBox, SIGNAL(rejected()), this, SLOT(reject())); //Cancel

	QPushButton* w_readButton =    new QPushButton(QString("Read Params"),    this);
	QPushButton* w_printButton =   new QPushButton(QString("Print Params"),   this);
	QPushButton* w_runButton =     new QPushButton(QString("Run Simulation"), this);
	QPushButton* w_oneStepButton = new QPushButton(QString("Do One Step"),    this);

	QHBoxLayout* w_buttonRow = new QHBoxLayout(this);
	w_buttonRow->addWidget( w_readButton);
	w_buttonRow->addWidget( w_printButton);
	w_buttonRow->addWidget( w_runButton);
	w_buttonRow->addWidget( w_oneStepButton);

	// connecting
	QObject::connect( w_readButton,    SIGNAL(clicked()), this, SLOT(readButtonClicked()()));
	QObject::connect( w_printButton,   SIGNAL(clicked()), this, SLOT(printRunParamsClicked()));
	QObject::connect( w_runButton,     SIGNAL(clicked()), this, SLOT(runButtonClicked()));
	QObject::connect( w_oneStepButton, SIGNAL(clicked()), this, SLOT(oneStepButtonClicked()));

	QVBoxLayout* w_singleCol = new QVBoxLayout(this);
	w_singleCol->addLayout(w_buttonRow); // row of buttons to control sim
	w_singleCol->addWidget(bigEditor);   // editor to display message
	w_singleCol->addWidget(buttonBox);   // bottom buttons to quit app

	// set central window
	QWidget* w_centralWidget = new QWidget(this);
	w_centralWidget->setLayout(w_singleCol);
	setCentralWidget( w_centralWidget);
}

bool QtGuiBrg1Demo::runButtonClicked()
{
	return false;
}

bool QtGuiBrg1Demo::oneStepButtonClicked()
{
	return false;
}

void QtGuiBrg1Demo::readButtonClicked()
{
	readParams();
}

void QtGuiBrg1Demo::printRunParamsClicked()
{
	printRunParams();
}

// void QtGuiBrg1Demo::setWindowSize()
// {
// }

void QtGuiBrg1Demo::readParams()
{
}

//------------------------------------------------------------
//   				Printing parameters
//------------------------------------------------------------
void QtGuiBrg1Demo::printRunParams()
{
//	extern int N;

	// Design Note QTextEditor instead of console. 

// 	printf("\n\n ------------------------------------------------- \n");
// 	printf("                 DEMO BURGER SOLVER 1D                 \n ");
// 	printf(" ------------------------------------------------------\n\n");
// 	printf(" N = %d \n", N);
// 	printf(" Nts = %d \n", Nts);
// 	printf(" Order = %d \n", order);
// 	printf(" nu = %e \n", nu);
// 	printf(" sten_type = %d \n", sten_type);
// 	printf(" scheme_type = %d \n", scheme_type);
// 	printf(" recon_test = %d \n", recon_test);
// 	printf(" deriv_test = %d \n", deriv_test);
// 	printf(" stencil_test %d \n", stencil_test);
// 	printf(" test0 = %d \n", test0);
// 	printf(" dt = %e \n", dt);
// 	printf("------------------------------------------------------------------------------\n\n");
}

// Initialization
void QtGuiBrg1Demo::Initialization()
{
	extern int N;
	extern float pi, dx;

// 	FILE* f1 = nullptr;
// 	FILE* f2 = nullptr;
// 	errno_t w_err;
// 
// 	if (w_err = fopen_s(&f1, "gridmesh.txt", "w") != 0)
// 	{
// 		std::cout << "Couldn't open gridMesh.txt file\n";
// 	}
// 
// 	if (w_err = fopen_s(&f2, "initialsolution.txt", "w") != 0)
// 	{
// 		std::cout << "Couldn't open initialsolution.txt file\n";
// 	}

	// Use Qt type
	QFile w_meshFile("gridmesh.txt");
	QFile w_initialSln("initialsolution.txt");

	if( !w_meshFile.open(QIODevice::WriteOnly|QIODevice::Text))
	{
		return;
	}

	if (!w_initialSln.open(QIODevice::WriteOnly|QIODevice::Text))
	{
		return;
	}

	// spatial stepping
	dx = (float)(2.0*pi) / (float)N;

	x = new_vector(); // create a vector

	// x coordinates of the grid
	gen_pts(x, dx);

	// writing data to file
	QTextStream w_xcoord(&w_meshFile);
	for( unsigned i = 0; i <= N; i++)
		w_xcoord >> x->r[i];
// 		fprintf(f1, "%8.4f \n", x->r[i]);

	va = initial_cond(x);   // averaged solution on each cell 
	v = reconstr(va);       // reconstruction via primitive variables

	printf("-----------------------------------------------\n");
	printf(" n");
	printf(" Initial Condition \n");
	printf(" ----------------------------------------------\n");

	QTextStream w_init(&w_initialSln);
	for (unsigned i = 0; i < N; i++)
		w_init >> va->r[i];
// 		fprintf(f2, "%8.4f \n", va->r[i]);

	// closing file 
// 	fclose(f1);
// 	fclose(f2);
	w_meshFile.close();
	w_initialSln.close();
}