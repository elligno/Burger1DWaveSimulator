
// C++ include
#include <iostream>
// QT includes
#include <Qtgui>
// Qt includes 
#include <QTextEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QMenuBar>
#include <QDialog>

//#include <QHBoxLayout>
// project include
#include "burg1dqtapp.h"

// good reference for this first prototype
// http://qt-project.org/doc/qt-4.8/layouts-basiclayouts.html
// Voir document sur un tavail pratique (francais) sur la modelisation
// de l'equation de Burger avec des schemas explicites et implicites.
// L'interface ets presente dans le document o l'on fait varie differents 
// parameters pour lancer des simulations.

Burg1DQtApp::Burg1DQtApp(QWidget *parent)
: QMainWindow(parent),
  m_runButton(nullptr),
  m_hlayout(nullptr),
  menuBar(nullptr),
  horizontalGroupBox(nullptr),
  gridGroupBox(nullptr),
  formGroupBox(nullptr),
  smallEditor(nullptr),
  bigEditor(nullptr),
//  labels(nullptr),
  Nts(5),               // number of time step
  dt(0.01f),            // time stepping
  nu(0.05f),            // viscosity
 // pi((float)PI),    
  t(0.0),          // 
 // dx(0.0),         
  x(nullptr),      //
  order(2),        // order of spatial scheme
  scheme_type(1),  // 1: upwind one-sided, 2: Roe scheme
  sten_type(0),    // stencil: 0 (centered), 1 (ENO)
  recon_test(false),   // test reconstruction function
  deriv_test(false),   // test first derivative function
  stencil_test(false), // test ENO stencil
  test0(false),        // test subroutines and exit
  readpar(true)       // readpar = 1 for readings parameters from file
{
	// what we do here? do we set central widget
	ui.setupUi(this);
//	setWindowSize();   // set rect of window
	// first use the createMenu() function to create and 
	// populate a menu bar and the createHorizontalGroupBox() 
	// function to create a group box containing four buttons 
	// with a horizontal layout.
 	createMenu();
// 	createHorizontalGroupBox();
// 	createGridGroupBox();
// 	createFormGroupBox();

	// We don't have to specify a parent for the widgets when we create them
	// The reason is that all the widgets we create here will be added to a layout, 
	// and when we add a widget to a layout, it is automatically re-parented to the 
	// widget the layout is installed on.
	// for example new QTextEdit(this); like this specify that the vreated widget  
	// has a parent which is the main window. 
	bigEditor = new QTextEdit;
	bigEditor->setPlainText( tr( "This widget takes up all the remaining space "
		"in the top-level layout."));

	// standard button ath the bottom of the window
	buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok
             		| QDialogButtonBox::Cancel);

	// set button signal and slot when push event occur
	// NOTE: i think i have to re-write the accept and reject method 
	QObject::connect( buttonBox, SIGNAL( accepted()), this, SLOT(accept()));
	QObject::connect( buttonBox, SIGNAL( rejected()), this, SLOT(reject()));

//	std::unique_ptr<QVBoxLayout> w_mainLayout( new QVBoxLayout);
// 	mainLayout->addWidget(bigEditor);
//     mainLayout->addWidget(buttonBox);
// 
// 	setLayout(mainLayout);
 	setWindowTitle( tr("Burger1D Simulator"));
	
	// set parent to this
//  	m_hlayout =new QHBoxLayout(this);
//  	QRect w_checkGeom=m_hlayout->geometry();
//  	if( m_hlayout->isEmpty() && m_hlayout->isWidgetType())
// 	{
// 		// how to debug msg in qt
// 	}
// 	m_hlayout->setGeometry( QRect(50,100,50,100));

	// Set layout
	QHBoxLayout *hlayout = new QHBoxLayout;
	QPushButton* myWidget1 = new QPushButton("&run1", this);
	QPushButton* myWidget2 = new QPushButton("&run2", this);
	hlayout->addWidget(myWidget1);
	hlayout->addWidget(myWidget2);
	hlayout->addWidget(bigEditor);

	QVBoxLayout* vlayout = new QVBoxLayout;
	vlayout->addWidget(buttonBox);  //"ok" and "Cancel" button
	
	QGridLayout* w_testGridLayout=new QGridLayout;
	w_testGridLayout->addLayout(hlayout,0,0);
	w_testGridLayout->addLayout(vlayout,1,0);

	// Set layout in QWidget
	QWidget *window = new QWidget();
//	window->setGeometry(0,0,100,100);
	window->setLayout(w_testGridLayout);

	// Set QWidget as the central layout of the main window
	setCentralWidget(window); // ...
	setMenuBar(menuBar);      // top of the window

// 	setCentralWidget(this);
// 
// 	// position button in the rect. win.
// 	// retrieve some geometry of the main widget (window)
// 	QRect w_widthSize=geometry();                 // inherited from QWidget class (Widget size)
// 	QPoint w_leftBottom=w_widthSize.bottomLeft(); // retrieve the bottom left corner
// 	QPoint w_rightUpper=w_widthSize.topRight();   // retrieve the bottom right corner
// 	QPoint w_winWidth=w_leftBottom-w_rightUpper;  // compute width of the window
// 
// 	// set location and size of the button (just testing some numbers)
// 	m_runButton->setGeometry( QRect(w_leftBottom.x()+200, w_leftBottom.y()-150, 40/*width*/,20/*height*/));
// 
// 	// set the run button when clicked to call or start simulation
// 	QObject::connect( m_runButton,SIGNAL(clicked()),this,SLOT(runButtonClicked()));
// 
// 	// let's open the log file
// 	if( !m_logFile.is_open())
// 	{
// 		// just open it
// 		m_logFile.open(std::string("WorkingDir"));
// 	}
}

Burg1DQtApp::~Burg1DQtApp()
{
    std::cout << "We are leaving the Qt application\n";

	// free some vector allocated
// 	free_vector(x); 
// 	free_vector(va);
	// and close files ???
}

bool Burg1DQtApp::plotButtonClicked()
{
	return false; // no implemented yet
}

// big bone of our simulation, perform time stepping
// according to initial solution and algorithm 
bool Burg1DQtApp::runButtonClicked()
{
	Calculation(); //run simulation

	return false; 
}

bool Burg1DQtApp::saveButtonClicked()
{
    return false; // no implemented yet
}

void Burg1DQtApp::accept()
{
	//QDialog::accept();
}

//-------------------------------------------------------------
//						Readings parameters
//-------------------------------------------------------------
void Burg1DQtApp::readParams()
{
	FILE* FichierInit=nullptr;
	char NomFichierInit[256];
	// file name selected from GUI 
	// (list box where i select the input file)
	std::string m_inputFileName="jbtest.dat";
	::strcpy_s(NomFichierInit, 256, m_inputFileName.c_str());

	errno_t w_err;
	// Open for write 
	w_err = fopen_s( &FichierInit, NomFichierInit, "r" );

	if( w_err == 0 )
	{
		printf( "The file 'jbtest.dat' was opened\n" );
	}
	else
	{
		printf( "The file 'jbtest.dat' was not opened\n" );
	}

	// original version
// 	if((in=fopen("jbtest.dat", "r")) == NULL){
// 		printf("Cannot open file.\n");
// 		printf(" \n");
// 		return 0;
// 	}

	// read parameters of run from file
	// NOTE: use boost numeric cast instead of C-casting style
	// make sure that cast succeed
	while(readpar) {
		fscanf( FichierInit,"%s%f",par,&val);
		if( id(par,"dt"))dt=val;
		if( id(par,"nu"))nu=val;
		if( id(par,"sten_type"))sten_type=(int)val;
		if( id(par,"scheme_type"))scheme_type=(int)val;
		if( id(par,"recon_test"))recon_test=(int)val;
		if( id(par,"deriv_test"))deriv_test=(int)val;
		if( id(par,"stencil_test"))stencil_test=(int)val;
		if( id(par,"test0"))test0=(int)val;
		if( id(par,"Nts"))Nts=(int)val;
		if( id(par,"order"))order=(int)val;
		if( id(par,"end"))readpar=(int)val; 
	}  

	// finish might as well close it!
	fclose(FichierInit);
}

void Burg1DQtApp::setWindowSize()
{
	// before resizing check the default size
	QRect w_checkSize=geometry();                 // inherited from QWidget class (Widget size)
	QPoint w_leftBottom=w_checkSize.bottomLeft(); // retrieve the bottom left corner
	QPoint w_rightUpper=w_checkSize.topRight();   // retrieve the bottom right corner
	QPoint w_winWidth=w_leftBottom-w_rightUpper;  // compute width of the window
	if (w_winWidth.isNull())                      // more testing than doing something smart
	{                                             // actually we check if the window is sized
		// we have a problem with the window size
		// resize it to a default size for now
		resize(600,400); // whatever it is
	}
	else // not null but not quite
	{
		// print size of the window or do something else
		// want to leave it with the current size?
		if (w_winWidth.y()<900 && w_winWidth.x()<600)
		{
			resize(w_winWidth.x(),900); // i am not sure about this one 
		}
	}
}

//------------------------------------------------------------
//   				Printing parameters
//------------------------------------------------------------
void Burg1DQtApp::printRunParams()
{
	extern int N;

	printf("\n\n ------------------------------------------------- \n");
	printf("                 DEMO BURGER SOLVER 1D                 \n ");
	printf(" ------------------------------------------------------\n\n");
	printf(" N = %d \n", N);
	printf(" Nts = %d \n", Nts);
	printf(" Order = %d \n", order);
	printf(" nu = %e \n", nu);
	printf(" sten_type = %d \n", sten_type);
	printf(" scheme_type = %d \n", scheme_type);
	printf(" recon_test = %d \n", recon_test);
	printf(" deriv_test = %d \n", deriv_test);
	printf(" stencil_test %d \n", stencil_test);
	printf(" test0 = %d \n", test0);
	printf(" dt = %e \n", dt);
	printf("------------------------------------------------------------------------------\n\n");
}

// Initialization
void Burg1DQtApp::Initialization()
{
	extern int N;
	extern float pi,dx;

	FILE* f1=nullptr;
	FILE* f2=nullptr;
	errno_t w_err;

	if (w_err=fopen_s(&f1, "gridmesh.txt", "w")!=0)
	{
		std::cout << "Couldn't open gridMesh.txt file\n";
	}
	if (w_err=fopen_s(&f2,"initialsolution.txt", "w")!=0)
	{
		std::cout << "Couldn't open initialsolution.txt file\n";
	}

	// spatial stepping
	dx = (float)(2.0*pi)/(float)N;        

	x = new_vector(); // create a vector
	
	// x coordinates of the grid
	gen_pts(x,dx);        
	for(unsigned i=0;i<=N;i++)
		fprintf(f1,"%8.4f \n", x->r[i] );

	va = initial_cond(x);   // averaged solution on each cell 
	v = reconstr(va);       // reconstruction via primitive variables

	printf("-----------------------------------------------\n");
	printf(" n");
	printf(" Initial Condition \n");
	printf(" ----------------------------------------------\n");
	
	for(unsigned i=0;i<N;i++)
		fprintf(f2,"%8.4f \n", va->r[i]);

	// closing file 
	fclose(f1);
	fclose(f2);

}

bool Burg1DQtApp::Testing()
{
	extern int N;

	// test functions used in the program
	if( recon_test==true) test_reconstr(x,N);
	if( deriv_test==true) test_derivative();
	if( stencil_test==true) test_stencil(N);
	if( test0==true) return true;

	return false;
}

void Burg1DQtApp::Calculation()
{
// 	extern int N;
// 
// 	FILE* f3=nullptr;

	printf("---------------------------------------------------\n");
	printf("  \n");
	printf(" Calculation \n");
	printf("---------------------------------------------------\n");

// 	errno_t w_err;
// 	if( w_err=fopen_s(&f3,"finalsolution.txt", "w")!=0)
// 	{
// 		std::cout << "Couldn't open gridMesh.txt file\n";
// 	}

	// time stepping and printing data
	// for the time integration, a Runge-Kutta of second order is used
// 	for( int n=1; n<=Nts;n++) {
// 		va = time_step(va,dt);
// 		t=t+dt;
// 		if(n == Nts) {
// 			printf(" \n");
// 			printf("------------------------------------\n");
// 			printf(" t = %f \n", t);
// 			printf("------------------------------------\n");
// 			for( unsigned i=0;i<N;i++)
// 				fprintf(f3,"%8.4f \n", va->r[i]);
// 		}
// 	}

	// closing file,simulation complete
//	fclose(f3);
}

void Burg1DQtApp::plot()
{
	//using namespace boost::filesystem;

	// retrieve env. variable
	std::string w_projDirRoot(::getenv("TESTVS12_ROOT"));
	w_projDirRoot+="\\Burg1DQtApp"; // set searching dir

	// ...
// 	path w_SearchDir( w_projDirRoot); 
	// Recursively search for a file within a directory and its subdirectories
	// contains all x coordinate
// 	if( FindFile( w_projDirRoot, w_DirContainingFile, std::string("gridmesh.txt")))  
// 	{  
// 		std::cout << "File location:" << std::endl;  
// 		std::cout << w_DirContainingFile.string();  
// 	}
// 	else
// 	{
// 		// logging file
// 	}

	// read value from file for plotting and store it in a vector
	// call QtCustom plot 
}

// create a File menu on the top window (menu bar)
void Burg1DQtApp::createMenu()
{
	menuBar = new QMenuBar;

	fileMenu = new QMenu(tr("&File"), this);
	exitAction = fileMenu->addAction(tr("E&xit"));
	menuBar->addMenu(fileMenu);

	connect( exitAction, SIGNAL(triggered()), this, SLOT(accept())); 
}

void Burg1DQtApp::createHorizontalGroupBox()
{
	// not implemented yet
}

void Burg1DQtApp::createGridGroupBox()
{
	// not implemented yet
}

void Burg1DQtApp::createFormGroupBox()
{
	// not implemented yet
}

//////////////////////////////////////////////////////////////////////////
// Recursively find the location of a file on a given directory   
// make use of some of the c++11 features : auto keyword and lambda function
// 
// bool Burg1DQtApp::FindFile( const boost::filesystem::path& directory,                                       
// 			  boost::filesystem::path& path,  
// 			  const std::string& filename )  
// {
// 	bool found = false;  

//	const boost::filesystem::path file = filename;  
// 	const boost::filesystem::recursive_directory_iterator end;  
// 	const boost::filesystem::recursive_directory_iterator dir_iter( directory);  

	// use auto and lambda function &file input argument of the lambda
// 	const auto it = std::find_if( dir_iter, end,  
// 		[&file]( const boost::filesystem::directory_entry& e )   
// 	{  
// 		return e.path().filename() == file;  
// 	});      
// 
// 	if ( it != end )          
// 	{  
// 		path = it->path();  
// 		found = true;  
// 	}  

// 	return found;  
// }  
