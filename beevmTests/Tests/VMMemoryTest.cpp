#include "../DataStructures/VMMemory.h"
#include "cute.h"
#include "cute_suite.h"

#include "stdlib.h"

extern cute::suite make_suite_VMMemoryTest();

using namespace std;
// File myclassTest.h

// nil 00 45 34 61 10 DC 0D 0A
//nil_hdr Object <0, 3445h, \             ; DATA XREF: GC_full+E3?o
// .data:10026058                                         ; sub_10010E40+33?w
// .data:10026058         ObjectFlag_reserved1 or ObjectFlag_zeroTermOrNamed or ObjectFlag_notIndexed,\
// .data:10026058         offset unk_A0DDC10>

// true 00 00 00 61 A0 6B 0B 0A
//true_hdr Object <0, 0, ObjectFlag_reserved1 or ObjectFlag_zeroTermOrNamed or ObjectFlag_notIndexed,
// offset unk_A0B6BA0>

// Array new: 3
// 03 F8 37 01 F0 92 07 0A
// 12:10means Object <3, 37F8h, ObjectFlag_reserved1, offset off_A0792F0>
// 12:10has 3 slots

void headerOf() {
	unsigned char * header;
	header_t * h;
	header = (unsigned char *) malloc(12);
	mockArray(header);

	h = HEADER_OF(header[8]);
	ASSERTM("Array 1 : Size", (unsigned short ) h->size == 3);
	ASSERTM("Array 1 : Hash", h->hash == (unsigned short ) 0x37F8);
	ASSERTM("Array 1 : Flags", (unsigned short ) (h->flags) == ObjectFlag_reserved1);
	ASSERTM("Array 1 : behavior", ((ulong )h->behavior) == 0x0A0792F0);

	mockArray2(header);
	h = HEADER_OF(header[8]);
	ASSERTM("Array 2 : Size", ((unsigned short ) h->size) == 3);
	ASSERTM("Array 2 : Hash", h->hash == (unsigned short ) 0x0000);
	ASSERTM("Array 2 : Flags", ((unsigned short ) h->flags == ObjectFlag_reserved1));
	ASSERTM("Array 2 : behavior", ((ulong )h->behavior) == 0x028571D8);

	mockTrue(header);
	h = HEADER_OF(header[8]);
	//00000061 A06B0B0A

	ASSERTM("True : Size", ((unsigned short ) h->size) == 0);
	ASSERTM("True : Hash", h->hash == (unsigned short ) 0x0000);
	ASSERTM("True : Flags", ((unsigned short ) h->flags) == (ObjectFlag_reserved1 | ObjectFlag_notIndexed | ObjectFlag_zeroTermOrNamed ));
	ASSERTM("True : behavior", ((ulong )h->behavior) == 0x0A0B6BA0);

	//00 45 34 61 10 DC 0D 0A
	mockNil(header);
	h = HEADER_OF(header[8]);
	ASSERTM("Nil : Size", ((unsigned short ) h->size) == 0);
	ASSERTM("Nil : Hash", h->hash == (unsigned short ) 0x3445);
	ASSERTM("Nil : Flags", (unsigned short ) h->flags == (ObjectFlag_reserved1 | ObjectFlag_notIndexed | ObjectFlag_zeroTermOrNamed ));
	ASSERTM("Nil : behavior", ((ulong )h->behavior) == 0x0A0DDC10);

}

void basicSize() {
	unsigned char * header;
	header = (unsigned char *) malloc(12);
	mockArray(header);
	ulong * object = (ulong *) &header[8];
	ASSERTM("size ", (_basicGetSize(object) == 3));
	_basicSetSize(object,122);
	ASSERTM("size ", (_basicGetSize(object) == 122));
}

void beeExtended() {
	unsigned char * header;
	header = (unsigned char *) malloc(12);
	mockArray(header);
	ulong * object = (ulong *) &header[8];
	_beExtended(object);
	header_t * h = HEADER_OF(header[8]);
	ASSERTM("beeExtended b", ((unsigned short)h->flags == (ObjectFlag_reserved1 | ObjectFlag_isExtended)));
}


cute::suite make_suite_VMMemoryTest() {
	cute::suite s;
	s.push_back(CUTE(headerOf));
	s.push_back(CUTE(basicSize));
	s.push_back(CUTE(beeExtended));

	//s.push_back(CUTE(growingTo));
	//s.push_back(CUTE(newGCSpaceShallowCopy));
	//s.push_back(CUTE(shallowCopy));
	//s.push_back(CUTE(shallowCopyBytes));
	//s.push_back(CUTE(shallowCopyBytes2));
	//s.push_back(CUTE(shallowCopyBytes3));
	//s.push_back(CUTE(shallowCopyBytesExtended));
	//s.push_back(CUTE(shallowCopyExtended));
	//s.push_back(CUTE(synchronousGCSpace));
	return s;
}

