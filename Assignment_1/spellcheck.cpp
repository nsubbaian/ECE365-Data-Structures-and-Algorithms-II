//  Nithilam Subbaian
//  DSA 2 : Programming Assignment #1
//  Carl Sable
// I used the FNV Hash Funcion: https://www.programmingalgorithms.com/algorithm/fnv-hash?lang=C%2B%2B
// Hash Table Prime Numbers : https://planetmath.org/goodhashtableprimes

/* This program, which consists of spellcheck.cpp, hash.cpp, makefile, and hash.h, serves as a simple spell check.
The program prompts the user to enter a dictionary to be used, which is loaded into a hash table. Then the user can
enter a document to be spell checked as well as a destination to output the results of the spell check. The spellcheck is
performed by searching the hash table for the word. The words are spell checked based on the criteria that Prof. Sable
outlined in the project assignement.  */

#include "hash.h"
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <string.h>

using namespace std;

int main(){

        string DictionaryFileName, DocumentFileName, OutputFileName;
        clock_t DictReadTimeStart, DictReadTimeEnd, SpellCheckReadTimeStart, SpellCheckReadTimeEnd;
        double DictReadTimeDifference, SpellCheckReadTimeDifference;
        int entriesloaded = 0;

        int m = 0;
        int wordlength = 0;
        string word, entry, line;
        int linenumber = 1;
        char checkDictionary;
        bool search;
        int wordinsertion;
        int flag = 0;
        int flagtwo = 0;
        int flagthree = 0;


        hashTable *dictionary = new hashTable(50000);

        cout << "Enter the name of the dictionary file: ";
        cin >> DictionaryFileName;

        DictReadTimeStart = clock();
        ifstream DictionaryFile;

        DictionaryFile.open(DictionaryFileName.c_str());

        if(!DictionaryFile) {
                cout << "ERROR: The File"<< DictionaryFileName<<"could not be opened" << endl;

                exit(1);
        } else{

                while(getline(DictionaryFile, line)) {
                        for(int i = 0; i < line.length(); i++) {
                                line[i] = tolower(line[i]);
                        }

                        if(line.length() > 0) {
                                wordinsertion = dictionary->insert(line, NULL);
                        }

                        switch (wordinsertion) {
                        case 0:
                                entriesloaded++;
                                break;
                        case 1:
                                // Use this if you want to know what words were repeated
                                //      cout << "Key already exists in dictionary "<< line<<endl;
                                break;

                        case 2:
                                cout << "Rehash Failed." << endl;
                                return 0;
                        }
                }

                cout << entriesloaded << " dictionary words loaded successfully"<< endl;
                DictionaryFile.close();
        }


        DictReadTimeEnd = clock();
        DictReadTimeDifference = ((double)(DictReadTimeEnd-DictReadTimeStart)) / (double)CLOCKS_PER_SEC;
        cout << "How long, in seconds, it takes to read the dictionary (measured in terms of CPU time):";
        cout << DictReadTimeDifference << endl;

        cout << "Enter the name of the document file to be spell-checked:";
        cin >> DocumentFileName;


        cout << "Enter the name of the file where the output should be written:";
        cin >> OutputFileName;

        ifstream DocumentFiletoSpellCheck;
        ofstream OutputFileAfterSpellCheck;

        DocumentFiletoSpellCheck.open(DocumentFileName.c_str());
        OutputFileAfterSpellCheck.open(OutputFileName.c_str());


        if (!DocumentFiletoSpellCheck) {
                cout << "ERROR: Unable to open the file : '" << DocumentFileName << "' that needs to be spell checked" << endl;
                exit(1);
        }

        if (!OutputFileAfterSpellCheck) {
                cout << "ERROR: Unable to open the file : '" << OutputFileName << "' to contain the output" << endl;
                exit(1);
        }

        SpellCheckReadTimeStart = clock();
        while(getline(DocumentFiletoSpellCheck, entry)) {
                while(m <= entry.size()) {
                        if(isalnum(entry[m]) || entry[m] == '-' || entry[m] == '\'') {
                                wordlength++;
                                word += entry[m];
                                if(isdigit(entry[m])) {
                                        flagthree = 1;
                                        word.clear();
                                        for(; isalnum(entry[m]) || entry[m] == '-' || entry[m] == '\''; ++m) {
                                        }
                                        wordlength = 0;
                                }
                                if(wordlength > 20 && !flagthree) {
                                        for(int i = 0; i < word.length(); i++) {
                                                word[i] = tolower(word[i]);
                                        }
                                        word = word.substr(0,20);
                                        OutputFileAfterSpellCheck << "Long word at line " << linenumber << ", starts: " << word << endl;
                                        wordlength = 0;
                                        word.clear();
                                        for(; isalnum(entry[m]) || entry[m] == '-' || entry[m] == '\''; ++m) {
                                        }
                                        flagthree = 0;
                                }
                        }else{
                                if (wordlength>0) {
                                        for(int i = 0; i < word.length(); i++) {
                                                word[i] = tolower(word[i]);
                                        }
                                        search = dictionary->contains(word);
                                        if(!search) {
                                                OutputFileAfterSpellCheck << "Unknown word at line " << linenumber << ": " << word << endl;
                                        }
                                        word.clear();
                                        wordlength = 0;
                                }
                        }
                        m++;
                }
                flagthree = 0;
                flag = 0;
                word.clear();
                wordlength = 0;
                linenumber++;
                m = 0;

        }

        SpellCheckReadTimeEnd = clock();
        SpellCheckReadTimeDifference = ((double)(SpellCheckReadTimeEnd-SpellCheckReadTimeStart)) / (double)CLOCKS_PER_SEC;


        cout << "How long it takes to spell check the text file (measured in terms of CPU time):";
        cout << SpellCheckReadTimeDifference << endl;

        DocumentFiletoSpellCheck.close();
        OutputFileAfterSpellCheck.close();

        return 0;
}
