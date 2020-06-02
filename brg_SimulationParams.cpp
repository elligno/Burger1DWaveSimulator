#include "brg_SimulationParams.h"

void brg::SimulationParams::readFromFile( const QFile & aFileofParams)
{
	//     // original version
	// 	if((in=fopen("jbtest.dat", "r")) == NULL){
	// 		printf("Cannot open file.\n");
	// 		printf(" \n");
	// 		return 0;
	// 	}
	// 
	// 	// read parameters of run from file
	// 	while(readpar){
	// 		fscanf(in,"%s%f",par,&val);
	// 		if(id(par,"dt"))dt=val;
	// 		if(id(par,"nu"))nu=val;
	// 		if(id(par,"sten_type"))sten_type=(int)val;
	// 		if(id(par,"scheme_type"))scheme_type=(int)val;
	// 		if(id(par,"recon_test"))recon_test=(int)val;
	// 		if(id(par,"deriv_test"))deriv_test=(int)val;
	// 		if(id(par,"stencil_test"))stencil_test=(int)val;
	// 		if(id(par,"test0"))test0=(int)val;
	// 		if(id(par,"Nts"))Nts=(int)val;
	// 		if(id(par,"order"))order=(int)val;
	// 		if(id(par,"end"))readpar=(int)val; 
	// 	}  
	// 	
	// 	fclose(in);
}

// not completed
brg::SimulationParams::SimulationParams()
{
	// need to initialize some variables
}
