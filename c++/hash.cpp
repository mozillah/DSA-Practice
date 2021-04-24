#include "hash.h"
/********************************
* default constuctor
********************************/
Hash::Hash(){
    tableSize_ = 17;
    Node *init;
    init = NULL;
    for (int i = 0; i<tableSize_; i++){
        table_.push_back(init);
    }
    elementNum = 0;
}
/*********************************
* destructor
*********************************/
Hash::~Hash(){
    Node *buf;
    for (int i=0; i< tableSize_;i++){
        buf = table_.at(i);
        while (table_.at(i)!=NULL){
            table_.at(i) = table_.at(i)->Next;
            delete buf;
            buf = table_.at(i);
        }
        buf = NULL;
    }
}
/*********************************
*   copy constructor
*********************************/
Hash::Hash(const Hash& orig):tableSize_(orig.tableSize_),elementNum(orig.elementNum)
{
    Node *init;
    init = NULL;
    for (int i = 0; i<orig.tableSize_; i++){
        this->table_.push_back(init);
    }
    Node *node = new Node;
    Node *temp ;
    for (int i = 0; i<this->tableSize_; i++){
        for(Node *buf = orig.table_.at(i);buf!= NULL; buf= buf->Next){
            node->value = buf->value;
            node->key   = buf->key;
            if (table_.at(i)==NULL){
                table_.at(i)=node;
            }
            else{
		// let temp point to the last node
		// of the list
                for( temp= this->table_.at(i);
                    temp->Next != NULL;
                    temp = temp->Next){}
                temp->Next = node;
            }
            node->Next = NULL;
        }
    }
}

/*******************************
 * insert element into
 * hash table
 *******************************/
void Hash::insert(const string &key, const int &value){
    int index = hashing(key, tableSize_);
    Node *buf = new Node;
    buf->key = key;
    buf->value = value;
    Node *cur = table_.at(index);
    if (cur == NULL){
        table_.at(index) = buf;
        table_.at(index)->Next = NULL;
        elementNum ++;
        if (elementNum >= tableSize_* 0.75){ 
	    //rehashing
            rehash();
        }
    }
    else {
        while (cur->Next != NULL){
            cur = cur->Next;
        }
        cur->Next = buf;
        buf->Next = NULL;
        if (linkLength(table_.at(index)) >= 4){
            //rehashing
            rehash();
        }
    }
}

/********************************
 * calculate link list length
 ********************************/
int Hash::linkLength( Node *node){
    Node *buf;
    int length = 0;
    for (buf = node; buf !=NULL; buf = buf-> Next){
        length++;
    }
    return length;
}


/*******************************
 * hash function
 * calculate hash table index
 *******************************/
int Hash::hashing(const string &key, const int & TableSize){
    int num = 0;
    for (int i = 0; i< key.length(); i++){
        num += key[i]*7;
    }
    num = num % TableSize;
    return num;
}
/*******************************
 * searching element by key
 *******************************/
string Hash::at(const string & key){
    int index = hashing(key,tableSize_);
    string str;
    if (table_.at(index)== NULL)
	return "NOT FOUND";
    else{
        for (Node *buf = table_.at(index); buf!=NULL; buf = buf -> Next){
            if (buf->key == key){
		str = std::to_string(buf->value);
		return str;
            }
        }
	return "NOT FOUND";
    }
}






/*******************************
 * searching element by key
 *******************************/
string Hash::findvalue( int value){
    string str;
        for (int i = 0; i<tableSize_; i++){
            for (Node * buf = table_.at(i); buf!= NULL; buf = buf->Next){
                if (buf->value == value){
                    str = std::to_string(buf->value);
                    string strs = buf->key;
                    cout<<"key "<<buf->key<<" value "<<buf->value<<endl;
                    return strs;
            }
                
            }
        }
    return "NOT FOUND";
    
}
/*********************************
 * erase element
 *********************************/
void Hash::erase(const string & key){
    int index = hashing (key, tableSize_);
    Node *buf = table_.at(index);
    int length = linkLength(table_.at(index));
    if (length == 0){}
    else if (length == 1){
        if (key == buf->key){
            delete buf;
            table_.at(index) = NULL;
        }
    }
    else if (length == 2){
	if (key == buf->key){
	    table_.at(index) =table_.at(index)->Next;
	    delete buf;
	}
	else{
	    buf= table_.at(index)->Next;
	    if (key == buf->key){
		delete buf;
		table_.at(index) ->Next = NULL;
	    }    
	}
    }
    else {
        Node *temp = table_.at(index)->Next;
        for (buf= table_.at(index); buf->Next->Next!=NULL; buf = buf ->Next){
            for (temp = table_.at(index)->Next; temp->Next != NULL; temp = temp ->Next){
		for (Node *buffer = table_.at(index)->Next->Next;
		     buffer!=NULL; buffer = buffer ->Next){
		    if (buf->key == key){
			table_.at (index) = temp;
			delete buf;
			return;
		    }
		    if(temp->key == key){
			buf->Next = buffer;
			delete temp;
			return;
		    }

		}
            }
        }
    }
}

/**********************************
 *print hash table
 **********************************/
void Hash::print(){
    if (elementNum != 0){
        for (int i = 0; i<tableSize_; i++){
            for (Node * buf = table_.at(i); buf!= NULL; buf = buf->Next){
                cout<<"key "<<buf->key<<" value "<<buf->value<<endl;
            }
        }
    }
}
/*********************************
 * rehash
 *********************************/
void Hash::rehash(){
    int oldtable = tableSize_;
    tableSize_ = tableSize_* 2 + 1;
    vector<Node *> newTable;
    Node *init;
    init = NULL;
    for (int i = 0; i<tableSize_; i++){
        newTable.push_back(init);
    }
    
    int index;
    int newElementNum =0;
    // copy old table content to new hash table
    for (int i = 0; i<oldtable;i++){
         Node *buf = table_.at(i);
         if (buf != NULL){
            Node *temp = new Node;
            Node *connect ;
            Node *buffer ;
	            
            connect = buf;
            index = hashing(connect->key, tableSize_);
            while(connect !=NULL){
                    
                    temp = connect;
                    connect  = connect->Next;
                    if (newTable.at(index)==NULL){
                        newTable.at(index) = temp;
                        temp->Next = NULL;
			newElementNum ++;
                    }
                    else{
                        for (buffer= newTable.at(index); buffer->Next !=NULL;
                             buffer = buffer -> Next){}
                        buffer->Next = temp;
                        temp->Next = NULL;
                    }
                    if (connect != NULL)
                        index = hashing(connect->key, tableSize_);
            }
        }
        
    }
    elementNum = newElementNum;
    table_ = newTable;
}

/*************************************
* operator []
*************************************/
int   Hash::operator[](const string &key){
    string buf = at(key);
    int num;
    num= stoi(buf);
    return num;
}
