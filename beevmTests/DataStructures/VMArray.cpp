/*
 * VMArray.cpp
 *
 *  Created on: 18 d�c. 2015
 *      Author: Arnaud Jean-Baptiste
 */

#include "VMArray.h"

using namespace Bee;

VMArray::VMArray() {
}

void VMArray::emptyWith(unsigned long * array)
{
	contents = array;
}


long VMArray::nextFree()
{
	return contents[0] - 1;
}

long VMArray::size()
{
	return nextFree() - 1;
}

long VMArray::nextFree(unsigned long newSize)
{
	contents[0] = newSize + 1;
}


long VMArray::size(unsigned long newSize)
{
	nextFree(newSize + 1);
}

bool VMArray::isEmpty()
{
	return size() == 0;
}

void VMArray::reset()
{
	size(0);
}

unsigned long VMArray::pop()
{
	unsigned long value = contents[this->size()];
	//contents[this->size()] = nil; Maybe reset to nil it is the point to have VM Array
	this->size(this->size() - 1);
	return value;

}

void VMArray::add(unsigned long value)
{
	this->size(this->size() + 1);
	contents[this->size()] = value;
}

void VMArray::addAll(unsigned long * array, unsigned long value)
{
	for (unsigned long index = 0 ;index < value; index++) {
		this->add(array[index]);
	}
}

unsigned long*& VMArray::operator[](int i)
{
	return (unsigned long*&) contents[i];
}
