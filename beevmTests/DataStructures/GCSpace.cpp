/*
 * GCSpace.cpp
 *
 *  Created on: 18 d�c. 2015
 *      Author: Arnaud Jean-Baptiste
 */
extern unsigned long *nil;

#include "GCSpace.h"
#include "GCSpaceInfo.h"
#include "stdlib.h"
using namespace Bee;

GCSpace::GCSpace() {
}

ulong * GCSpace::getBase() {
	return base;
}
ulong * GCSpace::getNextFree() {
	return nextFree;
}
ulong * GCSpace::getCommitedLimit() {
	return commitedLimit;
}
ulong * GCSpace::getReservedLimit() {
	return reservedLimit;
}
ulong * GCSpace::getSoftLimit() {
	return softLimit;
}

void GCSpace::setBase(ulong * localBase) {
	base = localBase;
}

void GCSpace::setRegionBase(ulong * localRegionBase) {
	regionBase = localRegionBase;
}

void GCSpace::setNextFree(ulong * localNextFree) {
	nextFree = localNextFree;
}

void GCSpace::setCommitedLimit(ulong * localCommitedLimit) {
	 commitedLimit = localCommitedLimit;

}

void GCSpace::setReservedLimit(ulong * localReservedLimit) {
	 reservedLimit = localReservedLimit;
}

void GCSpace::setSoftLimit(ulong * localSoftLimit) {
	 softLimit = localSoftLimit;
}

void GCSpace::setInfo(GCSpaceInfo * localInfo) {
	info = localInfo;

}

void GCSpace::load() {
	regionBase = (ulong *)info->base();
	base = regionBase;
	base_3 = (ulong *)info->base_3();
	commitedLimit = (ulong *)info->commitedLimit();
	nextFree = (ulong *)info->nextFree();
	reservedLimit = (ulong *)info->reservedLimit();
	softLimit = (ulong *)info->softLimit();
}

void GCSpace::save() {
	info->base(base);
	info->base_3(base_3);
	info->commitedLimit(commitedLimit);
	info->nextFree(nextFree);
	info->reservedLimit(reservedLimit);
	info->softLimit(softLimit);
}


GCSpace GCSpace::currentFrom() {
	GCSpace space = GCSpace();
		GCSpaceInfo info = GCSpaceInfo::currentFrom();
		space.setInfo(&info);
		return space;
}



GCSpace GCSpace::dynamicNew(unsigned long size) {
	GCSpace space = GCSpace();
	GCSpaceInfo info = GCSpaceInfo::newSize(size);
	space.setInfo(&info);
	return space;
}

void GCSpace::decommitSlack(){
	// -0x1000 === (long) 0xFFFFFC18
	long limit = (long) (this->getNextFree() + (_asPointer(0xFFF) &&  _asPointer(0xFFFFFC18)));
	long delta = (long)this->getCommitedLimit() - limit;
	if (delta < 0) this->grow();
	if (delta > 0) {
		_decommit(limit,delta);
		this->setCommitedLimit((ulong *)limit);}
}

ulong * GCSpace::shallowCopy(ulong * object) {
	ulong size = _sizeInBytes(object);
	ulong headerSize = _headerSizeInBytes(object);
	ulong * allocation = this->allocate(headerSize + size);
	ulong * objectPointer = _asPointer((ulong) allocation + headerSize);
	ulong * answer = (ulong *) _asObject(objectPointer);
	if (!(answer == object)) {
		for (ulong index = 1 - (headerSize / 4); index <= size; index++) {
			_basicAtPut(answer, index, (_basicAt(object, index)));
		}
		_beNotInRememberedSet(answer);

	}
	return answer;
}

ulong * GCSpace::allocate(ulong size) {
	return (ulong *) malloc(size);

}

ulong * GCSpace::shallowCopyGrowingTo(ulong * array, ulong newSize) {
	ulong headerSize = 4;
	ulong total = headerSize + newSize * 4;
	ulong * buffer = this->allocate(total);
	ulong * copy = (ulong *)_asObject(_asPointer((ulong) buffer + 16));
	for (int index = -1; index <= _size(array); index++) {
		_basicAtPut(copy, index, (_basicAt(array, index)));
	}
	_beExtended(copy);
	_basicSetSize(copy, 4);
	_extendedSize(copy, newSize);
	_basicAtPut(copy, -3, _basicAt(array, -1));

	for (int index = (_size(array) + 1); index <= newSize; index++) {
		_basicAtPut(copy, index, (ulong) nil);
	}
	_beNotInRememberedSet(copy);
	return copy;
}

void GCSpace::grow(){
//	hostVMGrow
//		| newLimit padding |
//		nextFree := self nextFree.
//		nextFree <= self softLimit ifTrue: [^self].
//		nextFree >= self reservedLimit ifTrue: [self _halt].
//		padding := false
//			ifTrue: [self somethingWithPadding + 16rFFF _asPointer
//				bitAnd: -16r1000 _asPointer]
//			ifFalse: [16r8000 _asPointer].
//		newLimit := nextFree + padding bitAnd: -16r1000 _asPointer.
//		regionBase := self regionBase.
//		(regionBase _commit: newLimit - regionBase)
//			== (regionBase bitAnd: -16r1000 _asPointer)
//			ifFalse: [self assert: false]
//			ifTrue: [self commitedLimit: newLimit]
}


