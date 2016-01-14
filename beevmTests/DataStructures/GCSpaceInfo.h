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
	ulong sizeInBytes;
	ulong * getBase();
	void setBase( ulong * );
	ulong * getBase_3();
	void setBase_3(ulong * );
	ulong * getCommitedLimit();
	void setCommitedLimit(ulong * );
	void setContents(ulong *);
	ulong * getNextFree();
	void setNextFree(ulong *);
	ulong * getReservedLimit();
	void setReservedLimit(ulong * );
	ulong * getSoftLimit();
	void setSoftLimit(ulong *);
	void freeMock();


	static GCSpaceInfo newSize(ulong);
	static GCSpaceInfo currentTo();
	static GCSpaceInfo currentFrom();
	static GCSpaceInfo old();
	static GCSpaceInfo mockForTest();
};

}

#endif /* GCSPACEINFO_H_ */
