// Implementation of the templated Vector class
// ECE4893/8893 lab 3
// YOUR NAME HERE

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
	T* memory = (T *)malloc(sizeof(T)*(count+reserved));
	new (memory) T[count+reserved];
	elements = memory;
	memory = NULL;

	for (unsigned int i = 0; i < (count+reserved); i++) {
		elements[i] = rhs.elements[i];
		//memcpy(&elements[i],&rhs.elements[i],sizeof(T));

	}
}

#ifdef GRAD_STUDENT
// Other constructors
template <typename T>
Vector<T>::Vector(size_t nReserved)
{ // Initialize with reserved memory
	//T *elements = (T *)malloc(nReserved);
	count = 0;
	reserved = nReserved;

	T* elements = (T *)malloc(sizeof(T)*reserved);
	new (elements) T[count+reserved];

	//elements = new T[nReserved];	
}

template <typename T>
Vector<T>::Vector(size_t n, const T& t)
{ // Initialize with "n" copies of "t"

	count = n;
	reserved = 0;	
	//elements = new T[n];
	T* memory = (T *)malloc(sizeof(T)*count);
	new (memory) T[count+reserved];
	elements = memory;
	memory = NULL;
	//cout<<"ERROR ERROR ERROR ERROR ERROR ERROR ERROR"<<endl;
	for (unsigned int i = 0; i < (count+reserved); i++) {
		elements[i] = t;
		//memcpy(&elements[i],&t,sizeof(T));
	}
}

template <typename T>
void Vector<T>::Reserve(size_t n)
{ //Use to reserve extra space for a vector object;
	//T* NewElements = new T[count+n];
	T* memory = (T *)malloc(sizeof(T)*(count+n+reserved));
	new (memory) T[count+n+reserved];

	for (unsigned int i = 0; i < count; i++)
	{
		memory[i] = elements[i];
		//memcpy(&memory[i],&elements[i],sizeof(T));
	}
	//free(elements);
	for (unsigned int i = 0; i < (count+reserved); i++)
	{
		elements[i].~T();
	}
	free(elements);
	elements = memory;
	//memory = NULL;
	//free(NewElements);
	reserved += n;
}
#endif

// Destructor
template <typename T>
Vector<T>::~Vector()
{
	//cout << "Destructor is called" << endl;
	
	for (unsigned int i = 0; i < (count+reserved); i++)
	{
		elements[i].~T();
	}	
	free(elements);
	//cout << "Destruction Finished" << endl;
}

// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& rhs)
{
	unsigned int num = count+reserved;
	if (reserved < 1)
	{
		num++;
		reserved++;
	}
	T* memory = (T *)malloc(sizeof(T)*num);
	new (memory) T[num];
	for (unsigned int i = 0; i < num-1; i++)
	{
		memory[i] = elements[i];
		//memmove(&elements[i],&elements[i-1],sizeof(T));
	}
	memory[num-1] = rhs;
	for (unsigned int i = 0; i < (count+reserved-1); i++)
	{
		elements[i].~T();
	}
	free(elements);
	elements = memory;
	//cout<<"ERROR ERROR ERROR ERROR ERROR ERROR ERROR"<<endl;
	//memcpy(&elements[count],&rhs,sizeof(T));
	reserved--;
	count++;
	memory = NULL;
	//cout << count << reserved << endl;
}

template <typename T>
void Vector<T>::Push_Front(const T& rhs)
{
	//T* exc = (T *)malloc(sizeof(T));
	//new (exc) T;
	unsigned int num = count+reserved;
	if (reserved < 1)
	{
		num++;
		reserved++;
	}
	T* memory = (T *)malloc(sizeof(T)*num);
	new (memory) T[num];
	for (unsigned int i = 0; i < num-1; i++)
	{
		memory[i+1]=elements[i];
		//memmove(&elements[i],&elements[i-1],sizeof(T));
	}
	memory[0] = rhs;
	for (unsigned int i = 0; i < (count+reserved-1); i++)
	{
		elements[i].~T();
	}
	free(elements);
	elements = memory;
	//cout<<"ERROR ERROR ERROR ERROR ERROR ERROR ERROR"<<endl;
	//memcpy(&elements[0],&rhs,sizeof(T));
	count++;
	reserved--;
	memory = NULL;
	//free(exc);
}

template <typename T>
void Vector<T>::Pop_Back()
{ // Remove last element
	//inplace-new
	if (!Empty())
	{
		T* memory = (T *)malloc(sizeof(T)*(count--));
	    new (memory) T[count];
		//T* NewElements = new T[count--];
		for (unsigned int i = 0; i < count; i++)
		{
			memory[i] = elements[i];
			//memcpy(&memory[i],&elements[i],sizeof(T));
		}
		
		for (unsigned int i = 0; i < (count+1); i++)
		{
			elements[i].~T();
		}
		free(elements);
		//T* elements = (T *)malloc(sizeof(T)*(count));
		//new (elements) T[count];
		
		//for (unsigned int i = 0; i < count; i++)
		//{
		//	elements[i] = memory[i];
			//memcpy(&elements[i],&memory[i],sizeof(T));
		//}
		elements = memory;
		/*
		
		
		/*
		for (unsigned int i = 0; i < (count); i++)
		{
			memory[i].~T();
		}*/
		//free(memory);
	}
}

template <typename T>
void Vector<T>::Pop_Front()
{ // Remove first element
	if (!Empty())
	{
		//T* NewElements = new T[count--];
		T* memory = (T *)malloc(sizeof(T)*(count--));
	    new (memory) T[count];
		for (unsigned int i = 0; i < count; i++)
		{
			memory[i] = elements[i+1];
			//memcpy(&memory[i],&elements[i+1],sizeof(T));
		}
		//free(elements);
		for (unsigned int i = 0; i < (count+1); i++)
		{
			elements[i].~T();
		}
		free(elements);
		elements = memory;
		memory = NULL;
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
/*
template <typename T>
T& Vector<T>::operator[](size_t i) 
{
	//cout << elements[i] << endl;
	return elements[i];
}
*/
template <typename T>
T& Vector<T>::operator[](size_t i) const
{
	return elements[i];
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
	//cout << "Entering Clear" << endl;
	//T* memory = (T *)malloc(sizeof(T)*(count+reserved));
	//


	//T * NewElements = new T[count];
	for (unsigned int i = 0; i < (count+reserved); i++)
	{
		elements[i].~T();
	}
	new (elements) T[count+reserved];
	//free(elements);
	//elements = memory;
	//memory = NULL;
	/*
	for (unsigned int i = 0; i < count; i++)
	{
		//cout << "Rount No." << i << endl;
		//cout << elements[i] << endl;
		delete &elements[i];
	}
	*/

	reserved = count;
	count = 0;
}

// Iterator access functions
template <typename T>
VectorIterator<T> Vector<T>::Begin() const
{
  //cout << "Hello from NonConst Begin" << endl;

  return VectorIterator<T>(elements);
}

/*
template <typename T>
ConstVectorIterator<T> Vector<T>::Begin() const
{
  //cout << "Hello from Const Begin" << endl;
  return ConstVectorIterator<T>(elements);
}
*/

template <typename T>
VectorIterator<T> Vector<T>::End() const
{
  //cout << "Hello from NonConst End" << endl;
  return VectorIterator<T>(elements+count);
}

/*
template <typename T>
ConstVectorIterator<T> Vector<T>::End() const
{
  //cout << "Hello from Const End" << endl;
  return ConstVectorIterator<T>(elements+count);
}
*/

#ifdef GRAD_STUDENT
// Erase and insert
template <typename T>
void Vector<T>::Erase(const VectorIterator<T>& it)
{
	if (!Empty())
	{
		count = count - 1;
		T* memory = (T *)malloc(sizeof(T)*(count));
	    new (memory) T[count];

		//T* NewElements = new T[count];
		size_t i = 0;
		while ( elements + i < it.current )
		{
			memory[i] = elements[i];
			//memcpy(&memory[i],&elements[i],sizeof(T));
			i++;
		}
		for (;i < count; i++)
		{
			memory[i] = elements[i+1];
			//memcpy(&memory[i],&elements[i+1],sizeof(T));

		}
		//free(elements);
		for (unsigned int i = 0; i < (count+1); i++)
		{
			elements[i].~T();
		}
		free(elements);
		elements = memory;
		memory = NULL;
	}
}

template <typename T>
void Vector<T>::Insert(const T& rhs, const VectorIterator<T>& it)
{
	count = count + 1;
	T* memory = (T *)malloc(sizeof(T)*(count));
	new (memory) T[count];
	size_t i = 0;//	memset(&memory,1,sizeof(T)*count);
	while ( elements + i < it.current )
	{
		memory[i] = elements[i];
		//memcpy(&memory[i],&elements[i],sizeof(T));
		i++;
	}
	//cout<<"ERROR ERROR ERROR ERROR ERROR ERROR ERROR"<<endl;
	memory[i++] = rhs;
	//memcpy(&memory[i++],&rhs,sizeof(T));
	while (i < count)
	{
		memory[i] = elements[i-1];
		//memcpy(&memory[i],&elements[i-1],sizeof(T));
		i++;
	}
	//free(elements);
	for (unsigned int i = 0; i < (count-1); i++)
	{
		elements[i].~T();
	}
	free(elements);
	elements = memory;
	//memory = NULL;
}
#endif

// Implement the iterators

// Constructors
template <typename T>
VectorIterator<T>::VectorIterator()
	:current(0)
{
}

/*
template <typename T>
ConstVectorIterator<T>::ConstVectorIterator()
	:current(0)
{
}
*/

template <typename T>
VectorIterator<T>::VectorIterator(T* c)
{
	//cout << "Hello Form T Constructors" << endl;
	current = c;
	//cout << current << endl;
	//cout << "Exiting T Constructors" << endl;
}

/*
template <typename T>
ConstVectorIterator<T>::ConstVectorIterator(T* c)
{
	//cout << "Hello Form T Constructors" << endl;
	current = c;
	//cout << current << endl;
	//cout << "Exiting T Constructors" << endl;
}
*/

// Copy constructor
template <typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T>& rhs)
{
	current = rhs.current;
}

/*
template <typename T>
ConstVectorIterator<T>::ConstVectorIterator(const ConstVectorIterator<T>& rhs)
{
	current = rhs.current;
}
*/

// Iterator defeferencing operator
template <typename T>
T& VectorIterator<T>::operator*() const
{
	//cout << "defeferencing operator" <<endl;
	//cout << current << endl;
	return *current;
	//cout << "Exiting defeferencing operator" <<endl;
}

/*
template <typename T>
T& ConstVectorIterator<T>::operator*() const
{
	//cout << "defeferencing operator" <<endl;
	//cout << current << endl;
	return *current;
	//cout << "Exiting defeferencing operator" <<endl;
}
*/

// Prefix increment
template <typename T>
VectorIterator<T>  VectorIterator<T>::operator++()
{
	//cout << "Prefix increment" << endl;
	current = current + 1;
	return current;
}

/*
template <typename T>
ConstVectorIterator<T>  ConstVectorIterator<T>::operator++()
{
	//cout << "Prefix increment" << endl;
	current = current + 1;
	return current;
}
*/

// Postfix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int)
{
	//cout << "Postfix increment" << endl;
	current = current + 1;
	return current - 1;
}

/*
template <typename T>
ConstVectorIterator<T> ConstVectorIterator<T>::operator++(int)
{
	//cout << "Postfix increment" << endl;
	current = current + 1;
	return current - 1;
}
*/

// Comparison operators
template <typename T>
bool VectorIterator<T>::operator !=(const VectorIterator<T>& rhs) const
{
	//cout << "Unequal operator" << endl;
	if(current == rhs.current)
	{
		return false;
	}
	else {
		return true;
	}
}
/*
template <typename T>
bool ConstVectorIterator<T>::operator !=(const ConstVectorIterator<T>& rhs) const
{
	//cout << "Unequal operator" << endl;
	if(current == rhs.current)
	{
		return false;
	}
	else {
		return true;
	}
}
*/
template <typename T>
bool VectorIterator<T>::operator ==(const VectorIterator<T>& rhs) const
{
	//cout << "Equal operator" << endl;
	if(current == rhs.current)
	{
		return true;
	}
	else {
		return false;
	}
}
/*
template <typename T>
bool ConstVectorIterator<T>::operator ==(const ConstVectorIterator<T>& rhs) const
{
	//cout << "Equal operator" << endl;
	if(current == rhs.current)
	{
		return true;
	}
	else {
		return false;
	}
}
*/
//int main(){
//
//}



