/*
 * VMArray.cpp
 *
 *  Created on: 18 d�c. 2015
 *      Author: Arnaud Jean-Baptiste
 */

#include "VMArray.h"
#include "Windows.h"

using namespace Bee;

VMArray::VMArray() {
	contents = this->init();
}

void VMArray::emptyWith(unsigned long * array)
{
	contents = array;
}

unsigned long * VMArray::init(){
	return (unsigned long *) malloc (4 * maxSize);
}


void VMArray::grow(){
	unsigned long * oldContents;
	oldContents = contents;
	maxSize += 10;
	contents = this->init();
	for(int index = 0; index < maxSize ; index ++){
		contents[index] = oldContents[index];
	}
	free(oldContents);
}

void VMArray::grow(unsigned long value){
	unsigned long * oldContents;
	oldContents = contents;
	maxSize += 10 + value;
	contents = this->init();
	for(int index = 0; index < maxSize ; index ++){
		contents[index] = oldContents[index];
	}
	free(oldContents);
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
	if((this->size() + 1) >= maxSize) this->grow();
	this->size(this->size() + 1);
	contents[this->size()] = value;
}

void VMArray::addAll(unsigned long * array, unsigned long value)
{
	if((this->size() + value) >= maxSize) this->grow(value);
	for (unsigned long index = 0 ;index < value; index++) {
		this->add(array[index]);
	}
}

unsigned long*& VMArray::operator[](int i)
{
	return (unsigned long*&) contents[i];
}
