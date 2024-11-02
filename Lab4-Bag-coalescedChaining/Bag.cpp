#include "Bag.h"
#include "BagIterator.h"
#include <exception>
#include <iostream>
using namespace std;


Bag::Bag() {
    mTableSpots = 200;
    alphaLoadFactor = 0.0f;
    firstEmpty = 0;
    sizeBag = 0;
    
    //simple node struct array
    Table = new chNode[mTableSpots];
    initTable();
    cout << "Bag created with " << mTableSpots << " spots." << endl;
    
}

void Bag::initTable(){
    for (int i = 0; i < mTableSpots; i++) {
        Table[i].next = -1;
    }
    //Table[mTableSpots-1].next = -1;
}

int Bag::h(TElem key) const {
    return abs(key) % mTableSpots;
}

void Bag::changeFirstEmpty() {
    firstEmpty++;
    while (firstEmpty < mTableSpots && Table[firstEmpty].TKey != -1) {
        firstEmpty++;
    }
}

void Bag::updateAlpha() {
    alphaLoadFactor = static_cast<float>(sizeBag) / mTableSpots;
}

void Bag::add(TElem elem) {
	//if no first empty resize rehash
    if (alphaLoadFactor > 0.7) {
        resizenRehash();
    }
    
    //hash position
    int position = h(elem);
    
    //add elem
    if (Table[position].TKey == -1) {
        Table[position].TKey = elem;
        Table[position].next = -1;
        
        if (position == firstEmpty) {
            changeFirstEmpty();
        }
    }
    else {
        int current = position;
        while (Table[current].next != -1) {
            current = Table[current].next;
        }
        Table[firstEmpty].TKey = elem;
        Table[firstEmpty].next = -1;
        Table[current].next = firstEmpty;
        changeFirstEmpty();
    }
    //increment stuff if needed
    sizeBag++;
    updateAlpha();
}

void Bag::resizenRehash(){
    int oldSize = mTableSpots;
    mTableSpots *= 2; // Double the table size
    chNode* oldTable = Table;

    // Create new table with updated size and initialize it
    Table = new chNode[mTableSpots];
    initTable();
    firstEmpty = 0;
    sizeBag = 0;

    // Rehash each element in the old table
    for (int i = 0; i < oldSize; i++) {
        if (oldTable[i].TKey != -1) {
            add(oldTable[i].TKey);
        }
    }
    delete[] oldTable;
    updateAlpha();
}
//////////////////////////////////////
/////////from course slides //////////
//////////////////////////////////////
//bool Bag::remove(TElem elem) {
//    int pos = h(elem);  // similar to nomad and previous nomad (current, currentprev)
//    int prevpos = -1;
//    
//    //start from hashed pos and go through array
//    while (pos != -1 && Table[pos].TKey != elem) {
//        prevpos = pos;
//        pos = Table[pos].next;
//    }
//    
//    if (pos == -1) {
//        return false; //element not found
//    } else {
//        bool over = false; // ?? becomes true when nothing hashes to pos ??
//        ////
//        do {
//            int p = Table[pos].next;
//            int pp = pos;
//            
//            while (p != -1 && h(Table[p].TKey) != pos) {
//                pp = p;
//                p = Table[p].next;
//            }
//            if (p == -1) {
//                over = true;
//            }
//            else {
//                Table[pos].TKey = Table[p].TKey;
//                prevpos = pp;
//                pos = p;
//            }
//        } while (!over);
//        //// elem from pos can be removed now (no elem hashes to it) ???
//        if (prevpos == -1) {
//            int idx = 0; // what the heck :)) another level down ??
//            while (idx < mTableSpots && prevpos == -1) {
//                if (Table[idx].next == pos) {
//                    prevpos = idx;
//                }
//                else {
//                    idx++;
//                }
//            }
//        }
//        
//    }
//    
//    //decrement
//    sizeBag--;
//    return true;
//}
///////////////////////////////////////////
/////////from slides through gpt //////////
///////////////////////////////////////////
bool Bag::remove(TElem elem) {
    int pos = h(elem);
    int prevpos = -1;

    // Traverse the chain to find the element
    while (pos != -1 && Table[pos].TKey != elem) {
        prevpos = pos;
        pos = Table[pos].next;
    }

    // Element not found
    if (pos == -1) {
        return false;
    }

    // Element found; remove it
    if (prevpos == -1) {
        // Element is at the head of the chain for its hash position
        int hashPos = h(elem);
        Table[hashPos].TKey = -1;  // Mark as empty
        Table[hashPos].next = Table[pos].next;
    } else {
        // Element is in the chain; update previous node's next pointer
        Table[prevpos].next = Table[pos].next;
    }

    // Clear the node and update `firstEmpty` if needed
    Table[pos].TKey = -1;
    Table[pos].next = -1;
    if (pos < firstEmpty) {
        firstEmpty = pos;
    }

    sizeBag--;
    return true;
}

//////////////////////////////////

bool Bag::search(TElem elem) const {
    int pos = h(elem);
    std::cout << "Searching for " << elem << " starting at position " << pos << std::endl;
    while (pos != -1) {
        if (Table[pos].TKey == elem) {
            std::cout << "Found element " << elem << " at position " << pos << std::endl;
            return true;
        }
        pos = Table[pos].next;
    }
    std::cout << "Element " << elem << " not found in the bag." << std::endl;
    return false;
}

int Bag::nrOccurrences(TElem elem) const {
    int count = 0;
    int pos = h(elem);
    //int current = pos;
    while (pos != -1) {
        if (Table[pos].TKey == elem) {
            count++;
        }
        pos = Table[pos].next;
    }
    return count;
}


int Bag::size() const {
	//TODO - Implementation
    return sizeBag;
}


bool Bag::isEmpty() const {
	//TODO - Implementation
	return sizeBag==0;
}

BagIterator Bag::iterator() const {
	return BagIterator(*this);
}


Bag::~Bag() {
    delete [] Table;
}

