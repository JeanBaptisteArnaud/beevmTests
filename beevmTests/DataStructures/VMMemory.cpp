/*
 * VMMemory.cpp
 *
 *  Created on: 6 janv. 2016
 *      Author: jbapt
 */
#include "VMMemory.h"

unsigned long *debugFrameMarker = (unsigned long *) 0x1001B633;
unsigned long *MEM_anyCompiledMethodInFromSpace = (unsigned long *) 0x10041710;
unsigned long *MEM_checkNewNativizedCompiledMethods =
		(unsigned long *) 0x10041714;
unsigned long *MEM_polymorphicMethodCacheReferesToNewCM =
		(unsigned long *) 0x1003EC48;
unsigned long **MEM_JIT_globalMethodCache = (unsigned long **) 0x1002EC20;
unsigned long *MEM_JIT_codeCachePointer = (unsigned long*) 0x1002E820;
unsigned long *MEM_globalFramePointerToWalkStack = (unsigned long*) 0x100407C4;
unsigned long *nil = (unsigned long*) 0x10026060;
unsigned long *stTrue = (unsigned long*) 0x10026070;
unsigned long *stFalse = (unsigned long*) 0x10026080;

unsigned long ObjectFlag_reserved1 = 1;
unsigned long ObjectFlag_generation = 2;
unsigned long ObjectFlag_isEphemeron = 4;
unsigned long ObjectFlag_isInRememberSet = 8;
unsigned long ObjectFlag_isBytes = 0x10;
unsigned long ObjectFlag_zeroTermOrNamed = 0x20;
unsigned long ObjectFlag_notIndexed = 0x40;
unsigned long ObjectFlag_isExtended = 0x80;

unsigned long memoryAt(unsigned long pointer) {
}
void memoryAtPut(unsigned long * pointer, unsigned long value) {
}

//SendInliner

unsigned char * mockNil(unsigned char * localNil) {
	// 00 E3 63 61 94 DF 91 02
	// nil 00 45 34 61 10 DC 0D 0A
	//nil_hdr Object <0, 3445h, \             ; DATA XREF: GC_full+E3?o
	// .data:10026058                                         ; sub_10010E40+33?w
	// .data:10026058         ObjectFlag_reserved1 or ObjectFlag_zeroTermOrNamed or ObjectFlag_notIndexed,\
	// .data:10026058         offset unk_A0DDC10>
	localNil[0] = (unsigned char) 0x00;
	localNil[1] = (unsigned char) 0x45;
	localNil[2] = (unsigned char) 0x34;
	localNil[3] = (unsigned char) 0x61;

	localNil[4] = (unsigned char) 0x10;
	localNil[5] = (unsigned char) 0xdc;
	localNil[6] = (unsigned char) 0x0d;
	localNil[7] = (unsigned char) 0x0a;

	localNil[8] = (unsigned char) 0xFF;
	localNil[9] = (unsigned char) 0xFF;
	localNil[10] = (unsigned char) 0xFF;
	localNil[11] = (unsigned char) 0xFF;

	return localNil;
}

unsigned char * mockTrue(unsigned char * localTrue) {
	// true 00 00 00 61 A0 6B 0B 0A
	//true_hdr Object <0, 0, ObjectFlag_reserved1 or ObjectFlag_zeroTermOrNamed or ObjectFlag_notIndexed,
	// offset unk_A0B6BA0>
	localTrue[0] = (unsigned char) 0x00;
	localTrue[1] = (unsigned char) 0x00;
	localTrue[2] = (unsigned char) 0x00;
	localTrue[3] = (unsigned char) 0x61;

	localTrue[4] = (unsigned char) 0xA0;
	localTrue[5] = (unsigned char) 0x6B;
	localTrue[6] = (unsigned char) 0x0B;
	localTrue[7] = (unsigned char) 0x0A;

	localTrue[8] = (unsigned char) 0xFF;
	localTrue[9] = (unsigned char) 0xFF;
	localTrue[10] = (unsigned char) 0xFF;
	localTrue[11] = (unsigned char) 0xFF;

	return localTrue;
}

unsigned char * mockArray(unsigned char * localArray) {

	// 03 00 00 01  D8 71 85 02

	// Array new: 3
	// 03 F8 37 01 F0 92 07 0A
	// 12:10means Object <3, 37F8h, ObjectFlag_reserved1, offset off_A0792F0>
	// 12:10has 3 slots
	localArray[0] = (unsigned char) 0x03;
	localArray[1] = (unsigned char) 0xF8;
	localArray[2] = (unsigned char) 0x37;
	localArray[3] = (unsigned char) 0x01;

	localArray[4] = (unsigned char) 0xF0;
	localArray[5] = (unsigned char) 0x92;
	localArray[6] = (unsigned char) 0x07;
	localArray[7] = (unsigned char) 0x0A;

	localArray[8] = (unsigned char) 0xFF;
	localArray[9] = (unsigned char) 0xFF;
	localArray[10] = (unsigned char) 0xFF;
	localArray[11] = (unsigned char) 0xFF;

	return localArray;
}

unsigned char * mockArray2(unsigned char * stArray) {

	// 03 00 00 01  D8 71 85 02
	// 12:10means Object <3, 37F8h, ObjectFlag_reserved1, offset off_A0792F0>
	// 12:10has 3 slots
	stArray[0] = (unsigned char) 0x03;
	stArray[1] = (unsigned char) 0x00;
	stArray[2] = (unsigned char) 0x00;
	stArray[3] = (unsigned char) 0x01;
	stArray[4] = (unsigned char) 0xD8;
	stArray[5] = (unsigned char) 0x71;
	stArray[6] = (unsigned char) 0x85;
	stArray[7] = (unsigned char) 0x02;
	stArray[8] = (unsigned char) 0xFF;
	stArray[9] = (unsigned char) 0xFF;
	stArray[10] = (unsigned char) 0xFF;
	stArray[11] = (unsigned char) 0xFF;

	return stArray;
}

ulong _basicAt(ulong * object, int index) {
	return object[index - 1];
}

void _basicAtPut(ulong * object, int index, ulong value) {
	object[index - 1] = value;
}

void _basicSetSize(ulong *object, ulong size) {
	header_t * h = HEADER_OF(*object);
	h->size = size;
}

ulong _basicGetSize(ulong *object) {
	header_t * h = HEADER_OF(*object);
	return (ulong) h->size;
}

ulong _size(ulong *object) {
	return _basicGetSize(object);
}

bool _isProxy(ulong *object) {
}
ulong * _proxee(ulong *object) {
}

void _beNotInRememberedSet(ulong *object) {
}
ulong _asObject(ulong * object) {
	return (ulong) object >> 1;
}

ulong * _asPointer(ulong object) {
	if (object & 1) {
		return (ulong *)(object >> 1 & 1);
	} else {
	return (ulong *)(object & 1);}
}

void _beExtended(ulong *object) {
header_t * h = HEADER_OF(*object);
	h->flags = (h->flags | ObjectFlag_isExtended);
}

ulong _decommit(ulong limit, ulong delta) {
}

ulong _extendedSize(ulong *object, ulong size) {
}

ulong _sizeInBytes(ulong *object) {
}
ulong _headerSizeInBytes(ulong *object) {
}
ulong _framePointer() {
}
bool isArray(ulong *object) {
}
ulong size(ulong *object) {
}
;
ulong asOop(ulong *object) {
}
;
ulong oop(ulong *object) {
}
;

