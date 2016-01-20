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
	//Memory::current()->releaseEverything();
}


cute::suite make_suite_GCTest() {
	cute::suite s;
	//Long time so just test it one by one
	s.push_back(CUTE(gcCollect));
	//s.push_back(CUTE(baseTests));
	//s.push_back(CUTE(softLimitTests));
	//s.push_back(CUTE(reservedLimitTests));
	//s.push_back(CUTE(commitedLimitTests));
	//s.push_back(CUTE(nextFreeTests));
	//s.push_back(CUTE(base_3Tests));
	return s;
}

