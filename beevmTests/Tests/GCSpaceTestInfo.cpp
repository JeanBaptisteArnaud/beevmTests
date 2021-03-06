#include "../DataStructures/GCSpaceInfo.h"
#include "cute.h"
#include "cute_suite.h"
extern cute::suite make_suite_GCSpaceInfoTest();

using namespace Bee;
using namespace std;

// File myclassTest.h

using namespace Bee;
ulong smallPointerMaxValue = 0x80000000;

void atTests() {
	GCSpaceInfo mock = GCSpaceInfo::mockForTest();
	ASSERTM("size", mock.sizeInBytes == 24);
	for (unsigned long value = 0; value < smallPointerMaxValue; value++) {
		mock.atPut(0, (unsigned long *) value);
		if (mock.at(0) != (unsigned long *) value) {
			ostringstream ostr;
			ostr << "v:";
			ostr << value;
			ostr << " readV:";
			ostr << mock.at(0);
			FAILM(ostr.str());
		}

		//ASSERTM("at", mock.at(0) == value);
	}
	mock.freeMock();
}

void base_3Tests() {
	GCSpaceInfo mock = GCSpaceInfo::mockForTest();
	ASSERTM("size", mock.sizeInBytes == 24);
	for (unsigned long value = 0; value < smallPointerMaxValue; value++) {
		mock.setBase_3((unsigned long *) value);
		ostringstream ostr;
		ostr << "commitedLimit v:";
		ostr << value;
		ostr << " readV:";
		ostr << mock.getBase_3();
		ASSERTM(ostr.str(), (ulong)mock.getBase_3() == value);
	}
	mock.freeMock();
}

void commitedLimitTests() {
	GCSpaceInfo mock = GCSpaceInfo::mockForTest();
	ASSERTM("size", mock.sizeInBytes == 24);
	for (unsigned long value = 0; value < smallPointerMaxValue; value++) {
		mock.setCommitedLimit((unsigned long *)value);
		ostringstream ostr;
		ostr << "commitedLimit v:";
		ostr << value;
		ostr << " readV:";
		ostr << mock.getCommitedLimit();
		ASSERTM(ostr.str(), (ulong)mock.getCommitedLimit() == value);
	}
	mock.freeMock();
}

void nextFreeTests() {
	GCSpaceInfo mock = GCSpaceInfo::mockForTest();
	ASSERTM("size", mock.sizeInBytes == 24);
	for (unsigned long value = 0; value < smallPointerMaxValue; value++) {
		mock.setNextFree((unsigned long *)value);
		ostringstream ostr;
		ostr << "nextFree v:";
		ostr << value;
		ostr << " readV:";
		ostr << mock.getNextFree();
		ASSERTM(ostr.str(), (ulong)mock.getNextFree() == value);
	}
	mock.freeMock();
}

void reservedLimitTests() {
	GCSpaceInfo mock = GCSpaceInfo::mockForTest();
	ASSERTM("size", mock.sizeInBytes == 24);
	for (unsigned long value = 0; value < smallPointerMaxValue; value++) {
		mock.setReservedLimit((unsigned long *)value);
		ostringstream ostr;
		ostr << "reservedLimit v:";
		ostr << value;
		ostr << " readV:";
		ostr << mock.getReservedLimit();
		ASSERTM(ostr.str(), (ulong)mock.getReservedLimit() == value);
	}
	mock.freeMock();
}

void softLimitTests() {
	GCSpaceInfo mock = GCSpaceInfo::mockForTest();
	ASSERTM("size", mock.sizeInBytes == 24);
	for (unsigned long value = 0; value < smallPointerMaxValue; value++) {
		mock.setSoftLimit((unsigned long *)value);
		ostringstream ostr;
		ostr << "softLimit v:";
		ostr << value;
		ostr << " readV:";
		ostr << mock.getSoftLimit();
		ASSERTM(ostr.str(), (ulong)mock.getSoftLimit() == value);
	}
	mock.freeMock();
}

void baseTests() {
	GCSpaceInfo mock = GCSpaceInfo::mockForTest();
	ASSERTM("size", mock.sizeInBytes == 24);
	for (unsigned long value = 0; value < smallPointerMaxValue; value++) {
		mock.setBase((unsigned long *)value);
		ostringstream ostr;
		ostr << "base v:";
		ostr << value;
		ostr << " readV:";
		ostr << mock.getBase();
		ASSERTM(ostr.str(), (ulong)mock.getBase() == value);
	}
	mock.freeMock();
}

cute::suite make_suite_GCSpaceInfoTest() {
	cute::suite s;
	//Long time so just test it one by one
	//s.push_back(CUTE(atTests));
	//s.push_back(CUTE(baseTests));
	//s.push_back(CUTE(softLimitTests));
	//s.push_back(CUTE(reservedLimitTests));
	//s.push_back(CUTE(commitedLimitTests));
	//s.push_back(CUTE(nextFreeTests));
	//s.push_back(CUTE(base_3Tests));
	return s;
}

