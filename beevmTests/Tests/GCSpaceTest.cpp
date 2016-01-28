#include "../DataStructures/GCSpace.h"
#include "../DataStructures/Memory.h"
#include "cute.h"
#include "cute_suite.h"
#include "Windows.h"
#include <iostream>

extern cute::suite make_suite_GCSpaceTest();

using namespace Bee;
using namespace std;

// File myclassTest.h

void allocate() {

	GCSpace localSpace = GCSpace::dynamicNew(1024 * 4 * 10);
	ASSERTM("init wrong", localSpace.getNextFree() == localSpace.getBase());
	ulong * address = localSpace.allocate(1024);
	address = localSpace.allocate(1024);
	ASSERTM("nextFree simple",
			((ulong )localSpace.getNextFree())
					== ((ulong )localSpace.getBase() + 1024 + 1024));
	address = localSpace.allocate(1024 * 38);
	ASSERTM("nextFree limit",
			((ulong )localSpace.getNextFree())
					== ((ulong )localSpace.getBase() + (1024 * 40)));
	address = localSpace.allocate(1024 * 30);
	Memory::current()->releaseEverything();
}

void extendedGrowingTo() {

	mockVMValue();
	unsigned long * array = mockArray1024();
	unsigned long * stTrue = mockTrue();
	GCSpace localSpace = GCSpace::dynamicNew(1024 * 4 * 10);
	ulong * copy = localSpace.shallowCopyGrowingTo( array, 2048);
	ulong * object = localSpace.shallowCopy(stTrue);
	ASSERTM("copy is not a Array", isArray(copy));
	ASSERTM("size of copy is wrong", _size(copy) == 2048);
	ASSERTM("copy is not consistent (first element diverge)",
			array[0] == copy[0]);
	ASSERTM("copy is not consistent (last element diverge)",
			array[1023] == copy[1023]);

	ASSERTM("value not correctly copied" ,checkValueMockArray1024(copy));
//	ASSERTM("The moon is red",
//						(ulong) copy == (ulong)((ulong) localSpace.getBase()) * 2 + 16);
//	ASSERTM("I feel a perturbation in the force",
//			_oop(object) + 8 == (_oop(copy) + (size(copy) * 4) + 16));
	//free(array);

	Memory::current()->releaseEverything();
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
			(space.getSoftLimit() <= space.getBase())
					&& (space.getSoftLimit() >= space.getReservedLimit()));
	ASSERTM("nextFree between base and commitedLimit",
			(space.getNextFree() <= space.getBase())
					&& (space.getNextFree() >= space.getCommitedLimit()));
	ulong * oldNextFree = space.getNextFree();
	//mockArray();
	ASSERTM("Maybe should be test only from Image",
			oldNextFree == space.getNextFree());
	Memory::current()->releaseEverything();
}

void grow() {
	unsigned long * array;
	array = (ulong *) malloc(100);
	GCSpace local = GCSpace();
	unsigned long reservedSize = 4 * 1024 * 100;
	unsigned long maxValueQuery = 28;
	PMEMORY_BASIC_INFORMATION queryAnswer = (PMEMORY_BASIC_INFORMATION) malloc(
			maxValueQuery);
	unsigned long * base = (unsigned long *) VirtualAlloc((void *) 0,
			reservedSize, MEM_RESERVE, PAGE_READWRITE);
	ASSERTM("Virtual Alloc Fail", base != 0);
	// TODO change small Pointer
	local.setBase(base);
	local.setRegionBase(base);
	local.setReservedLimit(((ulong) base) + _asPointer(reservedSize));
	local.setCommitedLimit(base);
	local.setNextFree((ulong) base + _asPointer(1024 * 4));

	VirtualQuery((void *) _asOop(base), queryAnswer, maxValueQuery);
	ASSERTM("memory not reserve",
			queryAnswer->State && MEM_RESERVE == MEM_RESERVE);

	local.grow();
	VirtualQuery((void *) _asOop(base), queryAnswer, maxValueQuery);
	ASSERTM("Memory not commit",
			queryAnswer->State && MEM_COMMIT == MEM_COMMIT);
//	ASSERTM("next free <= commited limit",
//			local.getNextFree() <= local.getCommitedLimit());
	local.setNextFree((ulong) local.getNextFree() + _asPointer(4 * 100 + 1024));
	local.shallowCopy(array);
// need to simulate that, somehow
//		self
//			assert: (local includes: array);
//			assert: array first = 1;
//			assert: array last = 1.
	local.setNextFree(base);
	local.decommitSlack();
	VirtualQuery((void *) _asOop(base), queryAnswer, maxValueQuery);
	ASSERTM("Memory not commit",
			queryAnswer->State && MEM_RESERVE == MEM_RESERVE);
	VirtualFree((void *) _asOop(base), 0, MEM_RELEASE);
	VirtualQuery((void *) _asOop(base), queryAnswer, maxValueQuery);
	ASSERTM("Memory not commit", queryAnswer->State && MEM_FREE == MEM_FREE);
	free(array);
	free(queryAnswer);
}

void growingTo() {
	unsigned long * array = mockArray();
	GCSpace localSpace = GCSpace::dynamicNew(1024 * 4 * 10);
	ulong * copy = localSpace.shallowCopyGrowingTo((unsigned long *) array,
			2048);
	ulong * object = localSpace.shallowCopy(stFalse);

	ASSERTM("copy is not a Array", isArray(copy));
	ASSERTM("size of copy is wrong", size(copy) == 2048);
	ASSERTM("copy is not consistent (first element diverge)", array[0] =
			copy[0]);
	//ASSERTM((copy count: [:x | x isNil]) = (copy size - array size));
//	ASSERTM("The moon is red",
//			_oop(copy) == _asOop((ulong * )(localSpace.getBase()) + 16));
//	ASSERTM("I feel a perturbation in the force",
//			_oop(object) + 8 == (_oop(copy) + (size(copy) * 4) + 16));
	//free(array);
	Memory::current()->releaseEverything();
}

void newGCSpaceShallowCopy() {
	ulong * array = (ulong *) mockArray1024();
	GCSpace space = GCSpace::dynamicNew(1024 * 4 * 10);
	ulong * copy = space.shallowCopy(array);
	ASSERTM("copy is not a Array", isArray(copy));
	ASSERTM("size of copy is wrong", size(copy) == size(array));
	ASSERTM("first element diverge", array[0] == copy[0]);
	//	Cant touch this tooo doudoudou, doudoudou, doudoudou	assert: array = copy;
//	ASSERTM("I feel a perturbation in the force",
//			_oop(copy) == _asOop(space.getBase() + 8));
}

void shallowCopy() {
	ulong * array = (ulong *) mockArray();
	GCSpace space = GCSpace::dynamicNew(1024 * 4 * 10);
	ulong * copy = space.shallowCopy(array);
	ASSERTM("the copy not array", isArray(copy));
	ASSERTM("size", size(copy) == size(array));
	ASSERTM("first element", array[0] == copy[0]);
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
	s.push_back(CUTE(allocate));
	s.push_back(CUTE(gcspace));
	s.push_back(CUTE(extendedGrowingTo));
//	s.push_back(CUTE(grow));
//	s.push_back(CUTE(growingTo));
//	s.push_back(CUTE(newGCSpaceShallowCopy));
//	s.push_back(CUTE(shallowCopy));
	//s.push_back(CUTE(shallowCopyBytes));
	//s.push_back(CUTE(shallowCopyBytes2));
	//s.push_back(CUTE(shallowCopyBytes3));
	//s.push_back(CUTE(shallowCopyBytesExtended));
	//s.push_back(CUTE(shallowCopyExtended));
	//s.push_back(CUTE(synchronousGCSpace));
	return s;
}

