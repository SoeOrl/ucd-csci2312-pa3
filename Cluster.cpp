#include <iostream>
#include <string>
#include<sstream>
#include <algorithm>
#include <vector>
#include <limits>
#include <cassert>

#include "Point.h"
#include "Cluster.h"
#include "Exceptions.h"

namespace Clustering
{
	unsigned int Cluster::__idGenerator = 0; // id generator

	Cluster::Centroid::Centroid(unsigned int d, const Cluster &c) : __c(c), __dimensions(d), __p(d)
	{

	}


	typedef Point * PointPtr;

	LNode::LNode(const Point &p, LNodePtr n) : point(p), next(n) {}

	Cluster::Cluster(unsigned int d): centroid(d,*this)
	{
		__dimensionality = d;
		__size = 0;
		__points = NULL;
		centroid.setValid(false);
		__id = __idGenerator;
		__idGenerator++;
	}
	
	
	const char POINT_CLUSTER_ID_DELIM = ':';

	Cluster::Cluster(const Cluster &cluster):Cluster(cluster.__dimensionality)
	{
		for (int count = 0; count < cluster.__size; ++count) 
		{
			add(cluster[count]);
		}
		__id = cluster.__id;
	}

	Cluster &Cluster::operator=(const Cluster &cluster)
	{
		if (this == &cluster)
			return *this;
		else 
		{
			if (__points != NULL) 
			{
				LNodePtr currPtr = __points;
				LNodePtr nextPtr = NULL;
				while (currPtr != NULL) 
				{
					nextPtr = currPtr->next;
					delete currPtr;
					currPtr = nextPtr;
				}
				__size = 0;
				__points = NULL;
			}
			for (int count = 0; count < cluster.__size; ++count) {
				add(cluster[count]);
			}
		}
		__id = cluster.__id;
		return *this;
	}

	Cluster::~Cluster()
	{	if (__points != NULL) 
		{
			LNodePtr prevPtr = __points;
			LNodePtr currPtr = NULL;
			while (prevPtr != NULL) 
			{
				currPtr = prevPtr->next;
				delete  prevPtr;
				prevPtr = currPtr;
			}
		}
	// simple an easy
	}

	unsigned int Cluster::getSize() const 
	{
		return __size;
	}

	unsigned int Cluster::getDimensionality() const
	{
		return __dimensionality;
	}

	unsigned int Cluster::getId() const
	{
		return __id;
	}

	void Cluster::pickCentroids(unsigned int k, Point **pointArray) {
		if (k >= __size) {
			for (unsigned int i = 0; i < __size; ++i) {
				*(pointArray[i]) = (*this)[i];
			}
			if (k > __size) {
				for (unsigned int i = __size; i < k; ++i) {
					//pointArray[i] = new Point(__dimensionality);
					for (unsigned int d = 0; d < __dimensionality; ++d) {
						pointArray[i]->setValue(d, std::numeric_limits<double>::max());
						//(pointArray[i])[d] = std::numeric_limits<double>::max();
					}
				}
			}
		}
		else {
			if (k > 100) {
				for (unsigned int i = 0; i < k; ++i) {
					*(pointArray[i]) = (*this)[i];
				}
			}
			else {
				*(pointArray[0]) = __points->point;
				
				for (unsigned int a = 1; a < k; ++a) {
					double avgD = 0;
					unsigned int furIndx = 0;
					for (unsigned int b = 0; b < __size; ++b) {
						double nextD = 0;
						bool used = false;

						
						for (unsigned int c = 0; c < a; ++c) {
							nextD += ((*this)[b]).distanceTo(*(pointArray[c]));
							if ((*this)[b] == *(pointArray[c]))
								used = true;
						}
						nextD /= a;

						
						if (nextD > avgD && !used) {
							avgD = nextD;
							furIndx = b;
						}
					}
					*pointArray[a] = (*this)[furIndx];
					
				}
			}
		}
	}
	void Cluster::add(const Point &point) {
		centroid.setValid(false);
		if (getDimensionality() != point.getDims())
		{
			throw DimensionalityMismatchEx(getDimensionality(), point.getDims());
		}
		
		Point p(point);
		LNodePtr newP = new LNode(p, NULL);
		LNodePtr prev = __points;
		LNodePtr next = __points;
		if (__points == NULL) {
			__points = newP;
			__size++;
		}
		else if (__points->next == NULL) {
			if (point < __points->point) {
				__points = newP;
				newP->next = prev;
				__size++;
			}
			else {
				__points->next = newP;
				__size++;
			}
		}
		else {
			next = next->next;
			if (point < prev->point) {
				__points = newP;
				newP->next = prev;
				__size++;
				return;
			}
			while (next != NULL) {
				if (point < next->point) {
					prev->next = newP;
					newP->next = next;
					__size++;
					return;
				}
				next = next->next;
				prev = prev->next;
			}
			prev->next = newP;
			__size++;
		}
	}

	const Point &Cluster::remove(const Point &point)
	{
		if (getDimensionality() != point.getDims())
		{
			throw DimensionalityMismatchEx(getDimensionality(), point.getDims());
		}
		centroid.setValid(false);
		LNodePtr prev = __points;

	 if (prev->next == NULL)
		{
			if (prev->point == point)
			{	
				__points = NULL;
				__size = 0;
				delete prev;
			}
		}
		else
		{
			bool found = false;
			prev = __points;
			LNodePtr current = __points->next;

			if (__points->point == point)
			{		
				found = true;
				__points = current;
				--__size;
				delete prev;
			}
			for (current ; !found && current != NULL; current = current->next)
			{
				if (current->point == point)
				{
					found = true;
					prev->next = current->next;
					--__size;
					delete current;
				}
				else
				{
					prev = current;
				}
			}
		}
		return point;
	}

	bool Cluster::contains(const Point &point) const
	{
		bool found = false;

		LNodePtr current = __points;

		for (current ; !found && current != NULL; current = current->next)
			if (current->point == point)
			{				
				found = true;
			}

		return found;
	}

	const Point &Cluster::operator[](unsigned int index) const 
	{
		if (index >= __size || __size == 0)
		{
			if (__size == 0)
			{
				throw EmptyClusterEx();
			}
			else
			{
				throw OutOfBoundsEx(__size, index);
			}
		}

		else if (__points != NULL && index < __size && index >= 0)
		{
			LNodePtr current = __points;

			for (int position = 0; position < index; ++position)
			{		
				current = current->next;
			}

			return current->point;
		}
		
	}

	Cluster &Cluster::operator+=(const Point &point)
	{
		if (getDimensionality() != point.getDims())
		{
			throw DimensionalityMismatchEx(getDimensionality(), point.getDims());
		}
		add(point);
		centroid.setValid(false);
		return *this;
	}

	Cluster &Cluster::operator-=(const Point &point)
	{
		if (getDimensionality() != point.getDims())
		{
			throw DimensionalityMismatchEx(getDimensionality(), point.getDims());
		}
		remove(point);
		centroid.setValid(false);
		return *this;
	}
	
	Cluster &Cluster::operator+=(const Cluster &clusterIn) 
	{
		if (getDimensionality() != clusterIn.getDimensionality())
		{
			throw DimensionalityMismatchEx(getDimensionality(), clusterIn.getDimensionality());
		}
		LNodePtr current_rhs = clusterIn.__points;

		for (current_rhs; current_rhs != NULL; current_rhs = current_rhs->next)
		{
			if (!(this->contains(current_rhs->point)))
				add(current_rhs->point);
		}
		return *this;
	}

	Cluster &Cluster::operator-=(const Cluster &clusterIn) 
	{
		if (getDimensionality() != clusterIn.getDimensionality())
		{
			throw DimensionalityMismatchEx(getDimensionality(), clusterIn.getDimensionality());
		}
	
		LNodePtr current_rhs = clusterIn.__points;

		while (current_rhs != NULL)
		{
			if ((contains(current_rhs->point)))
			{
				remove(current_rhs->point);
			}
			current_rhs = current_rhs->next;
		}
		
		return *this;
	}

	std::ostream &operator<<(std::ostream &os, const Cluster &clusterIn)
	{
		LNodePtr current = clusterIn.__points;

		if (&current != NULL)
		{
			for (current; &current->point != NULL; current = current->next)
			{
				os << current->point;
				if (&current->next != NULL)
					os << "\n";
			}
		}
		return os;
	}

	std::istream &operator>>(std::istream &istream, Clustering::Cluster &cluster)
	{
		std::string value;
		while (getline(istream,value) && istream.good()) 
		{
			int n = std::count(value.begin(), value.end(), Point::POINT_VALUE_DELIM);
			Point temp(n+1);
			std::stringstream ss(value);
			
			ss >> temp;
			
			if (temp.getDims() == cluster.getDimensionality())
			{
				cluster.add(temp);
			}
			
			temp.rewindIdGen();
			
		}
		
		return istream;
	}

	bool operator==(const Cluster &cluster1, const Cluster &cluster2)
	{
		if (cluster1.getDimensionality() != cluster2.getDimensionality())
		{
			throw DimensionalityMismatchEx(cluster1.getDimensionality(), cluster2.getDimensionality());
		}
		if (cluster1.__size != cluster2.__size)
		{
			return false;
		}
		LNodePtr current1 = cluster1.__points;
		LNodePtr current2 = cluster2.__points;
		bool equal = true;
		while (current1 != NULL && current2 != NULL)
		{
			if (current1->point != current2->point)
			{
				equal = false;
			}
			current1 = current1->next;
			current2 = current2->next;
		}

		return equal;
	}

	const Cluster operator+(const Cluster &clusterIn, const Point &point)
	{
		Cluster newCluster(clusterIn);
		return newCluster += point;
	}

	bool operator!=(const Cluster &lhs, const Cluster &rhs)
	{
		return (!(lhs == rhs));
	}

	const Cluster operator-(const Cluster &clusterIn, const Point &point)
	{
		Cluster newCluster(clusterIn);
		return newCluster -= point;
	}

	const Cluster operator-(const Cluster &lhs, const Cluster &rhs) // (asymmetric) difference
	{
		Cluster newCluster(lhs);
		return newCluster -= rhs;
	}

	const Cluster operator+(const Cluster &lhs, const Cluster &rhs) // union
	{
		Cluster newCluster(lhs);
		return newCluster += rhs;
	}



	const Point Cluster::Centroid::get() const
	{
		return __p;
	}

	void Cluster::Centroid::set(const Point &p)
	{
		for (int i = 0; i < p.getDims(); i++)
		{
			__p[i] = p[i];
		}
		setValid(true);
	}

	bool Cluster::Centroid::isValid() const
	{
		return __valid;
	}

	void Cluster::Centroid::setValid(bool valid)
	{
		__valid = valid;
	}

	void Cluster::Centroid::compute()
	{
		Point temp(__c.getDimensionality());
		LNodePtr curr = __c.__points;
		unsigned int sizeCheck=0;
		while (curr != nullptr)
		{
			temp += curr->point / __c.getSize();
			curr = curr->next;
			sizeCheck++;
				
		}
		assert(sizeCheck == __c.getSize());
		set(temp);
			
		

		setValid(true);
	}

	bool Cluster::Centroid::equal(const Point &p) const
	{

		for (int i = 0; i < __c.getDimensionality(); i++)
		{
			if (__p[i] != p[i])
			{
				return false;
			}
		}
		return true;
	}

	void Cluster::Centroid::toInfinity()
	{
		for (int i = 0; i < __c.getDimensionality(); i++)
			__p[i] = std::numeric_limits<double>::max();
	}





	Cluster::Move::Move(const Point &p, Cluster &from, Cluster &to) : __p(p),__from(from),__to(to)
	{
	}

	void Cluster::Move::perform()
	{
		__from.remove(__p);
		__from.centroid.setValid(false);
		__to.add(__p);
		__to.centroid.setValid(false);
	}
}

