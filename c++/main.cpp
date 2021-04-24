#include "hash.h"

int main(){
    Hash myMap;
    myMap.insert("d",1);
    myMap.insert("a",2);
    myMap.insert("a",52);
    myMap.insert("a",23);

    cout<<"x    " <<myMap.at("x")<<endl; 
    myMap.print();
    cout<<endl;

    myMap.insert("b",3);
    myMap.insert("c",4);
    myMap.insert("e",5);
    myMap.insert("f",6);
    myMap.insert("x",7);
    myMap.insert("x",77);
    myMap.insert("xx",8);

    myMap.print();
    
   cout<<"d value   "<<myMap.at("d")<<endl;

    // myMap.erase("d");
        
    // cout<<"erase d"<<endl;

    // myMap.print();
    // cout<<endl;
    // myMap.erase("xx");
    // myMap.print();
    // cout<<myMap["a"]<<endl;
    return 0;
}
