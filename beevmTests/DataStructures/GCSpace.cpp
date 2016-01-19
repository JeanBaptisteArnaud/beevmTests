/*
 * GCSpace.cpp
 *
 *  Created on: 18 d�c. 2015
 *      Author: Arnaud Jean-Baptiste
 */
extern unsigned long *nil;

#include "GCSpace.h"
#include "GCSpaceInfo.h"

#include "Memory.h"
#include "stdlib.h"
#include <iostream>

using namespace Bee;

using namespace std;

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

void GCSpace::release(){
	_decommit((ulong) base,(ulong) commitedLimit);
}

void GCSpace::setInfo(GCSpaceInfo localInfo) {
	info = localInfo;
	pinfo = &info;
}

void GCSpace::load() {
	regionBase = (ulong *) info.getBase();
	base = regionBase;
	base_3 = (ulong *) info.getBase_3();
	commitedLimit = (ulong *) info.getCommitedLimit();
	nextFree = (ulong *) info.getNextFree();
	reservedLimit = (ulong *) info.getReservedLimit();
	softLimit = (ulong *) info.getSoftLimit();
}

ulong GCSpace::size() {
	return (ulong) reservedLimit - (ulong) base;
}

ulong GCSpace::used() {
	return (ulong) nextFree - (ulong) base;
}

void GCSpace::save() {
	info.setBase(base);
	info.setBase_3(base_3);
	info.setCommitedLimit(commitedLimit);
	info.setNextFree(nextFree);
	info.setReservedLimit(reservedLimit);
	info.setSoftLimit(softLimit);
}

void GCSpace::debug(){
	cerr << "regionBase:" << regionBase << endl;
	cerr << "Base:" << base << endl;
	cerr << "Base_3:" << base_3 << endl;
	cerr << "commitedLimit:" << commitedLimit << endl;
	cerr << "nextFree:" << nextFree << endl;
	cerr << "reservedLimit:" << reservedLimit << endl;
	cerr << "softLimit:" << softLimit  << endl;
}



GCSpace GCSpace::currentFrom() {
	GCSpace space = GCSpace();
	GCSpaceInfo info = GCSpaceInfo::currentFrom();
	space.setInfo(info);
	return space;
}

GCSpace GCSpace::dynamicNew(unsigned long size) {
	GCSpace space = GCSpace();
	GCSpaceInfo info = Memory::current()->allocateWithoutFinalization(size);
	space.setInfo(info);
	space.load();
	return space;
}

void GCSpace::decommitSlack() {
// -0x1000 === (long) 0xFFFFFC18
	long limit = (long) (this->getNextFree()
			+ (_asPointer(0xFFF) && _asPointer(0xFFFFFC18)));
	long delta = (long) this->getCommitedLimit() - limit;
	if (delta < 0)
		this->grow();
	if (delta > 0) {
		_decommit(limit, delta);
		this->setCommitedLimit((ulong *) limit);
	}
}

ulong * GCSpace::obtainFreeSpaceAndAllocate(ulong size) {
	this->dispenseReservedSpace();
	this->garbageCollect();
	GCSpace * space = Memory::current()->growIfNeeded(size);
	return space->allocate(size);
}

void GCSpace::dispenseReservedSpace() {
	this->nextFree = this->commitedLimit;
}

ulong * GCSpace::allocate(ulong size) {
// lockedAllocate probably do not need the mutex
	ulong answer = (ulong) this->nextFree;
	ulong next = answer + size;

	if (next > (ulong) softLimit)
		return this->obtainFreeSpaceAndAllocate(size);
	this->setNextFree((ulong *) next);
	return (ulong *) answer;
}

ulong * GCSpace::shallowCopy(ulong * object) {
// method a little bit different than in Image
	ulong headerSize = _headerSizeInBytes(object);
	ulong total = headerSize + _sizeInBytes(object);
	ulong * buffer = this->allocate(total);
	ulong * copy = (ulong *) ((ulong) buffer + 16);
	for (int index =  (1 - (headerSize / 4)); index <= (int) _size(object); index++) {
		ulong inter = _basicAt(object, index);
		_basicAtPut(copy, index, inter);
	}
	_beNotInRememberedSet(copy);
	return copy;
}


ulong * GCSpace::shallowCopyGrowingTo(ulong * array, ulong newSize) {
	ulong headerSize = 4;
	ulong total = (headerSize + newSize) * 4;
	ulong * buffer = this->allocate(total);
	ulong * copy = (ulong *) ((ulong) buffer + 16);
	for (int index = -1; index <= (int) _size(array); index++) {
		ulong inter = _basicAt(array, index);
		_basicAtPut(copy, index, inter);
	}
	_beExtended(copy);
	_basicSetSize(copy, 4);
	_setExtendedSize(copy, newSize);
	_basicAtPut(copy, -3, _basicAt(array, -1));
	for (int index = (_size(array) + 1); index <= newSize; index++) {
		_basicAtPut(copy, index, (ulong) nil);
	}
	_beNotInRememberedSet(copy);
	return copy;
}

GCSpace * GCSpace::grow() {
	return Memory::current()->acquireMoreSpace();
}

