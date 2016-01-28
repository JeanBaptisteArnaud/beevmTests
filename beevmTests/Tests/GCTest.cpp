#include "../DataStructures/GCSpaceInfo.h"
#include "../DataStructures/Memory.h"
#include "../GarbageCollector/GenerationalGC.h"

#include "cute.h"
#include "cute_suite.h"

#include <iostream>

extern cute::suite make_suite_GCTest();

using namespace Bee;
using namespace std;

// File myclassTest.h

using namespace Bee;

void gcCollect() {
	mockVMValue();
	GenerationalGC * flipper = new GenerationalGC();
	GCSpace local = GCSpace::dynamicNew(1024 * 1024 * 4 * 6);
	flipper->localSpace = local;
	flipper->collect();
	Memory::current()->releaseEverything();
}

void copyToFlip() {
	mockVMValue();
	unsigned long * array = mockArray();
	GenerationalGC * flipper = new GenerationalGC();

	// this is done in the initialized of the VM
	GCSpace local = GCSpace::dynamicNew(1024 * 1024 * 4 * 6);
	flipper->localSpace = local;
	flipper->initLocals();
	flipper->initNonLocals();

	unsigned long * copy = flipper->fromSpace.shallowCopy(array);
	copy = flipper->moveToOldOrTo(copy);
	ASSERTM("copy is not an array", isArray(copy));
	ASSERTM("size is different", size(copy) == size(array));
	ASSERTM("First Element is different", array[0] == copy[0]);
	//assert: array = copy;
	ASSERTM("oopy is not in toSpace", flipper->toSpace.includes(copy));
	ASSERTM("remmenber set do not include copy",
			!(flipper->rememberSet.includes(copy)));

	Memory::current()->releaseEverything();
}

void testCopyToOld() {
	mockVMValue();
	GenerationalGC * flipper = new GenerationalGC();

	GCSpace local = GCSpace::dynamicNew(1024 * 1024 * 4 * 6);

	unsigned long * array = mockArray();
	flipper->localSpace = local;
	flipper->initLocals();
	flipper->initNonLocals();
	array[0] = 1;
	unsigned long * copy = flipper->fromSpace.shallowCopy(array);
	copy[0] = 3;
	copy = flipper->moveToOldOrTo(copy);
	copy[0] = 4;
	ASSERTM("copy is not in toSpace", flipper->toSpace.includes(copy));
	copy = flipper->moveToOldOrTo(copy);
	copy[0] = 1;
	ASSERTM("copy is not an array", isArray(copy));
	ASSERTM("size is different", size(copy) == size(array));
	ASSERTM("First Element is different", array[0] == copy[0]);
	//assert: array = copy;
	ASSERTM("oopy is not in toSpace", flipper->oldSpace.includes(copy));
	ASSERTM("remmenber set do not include copy",
			!(flipper->rememberSet.includes(copy)));

	Memory::current()->releaseEverything();
}

void testCopyToOldBee() {

	mockVMValue();
	GenerationalGC * flipper = new GenerationalGC();

	GCSpace local = GCSpace::dynamicNew(1024 * 1024 * 4 * 6);

	unsigned long * array = mockArray();
	flipper->localSpace = local;
	flipper->initLocals();
	flipper->initNonLocals();
	array[0] = 1;
	unsigned long * copy = flipper->fromSpace.shallowCopy(array);
	copy[0] = 3;
	copy = flipper->moveToOldOrTo(copy);
	copy[0] = 4;
	ASSERTM("copy is not in toSpace", flipper->toSpace.includes(copy));
	copy = flipper->moveToOldOrTo(flipper->moveToOldOrTo(copy));
	copy[0] = 1;
	ASSERTM("copy is not an array", isArray(copy));
	ASSERTM("size is different", size(copy) == size(array));
	ASSERTM("First Element is different", array[0] == copy[0]);
	//assert: array = copy;
	ASSERTM("oopy is not in toSpace", flipper->oldSpace.includes(copy));
	ASSERTM("remmenber set do not include copy",
			!(flipper->rememberSet.includes(copy)));

	Memory::current()->releaseEverything();
}

void testEphemeron() {
	mockVMValue();
	GenerationalGC * flipper = new GenerationalGC();
	Memory::current()->setGC(flipper);
	GCSpace local = GCSpace::dynamicNew(1024 * 1024 * 4 * 6);

	unsigned long * key = mockObjectFrom();
	unsigned long * value = mockObjectFrom();
	unsigned long * ephemeron = mockEphemeronFrom(key, value);
	unsigned long * tombstone = mockObjectFrom();
	unsigned long * root = mockArray1024();
	root[0] = (ulong) ephemeron;
	flipper->localSpace = local;
	flipper->initLocals();
	flipper->initNonLocals();

	ASSERTM("key change", ephemeron[0] == (ulong ) key);
	ASSERTM("value change", ephemeron[1] == (ulong ) value);

	flipper->addRoot(root);
	flipper->tombstone(tombstone);
	flipper->followRoots();
	flipper->rescueEphemerons();
	ASSERTM("rescued ephemeron is not empty",
			flipper->rescuedEphemerons.isEmpty());
	ephemeron = (ulong *) root[0];
	key = (ulong *) root[1];

	ASSERTM("To space do not include ephemeron",
			(flipper->toSpace.includes(ephemeron)));
//	ASSERTM("Key is not in to space" ,(flipper->toSpace.includes(key)));
//	ASSERTM("key change" , ephemeron[1] == key;));
//	ASSERTM("value not set" ,(flipper->toSpace.includes((unsigned long *) ephemeron[2])));

	Memory::current()->releaseEverything();
}


void testFollowObject() {
	mockVMValue();
	GenerationalGC * flipper = new GenerationalGC();
	Memory::current()->setGC(flipper);
	GCSpace local = GCSpace::dynamicNew(1024 * 1024 * 4 * 6);
	flipper->localSpace = local;
	flipper->initLocals();
	flipper->initNonLocals();

	unsigned long * array = flipper->fromSpace.shallowCopy(mockArray2());
	unsigned long * string = flipper->fromSpace.shallowCopy(mockArray2());
	flipper->fromSpace.shallowCopy(mockArray1024());
	unsigned long * byteArray = flipper->fromSpace.shallowCopy(mockArray2());
	unsigned long * root = flipper->fromSpace.shallowCopy(mockArray2());
	long flipSize = flipper->toSpace.getNextFree() - flipper->toSpace.getBase();
	long currentSize = flipper->fromSpace.getNextFree()
			- flipper->fromSpace.getBase();

	ASSERTM("ToSpace not empty 0", flipSize == 0);
	ASSERTM("ToSpace not correctly allocated ", currentSize == (1024 + 32));
	array[0] = 1;
	array[1] = (ulong) string;
	array[2] = (ulong) byteArray;
	root[0] = (ulong) array;
	//string = byteArray = array = nil;
	flipper->addRoot(root);
	flipper->followRoots();

	flipSize = flipper->toSpace.getNextFree() - flipper->toSpace.getBase();
	currentSize = flipper->fromSpace.getNextFree()
			- flipper->fromSpace.getBase();

	ASSERTM("flipSize is not empty", flipSize);
	ASSERTM("flipSize >= current size", flipSize < currentSize);
	// check the number
	ASSERTM("calculation of pointer", (currentSize - flipSize) == (1035));
	Memory::current()->releaseEverything();
}

void testGCReferencesAfterCollect() {
// cannot test that.
//	mockVMValue();
//	GenerationalGC * flipper = new GenerationalGC();
//	Memory::current()->setGC(flipper);
//	GCSpace local = GCSpace::dynamicNew(1024 * 1024 * 4 * 6);
//	flipper->localSpace = local;
//	flipper->collect();
//	testGCReferencesSetupFramePointer
//		| flipper local |
//		flipper := GenerationalGC new.
//		local := GCSpace dynamicNew: 1024 * 1024 * 4 * 6.
//		flipper localSpace: local.
//		self
//			execute: [:proxy | proxy initLocals; setupFramePointer]
//			proxying: flipper.
//		self
//			assert: (flipper instVars
//				conform: [:var | self checkValid: var using: local])
}

void testTombstone() {
	mockVMValue();
	GenerationalGC * flipper = new GenerationalGC();
	Memory::current()->setGC(flipper);
	GCSpace local = GCSpace::dynamicNew(1024 * 1024 * 4 * 6);
	flipper->localSpace = local;
	flipper->initLocals();
	flipper->initNonLocals();
	unsigned long * weakArray = flipper->fromSpace.shallowCopy(mockWeakArray());
	unsigned long * toGarbage = flipper->fromSpace.shallowCopy(mockArray2());
	weakArray[1] = (ulong) toGarbage;
	weakArray[0] = (ulong) toGarbage;
	unsigned long * root = mockArray2();
	root[0] = (ulong) weakArray;
	ASSERTM("from space include", flipper->arenaIncludes(toGarbage));
	unsigned long * tombstone = (ulong *) 42;
	flipper->addRoot(root);
	flipper->tombstone(tombstone);
	flipper->followRoots();
	flipper->fixWeakContainers();
	unsigned long * weak = (ulong *) root[0];
	ASSERTM("weak is not in the to space", flipper->toSpace.includes(weak));
	ASSERTM("tombstone fail", weak[1] == (ulong )tombstone);
	ASSERTM("tombstone fail", weak[0] == (ulong )tombstone);

	Memory::current()->releaseEverything();
}

cute::suite make_suite_GCTest() {
	cute::suite s;
	//Long time so just test it one by one
	s.push_back(CUTE(gcCollect));
	s.push_back(CUTE(copyToFlip));
	s.push_back(CUTE(testCopyToOld));
	s.push_back(CUTE(testCopyToOldBee));
	s.push_back(CUTE(testEphemeron));
	s.push_back(CUTE(testFollowObject));
	s.push_back(CUTE(testGCReferencesAfterCollect));
	s.push_back(CUTE(testTombstone));

	//s.push_back(CUTE(softLimitTests));
	//s.push_back(CUTE(reservedLimitTests));
	//s.push_back(CUTE(commitedLimitTests));
	//s.push_back(CUTE(nextFreeTests));
	//s.push_back(CUTE(base_3Tests));
	return s;
}

