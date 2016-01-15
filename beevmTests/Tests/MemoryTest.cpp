#include "../DataStructures/Memory.h"
#include "cute.h"
#include "cute_suite.h"

#include "Windows.h"
#include "stdlib.h"

extern cute::suite make_suite_MemoryTest();

using namespace std;
using namespace Bee;


void singleton() {
	Memory * m = Memory::current();
	Memory * m2 = Memory::current();
	m->createNextSpace();
	ASSERTM("Not a singleton", (ulong) m2->nextSpace.getBase() == (ulong) m->nextSpace.getBase());
}

cute::suite make_suite_MemoryTest() {
	cute::suite s;
	s.push_back(CUTE(singleton));
	//s.push_back(CUTE(basicSize));
	//s.push_back(CUTE(beeExtended));
	//s.push_back(CUTE(rotateLeftTest));
	//s.push_back(CUTE(objectFlagManipulation));
	//s.push_back(CUTE(virtualBehavior));
	//s.push_back(CUTE(newGCSpaceShallowCopy));
	//s.push_back(CUTE(basicAt));
	//s.push_back(CUTE(shallowCopyBytes));
	//s.push_back(CUTE(shallowCopyBytes2));
	//s.push_back(CUTE(shallowCopyBytes3));
	//s.push_back(CUTE(shallowCopyBytesExtended));
	//s.push_back(CUTE(shallowCopyExtended));
	//s.push_back(CUTE(synchronousGCSpace));
	return s;
}
