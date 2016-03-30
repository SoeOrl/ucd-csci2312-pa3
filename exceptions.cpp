#include "Exceptions.h"

//OutofBounds
namespace Clustering {
	Clustering::OutOfBoundsEx::OutOfBoundsEx(unsigned int c, int r)
	{
		__current = c;
		__rhs = r;
		__name = "OutOfBoundsEx";
	}

	unsigned int Clustering::OutOfBoundsEx::getCurrent() const
	{
		return __current;
	}

	int Clustering::OutOfBoundsEx::getRhs() const
	{
		return __rhs;
	}

	std::string Clustering::OutOfBoundsEx::getName() const
	{
		return  __name;
	}

	std::ostream & operator<<(std::ostream & os, const OutOfBoundsEx & ex)
	{
		os << ex.__name;
		return os;
	}


	//Dimensionality


	Clustering::DimensionalityMismatchEx::DimensionalityMismatchEx(unsigned int c, unsigned int r)
	{
		__current = c;
		__rhs = r;
		__name = "DimensionalityMismatchEx";
	}

	unsigned int Clustering::DimensionalityMismatchEx::getCurrent() const
	{
		return __current;
	}

	unsigned int Clustering::DimensionalityMismatchEx::getRhs() const
	{
		return __rhs;
	}

	std::string Clustering::DimensionalityMismatchEx::getName() const
	{
		return std::string(__name);
	}

	std::ostream & operator<<(std::ostream & os, const DimensionalityMismatchEx & ex)
	{
		os << ex.getName();
		return os;
	}


	//ZeroClusterEx

	Clustering::ZeroClustersEx::ZeroClustersEx()
	{
		__name = "ZeroClustersEx";
	}

	std::string Clustering::ZeroClustersEx::getName() const
	{
		return std::string(__name);
	}

	std::ostream & operator<<(std::ostream & os, const ZeroClustersEx & ex)
	{
		os << ex.getName();		
		return os;
	}



	//DataFileOpenEx

	Clustering::DataFileOpenEx::DataFileOpenEx(std::string filename)
	{
		__filename = filename;
		__name = "DataFileOpenEx";
	}

	std::string Clustering::DataFileOpenEx::getFilename() const
	{
		return std::string(__filename);
	}

	std::string Clustering::DataFileOpenEx::getName() const
	{
		return std::string(__name);
	}

	std::ostream & operator<<(std::ostream & os, const DataFileOpenEx & ex)
	{
		os << ex.getName() << ": " << ex.getFilename();
		return os;
	}



	//ZeroDimensionEx

	Clustering::ZeroDimensionsEx::ZeroDimensionsEx() :__name("ZeroDimensionsEx")
	{
		
	}

	std::string Clustering::ZeroDimensionsEx::getName() const 
	{
		return __name;
	}

	std::ostream & operator<<(std::ostream & os, const ZeroDimensionsEx & ex)
	{
		 os << ex.getName();
		 return os;
	}






	//EmptyClusterEx

	Clustering::EmptyClusterEx::EmptyClusterEx()
	{
		__name = "EmptyClusterEx";
	}

	std::string Clustering::EmptyClusterEx::getName() const
	{
		return std::string(__name);
	}

	std::ostream & operator<<(std::ostream & os, const EmptyClusterEx & ex)
	{
		os << ex.getName();
		return os;
	}
}