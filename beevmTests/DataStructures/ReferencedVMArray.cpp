/*
 * ReferencedVMArray.cpp
 *
 *  Created on: 18 d�c. 2015
 *      Author: Arnaud Jean-Baptiste
 */

#include "ReferencedVMArray.h"

using namespace Bee;

ReferencedVMArray::ReferencedVMArray() {
}

void ReferencedVMArray::contentsFromReferer() {
		contents = (unsigned long *) *((ulong *)_asObject(referer));
}

void ReferencedVMArray::emptyReserving(unsigned long size){
	ReferencedVMArray::emptyReserving(size);
	this->updateReference();
	this->setContentsMDA();
}

void ReferencedVMArray::loadMDAFrom(ReferencedVMArray * other){
	mda = other->mda;
}
void ReferencedVMArray::setReferer(unsigned long value) {
	referer = (ulong * )value;
	this->contentsFromReferer();
	this->loadContentsMDA();
}
void ReferencedVMArray::forget() {
	contents = mda = nil;
}

void ReferencedVMArray::grow() {
	VMArray::grow();
	this->updateReference();
}

void ReferencedVMArray::loadContentsMDA(){
	mda = (ulong*) *((ulong *)contents[-1]);
}
void ReferencedVMArray::setContentsMDA() {
	if(!(mda == nil)) contents[-1] = (ulong) mda;
}

void ReferencedVMArray::updateReference() {
	if(!(referer == nil)) (* (ulong *)_asObject(referer)) = (ulong) contents;
}
