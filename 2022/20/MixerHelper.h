//
// Created by Synergiance on 12/20/22.
//

#pragma once

#include <vector>

struct MixerLink {
	int value;
	MixerLink *prev;
	MixerLink *next;
};

class MixerHelper {
public:
	~MixerHelper();
	void Add(int value);
	void MoveLink(MixerLink *link, int distance);
	MixerLink *GetLinkOffset(MixerLink *link, int distance) const;
	void Print();
	[[nodiscard]] size_t Length() const;
	[[nodiscard]] MixerLink *First() const;
	[[nodiscard]] MixerLink *LastAdded() const;
	MixerLink *GetOriginalLinkAt(size_t location);
private:
	MixerLink *mpFirst;
	MixerLink *mpLastAdded;
	size_t mNumLinks;
	std::vector<MixerLink*> mOriginalOrder;
};
