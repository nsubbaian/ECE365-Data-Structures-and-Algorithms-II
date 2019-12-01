// Nithilam Subbaian
// DSA2 Programming Assignment #4

/*  General Summary:
   The problem defines a "merge" of two strings as a third string containing all
   the characters from each of the original two strings mixed together. The two
   sets of characters can be interspersed, but the characters from each individual
   string cannot be permuted.
 */

#include <iostream>
#include <fstream>

using namespace std;
//using method of string interleaving with Dynamic Programming
bool doubleMatrix[10001][10001];

int main(){

        int i,j, lengthA, lengthB, lengthC;
        string inputFile, outputFile, A, B, C;
        cout << "Enter name of input file: ";
        cin >> inputFile;
        cout << "Enter name of output file: ";
        cin >> outputFile;
        ifstream inFile(inputFile);
        ofstream outFile(outputFile);

        while (getline(inFile, A) && getline(inFile, B) && getline(inFile, C)) {

                lengthA = A.length(); lengthB = B.length(); lengthC = C.length();

                //initial checks
                if ( ( (lengthA + lengthB) != lengthC)||((C[0]!=A[0]) && (C[0]!=B[0])) ||((C[lengthC-1]!=A[lengthA-1]) && (C[lengthC-1]!=B[lengthB-1]))) {
                        outFile << "*** NOT A MERGE ***" << endl;
                        continue;
                }

                //fill in doubleMatrix with comparisions of A and B to C
                for (i = 0; i <= lengthB; ++i) {
                        for (j = 0; j <= lengthA; ++j) {
                                int l = i + j -1;
                                if(i == 0 && j == 0) {
                                        doubleMatrix[i][j] = true;
                                } else if(i == 0) {
                                        doubleMatrix[i][j] = (C[l] == A[j-1] ? doubleMatrix[i][j-1] : false);
                                } else if(j == 0) {
                                        doubleMatrix[i][j] = (B[i-1] == C[l] ? doubleMatrix[i-1][j] : false);
                                } else{
                                        doubleMatrix[i][j] = (B[i-1] == C[l] ? doubleMatrix[i-1][j] : false) || (A[j-1] == C[l] ? doubleMatrix[i][j-1] : false);
                                }
                        }
                }

                //capitalize if it is a valid merge
                if (doubleMatrix[lengthB][lengthA]==1) {
                        while (lengthA > 0 && lengthB >= 0) {
                                // move left if cant move up in matrix, otherwise move up
                                int traversal = lengthB + lengthA - 1;
                                if (doubleMatrix[lengthB][lengthA] && (lengthB == 0 || doubleMatrix[lengthB - 1][lengthA] != true)) {
                                        C[traversal] = toupper(C[traversal]);
                                        lengthA--;
                                } else{
                                        lengthB--;
                                }
                        }
                        outFile << C << endl;
                } else{
                        outFile << "*** NOT A MERGE ***" << endl;
                }
        }

        inFile.close();
        outFile.close();
        return 0;
}
