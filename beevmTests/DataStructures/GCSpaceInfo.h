/*
 * GCSpaceInfo.h
 *
 *  Created on: 4 janv. 2016
 *      Author: jbapt
 */

#ifndef GCSPACEINFO_H_
#define GCSPACEINFO_H_

typedef unsigned long ulong;

namespace Bee {
class GCSpaceInfo {

protected:
	unsigned char * contents;

public:

	unsigned short shortAt(ulong);
	ulong * at(ulong);
	void atPut(ulong,ulong *);
	GCSpaceInfo();
	const ulong sizeInBytes = 24;
	ulong * base();
	void base( ulong * );
	ulong * base_3();
	void base_3(ulong * );
	ulong * commitedLimit();
	void commitedLimit(ulong * );
	void setContents(ulong *);
	ulong * nextFree();
	void nextFree(ulong *);
	ulong * reservedLimit();
	void reservedLimit(ulong * );
	ulong * softLimit();
	void softLimit(ulong *);
	void freeMock();


	static GCSpaceInfo newSize(ulong);
	static GCSpaceInfo currentTo();
	static GCSpaceInfo currentFrom();
	static GCSpaceInfo old();
	static GCSpaceInfo mockForTest();
};

}

#endif /* GCSPACEINFO_H_ */
