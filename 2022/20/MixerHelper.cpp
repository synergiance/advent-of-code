//
// Created by Synergiance on 12/20/22.
//

#include "MixerHelper.h"

#include <iostream>

void MixerHelper::Add(long value) {
	if (mpFirst == nullptr) {
		mpFirst = new MixerLink { value };
		mpFirst->prev = mpFirst->next = mpLastAdded = mpFirst;
		mOriginalOrder.push_back(mpFirst);
	} else {
		mpLastAdded->next = new MixerLink {value, mpLastAdded, mpFirst };
		mpFirst->prev = mpLastAdded = mpLastAdded->next;
		mOriginalOrder.push_back(mpLastAdded);
	}
	mNumLinks++;
}

void MixerHelper::MoveLink(MixerLink *link, long distance) {
	if (distance == 0) return;
	link->next->prev = link->prev;
	link->prev->next = link->next;
	mNumLinks--;
	MixerLink *refLink = GetLinkOffset(link, distance);
	if (distance < 0) {
		link->prev = refLink->prev;
		link->next = refLink;
		refLink->prev = link;
		link->prev->next = link;
	} else {
		link->next = refLink->next;
		link->prev = refLink;
		refLink->next = link;
		link->next->prev = link;
	}
	mNumLinks++;
}

MixerLink *MixerHelper::GetLinkOffset(MixerLink *link, long distance) const {
	if (distance == 0) return link;
	if (link == nullptr) return nullptr;
	distance %= (long)mNumLinks;
	if (distance < 0) for (long i = 0; i > distance; i--) link = link->prev;
	else for (long i = 0; i < distance; i++) link = link->next;
	return link;
}

void MixerHelper::Print() {
	MixerLink *pCurrentLink = mpFirst;
	for (long i = 0; i < mNumLinks; i++) {
		std::cout << pCurrentLink->value << ", ";
		pCurrentLink = pCurrentLink->next;
	}
	std::cout<<std::endl;
}

size_t MixerHelper::Length() const {
	return mNumLinks;
}

MixerLink *MixerHelper::First() const {
	return mpFirst;
}

MixerLink *MixerHelper::LastAdded() const {
	return mpLastAdded;
}

MixerLink *MixerHelper::GetOriginalLinkAt(size_t location) {
	return mOriginalOrder[location];
}

MixerHelper::~MixerHelper() {
	for (MixerLink *link : mOriginalOrder)
		delete link;
}
