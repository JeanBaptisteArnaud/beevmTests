/*
 * ReferencedVMArray.h
 *
 *  Created on: 18 d�c. 2015
 *      Author: Arnaud Jean-Baptiste
 */

#ifndef DATASTRUCTURES_REFERENCEDVMARRAY_H_
#define DATASTRUCTURES_REFERENCEDVMARRAY_H_

#include "VMArray.h"


namespace Bee {

class ReferencedVMArray: public VMArray {
public:
	ReferencedVMArray();
	unsigned long * referer;
	unsigned long * mda;

	void contentsFromReferer();
	void emptyReserving(unsigned long size);
	void loadMDAFrom(ReferencedVMArray * size);
	void setReferer(unsigned long size);
	void forget();
	void grow();
	void loadContentsMDA();
	void setContentsMDA();
	void updateReference();

};

}

#endif /* DATASTRUCTURES_REFERENCEDVMARRAY_H_ */
