 

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include<algorithm>
#include<vector>
#include "Point.h"
#include "Exceptions.h"
// using namespace Clustering;


								 // iostream
using std::cout;
using std::endl;

// fstream
using std::ifstream;

// sstream
using std::stringstream;
using std::string;
namespace Clustering {

	const char Point::POINT_VALUE_DELIM = ',';

	unsigned int Point::__idGen = 0; // id generator

	void Point::rewindIdGen()
	{
		__idGen = __idGen - 1;
	}

	Point::Point(unsigned int d) : __values{}
	{
		if (d == 0)
			throw ZeroDimensionsEx();


		__dim = d;
		__id = __idGen;
		__idGen++;
		__values = new double[d];
		for (int i = 0; i < __dim; i++)
		{
			setValue(i, 0);
		}
	}

	Point::Point(unsigned int d, double * values)
	{
		if (d == 0)
			throw ZeroDimensionsEx();
		__dim = d;
		__id = __idGen;
		__idGen++;
		__values = values;

	}

	Point::Point(const Point &point)
	{
		__id = point.__id;
		__dim = point.__dim;
		__values = new double[__dim];
		for (int i = 0; i < __dim; i++)
		{
			__values[i] = point.__values[i];
		}
	}

	Point& Point::operator= (const Point &point)
	{
		if (getDims() != point.getDims())
		{
			throw DimensionalityMismatchEx(getDims(), point.getDims());
		}
		
		if (this == &point)
		{
			return *this;
		}
		this->__dim = point.__dim;
		this->__id = point.__id;
		this->__values = new double[point.__dim];

		for (int i = 0; i < __dim; i++)
		{
			this->__values[i] = point.__values[i];
		}
		return *this;
	}

	Point::~Point()
	{
		delete[] __values;
	}

	int Point::getId() const
	{
		return __id;
	}

	unsigned int Point::getDims() const
	{
		return __dim;
	}

	void Point::setValue(unsigned int index, double value)
	{
		if (index >= __dim)
		{
			throw OutOfBoundsEx(__dim, index);
			
		}
		else
		{
			__values[index] = value;
		}
	}

	double Point::getValue(unsigned int index) const
	{
		if (index >= __dim)
		{
			throw OutOfBoundsEx(__dim, index);
			
		}
		else
		{
			return __values[index];
		}
	}

	// Distance Formula
	double Point::distanceTo(const Point& point) const
	{
		if (point.getDims() != getDims())
		{
			throw DimensionalityMismatchEx(getDims(), point.getDims());
		}
		else
		{
			double answer = 0;
			for (int count = 0; count < __dim; count++)
			{
				answer += pow(point.__values[count] - __values[count], 2);
			}
			return sqrt(answer);
		}
	}

	Point& Point::operator*=(double multi)
	{
		for (int i = 0; i < __dim; i++)
		{
			__values[i] *= multi;
		}
		return *this;
	}

	Point& Point::operator/=(double div)
	{
		for (int i = 0; i < __dim; i++)
		{
			__values[i] /= div;
		}
		return *this;
	}

	const Point Point::operator*(double multi) const
	{
		Point p(*this);
		return p *= multi;
	}

	const Point Point::operator/(double div) const
	{
		Point p(*this);
		return p /= div;
	}

	double &Point::operator[] (unsigned int index)
	{
		if (index >= __dim)
		{
			
			throw OutOfBoundsEx(__dim, index);
			
		}
		else
		{
			return __values[index];
		}
	}

	Point &operator+=(Point &lhs, const Point &rhs)
	{
		if (lhs.getDims() != rhs.getDims())
		{
			throw DimensionalityMismatchEx(lhs.getDims(), rhs.getDims());
		}
		else
		{
			for (int i = 0; i < lhs.__dim; i++)
			{
				lhs.__values[i] += rhs.__values[i];
			}
			return lhs;
		}
	}

	Point &operator-=(Point &lhs, const Point &rhs)
	{
		if (lhs.getDims() != rhs.getDims())
		{
			throw DimensionalityMismatchEx(lhs.getDims(), rhs.getDims());
		}
		else
		{
			for (int i = 0; i < lhs.__dim; i++)
			{
				lhs.__values[i] -= rhs.__values[i];
			}
			return lhs;
		}
	}

	const Point operator+(const Point &lhs, const Point &rhs)
	{
		Point p(lhs);
		return p += rhs;
	}

	const Point operator-(const Point &lhs, const Point &rhs)
	{
		Point p(lhs);
		return p -= rhs;
	}

	bool operator==(const Point &lhs, const Point &rhs)
	{
		if (lhs.getDims() != rhs.getDims())
		{
			throw DimensionalityMismatchEx(lhs.getDims(), rhs.getDims());
		}
		else if (lhs.__id != rhs.__id)
		{
			return false;
		}

		for (int i = 0; i < lhs.__dim; i++)
		{
			if (lhs.__values[i] != rhs.__values[i])
			{
				return false;
			}
		}
		return true;
	}

	bool operator!=(const Point &lhs, const Point &rhs)
	{
		return (!(lhs == rhs));
	}

	bool operator<(const Point &lhs, const Point &rhs)
	{
		if (lhs.getDims() != rhs.getDims())
		{
			throw DimensionalityMismatchEx(lhs.getDims(), rhs.getDims());
		}
		else
		{
			for (int i = 0; i < lhs.__dim; i++)
			{
				if (lhs.__values[i] < rhs.__values[i])
				{
					return true;
				}
			}
			return false;
		}
	}

	bool operator>(const Point &lhs, const Point &rhs)
	{
		return !(lhs < rhs);
	}

	bool operator>=(const Point &lhs, const Point &rhs)
	{

		if (lhs.getDims() != rhs.getDims())
		{
			throw DimensionalityMismatchEx(lhs.getDims(), rhs.getDims());
		}
		else if (!(lhs<rhs) && (lhs>rhs))
		{
			return true;
		}
		else
		{
			return !(lhs < rhs);
		}
	}

	bool operator<=(const Point &lhs, const Point &rhs)
	{
		if (!(lhs<rhs) && (lhs>rhs))
		{
			return true;
		}
		else
		{
			return !(lhs > rhs);
		}
	}

	std::ostream &operator<<(std::ostream &out, const Point &point)
	{
		int i = 0;
		for (; i < point.__dim - 1; i++)
		{
			out << point.getValue(i) << ", ";
		}
		out << point.__values[i];
		return out;
	}

	std::istream &operator>>(std::istream &in, Point &p)
	{
	
			std::string value;
			
			getline(in, value);

			int d = std::count(value.begin(), value.end(), Point::POINT_VALUE_DELIM) + 1;	

			if (d != p.getDims())
			{
				throw DimensionalityMismatchEx(d, p.getDims());
			}

			std::stringstream lineStream(value);

			for (int i = 0; i < p.__dim; i++)
			{
				getline(lineStream, value, Point::POINT_VALUE_DELIM);
				std::stringstream ss(value);
				ss >> p.__values[i];
			}
			
		return in;
	}

	const double & Point::operator[](unsigned int index) const
	{
		if (index > __dim)
		{
			throw OutOfBoundsEx(__dim, index);
		}
		else
		{
			return __values[index];
		}
	}

}
