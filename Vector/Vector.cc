// Implementation of the templated Vector class
// ECE4893/8893 lab 3
// Qichao Chu

#include <iostream> // debugging
#include <stdlib.h>
#include "Vector.h"
#include <memory.h>

// Your implementation here
// Fill in all the necessary functions below
using namespace std;

// Default constructor
template <typename T>
Vector<T>::Vector()
	:count(0),reserved(0),elements(0)
{
}

// Copy constructor
template <typename T>
Vector<T>::Vector(const Vector& rhs)
{
	count = rhs.count;
	reserved = rhs.reserved;
	T* memory = (T *)malloc(sizeof(T)*(reserved));
	//new (memory) T[reserved];
	elements = memory;
	memory = NULL;

	for (unsigned int i = 0; i < (reserved); i++) {
		new (&elements[i]) T(rhs.elements[i]);
	}
}

#ifdef GRAD_STUDENT
// Other constructors
template <typename T>
Vector<T>::Vector(size_t nReserved)
{ // Initialize with reserved memory
	count = 0;
	reserved = nReserved;

	T* elements = (T *)malloc(sizeof(T)*reserved);
	//new (elements) T[reserved];
}

template <typename T>
Vector<T>::Vector(size_t n, const T& t)
{ // Initialize with "n" copies of "t"

	count = n;
	reserved = n;	
	T* memory = (T *)malloc(sizeof(T)*count);
	//new (memory) T[reserved];
	elements = memory;
	memory = NULL;
	for (unsigned int i = 0; i < (count); i++) {
		new (&elements[i]) T(t);
	}
}

template <typename T>
void Vector<T>::Reserve(size_t n)
{ //Use to reserve extra space for a vector object;
	T* memory = (T *)malloc(sizeof(T)*(n));
	//new (memory) T[n];

	for (unsigned int i = 0; i < count; i++)
	{
		new (&memory[i]) T(elements[i]);
	}
	for (unsigned int i = 0; i < (reserved); i++)
	{
		elements[i].~T();
	}
	free(elements);
	elements = memory;
	reserved = n;
}
#endif

// Destructor
template <typename T>
Vector<T>::~Vector()
{
	for (unsigned int i = 0; i < (reserved); i++)
	{
		elements[i].~T();
	}	
	free(elements);
}

// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& rhs)
{
	unsigned int num = reserved;
	if (reserved - count < 1)
	{
		num++;
		reserved++;

		T* memory = (T *)malloc(sizeof(T)*num);
		for (unsigned int i = 0; i < num-1; i++)
		{
			new (&memory[i]) T(elements[i]);
		}
		new (&memory[num-1]) T(rhs);
		for (unsigned int i = 0; i < (reserved-1); i++)
		{
			elements[i].~T();
		}
		free(elements);
		elements = memory;
		count++;
		memory = NULL;
	} else {
		new (&elements[count]) T(rhs);
		count++;
	}
}

template <typename T>
void Vector<T>::Push_Front(const T& rhs)
{
	unsigned int num = reserved;
	if (reserved - count < 1)
	{
		num++;
		reserved++;
	}
	T* memory = (T *)malloc(sizeof(T)*num);
	//new (memory) T[num];
	for (unsigned int i = 0; i < num-1; i++)
	{
		new (&memory[i+1]) T(elements[i]);
	}
	new (&memory[0]) T (rhs);
	for (unsigned int i = 0; i < (reserved-1); i++)
	{
		elements[i].~T();
	}
	free(elements);
	elements = memory;
	count++;
}

template <typename T>
void Vector<T>::Pop_Back()
{ // Remove last element
	//inplace-new
	if (!Empty())
	{
		/*
		T* memory = (T *)malloc(sizeof(T)*(reserved--));
		for (unsigned int i = 0; i < count-1; i++)
		{
			new (&memory[i]) T(elements[i]);
		}
		
		for (unsigned int i = 0; i < (reserved+1); i++)
		{
			elements[i].~T();
		}
		free(elements);
		elements = memory;
		count--;
		*/
		elements[count-1].~T();
		count--;
		reserved--;
	}
}

template <typename T>
void Vector<T>::Pop_Front()
{ // Remove first element
	if (!Empty())
	{
		T* memory = (T *)malloc(sizeof(T)*(reserved--));
	    //new (memory) T[reserved];
		for (unsigned int i = 0; i < count-1; i++)
		{
			new (&memory[i]) T(elements[i+1]);
		}
		for (unsigned int i = 0; i < (reserved+1); i++)
		{
			elements[i].~T();
		}
		free(elements);
		elements = memory;
		count--;
		//memory = NULL;
	}
}	

// Element Access
template <typename T>
T& Vector<T>::Front() const
{
	return elements[0];
}

// Element Access
template <typename T>
T& Vector<T>::Back() const
{
	return elements[count-1];
}

template <typename T>
T& Vector<T>::operator[](size_t i) const
{
	return *(elements+i);
}

template <typename T>
size_t Vector<T>::Size() const
{
	return (count);
}

template <typename T>
bool Vector<T>::Empty() const
{
	if (count == 0)
	{
		return true;
	}
	else {
		return false;
	}
}

// Implement clear
template <typename T>
void Vector<T>::Clear()
{
	count = 0;
}

// Iterator access functions
template <typename T>
VectorIterator<T> Vector<T>::Begin() const
{
  return VectorIterator<T>(elements);
}

template <typename T>
VectorIterator<T> Vector<T>::End() const
{
  return VectorIterator<T>(elements+count);
}


#ifdef GRAD_STUDENT
// Erase and insert
template <typename T>
void Vector<T>::Erase(const VectorIterator<T>& it)
{
	if (!Empty())
	{
		count = count - 1;
		//reserved = reserved - 1;
		//T* memory = (T *)malloc(sizeof(T)*(reserved));
	    //new (memory) T[count];

		unsigned i = 0;
		while ( elements + i < it.current )
		{
			//new (&memory[i]) T(elements[i]);
			i++;
		}
		//i = elements - it.current;
		for (;i < count; i++)
		{
			elements[i].~T();
			new (&elements[i]) T(elements[i+1]);
		}
		elements[count].~T();
		//reserved = reserved - 1;
		/*
		for (unsigned int i = 0; i < (reserved + 1); i++)
		{
			elements[i].~T();
		}*/
		//free(elements);
		//elements = memory;
		//memory = NULL;
	}
}

template <typename T>
void Vector<T>::Insert(const T& rhs, const VectorIterator<T>& it)
{
	count = count + 1;
	T* memory = (T *)malloc(sizeof(T)*(count));
	//new (memory) T[count];
	unsigned i = 0;
	while ( elements + i < it.current )
	{
		new (&memory[i]) T(elements[i]);
		i++;
	}
	new (&memory[i++]) T(rhs);
	while (i < count)
	{
		new (&memory[i]) T(elements[i-1]);
		i++;
	}
	for (unsigned int i = 0; i < (count-1); i++)
	{
		elements[i].~T();
	}
	free(elements);
	elements = memory;
}
#endif

// Implement the iterators

// Constructors
template <typename T>
VectorIterator<T>::VectorIterator()
	:current(0)
{
}

template <typename T>
VectorIterator<T>::VectorIterator(T* c)
{
	current = c;
}

// Copy constructor
template <typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T>& rhs)
{
	current = rhs.current;
}

// Iterator defeferencing operator
template <typename T>
T& VectorIterator<T>::operator*() const
{
	return *current;
}

// Prefix increment
template <typename T>
VectorIterator<T>  VectorIterator<T>::operator++()
{
	current = current + 1;
	return current;
}

// Postfix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int)
{
	current = current + 1;
	return current - 1;
}
// Comparison operators
template <typename T>
bool VectorIterator<T>::operator !=(const VectorIterator<T>& rhs) const
{
	if(current == rhs.current)
	{
		return false;
	}
	else {
		return true;
	}
}

template <typename T>
bool VectorIterator<T>::operator ==(const VectorIterator<T>& rhs) const
{
	if(current == rhs.current)
	{
		return true;
	}
	else {
		return false;
	}
}
//int main(){
//
//}
