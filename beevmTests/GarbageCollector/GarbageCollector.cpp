/*
 * GarbageCollector.cpp
 *
 *  Created on: 18 d�c. 2015
 *      Author: Arnaud Jean-Baptiste
 */

#include "../DataStructures/VMArray.h"
#include "../DataStructures/GCSpace.h"
#include "../DataStructures/ReferencedVMArray.h"
#include "../DataStructures/VMMemory.h"
#include "GarbageCollector.h"
#include <cstddef>
#include <iostream>



// just until I fix my environment
#define NULL 0

using namespace std;
using namespace Bee;

GarbageCollector* GarbageCollector::flipper = NULL;

GarbageCollector::GarbageCollector() {
	fromSpace = GCSpace::currentFrom();
	toSpace = GCSpace::currentTo();
	oldSpace = GCSpace::old();
	residueObject = 0;
}

void GarbageCollector::collect() {

}

void GarbageCollector::rescueEphemeron(unsigned long *ephemeron) {
	this->followCountStartingAt(ephemeron, _getExtendedSize(ephemeron), 1);
	rescuedEphemerons.add((ulong) ephemeron);
	return;
}

bool GarbageCollector::followEphemeronsCollectingUnknowns() {
	bool rescan = false;
	while (!ephemerons.isEmpty()) {
		unsigned long * ephemeron = (ulong *) ephemerons.pop();
		if (!(ephemeron == nil)) {
			if (this->checkReachablePropertyOf(ephemeron)) {
				this->followCountStartingAt(ephemeron,
						_getExtendedSize(ephemeron), 1);
				rescan = true;
			} else {
				unknowns.add((ulong) ephemeron);
			}
		}
	}
	return rescan;
}

void GarbageCollector::rescueEphemerons() {
	bool rescued = false;
	VMArray aux;
	while (!ephemerons.isEmpty()) {
		if (this->followEphemeronsCollectingUnknowns()) {
			aux = ephemerons;
			ephemerons = unknowns;
			unknowns = aux;
		} else {
			for (ulong ephemeronIndex = 0; ephemeronIndex < ephemerons.size();
					ephemeronIndex++)
				this->rescueEphemeron(unknowns[ephemeronIndex]);
			rescued = true;
		}
		unknowns.reset();
	}

	if (rescued)
		this->someEphemeronsRescued();
}

void GarbageCollector::clearPolymorphicMethodCache() {
	for (int index = 1; index <= 0x4000; index++) {
		MEM_JIT_globalMethodCache[index] = nil;
	}
}

void GarbageCollector::followStack() {
	unsigned long * frame = MEM_globalFramePointerToWalkStack;
	unsigned long nextFrame = *frame;
	while (nextFrame) {
		unsigned long start;
		unsigned long size = (ulong) nextFrame - (ulong) frame / 4;
		if (frame[1] == (ulong) debugFrameMarker) {
			this->followCountStartingAt(frame, 5, 3);
			start = 9;
		} else
			start = 3;
		this->followFrameCountStartingAt(frame, size, start);
		frame = (ulong *) nextFrame;
		nextFrame = frame[0];
	}
}

void GarbageCollector::followFrameCountStartingAt(unsigned long * frame,
		unsigned long count, unsigned long start) {
	cerr << "Need to implement " << "followCountStartingAt" << endl;
//	followFrame: frame count: size startingAt: startIndex
//		| start index gapMarker callbackFrame |
//		gapMarker := 2 _asPointer _asObject.
//		start := index := startIndex.
//		[index < size] whileTrue: [| object |
//			object := frame _basicAt: index.
//			object == gapMarker ifTrue: [
//				callbackFrame := frame _basicAt: index + 1.
//				self follow: frame count: index - start startingAt: start.
//				self follow: callbackFrame count: 5 startingAt: 1.
//				index := callbackFrame _asPointer - frame _asPointer // 4 _asPointer.
//				start := (index := index + 7) + 1].
//			index := index + 1].
//		self follow: frame count: size - start + 1 startingAt: start

}

void GarbageCollector::follow(unsigned long * pointer) {
	this->rememberIfWeak(pointer);
	this->followCountStartingAt(pointer, _strongPointersSize(pointer), 0);
}

void GarbageCollector::rememberIfWeak(unsigned long * object) {
	if (_hasWeaks(object))
		this->addWeakContainer(object);
}

void GarbageCollector::addWeakContainer(unsigned long * object) {
	if (_isActiveEphemeron(object)) {
		ephemerons.add((ulong) object);
	} else
		weakContainers.add((ulong) object);
}

void GarbageCollector::fixWeakContainers() {
	for (ulong index = 1; index <= weakContainers.size(); index++) {
		this->fixReferencesOrSetTombstone(weakContainers[index]);
	}
	weakContainers.reset();
}
void GarbageCollector::tombstone(unsigned long * pointer) {
	this->residueObject = pointer;
}

void GarbageCollector::forgetNativeObjects() {

	cerr << "Need to implement " << "forgetNativeObjects" << endl;
//	rememberSet forget.
//	literalsReferences forget.
//	rescuedEphemerons forget.
//	classCheckReferences forget.
//	nativizedMethods forget.
//	residueObject := globalFramePointerToWalkStack := nil
}

void GarbageCollector::saveSpaces() {
//fromSpace save.
//oldSpace save.
//toSpace save
}
void GarbageCollector::makeRescuedEphemeronsNonWeak() {
	for (ulong index = 1; index <= rescuedEphemerons.size(); index++) {
		_haveNoWeaks(rescuedEphemerons[index]);
	}
}

void GarbageCollector::loadSpaces() {
//	fromSpace.load();
//	oldSpace.load();
//	toSpace.load();
}
;
