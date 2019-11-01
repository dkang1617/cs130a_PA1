#include <string>
#include <vector>
using namespace std;

class Dictionary {
public:
    // Insert an input set of n keys to the dictionary. This method should print out the following information:
    // 1. The hash functions comprising the perfect hash (both levels)
    // 2. The sum of squares of the number of keys mapped to each bin of the first level hash function, and
    // 3. The number of trials needed to generate each hash function.
    void bulkInsert(int n, string *keys);

    // Insert a key to the dictionary.
    // Print out whether the insertion caused a collision in the second level hash.
    // Handle collision with separate chaining.
    void insert(string key);

    // Remove a key from the dictionary, if it exists.
    void remove(string key);

    // Return whether a key is found in the dictionary.
    // Print the buckets (both first and second level) accessed during the operation.
    bool find(string key);

    //make random matrix and print after
    void makeRandom(int nR, int nC);

    //make random matrix and return it
    vector<vector<int>> makeNewRandom(int nR, int nC);

    //format key and hash for first level, return index of hash  
    int firstLevelHash(string input, int nR, int nC);

    //format key and hash for second level, return index
    int secondLevelHash(string input, int nR, int nC, vector<vector<int>> mult);   

    //print hash
    void printHash();
	
private:
    vector<vector<int>> randomMat;
    vector<vector<string>> hashTable;
    vector<struct secondHash> secondHashRan;
};

struct secondHash {
	vector<vector<int>> randomMat2;
};

