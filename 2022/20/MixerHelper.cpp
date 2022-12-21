//
// Created by Synergiance on 12/20/22.
//

#include "MixerHelper.h"

#include <iostream>

void MixerHelper::Add(int value) {
	if (mpFirst == nullptr) {
		mpFirst = new MixerLink { value };
		mpFirst->prev = mpFirst->next = mpLastAdded = mpFirst;
		mOriginalOrder.push_back(mpFirst);
		std::cout<<"Inserting first value: "<<value<<std::endl;
	} else {
		mpLastAdded->next = new MixerLink {value, mpLastAdded, mpFirst };
		mpFirst->prev = mpLastAdded = mpLastAdded->next;
		mOriginalOrder.push_back(mpLastAdded);
		std::cout << "Inserting new value " << value << " between " << mpLastAdded->prev->value << " and " << mpFirst->value << std::endl;
	}
	mNumLinks++;
}

void MixerHelper::MoveLink(MixerLink *link, int distance) {
	std::cout<<"Move link "<<link->value<<" by "<<distance<<" locations."<<std::endl;
	if (distance == 0) return;
	MixerLink *refLink = GetLinkOffset(link, distance);
	if (refLink == nullptr) return;
	link->next->prev = link->prev;
	link->prev->next = link->next;
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
	std::cout<<"Link is now between "<<link->prev->value<<" and "<<link->next->value<<std::endl;
	//Print();
}

MixerLink *MixerHelper::GetLinkOffset(MixerLink *link, int distance) const {
	if (distance == 0) return link;
	if (link == nullptr) return nullptr;
	distance %= (int)mNumLinks;
	if (distance < 0) for (int i = 0; i > distance; i--) link = link->prev;
	else for (int i = 0; i < distance; i++) link = link->next;
	return link;
}

void MixerHelper::Print() {
	MixerLink *pCurrentLink = mpFirst;
	for (int i = 0; i < mNumLinks; i++) {
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