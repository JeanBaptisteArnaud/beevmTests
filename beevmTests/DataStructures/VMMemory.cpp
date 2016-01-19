/*
 * VMMemory.cpp
 *
 *  Created on: 6 janv. 2016
 *      Author: jbapt
 */
#include "VMMemory.h"
#include "Windows.h"
#include <limits.h>

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

unsigned char ObjectFlag_reserved1 = 1;
unsigned char ObjectFlag_generation = 2;
unsigned char ObjectFlag_isEphemeron = 4;
unsigned char ObjectFlag_isInRememberSet = 8;
unsigned char ObjectFlag_isBytes = 0x10;
unsigned char ObjectFlag_zeroTermOrNamed = 0x20;
unsigned char ObjectFlag_notIndexed = 0x40;
unsigned char ObjectFlag_isExtended = 0x80;
unsigned char ObjectFlagMaxValue = 0xFF;

unsigned long ObjectExtendedShortSize = -15;
unsigned long ObjectSize = -7;
unsigned long ObjectExtendedFlags = -12;
unsigned long ObjectExtendedHash = -15;
unsigned long ObjectExtendedSize = -2;
unsigned long ObjectHash = -7;
unsigned long ObjectFlags = -4;
unsigned long ObjectHeaderBits = -1;

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

unsigned char * mockTrue() {
	// true 00 00 00 61 A0 6B 0B 0A
	//true_hdr Object <0, 0, ObjectFlag_reserved1 or ObjectFlag_zeroTermOrNamed or ObjectFlag_notIndexed,
	// offset unk_A0B6BA0>
	unsigned char * localTrue = (unsigned char *) malloc(11);

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

	return &localTrue[8];
}

unsigned char * mockArray() {

	// 03 00 00 01  D8 71 85 02

	// Array new: 3
	// 03 F8 37 01 F0 92 07 0A
	// 12:10means Object <3, 37F8h, ObjectFlag_reserved1, offset off_A0792F0>
	// 12:10has 3 slots

	unsigned char * localArray = (unsigned char *) malloc(8 + 4);
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

	return &localArray[8];
}

unsigned char * mockArray2() {

	// 03 00 00 01  D8 71 85 02
	// 12:10means Object <3, 37F8h, ObjectFlag_reserved1, offset off_A0792F0>
	// 12:10has 3 slots
	//
	unsigned char * stArray = (unsigned char *) malloc(8 + 4);
	stArray[0] = (unsigned char) 0x03;
	stArray[1] = (unsigned char) 0x00;
	stArray[2] = (unsigned char) 0x00;
	stArray[3] = (unsigned char) 0x01;
	stArray[4] = (unsigned char) 0xF0;
	stArray[5] = (unsigned char) 0x92;
	stArray[6] = (unsigned char) 0x07;
	stArray[7] = (unsigned char) 0x0A;
	stArray[8] = (unsigned char) 0xFF;
	stArray[9] = (unsigned char) 0xFF;
	stArray[10] = (unsigned char) 0xFF;
	stArray[11] = (unsigned char) 0xFF;

	return &stArray[8];
}

unsigned char * mockArray1024() {

	// 03 00 00 01  D8 71 85 02
	unsigned char * localArray = (unsigned char *) malloc(16 + (4 * 1024));
	// Array new: 3
	// 03 F8 37 01 F0 92 07 0A
	// 12:10means Object <3, 37F8h, ObjectFlag_reserved1, offset off_A0792F0>
	// 12:10has 3 slots
	localArray[0] = (unsigned char) 0x04;
	localArray[1] = (unsigned char) 0xF8;
	localArray[2] = (unsigned char) 0x37;
	localArray[3] = (unsigned char) 0x81;

	localArray[4] = (unsigned char) 0x00;
	localArray[5] = (unsigned char) 0x04;
	localArray[6] = (unsigned char) 0x00;
	localArray[7] = (unsigned char) 0x00;

	localArray[8] = (unsigned char) 0x04;
	localArray[9] = (unsigned char) 0xF8;
	localArray[10] = (unsigned char) 0x37;
	localArray[11] = (unsigned char) 0x81;

	localArray[12] = (unsigned char) 0xF0;
	localArray[13] = (unsigned char) 0x92;
	localArray[14] = (unsigned char) 0x07;
	localArray[15] = (unsigned char) 0x0A;


	for (int index = 4; index <= 1024; index++) {
		localArray[index * 4] = (unsigned char) (index << 1);
		localArray[(index * 4) + 1] = (unsigned char) 0x00;
		localArray[(index * 4) + 2] = (unsigned char) 0x00;
		localArray[(index * 4) + 3] = (unsigned char) 0x00;
	}

	return &localArray[16];
}


void freeSimpleObject(unsigned char * object) {
	free(&object[-8]);
}

void freeComplexObject(unsigned char * object) {
	free(&object[-16]);
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

void _setExtendedSize(ulong * object, ulong size) {
	big_header_t * h = BIG_HEADER_OF(*object);
	h->size = size;
}

ulong _getExtendedSize(ulong *object) {
	big_header_t * h = BIG_HEADER_OF(*object);
	return (ulong) h->size;
}

ulong _size(ulong *object) {
	if (_isExtended(object))
		return _getExtendedSize(object);
	else
		return _basicGetSize(object);

}

ulong _asObject(ulong * object) {
	return (ulong) object >> 1;
}

ulong * _asPointer(ulong object) {
	if (object & 1) {
		return (ulong *) (object >> 1 | 1);
	} else {
		return (ulong *) (object | 1);
	}
}

void _decommit(ulong limit, ulong delta) {
	VirtualFree((void *) limit, delta, MEM_DECOMMIT);
}

void _commit(ulong limit, ulong delta) {
	(ulong *) VirtualAlloc((void *) limit, delta, MEM_COMMIT, PAGE_READWRITE);
}

// flags Tests

bool testFlags(ulong *object, unsigned char flag) {
	header_t * h = HEADER_OF(*object);
	return (h->flags & (flag)) == flag;
}

void setFlags(ulong *object, unsigned char flag) {
	header_t * h = HEADER_OF(*object);
	h->flags = (unsigned char) (h->flags | flag);
}

void unsetFlags(ulong *object, unsigned char flag) {
	header_t * h = HEADER_OF(*object);
	h->flags = (h->flags & ((flag) ^ ObjectFlagMaxValue));
}

void _beExtended(ulong *object) {
	setFlags(object, ObjectFlag_isExtended);
}

bool _isBytes(ulong *object) {
	return testFlags(object, ObjectFlag_isBytes);
}

bool _isExtended(ulong *object) {
	return testFlags(object, ObjectFlag_isExtended);
}

bool _isZeroTerminated(ulong *object) {
	return testFlags(object, ObjectFlag_zeroTermOrNamed);
}

bool _isInRememberedSet(ulong *object) {
	return testFlags(object, ObjectFlag_isInRememberSet);
}

void _beNotInRememberedSet(ulong *object) {
	unsetFlags(object, ObjectFlag_isInRememberSet);
}

ulong _sizeInBytes(ulong *object) {
	if (_isBytes(object)) {
		int zero;
		if (_isZeroTerminated(object))
			zero = 1;
		else
			zero = 0;
		return ((_size(object) + 3 + zero) & -4);

	} else
		return _size(object) * 4;
}

unsigned long rotateLeft(unsigned long n, unsigned int c) {
	const unsigned int mask = (CHAR_BIT * sizeof(n) - 1);
	c &= mask;
	return (n << c) | (n >> ((-c) & mask));
}

unsigned long rotateRight(unsigned long n, unsigned int c) {
	const unsigned int mask = (CHAR_BIT * sizeof(n) - 1);
	c &= mask;
	return (n >> c) | (n << ((-c) & mask));
}

ulong _headerSizeInBytes(ulong *object) {
	if (_isExtended(object)) {
		return _basicGetSize(object) * 4;
	} else {
		return 8;
	}
}

ulong size(ulong *object) {
	int total;
	if (_isExtended(object))
		total = _getExtendedSize(object);
	else
		total = _basicGetSize(object);
	if (_isBytes(object) && _isZeroTerminated(object))
		return total - 1;
	else
		return total;
}

ulong _asOop(ulong *object) {
}

ulong _oop(ulong *object) {
}

bool _isProxy(ulong *object) {
	return !testFlags(object, ObjectFlag_reserved1);
}

void _setProxee(ulong *object, ulong value) {
	_basicAtPut(object, -1, rotateRight(value, 8));
}

ulong _getProxee(ulong *object) {
	return rotateLeft(_basicAt(object, -1), 8);
}

bool isArray(ulong *object) {

	return (_basicAt(object,0) == 0x0A0792F0);
}

unsigned long memoryAt(unsigned long pointer) {
}

void memoryAtPut(unsigned long * pointer, unsigned long value) {
}

