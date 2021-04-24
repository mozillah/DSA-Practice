#ifndef _HASH_H_
#define _HASH_H_
#include <iostream>
#include <vector>
#include <string>

using std::endl;
using std::vector;
using std::cout;
using std::cin;
using std::string;

struct Node{
    string key;
    int	   value;
    Node   *Next;
};

class Hash{
public:
    // constructor
    Hash();
    // copy constructor
    Hash(const Hash& orig);
    // destructor
    ~Hash();
    // searching element by key
    string at(const string & key);
    // delete element
    void erase(const string &key);
    // insert element
    void insert(const string &key, const int &value);
    // insert element
    // searching element by value
    string findvalue(int value);

    // print out hash table
    void print();
    // operator []
    int  operator[](const string &key);
private:    
    // calculate hash table index
    int	 hashing(const string &key, const int & TableSize);
    // calculate link list length
    int linkLength( Node *node);
    // increase hash table size
    // and rehash 
    void rehash();
 
    vector<Node *> table_; 
    int tableSize_;
    int elementNum;
};

#endif
