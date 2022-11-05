#include <iostream>
#include "SMMIterator.h"
#include "SortedMultiMap.h"

//Complexitate:
//best case: Θ(1)
//worst case: Θ(n^2), wegen Selektionsort
//average case: Θ(n^2), wegen Selektionsort
SMMIterator::SMMIterator(const SortedMultiMap &d) : map(d) {
    sorted_cap = map.nrelems;
    sorted_nrkeys = 0;
    sorted_array = new Node[sorted_cap];

    //store elements from the hashtable
    for (int index = 0; index < map.m; index++) {
        if(map.HashTable[index].key != NULL_TKEY){
            sorted_array[sorted_nrkeys] = map.HashTable[index];
            sorted_nrkeys++;
        }
    }

    //sort key by selectionsort
    for (int index = 0; index < sorted_nrkeys - 1; index++) {
        int minim = index;
        for (int i = index + 1; i < sorted_nrkeys; i++)
            if (map.rel(sorted_array[i].key, sorted_array[minim].key)) {
                minim = i;
            }
        Node aux = sorted_array[index];
        sorted_array[index] = sorted_array[minim];
        sorted_array[minim] = aux;
    }

    current_key = 0;
    current_value = 0;
}

//Complexitate:
//best case: Θ(1)
//worst case: Θ(1)
//average case: Θ(1)
void SMMIterator::first() {
    current_key = 0;
    current_value = 0;
}

//Complexitate:
//best case: Θ(1)
//worst case: Θ(1)
//average case: Θ(1)
void SMMIterator::next() {
    if (valid()) {
        if (current_value < sorted_array[current_key].nr_values - 1)
            current_value++;
        else {
            current_value = 0;
            current_key++;
        }
    } else {
        throw exception();
    }
}

//Complexitate:
//best case: Θ(1)
//worst case: Θ(1)
//average case: Θ(1)
bool SMMIterator::valid() const {
    if (current_key < sorted_nrkeys)
        return true;
    return false;
}

//Complexitate:
//best case: Θ(1)
//worst case: Θ(1)
//average case: Θ(1)
TElem SMMIterator::getCurrent() const {
    if (valid()) {
        TElem elem;
        elem.first = sorted_array[current_key].key;
        elem.second = sorted_array[current_key].values[current_value];
        return elem;
    } else {
        throw exception();
    }
}

//Complexitate:
//best case: Θ(1)
//worst case: Θ(n^2), wegen Sortierung
//average case: Θ(n^2), wegen Sortierung
void SMMIterator::modify_current(TElem e) {

    //remove current
    for (int index = 0; index < map.m; index++) {
        if (map.HashTable[map.doppel_hash(getCurrent().first, index)].key == getCurrent().first) {
            int pos = map.doppel_hash(getCurrent().first, index);

            //remove value and key if current has only one element
            if(map.HashTable[pos].nr_values == 1 && map.HashTable[pos].values[0] == getCurrent().second) {
                map.HashTable[pos].values[0] = NULL_TVALUE;
                map.HashTable[pos].nr_values--;
                map.HashTable[pos].key = NULL_TKEY;
            }

            for(int sub_index = 0; sub_index < map.HashTable[pos].nr_values; sub_index++){
                //remove current.second
                if(map.HashTable[pos].values[sub_index] == getCurrent().second) {
                    while (sub_index < map.HashTable[pos].nr_values) {
                        map.HashTable[pos].values[sub_index] = map.HashTable[pos].values[sub_index + 1];
                        sub_index++;
                    }
                    map.HashTable[pos].nr_values--;
                }
            }
        }
    }

    //add if TElem e already exists
    int pos;
    bool added = false;
    for (int index = 0; index < map.m && !added; index++) {
        if (map.HashTable[map.doppel_hash(e.first, index)].key == e.first) {
            pos = map.doppel_hash(e.first,index);

            //make more space for values
            if(map.HashTable[pos].nr_values == map.HashTable[pos].cap_values){
                map.HashTable[pos].cap_values = map.HashTable[pos].cap_values * 2;
                TValue *newvec = new TValue[map.HashTable[pos].cap_values];
                int index_newvec = 0;
                while (index_newvec < map.HashTable[pos].nr_values) {
                    newvec[index_newvec] = map.HashTable[pos].values[index_newvec];
                    index_newvec++;
                }
                delete[] map.HashTable[pos].values;
                map.HashTable[pos].values = newvec;
            }

            map.HashTable[pos].values[map.HashTable[pos].nr_values] = e.second;
            map.HashTable[pos].nr_values++;
            added = true;
        }
    }

    //add new key
    if(!added){
        int index = 0;
        pos = map.doppel_hash(e.first,index);
        while(index < map.m && map.HashTable[pos].key != NULL_TKEY){
            index++;
            pos = map.doppel_hash(e.first,index);
        }

        map.HashTable[pos].key = e.first;
        map.HashTable[pos].values[0] = e.second;
        map.HashTable[pos].nr_values++;
    }


    //store elements from the hashtable
    for (int index = 0; index < map.m; index++) {
        if(map.HashTable[index].key != NULL_TKEY){
            sorted_array[sorted_nrkeys] = map.HashTable[index];
            sorted_nrkeys++;
        }
    }

    //sort key by selectionsort
    for (int index = 0; index < sorted_nrkeys - 1; index++) {
        int minim = index;
        for (int i = index + 1; i < sorted_nrkeys; i++)
            if (map.rel(sorted_array[i].key, sorted_array[minim].key)) {
                minim = i;
            }
        Node aux = sorted_array[index];
        sorted_array[index] = sorted_array[minim];
        sorted_array[minim] = aux;
    }
}




