
#pragma once

namespace brg 
{
	/** CRTP: Curious Recursive Template Pattern implementation.
	 * This implementation is based on Scott Meyers', using a 
	 * function static and returning a reference. It also uses 
	 * James Coplien's curious recurring template pattern.
	 *
	 * To make a class a singleton, derive it from this template, 
	 * passing the derived class as a template parameter as below
	 *
	 * class MySingleton : public Sfx_Singleton <MySingleton> {}
  */
	template<typename T> class SfxSingleton 
	{
	  public:	  
		  static T& getSingleton();
	  protected:
		  // Default constructor
		  SfxSingleton();

		  /** Force destructor to be virtual (It is a pure virtual function, 
		   * it means it is left to derived class to implement the destructor).
		   * It is not the intent of this class to delete, let it to the subclass. 
			 */
		  virtual ~SfxSingleton() = 0;

	  private:
		  // Copy constructor (make it private ...)
		  SfxSingleton( const SfxSingleton & aOther); 
		  // Assignment operator
		  SfxSingleton & operator = ( const SfxSingleton & aOther);
	};

	//
	// Inline definitions
	//

	// ______________________________________________________________________
	//
	template<typename T> inline SfxSingleton<T>::SfxSingleton()
	{
	}

	// _______________________________________________________________________
	//
	template<typename T> inline SfxSingleton<T>::~SfxSingleton()
	{
	}

	// _______________________________________________________________________
	//
	template< typename T> inline 
		T & SfxSingleton<T>::getSingleton ()
	{
		static T wSingleton;

		return wSingleton;
	}
}  // End of namespace
