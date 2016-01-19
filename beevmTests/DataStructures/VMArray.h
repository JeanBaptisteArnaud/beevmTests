/*
 * VMArray.h
 *
 *  Created on: 18 d�c. 2015
 *      Author: Arnaud Jean-Baptiste
 */

#ifndef DATASTRUCTURES_VMARRAY_H_
#define DATASTRUCTURES_VMARRAY_H_

namespace Bee {

class VMArray {


public:
	int maxSize = 10;
	unsigned long * init();
	void grow();
	void grow(unsigned long);
	VMArray();
	long nextFree();
	long nextFree(unsigned long newSize);
	void add(unsigned long);
	void addAll(unsigned long * array , unsigned long size);

	long size();
	long size(unsigned long newSize);
	unsigned long pop();
	bool isEmpty();
	void reset();
	unsigned long*& operator[](int i);
	void emptyWith(unsigned long *);

private:
	unsigned long *contents;
};

}

#endif /* DATASTRUCTURES_VMARRAY_H_ */
