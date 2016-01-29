/*
 * GCSpaceTest.h
 *
 *  Created on: 29 janv. 2016
 *      Author: jbapt
 */

#ifndef GCSPACETEST_H_
#define GCSPACETEST_H_

#include "../GarbageCollector/GenerationalGC.h"
#include "../DataStructures/GCSpace.h"


namespace Bee {
class GCSpaceTest {


protected:

	void setUp();
	void tearDown();

public:
	GenerationalGC * flipper;
	GCSpace localSpace;
	void testAllocate();
	void testGCSpace();
	void testExtendedGrowingTo();
	void testGrow();
	void testGrowingTo();
	void testNewGCSpaceShallowCopy();
	void testShallowCopy();
	void testShallowCopyBytes();
	void testShallowCopyBytes2();
	void testShallowCopyBytes3();
	void testShallowCopyBytesExtended();
	void testShallowCopyExtended();
	void testSynchronousGCSpace();

	GCSpaceTest() {
		this->setUp();
	}
	~GCSpaceTest() {
		this->tearDown();
	}
};
}


#endif /* GCSPACETEST_H_ */
