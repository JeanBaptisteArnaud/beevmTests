/*
 * Memory.cpp
 *
 *  Created on: 14 janv. 2016
 *      Author: jbapt
 */

#include "cute.h"
#include "cute_suite.h"

#include <iostream>

#include "Memory.h"
#include "Windows.h"

using namespace Bee;
using namespace std;

Memory::Memory() {
}

Memory * Memory::singleton = 0;

void GCSpace::garbageCollect() {
	//	^Memory current garbageCollect: self
}

GCSpace * Memory::growIfNeeded(ulong size) {
	ulong total = currentSpace.size();
	ulong used = currentSpace.used();
	long free = total - size;
	if ((used / total) > 0.9 || (free < size)) {
		return this->acquireMoreSpace();
	} else {
		return &currentSpace;
	}
}

void Memory::addSpace(GCSpace * space) {
	spaces->add((ulong) space);
}

void Memory::addCollectedSpace(GCSpace * space) {
	collectedSpaces->add((ulong) space);
}

void Memory::initializeCollector() {
}

void Memory::createPinnedSpace() {
}

void Memory::startUp() {
	this->initializeCollector();
	this->createNextSpace();
	this->acquireMoreSpace();
	this->createPinnedSpace();
}

GCSpace * Memory::acquireMoreSpace() {
	currentSpace = nextSpace;
	this->addSpace(&nextSpace);
	this->createNextSpace();
	this->addCollectedSpace(&currentSpace);
	return &currentSpace;
}

GCSpaceInfo Memory::allocateWithoutFinalization(ulong size) {
	ulong * address = (ulong *) VirtualAlloc ((void *) 0, size,
	MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!address)
		cout << GetLastError() << endl;
	GCSpaceInfo info = GCSpaceInfo();
	ulong *  limit = (ulong *) (((ulong) address + size));
	info.setBase(address);
	info.setNextFree( address);
	info.setCommitedLimit(limit);
	info.setSoftLimit(limit);
	info.setReservedLimit(limit);
	return info;
}

Memory * Memory::current() {
	if (singleton == 0) {
		singleton = new Memory();
		singleton->spaces = new VMArray();
		singleton->spaces->reset();
		singleton->collectedSpaces = new VMArray();
		singleton->collectedSpaces->reset();
		singleton->startUp();
	}
	return singleton;
}

void Memory::releaseEverything() {
	singleton = 0;
	for (int index = 1; index <= spaces->size(); index++) {
		GCSpace * localSpace = (GCSpace *) this->spaces->pop();
		localSpace->release();
	}
}

void Memory::createNextSpace() {
	nextSpace = GCSpace();
	nextSpace.setInfo(this->allocateWithoutFinalization(64 * 1024 * 1024));
	nextSpace.load();
	nextSpace.setSoftLimit(
	(ulong *) nextSpace.getSoftLimit() - (64 * 1024 / 2));
}
