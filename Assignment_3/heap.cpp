// Nithilam Subbaian
//DSA 2 Assignment 2
// This program uses parts from assignment 1.

//Premise:
// In this program, a class called "heap" provides programmers with the functionality
// of a priority queue using binary heap implementation. Each item inserted into the
//binary heap will specify a unique string id, an integer key, and optionally any pointer.
// When the program starts, the user is prompted to enter the capacity of the heap,
//which is passed to the constructor. The user is then prompted the options to
//insert a new item, set the key of a specified item, delete a specified item, or
//delete the minimum item.


#include "heap.h"

// heap - The constructor allocates space for the nodes of the heap
// and the mapping (hash table) based on the specified capacity
heap::heap(int capacity){
        heapCapacity = capacity;
        filledHeapItems = 0;
        data.resize(capacity+1);
        mapping = new hashTable(capacity);
}

// insert - Inserts a new node into the binary heap
//
// Inserts a node with the specified id string, key,
// and optionally a pointer. They key is used to
// determine the final position of the new node.
//
// Returns:
//   0 on success
//   1 if the heap is already filled to capacity
//   2 if a node with the given id already exists (but the heap
//     is not filled to capacity)
int heap::insert(const std::string &id, int key, void *pv){

        if (filledHeapItems<heapCapacity) {
                if(mapping->contains(id)) {
                        return 2;
                }else{
                        data[++filledHeapItems].id = id;
                        data[filledHeapItems].key = key;
                        data[filledHeapItems].pData = pv;
                        mapping->insert(id, &data[filledHeapItems]);
                        percolateUp(filledHeapItems);
                }
        } else{
                return 1;
        }
        return 0;
}


//
// setKey - set the key of the specified node to the specified value
//
// I have decided that the class should provide this member function
// instead of two separate increaseKey and decreaseKey functions.
//
// Returns:
//   0 on success
//   1 if a node with the given id does not exist
//
int heap::setKey(const std::string &id, int key){
        if (mapping->contains(id)) {
                node *pn = static_cast<node *> (mapping->getPointer(id));
                int original = pn->key;
                pn->key = key;
                int index = getPos(pn);
                if (key < original) {
                        percolateUp(index);
                } else if (key > original) {
                        percolateDown(index);
                }
                return 0;
        }
        return 1;
}

//
// deleteMin - return the data associated with the smallest key
//             and delete that node from the binary heap
//
// If pId is supplied (i.e., it is not NULL), write to that address
// the id of the node being deleted. If pKey is supplied, write to
// that address the key of the node being deleted. If ppData is
// supplied, write to that address the associated void pointer.
//
// Returns:
//   0 on success
//   1 if the heap is empty
//
int heap::deleteMin(std::string *pId, int *pKey, void *ppData){
        if(filledHeapItems<1) {
                return 1;
        }
        else{
                if(pId != nullptr)
                        *pId = data[1].id;
                if(pKey != nullptr)
                        *pKey = data[1].key;
                if(ppData != nullptr)
                        *(static_cast<void **> (ppData)) = data[1].pData;

                mapping->remove(data[1].id);
                if(filledHeapItems > 1) {
                        data[1] = data[filledHeapItems];
                }
                filledHeapItems--;
                if(filledHeapItems > 1) {
                        percolateDown(1);
                }
                return 0;
        }
}

//
// remove - delete the node with the specified id from the binary heap
//
// If pKey is supplied, write to that address the key of the node
// being deleted. If ppData is supplied, write to that address the
// associated void pointer.
//
// Returns:
//   0 on success
//   1 if a node with the given id does not exist
//
int heap::remove(const std::string &id, int *pKey, void *ppData){
        if(mapping->contains(id)) {

                node *pn = static_cast<node *> (mapping->getPointer(id));
                int todeletekey = pn->key;
                int index = getPos(pn);
                if(pKey != nullptr) {
                        *pKey = data[index].key;
                }
                if(ppData != nullptr) {
                        *(static_cast<void **> (ppData)) = data[index].pData;
                }

                mapping->remove(data[index].id);
                data[index] = data[filledHeapItems];
                filledHeapItems--;
                if(data[index].key < todeletekey) {
                        percolateUp(index);
                }
                else if(data[index].key > todeletekey) {
                        percolateDown(index);
                }
                return 0;
        } else{
                return 1;
        }

}


void heap::percolateUp(int posCur){
        //set up a temporary holder
        node tmp = data[posCur];
        //checks to make sure there is more than one entry, and if percolating up is necessary
        while (posCur > 1 && tmp.key < data[posCur/2].key) {
                //parent moved to child's position
                data[posCur] = data[posCur/2];
                //hash table pointer is updated for moving parent
                mapping->setPointer(data[posCur].id, &data[posCur]);
                posCur /= 2;
        }
        data[posCur] = tmp;
        //hash table pointer is updated for final move
        mapping->setPointer(data[posCur].id, &data[posCur]);

}

void heap::percolateDown(int posCur){
        node tmp = data[posCur];
        //can only percolateDown if there are children
        while (posCur * 2 <= filledHeapItems) {
                int child = posCur * 2;
                //find the smaller child
                if (child < filledHeapItems && data[child + 1].key < data[child].key) {
                        ++child;
                }
                if (data[child].key < tmp.key) {
                        data[posCur] = data[child];
                        // update hash table
                        mapping->setPointer (data[posCur].id, &data[posCur]);
                } else {
                        break;
                }
                posCur = child;
        }
        data[posCur] = tmp;
        //update hash table pointer for final move
        mapping->setPointer (data[posCur].id, &data[posCur]);

}


int heap::getPos(node *pn){
        int pos = pn - &data[0];
        return pos;
}


// I would like to cite this source for helping me review implementation: https://bradfieldcs.com/algos/trees/priority-queues-with-binary-heaps/
