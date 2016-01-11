#include "../DataStructures/GCSpace.h"
#include "cute.h"
#include "cute_suite.h"
#include "Windows.h"

extern cute::suite make_suite_GCSpaceTest();

using namespace Bee;

// File myclassTest.h

void extendedGrowingTo() {
//	testExtendedGrowingTo
//		| array copy localSpace object |
	unsigned long * array;
	array = (ulong *) malloc(4 * 1024);
//		array := Array new: 1024 withAll: 1.
	GCSpace localSpace = GCSpace::dynamicNew(1024 * 4 * 10);
	ulong * copy = localSpace.shallowCopyGrowingTo(array, 2048);
	ulong * object = localSpace.shallowCopy(stFalse);
	ASSERTM("copy is not a Array", isArray(copy));
	ASSERTM("size of copy is wrong", size(copy) == 2048);
	ASSERTM("copy is not consistent (first element diverge)", array[0] =
			copy[0]);
	//ASSERTM((copy count: [:x | x isNil]) = (copy size - array size));
	ASSERTM("The moon is red",
			oop(copy) == asOop((ulong * )(localSpace.getBase()) + 16));
	ASSERTM("I feel a perturbation in the force",
			oop(object) + 8 == (oop(copy) + (size(copy) * 4) + 16));
	free(array);

}

void gcspace() {
	GCSpace space = GCSpace::currentFrom();
	space.load();
	ASSERTM("base < nextfree", space.getBase() < space.getNextFree());
	ASSERTM("base < commitedLimit", space.getBase() < space.getCommitedLimit());
	ASSERTM("commitedLimit <= reservedLimit",
			space.getCommitedLimit() <= space.getReservedLimit());
	ASSERTM("base < commitedLimit", space.getBase() < space.getCommitedLimit());
	ASSERTM("commitedLimit < reservedLimit",
			space.getCommitedLimit() < space.getReservedLimit());
	ASSERTM("softLimit between base and reservedLimit",
			(space.getSoftLimit() <= space.getBase()) && (space.getSoftLimit() >= space.getReservedLimit()));
	ASSERTM("nextFree between base and commitedLimit",
			(space.getNextFree() <= space.getBase()) && (space.getNextFree() >= space.getCommitedLimit()));
	ulong * oldNextFree = space.getNextFree();
	//mockArray();
	ASSERTM("Maybe should be test only from Image", oldNextFree == space.getNextFree());
}

void grow() {
	unsigned long * array;
	array = (ulong *) malloc(100);
	GCSpace local = GCSpace();
	unsigned long  reservedSize = 4 * 1024 * 100;
	unsigned long maxValueQuery = 28;
	PMEMORY_BASIC_INFORMATION queryAnswer = (PMEMORY_BASIC_INFORMATION) malloc(maxValueQuery);
	unsigned long * base = (unsigned long *) VirtualAlloc((void *)0, reservedSize ,MEM_RESERVE,PAGE_READWRITE);
	ASSERTM("Virtual Alloc Fail", base != 0);
	local.setBase(base);
	local.setRegionBase(base);
	local.setReservedLimit(((ulong) base) + _asPointer(reservedSize));
	local.setCommitedLimit(base);
	local.setNextFree((ulong)base + _asPointer(1024 * 4));
	VirtualQuery((void *) asOop(base), queryAnswer, maxValueQuery);
	ASSERTM("memory not reserve",queryAnswer->State && MEM_RESERVE == MEM_RESERVE);
	local.grow();
	VirtualQuery((void *) asOop(base), queryAnswer, maxValueQuery);
	ASSERTM("Memory not commit",queryAnswer->State && MEM_COMMIT == MEM_COMMIT);
	ASSERTM("next free <= commited limit", local.getNextFree() <= local.getCommitedLimit());
	local.setNextFree((ulong)local.getNextFree() + _asPointer(4 * 100 + 1024) );
	local.shallowCopy(array);
// need to simulate that, somehow
//		self
//			assert: (local includes: array);
//			assert: array first = 1;
//			assert: array last = 1.
	local.setNextFree(base);
	local.decommitSlack();
	VirtualQuery((void *) asOop(base), queryAnswer, maxValueQuery);
	ASSERTM("Memory not commit",queryAnswer->State && MEM_RESERVE == MEM_RESERVE);
	VirtualFree((void *) asOop(base),0,MEM_RELEASE);
	VirtualQuery((void *) asOop(base), queryAnswer, maxValueQuery);
	ASSERTM("Memory not commit",queryAnswer->State && MEM_FREE == MEM_FREE);
	free(array);
	free(queryAnswer);
}

ulong * mockArray(){}


void growingTo() {
		ulong * array = (ulong *) malloc(4 * 1024);
		GCSpace localSpace = GCSpace::dynamicNew(1024*4*10);
		ulong * copy = localSpace.shallowCopyGrowingTo(array, 2048);
		ulong * object = localSpace.shallowCopy(stFalse);


		ASSERTM("copy is not a Array", isArray(copy));
		ASSERTM("size of copy is wrong", size(copy) == 2048);
		ASSERTM("copy is not consistent (first element diverge)", array[0] =
				copy[0]);
		//ASSERTM((copy count: [:x | x isNil]) = (copy size - array size));
		ASSERTM("The moon is red",
				oop(copy) == asOop((ulong * )(localSpace.getBase()) + 16));
		ASSERTM("I feel a perturbation in the force",
				oop(object) + 8 == (oop(copy) + (size(copy) * 4) + 16));
		free(array);

}

void newGCSpaceShallowCopy() {
		ulong * array = (ulong *) malloc(4 * 1024);
		GCSpace	space = GCSpace::dynamicNew( 1024 * 4 * 10);
		ulong * copy = space.shallowCopy(array);
		ASSERTM("copy is not a Array", isArray(copy));
				ASSERTM("size of copy is wrong", size(copy) == size(array));
				ASSERTM("first element diverge", array[0] == copy[0]);
				//	Cant touch this tooo doudoudou, doudoudou, doudoudou	assert: array = copy;
				ASSERTM("I feel a perturbation in the force",
						oop(copy) == asOop(space.getBase() + 8));
}

void shallowCopy() {
// thinks about that. shallow copy is actual. Check later.
//	testShallowCopy
//		| array copy |
//		array := #(1 2 3 4).
//		copy := self shallowCopy: array.
//		self
//			assert: copy isArray;
//			assert: copy size = array size;
//			assert: array first = copy first;
//			assert: array = copy;
//			deny: array == copy
}

void shallowCopyBytes() {

// thinks about that. shallow copy is actual. Check later.
//	testShallowCopyBytes
//		| copy string |
//		string := 'ABCDEFG'.
//		copy := self shallowCopy: string.
//		self
//			assert: copy isString;
//			assert: copy size = string size;
//			assert: copy first = string first;
//			assert: copy = string
}

void shallowCopyBytes2() {

	// thinks about that. shallow copy is actual. Check later.
//	testShallowCopyBytes2
//		| copy string |
//		string := 'ABCDEFGI'.
//		copy := self shallowCopy: string.
//		self
//			assert: copy isString;
//			assert: copy size = string size;
//			assert: copy first = string first;
//			assert: copy = string
}

void shallowCopyBytes3() {

	// thinks about that. shallow copy is actual. Check later.
//	testShallowCopyBytes3
//		| copy string |
//		string := 'A'.
//		copy := self shallowCopy: string.
//		self
//			assert: copy isString;
//			assert: copy size = string size;
//			assert: copy first = string first;
//			assert: copy = string
}

void shallowCopyBytesExtended() {

	// thinks about that. shallow copy is actual. Check later.
//	testShallowCopyBytesExtended
//		| copy string |
//		string := String new: 1027 withAll: $A.
//		copy := self shallowCopy: string.
//		self
//			assert: copy isString;
//			assert: copy size = string size;
//			assert: copy first = string first;
//			assert: copy = string
}

void shallowCopyExtended() {

// thinks about that. shallow copy is actual. Check later.
//	testShallowCopyExtended
//		| array copy |
//		array := Array new: 1024 withAll: true.
//		copy := self shallowCopy: array.
//		self
//			assert: copy isArray;
//			assert: copy size = array size;
//			assert: array first = copy first;
//			assert: array = copy
}

void synchronousGCSpace() {

	// thinks about that. shallow copy is actual. Check later.
//	testSynchronousGCSpace
//		| space oldNextFree array |
//		space := SynchronousGCSpace currentFrom.
//		self
//			assert: space base < space nextFree;
//			assert: space base < space commitedLimit;
//			assert: space commitedLimit <= space reservedLimit;
//			assert: (space softLimit between: space base and: space reservedLimit);
//			assert: (space nextFree between: space base and: space commitedLimit).
//		oldNextFree := space nextFree.
//		array := Array new: 20.
//		self deny: oldNextFree = space nextFree
}

cute::suite make_suite_GCSpaceTest() {
	cute::suite s;
	//s.push_back(CUTE(extendedGrowingTo));
	//s.push_back(CUTE(grow));
	//s.push_back(CUTE(growingTo));
	//s.push_back(CUTE(newGCSpaceShallowCopy));
	//s.push_back(CUTE(shallowCopy));
	//s.push_back(CUTE(shallowCopyBytes));
	//s.push_back(CUTE(shallowCopyBytes2));
	//s.push_back(CUTE(shallowCopyBytes3));
	//s.push_back(CUTE(shallowCopyBytesExtended));
	//s.push_back(CUTE(shallowCopyExtended));
	//s.push_back(CUTE(synchronousGCSpace));
	return s;
}

