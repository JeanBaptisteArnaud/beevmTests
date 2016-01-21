#include "../DataStructures/GCSpaceInfo.h"
#include "../DataStructures/Memory.h"
#include "../GarbageCollector/GenerationalGC.h"

#include "cute.h"
#include "cute_suite.h"
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
	ASSERTM("copy is not an array",  isArray(copy));
	ASSERTM("size is different", size(copy)  == size(array) );
	ASSERTM("First Element is different", array[0] == copy[0]);
	//assert: array = copy;
	ASSERTM("oopy is not in toSpace", flipper->toSpace.includes(copy));
	ASSERTM("remmenber set do not include copy", !(flipper->rememberSet.includes(copy)));
}


void testCopyToOld (){
	mockVMValue();
	GenerationalGC * flipper = new GenerationalGC();

	GCSpace local = GCSpace::dynamicNew(1024 * 1024 * 4 * 6);

	//unsigned long * array = mockArray();
	flipper->localSpace = local;
	flipper->initLocals();
//	flipper->initNonLocals();
	//array[0] = 2;
	//unsigned long * copy = flipper->fromSpace.shallowCopy(array);
	//array[0] = 3;
	//copy = flipper->moveToOldOrTo(copy);
	//array[0] = 4;
	//self assert: (flipper toSpace includes: copy).
	//copy = flipper->moveToOldOrTo( copy);
	//array[0] = 1;
//	ASSERTM("copy is not an array",  isArray(copy));
//	ASSERTM("size is different", size(copy)  == size(array) );
//	ASSERTM("First Element is different", array[0] == copy[0]);
	//assert: array = copy;
//	ASSERTM("oopy is not in toSpace", flipper->toSpace.includes(copy));
//	ASSERTM("remmenber set do not include copy", !(flipper->rememberSet.includes(copy)));
}


cute::suite make_suite_GCTest() {
	cute::suite s;
	//Long time so just test it one by one
	s.push_back(CUTE(gcCollect));
//	s.push_back(CUTE(copyToFlip));
	//s.push_back(CUTE(testCopyToOld));
	//s.push_back(CUTE(baseTests));
	//s.push_back(CUTE(softLimitTests));
	//s.push_back(CUTE(reservedLimitTests));
	//s.push_back(CUTE(commitedLimitTests));
	//s.push_back(CUTE(nextFreeTests));
	//s.push_back(CUTE(base_3Tests));
	return s;
}

