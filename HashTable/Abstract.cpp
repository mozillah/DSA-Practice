#include <iostream>
using namespace std;

class DataStructure{

     // Array is a reference to an array of Linked Lists.
    LinkedList * array;
    
    // Length is the size of the Hash Table array.
    int length;

 DataStructure(){
     int BUCKET;    // No. of buckets 
  
    // Pointer to an array containing buckets 
    list<int> *table; 

    HashTable::HashTable( int tableLength )
{
    if (tableLength <= 0) tableLength = 13;
    array = new LinkedList[ tableLength ];
    length = tableLength;
}
 }

}

int main() 
{ 
  // array that contains keys to be mapped 
  int a[] = {15, 11, 27, 8, 12}; 
  int n = sizeof(a)/sizeof(a[0]); 
  
  // insert the keys into the hash table 
  Hash h(7);   // 7 is count of buckets in 
               // hash table 
  for (int i = 0; i < n; i++)  
    h.insertItem(a[i]);   
  
  // delete 12 from hash table 
  h.deleteItem(12); 
  
  // display the Hash table 
  h.displayHash(); 
  
  return 0; 
} 