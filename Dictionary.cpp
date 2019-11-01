// Implement Dictionary methods.
// Daniel Kang
// 5876636

#include "Dictionary.h"
#include <iostream>
#include <string>
#include <cmath>
#include <bitset>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
using namespace std;

void Dictionary::bulkInsert(int n, string *keys) {

	int numRow = (ceil(log2(n)));
	int numCol = 64;
	int trialNumFirst = 1;
	int trialNumSecond = 1;
	bool repeat = true;
	while ( repeat ) {

		//make random matrix
		cout<<"\nThis is the first level hash function for trial #"<<trialNumFirst<<".\n\n";
		makeRandom(numRow, numCol);
			
		//check hashtable empty, clear if not
		if ( !( hashTable.empty() ) ) {
			hashTable.clear();
		}

		//create hashtable and list of randomMats for second level, given by 2^numRow
		vector<string> row;
		for ( int i = 0; i < (pow(2,numRow)); i++ ) {
			hashTable.push_back(row);
		}
		
		//loop through keys and hash first level
		for ( int i = 0; i<n; i++) {
			int ind = firstLevelHash(keys[i], numRow, numCol);
			hashTable[ind].push_back(keys[i]);		
		}
		
		//check condition on whether to repeat hash
		int summation = 0;
		for ( int i = 0; i < hashTable.size(); i++ ) {
			if(hashTable[i].size()==0){
				summation++;
			} else {
				summation = summation + (pow(hashTable[i].size(),2));
			}
		}

		cout << "\nThe sum of squares for trial #" << trialNumFirst << " is " << summation << ".\n\n";
		trialNumFirst++;

		if ( summation < 4*n ) {
			repeat = false;
			cout << "First level hash function passed!\n";
		} else {
			cout << "The sum of squares is greater than " << (4*n) << ". Moving to trial #" << trialNumFirst << ".\n";
		}
		
		//second level hashing
		//if first level cleared
		if ( !repeat ) {
			//loop through the hash table ROWS, i is bucket!!!
			for ( int i = 0; i < hashTable.size(); i++ ) {
				trialNumSecond = 1;
				bool repeatSecond = true;
				//allocate space n^2
				for ( int j = 0; j < ( 2*(ceil(log2( hashTable[i].size() ))))-hashTable[i].size(); j++ ) {
                                	hashTable[i].push_back("");
				}

				while ( repeatSecond ) {
					if ( hashTable[i].size() > 1 ) {
						bool myFlag = false;
						vector<string> newRow;
						newRow.clear();
						for ( int j = 0; j < hashTable[i].size(); j++) {
							newRow.push_back("");
						}
						int secNumRows = ( ceil(log2(hashTable[i].size())));
						cerr<<"\nThis is the second level hash function for bucket #" << i << ", trial #" << trialNumSecond<< "\n\n";
						vector<vector<int>> random = makeNewRandom(secNumRows, numCol);
						struct secondHash setValue;
						setValue.randomMat2 = random;
						for ( int j = 0; j < hashTable[i].size(); j++ ) {
							int ind = secondLevelHash(hashTable[i][j], secNumRows, numCol, random);
							if ( newRow[ind]!="" ) {
								myFlag = true;
							}
							newRow[ind] = hashTable[i][j];
						}	
						if ( !myFlag ) {
							repeatSecond = false;
							for ( int j = 0; j < hashTable[i].size(); j++ ) {
								hashTable[i][j] = newRow[j];
							}
							secondHashRan.push_back(setValue);
						}
						trialNumSecond++;
					} else {
						cout << "\nSecond level hash function not needed for bucket " << i << ".\n\n";
						struct secondHash empty;
						vector<vector<int>> r;
						vector<int> c;
						c.push_back(0);
						r.push_back(c);
						empty.randomMat2 = r;
						secondHashRan.push_back(empty);
						repeatSecond = false;
					}
				}
			}
		}
	}
}

void Dictionary::insert(string key) {
	//first level
	int numRows = randomMat.size();
	int numCol = 64;
	int firstIndex = firstLevelHash(key, numRows, numCol);

	if ( hashTable[firstIndex].size()== 0 ) {
		hashTable[firstIndex].push_back(key);
		cerr<< "Hashed into empty first level.\n";
	} else {
		cerr<< "Collision!" << endl;
		hashTable[firstIndex].push_back(key);
	}
}

void Dictionary::remove(string key) {

	int numRows = randomMat.size();
	int numCol = 64;
	int firstIndex = firstLevelHash(key, numRows, numCol);
	if ( hashTable[firstIndex].size() == 1 ) {
		if ( hashTable[firstIndex][0] == key ) {
			hashTable[firstIndex][0] = "";
			cerr<<"Removed element: " << key << endl;
		}
	} else {
		int secNumRows = (secondHashRan[firstIndex].randomMat2).size();
		int secondIndex = secondLevelHash(key, secNumRows, numCol, secondHashRan[firstIndex].randomMat2);
		bool removed = false;
			
		if ( hashTable[firstIndex][secondIndex]==key ) {
			hashTable[firstIndex][secondIndex] = "";
			cerr<<"Removed element: " << key << endl;
			removed = true;
		} else {
			for ( int i = secondIndex; i < hashTable[firstIndex].size(); i++ ) {
				if ( hashTable[firstIndex][i]==key ) {
					hashTable[firstIndex][i] = "";
					cerr<<"Removed element: " << key << endl;
					removed = true;
				}
			}
		}
	
		if ( !removed ) {
			cerr<<"Element: " << key << " does not exist!!!" << endl;
		}
	}
}

bool Dictionary::find(string key) {

	int numRows = randomMat.size();
	int numCol = 64;
	int firstIndex = firstLevelHash(key, numRows, numCol);
	bool found = false;
	if ( hashTable[firstIndex].size() == 1 ) {
		if ( hashTable[firstIndex][0] == key ) {
			found = true;
			cerr<<"Element: " << key << " found!" << endl;
			cerr<<"First index: " << firstIndex << endl;
			cerr<<"Second index: 0" << endl;
			return found;
		}
	}

        int secNumRows = (secondHashRan[firstIndex].randomMat2).size();
	int secondIndex = secondLevelHash(key, secNumRows, numCol, secondHashRan[firstIndex].randomMat2);

	if ( hashTable[firstIndex][secondIndex] == key ) {
		found = true;
		cerr<<"Element: " << key << " found!" << endl;
		cerr<<"First index: " << firstIndex << endl;
		cerr<<"Second index: " << secondIndex << endl;
		return found;
	} else {
		for ( int i = secondIndex; i < hashTable[firstIndex].size(); i++ ) {
			if ( hashTable[firstIndex][i]==key ) {
				found = true;
				cerr<<"Element: " << key << " found!"<<endl;
				cerr<<"First index: " << firstIndex << endl;
				cerr<<"Second index: " << i << endl;
				return found;
			}
		}
	}
	cerr<<"Element: " << key << " not found.\n";
	return found;
}
//this is the final take
//randomizes randomMat and prints;
void Dictionary::makeRandom(int nR, int nC) {
	srand(time(NULL));
	vector<int> row;
	for ( int i = 0; i < nR; i++ ) {
	       row.clear();
	       for ( int j = 0; j < nC; j++ ) {
		       int random = rand()%2;
		       row.push_back(random);
	       }
	       randomMat.push_back(row);
	}

	random_shuffle(randomMat.begin(), randomMat.end());

	//print random
	for ( int i = 0; i < nR; i++ ) {
		for ( int j = 0; j < nC; j++ ) {
			cout << randomMat[i][j];
			cout << " ";
		}
		cout << "\n";
	}


}

//returns a random vector of dimensions specified
//PLEASE NOTE: HAD TO USE SLEEP_FOR TO ENSURE ACTUAL DIFFERENT RANDOM MATRICES
vector<vector<int>> Dictionary::makeNewRandom(int nR, int nC) {
	srand(time(NULL));
	this_thread::sleep_for(chrono::milliseconds(1000));
	vector<int> row;
	vector<vector<int>> returnThis;
	for ( int i = 0; i < nR; i++ ) {
               row.clear();
	       for ( int j = 0; j < nC; j++ ) {
		       int random = rand()%2;
		       row.push_back(random);
	       }
	       random_shuffle(row.begin(), row.end());
	       random_shuffle(row.begin(), row.end());
	       returnThis.push_back(row);
	}

	//print the random matrix
	for ( int i = 0; i < nR; i++ ) {
		for ( int j = 0; j < nC; j++ ) {
			cout<<returnThis[i][j];
			cout<<" ";
		}
		cout<< "\n";
	}
	cout << "\n";

	return returnThis;
}


//returns index of first hash bucket
int Dictionary::firstLevelHash(string input, int nR, int nC) {
	
	if ( input.size() < 8 ) {
		for( int p = 0; p < 8-(input.size()); p++) {
			input = " " + input;
                }
        }

	string binStr = "";
	for ( char& chr : input) {
		binStr += bitset<8>(chr).to_string();
	}

	vector<int> keyVector;

	for ( int i = 0; i < binStr.size(); i++ ) {
		string substr = binStr.substr(i, 1);
		int val = stoi(substr);
		keyVector.push_back( val );
	}
	
	vector<int> hashFinalValue;
	
	for ( int j = 0; j < nR; j++ ) {
		int hashVal = 0;
		for ( int p = 0; p < nC; p++ ) {
			hashVal = hashVal + (randomMat[j][p] * keyVector[p]);
		}
		hashVal = hashVal % 2;
		hashFinalValue.push_back( hashVal );
	}
	
	int index = 0;
	int q = 1;
	for ( int m = 0; m < hashFinalValue.size(); m++ ) {
		index += hashFinalValue[m] * ( pow(2,(hashFinalValue.size()-q)) );
		q++;
	}

	return index;
}

//return index given hash function mult
int Dictionary::secondLevelHash(string input, int nR, int nC, vector<vector<int>> mult) {
	if ( input.size() < 8 ) {
		for( int p = 0; p < 8-(input.size()); p++) {
			input = " " + input;
		}
	}


	string binStr = "";
	
	for ( char& chr : input) {
		binStr += bitset<8>(chr).to_string();
	}


	vector<int> keyVector;
	
	for ( int i = 0; i < binStr.size(); i++ ) {
		string substr = binStr.substr(i, 1);
		int val = stoi(substr);
		keyVector.push_back( val );
	}
	
	vector<int> hashFinalValue;
	
	for ( int j = 0; j < nR; j++ ) {
		int hashVal = 0;
		for ( int p = 0; p < nC; p++ ) {
			hashVal = hashVal + (mult[j][p] * keyVector[p]);
		}
		hashVal = hashVal % 2;
		hashFinalValue.push_back( hashVal );
	}
	
	int index = 0;
	int q = 1;
	for ( int m = 0; m < hashFinalValue.size(); m++ ) {
		index += hashFinalValue[m] * ( pow(2,(hashFinalValue.size()-q)) );
		q++;
	}
	
	return index;
}

//prints hashTable
void Dictionary::printHash() {
	
	for ( int i = 0; i < hashTable.size(); i++ ) {
		cout << " On Row " << i << " ";
		for ( int j = 0; j < hashTable[i].size(); j++ ) {
			
			cout << hashTable[i][j];
			cout << " | ";
		}
		
		cout << "\n";
	}

}

