/*
 * GCSpaceInfo.cpp
 *
 *  Created on: 4 janv. 2016
 *      Author: jbapt
 */

#include "GCSpaceInfo.h"
#include "stdlib.h"

using namespace Bee;

GCSpaceInfo::GCSpaceInfo(){
}



unsigned short GCSpaceInfo::shortAt(ulong offset) {
	return (contents[offset]);
}

ulong * GCSpaceInfo::at(ulong offset) {
	ulong* position = (ulong *)&contents[offset];
	return (ulong*)((*position)>>1);
}

void GCSpaceInfo::atPut(ulong offset, ulong * value) {
	ulong* position = (ulong *)&contents[offset];
	*position = ((ulong)value)<<1;
}


//ulong * GCSpaceInfo::at(ulong offset) {
//	return (ulong *)(((contents[offset] / 2))
//			+ ((contents[offset + 1] << 15)));
//}
//
//void GCSpaceInfo::atPut(ulong offset, ulong * pointer) {
//	ulong vPointer = (ulong) pointer;
//	contents[offset] = (unsigned short) ((vPointer & 0x7FFF) * 2);
//	contents[offset + 1] = (unsigned short)(vPointer >> 15);
//}

ulong * GCSpaceInfo::base() {
	return this->at(4);
}
ulong * GCSpaceInfo::base_3() {
	return this->at(16);
}
ulong * GCSpaceInfo::commitedLimit() {
	return this->at(12);
}
ulong * GCSpaceInfo::nextFree() {
	return this->at(4);
}
ulong * GCSpaceInfo::reservedLimit() {
	return this->at(20);
}
ulong * GCSpaceInfo::softLimit() {
	return this->at(0);
}

void GCSpaceInfo::base(ulong * value) {
	this->atPut(4, value);
}

void GCSpaceInfo::base_3(ulong * value) {
	this->atPut(16, value);
}
void GCSpaceInfo::commitedLimit(ulong * value) {
	this->atPut(12, value);
}
void GCSpaceInfo::nextFree(ulong * value) {
	this->atPut(8, value);
}

void GCSpaceInfo::reservedLimit(ulong * value) {
	this->atPut(20, value);
}
void GCSpaceInfo::softLimit(ulong * value) {
	this->atPut(0, value);
}

void GCSpaceInfo::setContents(ulong * value){
	contents = (unsigned char *) value;
}

void GCSpaceInfo::freeMock(){
	free(contents);

}

GCSpaceInfo GCSpaceInfo::newSize(ulong size) {
	GCSpaceInfo returnValue = GCSpaceInfo();
	returnValue.setContents((ulong *) malloc(size));
	return returnValue;
}

GCSpaceInfo GCSpaceInfo::currentTo() {
	GCSpaceInfo returnValue = GCSpaceInfo();
	returnValue.setContents((ulong *) 0x100416C8);
	return returnValue;
}
GCSpaceInfo GCSpaceInfo::currentFrom() {
	GCSpaceInfo returnValue = GCSpaceInfo();
	returnValue.setContents((ulong *) 0x100416B0);
	return returnValue;
}
GCSpaceInfo GCSpaceInfo::old() {
	GCSpaceInfo returnValue = GCSpaceInfo();
	returnValue.setContents((ulong *) 0x100406B0);
	return returnValue;
}

GCSpaceInfo GCSpaceInfo::mockForTest() {
	GCSpaceInfo returnValue = GCSpaceInfo();
	returnValue.setContents((ulong *) malloc(100));
	return returnValue;
}



