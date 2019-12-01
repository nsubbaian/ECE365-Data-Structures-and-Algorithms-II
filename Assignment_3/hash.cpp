
#include "hash.h"
#include <iostream>
#include <string.h>

//This function is the constructor
hashTable::hashTable(int size){

        capacity = getPrime(size);
        data.resize(capacity);
        filled = 0;

        for(int i = 0; i < data.size(); i++) {
                data[i].isOccupied = false;
                data[i].isDeleted = false;

        }
}

//This function returns the closest prime number to the size that is entered. Typically
//a hashtable that is twice the size of the amount of item in it is desired
unsigned int hashTable::getPrime(int size){
        int arrayval;
        unsigned int prime[] = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593,
                                49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917,
                                25165843, 50331653, 100663319, 201326611, 402653189,805306457, 1610612741};
        for(int i=0; size*2 > prime[i]; i++) {
                arrayval = i;
        }
        return prime[++arrayval];
}

//This is the FNV hash function. It is used to find where the word should be
//placed in the hash table.
int hashTable::hash(const std::string &key){
        const unsigned int fnv_prime = 0x811C9DC5;
        unsigned int hash = 0;
        unsigned int i = 0;
        unsigned int len = key.length();

        for (i = 0; i < len; i++)
        {
                hash *= fnv_prime;
                hash ^= (key[i]);
        }
        return (hash+1)%capacity;
}


//This function is used to find the location of the word that is entered, to
//see if it already exists in the hash table.
int hashTable::findPos(const std::string &key){

        int decision;
        int hashtableindex = hash(key);
        while(data[hashtableindex].isOccupied) {
                if(data[hashtableindex].key == key) {

                        if(data[hashtableindex].isDeleted) {
                                decision = -1;
                        } else{
                                decision = hashtableindex;
                        }

                        return decision;
                } else{
                        if(++hashtableindex == capacity) { //capacity is upper bound
                                hashtableindex = 0;
                        }
                }
        }
        return -1;
}

// This function is used to insert new words into the dictionary. It employs the findPos
// function to determine if a word already exists in the dictionary, and if the hash Table
// is filled more than 50 percent, the rehash function is called.
int hashTable::insert(const std::string &key, void *pv){
        int blah = findPos(key);
        int hashIndex;
        if( blah != -1) {
                return 1;
        } else {

                hashIndex = hash(key);

                while(data[hashIndex].isOccupied) {
                        if (data[hashIndex].isDeleted) {
                                data[hashIndex].key = key;
                                data[hashIndex].isOccupied = true;
                                data[hashIndex].isDeleted = false;
                                data[hashIndex].pv = pv;
                                return 0;
                        }
                        hashIndex++;
                        hashIndex %= capacity;
                }
                filled++;
        }

        data[hashIndex].key = key;
        data[hashIndex].isOccupied = true;
        data[hashIndex].isDeleted = false;
        data[hashIndex].pv = pv;

        if ((float)(filled)/(float)(capacity) > 0.50) {
                if (!rehash()) {
                        return 2;
                }
        }

        return 0;
}

//This function is used to determine if the word is in the hash table.
bool hashTable::contains(const std::string &key){
        int contains = findPos(key);
        return (contains != -1);

}



// This function is used to increase the size of the hash table.
bool hashTable::rehash(){

        std::vector<hashItem> temp = data;

        for(int i = 0; i < data.size(); i++) {
                data[i].isOccupied = false;
                data[i].isDeleted = true;
        }

        capacity = getPrime(capacity);
        data.resize(capacity);
        filled = 0;

        if(capacity > data.size()) {
                return false;
        }

        for(int i = 0; i < temp.size(); i++) {
                if(temp[i].isOccupied && !temp[i].isDeleted)
                        insert(temp[i].key, temp[i].pv);
        }

        return true;

}


// Delete the item with the specified key.
// Returns true on success,
// false if the specified key is not in the hash table.
bool hashTable::remove(const std::string &key){
        int removeposition = findPos(key);
        if( removeposition != -1) {
                data[removeposition].isDeleted = true;
                return true;
        } else{
                return false;
        }
}

// Get the pointer associated with the specified key.
// If the key does not exist in the hash table, return NULL.
// If an optional pointer to a bool is provided,
// set the bool to true if the key is in the hash table,
// and set the bool to false otherwise.
void *hashTable::getPointer(const std::string &key, bool *b){
        int getpointerposition = findPos(key);
        if( getpointerposition != -1) {
                if (b!= NULL) {
                        *b = true;
                }
                return data[getpointerposition].pv;
        } else{
                if(b!= NULL) {
                        *b = false;
                }
                return NULL;
        }
}

// Set the pointer associated with the specified key.
// Returns 0 on success,
// 1 if the key does not exist in the hash table.
int hashTable::setPointer(const std::string &key, void *pv){
        int setpointerposition = findPos(key);
        if( setpointerposition != -1) {
                data[setpointerposition].pv = pv;
                return 0;
        } else{
                return 1;
        }

}
