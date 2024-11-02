#include <exception>
#include "BagIterator.h"
#include "Bag.h"

using namespace std;


BagIterator::BagIterator(const Bag& c): bag(c)
{
    currentPosition = 0;
    while (currentPosition < bag.mTableSpots && bag.Table[currentPosition].TKey != -1) {
        currentPosition++;
    }
}

void BagIterator::first() {
    currentPosition = 0;
    while (currentPosition < bag.mTableSpots && bag.Table[currentPosition].TKey != -1) {
        currentPosition++;
    }
}


void BagIterator::next() {
    if (!valid()) {
        throw exception();
    }
    // If there's a next item in the chain, follow it
    if (bag.Table[currentPosition].next != -1) {
        currentPosition = bag.Table[currentPosition].next;
    } else {
        // Otherwise, move to the next non-empty position in the table
        currentPosition++;
        while (currentPosition < bag.mTableSpots && bag.Table[currentPosition].TKey == -1) {
            currentPosition++;
        }
    }
}


bool BagIterator::valid() const {
    return currentPosition < bag.mTableSpots && bag.Table[currentPosition].TKey != -1;
}



TElem BagIterator::getCurrent() const
{
    if (!valid()) {
        throw exception();
    }
    return bag.Table[currentPosition].TKey;
}
