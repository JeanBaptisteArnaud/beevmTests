/*
 * GCSpace.h
 *
 *  Created on: 18 d�c. 2015
 *      Author: Arnaud Jean-Baptiste
 */

#ifndef GCSPACE_H_
#define GCSPACE_H_
#include "GCSpaceInfo.h"
#include "VMMemory.h"

namespace Bee {

class GCSpace {
protected:
	GCSpaceInfo info;
	GCSpaceInfo * pinfo;
	ulong * base;
	ulong * nextFree;
	ulong * commitedLimit;
	ulong * reservedLimit;
	ulong * softLimit;
	ulong * base_3;
	ulong * regionBase;


public:
	// accessor
	void debug();
	ulong size();
	ulong used();
	ulong * getBase();
	ulong * getNextFree();
	ulong * getCommitedLimit();
	ulong * getReservedLimit();
	ulong * getRegionBase();
	ulong * getSoftLimit();
	void setBase(ulong * localBase);
	void setNextFree(ulong * localNextFree);
	void setCommitedLimit(ulong * localCommitedLimit);
	void setReservedLimit(ulong * localReservedLimit);
	void setSoftLimit(ulong * localSoftLimit);
	void setRegionBase(ulong  * localRegionBase);

	GCSpace();
	void load();
	void save();
	void loadFrom(GCSpace from);
	void reset();


	GCSpace * grow();
	void decommitSlack();
	void release();
	GCSpace * hostVMGrow();
	ulong * obtainFreeSpaceAndAllocate(ulong size);
	void dispenseReservedSpace();
	void garbageCollect();
	ulong * allocate(ulong size);
	void setInfo(GCSpaceInfo gcspaceinfo);
	ulong* shallowCopy(ulong *pointer);
	ulong * shallowCopyGrowingTo(ulong * array , ulong grow);
	static GCSpace currentFrom();
	static GCSpace currentTo();
	static GCSpace old();
	static GCSpace dynamicNew(ulong size);

	//Todo

	bool includes(ulong * pointer);
};

}

#endif /* GCSPACE_H_ */
