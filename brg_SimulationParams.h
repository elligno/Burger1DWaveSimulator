#pragma once

// Qt include
#include <QFile>
// Application include
#include "Brg_Singleton.hpp"

namespace brg 
{
	/** Class that manage simulation parameters globally. Use CRTP design pattern.
	 *  In the original code these parameters where global variables 
	 *  of the simulation. Read from a config file and set in the main
	 *  as as global. Used by many routines of the simulator as extern 
	 *  variables. 
	*/
  class SimulationParams : public SfxSingleton<SimulationParams>
	{
	public:
		// copy/assignment ctor not allowed
		SimulationParams( const SimulationParams&) = delete;
		SimulationParams& operator= ( const SimulationParams&) = delete;
		//setter/getter
		void setTimeStep( const double aTimeStep) { m_timeStep = aTimeStep; }
		void setViscosity( const double aViscosity) { m_viscosity = aViscosity; }
		void setSchemeOrder(const unsigned aSchemeOrder) { m_schemOrder = aSchemeOrder; }
		double getTimeStep() const { return m_timeStep; }
		double getViscosity() const { return m_viscosity; }
		unsigned getSchmeOrder() const { return m_schemOrder; }
		bool testing() const { return m_testing; }

		// read parameters from file (default settings) at app startup
		void readFromFile( const QFile& aFileofParams);

		// friend declaration for access to private ctor
		friend class SfxSingleton<SimulationParams>;

	protected:
		// Client have no choice to use the Singleton static method  
		// instance() which manage the creation of the sole instance.
		SimulationParams(); // logger creation

	private:
		double m_timeStep=0.01; /**< */
		double m_viscosity=0.1; /**< */
		unsigned m_schemOrder=2; /**< central scheme*/
		// some variable used in ref counting
		bool m_testing = false;
	};
} // End of namespace