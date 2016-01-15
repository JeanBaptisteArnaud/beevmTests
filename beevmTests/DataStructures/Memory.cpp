/*
 * Memory.cpp
 *
 *  Created on: 14 janv. 2016
 *      Author: jbapt
 */


#include "cute.h"
#include "cute_suite.h"



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
	if ((used / total) > 0.9 || (free < size))
		{return this->acquireMoreSpace();}
	else
		{return &currentSpace;}
}


void Memory::addSpace(GCSpace *){

}

void Memory::addCollectedSpace(GCSpace *){

}

void Memory::initializeCollector() {
}

void Memory::createPinnedSpace() {
}

void Memory::startUp() {
	this->initializeCollector();
	this->createNextSpace();
	//this->acquireMoreSpace();
	//this->createPinnedSpace();
}

GCSpace * Memory::acquireMoreSpace() {
	currentSpace = nextSpace;
	this->addSpace(&nextSpace);
	this->createNextSpace();
	this->addCollectedSpace(&currentSpace);
	return &currentSpace;
}


GCSpaceInfo Memory::allocateWithoutFinalization(ulong size){
	ulong * address = (ulong *) VirtualAlloc((void *) nil, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	GCSpaceInfo info = GCSpaceInfo();
	ostringstream ostr;
			ostr << "base v:";
			ostr << (ulong)address / 2;
	FAILM(ostr.str());



	info.setBase((ulong *)((ulong)address / 2));
//	info.setNextFree(info.getBase());
//	info.setCommitedLimit(info.getBase() + (size / 2));
	return info;
}

Memory * Memory::current() {
	if (singleton == 0)
		singleton = new Memory();
		singleton->startUp();
	return singleton;
}

GCSpace * Memory::createNextSpace() {
	nextSpace = GCSpace();
	nextSpace.setInfo(this->allocateWithoutFinalization(64 * 1024 * 1024));
//	nextSpace.load();
//	nextSpace.setSoftLimit((ulong *) nextSpace.getSoftLimit() - (64 * 1024 / 2));
}