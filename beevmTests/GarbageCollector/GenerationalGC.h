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
	unsigned long * moveToOldOrTo(unsigned long * object);
	void initLocals();
	void initNonLocals();
	void addRoot(unsigned long * object);
	void followRoots();
	bool arenaIncludes(unsigned long *pointer);
	unsigned long * copyTo(unsigned long *object, GCSpace &to);

	void purgeRoots();

protected:
	bool hasToPurge(unsigned long *pointer);
	unsigned long* codeCacheAtOffset(unsigned long offset);
	void moveToOldAll(ReferencedVMArray &objects);
	void followRememberSet();
	void purgeLiteralsReference();
	void purgeRememberSet();
	bool checkReachablePropertyOf(unsigned long *ephemeron);
	void followCodeCacheReferences();
	void moveClassCheckReferences();
	void cleanRememberSet();
	void addInterrupt();
	void fixReferencesFromNativeMethods();
	void flipSpaces();
	void updateSpacesDelta();
	unsigned long * moveToOldSpace(unsigned long * object);
	unsigned long * moveToToSpace(unsigned long * object);

	void spacesDelta(unsigned long delta);
	unsigned long spacesDelta();
	unsigned long* framePointerToStartWalkingTheStack();
	void fixReferencesOrSetTombstone(unsigned long *weakContainer);
	void followCountStartingAt(unsigned long * pointer, int count, long start);
	void someEphemeronsRescued();
	void holdReferenceTo(unsigned long * object);
};

extern "C" {
__declspec(dllexport) void collect(int val);
}

}

#endif /* GARBAGECOLLECTOR_GENERATIONALGC_H_ */
