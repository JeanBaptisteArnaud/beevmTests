/*
 * GenerationalGC.h
 *
 *  Created on: 18 d�c. 2015
 *      Author: Arnaud Jean-Baptiste
 */

#ifndef GARBAGECOLLECTOR_GENERATIONALGC_H_
#define GARBAGECOLLECTOR_GENERATIONALGC_H_

#include "GarbageCollector.h"
#include "../DataStructures/GCSpace.h"

namespace Bee {

class GenerationalGC: public GarbageCollector {

public:
	GenerationalGC();
	~GenerationalGC();
	void collect();

protected:
	bool hasToPurge(unsigned long *pointer);
	bool arenaIncludes(unsigned long *pointer);
	unsigned long* codeCacheAtOffset(unsigned long offset);
	void moveToOldAll(ReferencedVMArray &objects);
	void followRememberSet();
	void purgeLiteralsReference();
	void purgeRememberSet();
	void followCodeCacheReferences();
	void initLocals();
	void initNonLocals();
	GenerationalGC * gcForTesting();
	void moveClassCheckReferences();
	void followRoots();
	void cleanRememberSet();
	void addInterrupt();
	void fixReferencesFromNativeMethods();
	void flipSpaces();
	void updateSpacesDelta();
	unsigned long* copyTo(unsigned long *object, GCSpace &to);
	unsigned long* moveToOldSpace(unsigned long *object);
	unsigned long holdReferenceTo(unsigned long *object);
	void spacesDelta(unsigned long delta);
	unsigned long spacesDelta();
	unsigned long* framePointerToStartWalkingTheStack();
	void fixReferencesOrSetTombstone(unsigned long *weakContainer);
};

extern "C" {
__declspec(dllexport) void collect(int val);
}

}

#endif /* GARBAGECOLLECTOR_GENERATIONALGC_H_ */