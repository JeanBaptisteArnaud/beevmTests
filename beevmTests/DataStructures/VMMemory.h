/*
 * Globales.h
 *
 *  Created on: 18 d�c. 2015
 *      Author: Arnaud Jean-Baptiste
 */

#ifndef DATASTRUCTURES_VMMEMORY_H_
#define DATASTRUCTURES_VMMEMORY_H_

// this should manage all the raw access memory

#define NULL 0
#define  _strongPointersSize(pointer) 0

//#pragma pack (1)
#pragma pack (push,1)
typedef struct header_t {
	unsigned char size;
	unsigned short hash;
	unsigned char flags;
	unsigned long * behavior;
}header_t;
#pragma pack (pop)

#define HEADER_OF(object) (header_t*)(((ulong)&object)-8)

typedef unsigned long ulong;

extern unsigned long *debugFrameMarker;
extern unsigned long *MEM_anyCompiledMethodInFromSpace;
extern unsigned long *MEM_checkNewNativizedCompiledMethods;
extern unsigned long *MEM_polymorphicMethodCacheReferesToNewCM;
extern unsigned long **MEM_JIT_globalMethodCache;
extern unsigned long *MEM_JIT_codeCachePointer;
extern unsigned long *MEM_globalFramePointerToWalkStack;
extern unsigned long *nil;
extern unsigned long *stTrue;
extern unsigned long *stFalse;

extern unsigned long ObjectFlag_reserved1;
extern unsigned long ObjectFlag_generation;
extern unsigned long ObjectFlag_isEphemeron;
extern unsigned long ObjectFlag_isInRememberSet;
extern unsigned long ObjectFlag_isBytes;
extern unsigned long ObjectFlag_zeroTermOrNamed;
extern unsigned long ObjectFlag_notIndexed;
extern unsigned long ObjectFlag_isExtended;


unsigned char * mockNil(unsigned char *);
unsigned char * mockTrue(unsigned char *);
unsigned char * mockArray(unsigned char *);
unsigned char * mockArray2(unsigned char *);


//done
ulong _basicAt(ulong *object, int index);
void  _basicAtPut(ulong *object, int index, unsigned long value);
void _basicSetSize(ulong *object, ulong size);
ulong _basicGetSize(ulong *object);


unsigned long memoryAt(unsigned long pointer);
void memoryAtPut(unsigned long * pointer, unsigned long value);
//SendInliner
bool _isProxy(ulong *object);
bool isArray(ulong *object);
ulong size(ulong *object);
ulong asOop(ulong *object);
ulong oop(ulong *object);

ulong * _proxee(ulong *object);
ulong _size(ulong *object);
void _beNotInRememberedSet(ulong *object);
ulong _decommit(ulong limit, ulong delta);
ulong _asObject(ulong * object);
ulong * _asPointer(ulong object);
ulong _sizeInBytes(ulong * object);
ulong _headerSizeInBytes(ulong * object);

void _beExtended(ulong *object);
ulong _extendedSize(ulong *object, ulong size);
ulong _framePointer();
void _beNotInRememberSet(ulong *object);
ulong _size(ulong *object);

#endif /* DATASTRUCTURES_VMMEMORY_H_ */
