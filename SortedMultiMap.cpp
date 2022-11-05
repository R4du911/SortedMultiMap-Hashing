#include "SMMIterator.h"
#include "SortedMultiMap.h"
#include <iostream>
#include <vector>
#include <exception>

using namespace std;

//Complexitate:
//best case: Θ(1)
//worst case: Θ(n)
//average case: Θ(n)
SortedMultiMap::SortedMultiMap(Relation r) {
    this->m = 19;
    this->nrelems = 0;
    this->HashTable = new Node[this->m];
    for (int index = 0; index < m; index++)
        HashTable[index].key = NULL_TKEY;
    this->rel = r;
}

//Complexitate:
//best case: Θ(1)
//worst case: Θ(n)
//average case: Θ(n)
void SortedMultiMap::resize_rehash() {
    int old_cap = m;
    m *= 2;
    int pos,j;
    Node* NewHashTable = new Node[m];

    for(int index = 0; index < m; index++) {
        NewHashTable[index].key = NULL_TKEY;
    }

    for(int index = 0; index < old_cap; index++)
    {
        if(HashTable[index].key != NULL_TKEY)
        {
            j = 0;
            pos = doppel_hash(HashTable[index].key,j);
            while(NewHashTable[pos].key != NULL_TKEY)
            {
                j++;
                pos = doppel_hash(HashTable[index].key,j);
            }
            NewHashTable[pos]=HashTable[index];
            NewHashTable[pos].values = new TValue[NewHashTable[pos].cap_values];
            for(int values_index = 0; values_index < HashTable[index].nr_values; values_index++)
                NewHashTable[pos].values[values_index] = HashTable[index].values[values_index];
        }
    }

    for (int index = 0; index < old_cap; index++)
        delete[] HashTable[index].values;
    delete[] HashTable;

    HashTable = NewHashTable;
}

//Complexitate:
//best case: Θ(1)
//worst case: Θ(n)
//average case: Θ(1)
void SortedMultiMap::add(TKey c, TValue v) {
    //add if key already exists
    int pos;
    bool added = false;
    for (int index = 0; index < m && !added; index++) {
        if (HashTable[doppel_hash(c, index)].key == c) {
            pos = doppel_hash(c,index);

            //make more space for values
            if(HashTable[pos].nr_values == HashTable[pos].cap_values){
                HashTable[pos].cap_values = HashTable[pos].cap_values * 2;
                TValue *newvec = new TValue[HashTable[pos].cap_values];
                int index_newvec = 0;
                while (index_newvec < HashTable[pos].nr_values) {
                    newvec[index_newvec] = HashTable[pos].values[index_newvec];
                    index_newvec++;
                }
                delete[] HashTable[pos].values;
                HashTable[pos].values = newvec;
            }

            HashTable[pos].values[HashTable[pos].nr_values] = v;
            HashTable[pos].nr_values++;
            nrelems++;
            added = true;
        }
    }

    //add new key
    if(!added){
        int index = 0;
        pos = doppel_hash(c,index);
        while(index < m && HashTable[pos].key != NULL_TKEY){
            index++;
            pos = doppel_hash(c,index);
        }
        if(index == m){
           resize_rehash();
           index = 0;
           pos = doppel_hash(c,index);
           while(index < m && HashTable[pos].key != NULL_TKEY){
               index++;
               pos = doppel_hash(c,index);
           }
           HashTable[pos].key = c;
           nrelems++;
           HashTable[pos].values[0] = v;
           HashTable[pos].nr_values++;
           added = true;
        }
        else{
            HashTable[pos].key = c;
            nrelems++;
            HashTable[pos].values[0] = v;
            HashTable[pos].nr_values++;
            added = true;
        }
    }
}

//Complexitate:
//best case: Θ(1)
//worst case: Θ(n)
//average case: Θ(1)
vector<TValue> SortedMultiMap::search(TKey c) const {
    vector<TValue> return_values;
    int pos;
    for (int index = 0; index < m; index++) {
        if (HashTable[doppel_hash(c, index)].key == c) {
            pos = doppel_hash(c, index);
            for (int sub_index = 0; sub_index < HashTable[pos].nr_values; sub_index++) {
                return_values.emplace_back(HashTable[pos].values[sub_index]);
            }
            return return_values;
        }
    }
    return return_values;
}

//Complexitate:
//best case: Θ(1)
//worst case: Θ(n)
//average case: Θ(1)
bool SortedMultiMap::remove(TKey c, TValue v) {
    int pos;
    for (int index = 0; index < m; index++) {
        if (HashTable[doppel_hash(c, index)].key == c) {
            pos = doppel_hash(c,index);

            //remove value and key if key has only one element
            if(HashTable[pos].nr_values == 1 && HashTable[pos].values[0] == v) {
                HashTable[pos].values[0] = NULL_TVALUE;
                HashTable[pos].nr_values--;
                HashTable[pos].key = NULL_TKEY;
                nrelems--;
                return true;
            }

            for(int sub_index = 0; sub_index < HashTable[pos].nr_values; sub_index++){
                //remove v
                if(HashTable[pos].values[sub_index] == v) {
                    while (sub_index < HashTable[pos].nr_values) {
                        HashTable[pos].values[sub_index] = HashTable[pos].values[sub_index + 1];
                        sub_index++;
                    }
                    nrelems--;
                    HashTable[pos].nr_values--;

                    // make less space for values
                    if (HashTable[pos].nr_values <= HashTable[pos].cap_values / 4) {
                        HashTable[pos].cap_values = HashTable[pos].cap_values / 2;
                        TValue *newvec = new TValue[HashTable[pos].cap_values];
                        int index_newvec = 0;
                        while (index_newvec < HashTable[pos].nr_values) {
                            newvec[index_newvec] = HashTable[pos].values[index_newvec];
                            index_newvec++;
                        }
                        delete[] HashTable[pos].values;
                        HashTable[pos].values = newvec;
                    }
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}

//Complexitate:
//best case: Θ(1)
//worst case: Θ(1)
//average case: Θ(1)
int SortedMultiMap::size() const {
    return this->nrelems;
}

//Complexitate:
//best case: Θ(1)
//worst case: Θ(1)
//average case: Θ(1)
bool SortedMultiMap::isEmpty() const {
    if (this->nrelems == 0)
        return true;
    return false;
}

SMMIterator SortedMultiMap::iterator() const {
    return SMMIterator(*this);
}

//Complexitate:
//best case: Θ(1)
//worst case: Θ(n)
//average case: Θ(n)
SortedMultiMap::~SortedMultiMap() {
    for (int index = 0; index < m; index++)
        delete[] HashTable[index].values;
    delete[] HashTable;
}

//first hashing
int SortedMultiMap::h1(TKey e) const {
    return abs(e % m);
}

//second hashing
int SortedMultiMap::h2(TKey e) const {
    return abs(1 + e % m);
}

//Doppelhashing Funktion
int SortedMultiMap::doppel_hash(TKey e, int i) const {
    return abs(h1(e) + i * h2(e)) % m;
}
