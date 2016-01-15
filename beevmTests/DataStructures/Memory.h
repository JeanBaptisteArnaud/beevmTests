/*
 * Memory.h
 *
 *  Created on: 14 janv. 2016
 *      Author: jbapt
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include "GCSpace.h"

namespace Bee {

class Memory {

	static Memory * singleton;
protected:
	GCSpace currentSpace;
	unsigned long * spaces;
	GCSpace * collectedSpaces;

	//Collector

public:

	GCSpace nextSpace;
	static Memory * current();
	Memory();
	GCSpace * growIfNeeded(ulong size);
	ulong * obtainFreeSpaceAndAllocate(ulong size);
	GCSpaceInfo allocateWithoutFinalization(ulong);
	void initializeCollector();
	void startUp();
	void createPinnedSpace();
	GCSpace * acquireMoreSpace();
	GCSpace * createNextSpace();
	void addSpace(GCSpace *);
	void addCollectedSpace(GCSpace *);
};

}

#endif /* MEMORY_H_ */