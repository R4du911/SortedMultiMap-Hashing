#pragma once

#include "SortedMultiMap.h"


class SMMIterator{
	friend class SortedMultiMap;
private:
	const SortedMultiMap& map;
	SMMIterator(const SortedMultiMap& map);

	Node* sorted_array;
    int sorted_cap;
    int sorted_nrkeys;

    int current_key;
    int current_value;

public:
	void first();
	void next();
	bool valid() const;
    void modify_current(TElem e);
   	TElem getCurrent() const;
};


