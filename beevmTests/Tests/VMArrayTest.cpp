#include "../DataStructures/VMArray.h"
#include "cute.h"
#include "cute_suite.h"
extern cute::suite make_suite_VMArrayTest();

using namespace Bee;

// File myclassTest.h

void add() {
	unsigned long emptyArray[1024];
	VMArray array; // = VMArray new;
	array.emptyWith(emptyArray);
	array.reset();
	ASSERTM("Initial Size is wrong", array.size() == 0);
	array.add(1);
	array.add(2);
	array.add(3);
	ASSERTM("Size is wrong", array.size() == 3);
	ASSERTM("NextFree is wrong", array.nextFree() == 4);
	ASSERTM("Should not be empty", !array.isEmpty());
}

void arrayGrow() {
	VMArray array; // = VMArray new;
	array.reset();
	array.add(1);
	array.add(2);
	array.add(3);
	ASSERTM("Size is wrong", array.size() == 3);
	ASSERTM("NextFree is wrong", array.nextFree() == 4);
	ASSERTM("Should not be empty", !array.isEmpty());
	array.add(1);
	array.add(2);
	array.add(3);
	array.add(1);
	array.add(2);
	array.add(3);
	array.add(3);
	array.add(3);
	array.add(1);
	array.add(2);
	ASSERTM("Size is wrong", array.size() == 13);
	ASSERTM("raw size is wrong", array.maxSize == 20);
	array.add(1);
	array.add(2);
	array.add(3);
	array.add(1);
	array.add(2);
	array.add(3);
	array.add(3);
	array.add(3);
	array.add(1);
	array.add(2);
	ASSERTM("raw size is wrong", array.maxSize == 30);
}

void addAll() {
	unsigned long emptyArray[1024];
	unsigned long anotherArray[3];
	anotherArray[0] = 5;
	anotherArray[1] = 6;
	anotherArray[2] = 7;
	VMArray array; // = VMArray new;
	array.emptyWith(emptyArray);
	array.reset();
	ASSERTM("Initial Size is wrong", array.size() == 0);
	array.add(2);
	array.add(3);
	array.add(4);
	array.addAll(anotherArray, 3);
	ASSERTM("Size is wrong", array.size() == 6);
	ASSERTM("NextFree is wrong", array.nextFree() == 7);
	ASSERTM("Should not be empty", !array.isEmpty());
	unsigned long answer = 1;
	for (int index = 1; index <= array.size(); index++) {
		answer = answer * (unsigned long) array[index];
	}
	ASSERTM("loop result should be 5040 ", answer == 5040);
	ASSERTM("First pop should be 7", array.pop() == 7);
	ASSERTM("First pop should be 6", array.pop() == 6);
	ASSERTM("First pop should be 5", array.pop() == 5);
	ASSERTM("Should not be empty", !array.isEmpty());
}

void letDo() {
	unsigned long emptyArray[1024];
	VMArray array; // = VMArray new;
	array.emptyWith(emptyArray);
	array.reset();
	ASSERTM("Initial Size is wrong", array.size() == 0);
	array.add(2);
	array.add(3);
	array.add(4);
	ASSERTM("Size is wrong", array.size() == 3);
	ASSERTM("NextFree is wrong", array.nextFree() == 4);
	ASSERTM("Should not be empty", !array.isEmpty());
	unsigned long answer = 1;
	for (int index = 1; index <= array.size(); index++) {
		answer = answer * (unsigned long) array[index];
	}
	ASSERTM("loop result should be 2"
			" ", answer == 24);
	ASSERTM("First pop should be 4", array.pop() == 4);
	ASSERTM("First pop should be 3", array.pop() == 3);
	ASSERTM("First pop should be 2", array.pop() == 2);
	ASSERTM("Should be empty", array.isEmpty());
}

void vmArrayGrow() {
//testGrow
//	| space array |
//	space := GCSpace dynamicNew: 1024 * 4 * 10.
//	array := VMArray new on: space; yourself.
//	self
//		execute: [:vmArray | | size |
//			vmArray emptyReserving: 100.
//			size := vmArray contents size.
//			self assert: size >= 100.
//			99 timesRepeat: [vmArray push: 1].
//			self assert: vmArray contents size = size.
//			vmArray push: 1.
//			self
//				assert: vmArray contents size > 100;
//				assert: vmArray contents size > size]
//		proxying: array
}

void pop() {
	unsigned long emptyArray[1024];
	VMArray array; // = VMArray new;
	array.emptyWith(emptyArray);
	array.reset();
	ASSERTM("Initial Size is wrong", array.size() == 0);
	array.add(1);
	array.add(2);
	array.add(3);
	ASSERTM("Size is wrong", array.size() == 3);
	ASSERTM("NextFree is wrong", array.nextFree() == 4);
	ASSERTM("Should not be empty", !array.isEmpty());
	ASSERTM("First pop should be 3", array.pop() == 3);
	ASSERTM("First pop should be 2", array.pop() == 2);
	ASSERTM("First pop should be 1", array.pop() == 1);
	ASSERTM("Should be empty", array.isEmpty());
}

void reset() {
	unsigned long emptyArray[1024];
	unsigned long anotherArray[3];
	anotherArray[0] = 5;
	anotherArray[1] = 6;
	anotherArray[2] = 7;
	VMArray array; // = VMArray new;
	array.emptyWith(emptyArray);
	array.reset();
	ASSERTM("Initial Size is wrong", array.size() == 0);
	array.add(2);
	array.add(3);
	array.add(4);
	array.addAll(anotherArray, 3);
	ASSERTM("Size is wrong", array.size() == 6);
	ASSERTM("NextFree is wrong", array.nextFree() == 7);
	ASSERTM("Should not be empty here", !array.isEmpty());
	unsigned long answer = 1;
	for (int index = 1; index <= array.size(); index++) {
		answer = answer * (unsigned long) array[index];
	}
	ASSERTM("loop result should be 5040 ", answer == 5040);
	ASSERTM("First pop should be 7", array.pop() == 7);
	ASSERTM("First pop should be 6", array.pop() == 6);
	ASSERTM("First pop should be 5", array.pop() == 5);
	ASSERTM("Should not be empty", !array.isEmpty());
	array.reset();
	ASSERTM("Should be empty after reset", array.isEmpty());
}

void updateReference() {
//	testUpdateReference
//		| space array referer size |
//		space := GCSpace dynamicNew: 1024 * 4 * 10.
//		array := ReferencedVMArray new on: space.
//		referer := Array new: 1.
//		size := array contents size.
//		self
//			execute: [:vmArray |
//				vmArray emptyReserving: 100.
//				referer at: 1 put: vmArray contents.
//				vmArray contents: nil; referer: referer oop.
//				self assert: vmArray contents == referer first.
//				size timesRepeat: [vmArray push: 1]]
//			proxying: array.
//		self
//			assert: array contents size > size;
//			assert: array contents == referer first
}

cute::suite make_suite_VMArrayTest() {
	cute::suite s;
	s.push_back(CUTE(arrayGrow));
	s.push_back(CUTE(add));
	s.push_back(CUTE(addAll));
	s.push_back(CUTE(letDo));
	s.push_back(CUTE(pop));
	s.push_back(CUTE(reset));
	s.push_back(CUTE(vmArrayGrow));
	s.push_back(CUTE(updateReference));
	return s;
}

