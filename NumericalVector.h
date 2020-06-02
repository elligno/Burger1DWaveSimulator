
#pragma once

namespace brg 
{
	class NumVector
	{
	public:
		explicit NumVector(unsigned aSize = 101);
		//copy/assignment ctor
		NumVector(const NumVector& aOther);
		NumVector& operator=(const NumVector& aOther);
		// move semantic copy/assignment
		NumVector(NumVector&& aOther);
		NumVector& operator= (NumVector&& aOther);
	protected:
	private:
		double* m_data;
	};
}// End of namespace
