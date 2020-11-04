#include <iostream>
#include <vector>

using namespace std;

//object that will be stored in hash tables
class Item{
    private:
        //identifier
        int key;

        char data;
        //points to subsequent Item
        //used for chaining collision resolution
        Item* nextptr;

    public:
        //constructor takes key and data and initializes nextptr to null
        Item(int key, char data){
            this->key = key;
            this->data = data;
            this->nextptr = nullptr;
        }

        int get_key(){
            return key;
        }

        int get_data(){
            return data;
        }

        Item* get_next_item(){
            return this->nextptr;
        }

        void set_key(int key){
            this->key = key;
        }

        void set_data(char data){
            this->data = data;
        }

        void set_next_item(Item* nextItem){
            this->nextptr = nextItem;
        }
};

//DirectAddressTable(DAT) stores each Item at the exact index of the key, resulting in constant lookup time in all scenarios
class DirectAddressTable{
    private:
        //vector to store Items
        vector<Item*> dat;
    public:
        //adds Item to DAT
        void add_item(Item* item){
            //if the size of the DAT is less than the key, then the DAT is resized
            if(dat.size() < item->get_key()){
                dat.resize(item->get_key()+1, nullptr);
        }
            dat.at(item->get_key()) = item;
        }

        //given a key, will return the matching data if the key exists in the DAT
        char search_dat(int key){
            //if the key is not in the DAT, then an asterisk is returned
            if(dat.at(key) == nullptr){
                return '*';
            }
            else{
                return dat.at(key)->get_data();
            }
        }
};

//ModTable utilizes the modulus function to place Items in the table, minimizing lost space for the table if there are large gaps between keys
//However, multiple different keys may be placed into the same index of the table so two collision resolution methods are used: chaining and quadratic probing
class ModTable{
    private:
        //vector to store Items
        vector<Item*> table;
        //number that will act as the modulus
        int modNum;
        //number to represent which collision resolution method shall be used
        int collisionResolution;
    
    public:
        //constructor takes in modNum and collisionResolution and initializes table with modNum nullptrs
        ModTable(int modNum, int collisionResolution){
            this->modNum = modNum;
            this->collisionResolution = collisionResolution;
            //if modNum is 5, there only needs to be 5 indices of the vector (0-4) as no number mod 5 will have a remainder greater than 4
            table.resize(modNum, nullptr);
        }
        
        //adds Item to ModTable
        void add_item(Item* item){
            //takes the mod of the key and stores the result as the index the Item should be inserted at
            int insertIdx = item->get_key() % modNum;
            
            //if there does not exist an Item at insertIdx, then the Item is inserted at insertIdx
            if(table.at(insertIdx) == nullptr){
                table.at(insertIdx) = item;
            }
            else{
                //if there is an Item at insertIdx and collisionResolution is one, then chaining is used to resolve the collision
                if(collisionResolution == 1){
                    this->chain(item, insertIdx);
                }
                //if there is an Item at insertIdx and collisionResolution is two, then quadratic probing is used to resolve the collision
                else if (collisionResolution == 2){
                    this->quad_probe(item, insertIdx);
                }
            }
        }

        //given a key, searches the ModTable to find matching data
        char search_mod_table(int key){
            //takes the mod of the key and stores the result as the index so it can be searched first
            int initIdx = key % modNum;

            //if collisionResolution is 1(meaning chaining), then...
            if(collisionResolution == 1){
                //stores pointer to first Item found at initIdx
                Item* currItem = table.at(initIdx);
                //while currItem is not a nullptr...
                while(currItem != nullptr){
                    //if currItem's key matches the search key, then its data is returned and the loop breaks
                    if(currItem->get_key() == key){
                        return currItem->get_data();
                    }
                    //otherwise, currItem moves to the next linked Item
                    currItem = currItem->get_next_item();
                }
                //key must not exist in list so an asterisk is returned
                return '*';
            }
            //if collisionResolution is 2(meaning quadratic probing), then...
            else {
                //initializes counter for use in quadratic hashing function
                int i = 0;
                //initializes searchIdx
                int searchIdx = (initIdx + i + i*i) % table.size();
                
                //while the Item at searchIdx is not a nullptr...
                while(table.at(searchIdx) != nullptr){
                    //if the key at searchIdx matches the search key, then its data is returned and the loop breaks
                    if(table.at(searchIdx)->get_key() == key){
                        return table.at(searchIdx)->get_data();
                    }
                    //otherwise the counter is incremented and searchIdx is recalculated
                    ++i;
                    searchIdx = (key + i + i*i) % table.size();
                }
                //key must not exist in list so an asterisk is returned
                return '*';
            }
        }

        //collision resolution method in which Items placed at the same index of the table form a linked list, with the index pointing to the head of the linked list
        void chain(Item* item, int idx){
            //initialized to the head of the linked list at idx
            Item* currItem = table.at(idx);
            //finds the end of the linked list
            while(currItem->get_next_item() != nullptr){
                currItem = currItem->get_next_item();
            }
            //sets last node of the linked list as item
            currItem->set_next_item(item);
        }

        //collision resolution method in which the index at which Items are stored continues to change until an empty index is found
        void quad_probe(Item* item, int idx){
            //i initialized to 1 as this method is only called if a collision is found, meaning i=0 was already checked
            int i = 1;
            //index to check and see if it is empty
            int calcIdx = (idx + i + i*i) % table.size();

            //while there is an Item at calcIdx...
            while(table.at(calcIdx) != nullptr){
                //increment i
                ++i;
                //recalculate calcIdx
                calcIdx = (idx + i + i*i) % table.size();
            }
            //empty index was found and item is stored there
            table.at(calcIdx) = item;
        }
};


int main(){
    //variable to store if table is DAT, ModTable with chaining, or ModTable with quad probing
    int typeOfTable;
    //vector to store input keys
    vector<int> keys;
    //vector to store input data
    vector<char> data;
    //vector to store search keys
    vector<int> searchKeys;
    //vector to store items that will be created from the corresponding keys and data
    vector<Item*> items;
    //variable to store the mod number
    int modNum;

    //variable to store current input key
    int key;
    //variable to store current data point
    char datapt;
    //variable to store current search key
    int searchKey;
    
    //takes in the first key
    cin >> key;
    //key input sequence ends when there is a -1 in the stream
    while(key > -1 ){
        //adds key to keys vector
        keys.push_back(key);
        //gets next key
        cin >> key;
    }

    //takes in first data point
    cin >> datapt;
    //data input sequence ends when there is a * in the stream
    while(datapt != '*'){
        //adds data point to data vector
        data.push_back(datapt);
        //gets next data point
        cin >> datapt;
    }

    //takes in the table type
    cin >> typeOfTable;
    //if the table is not a DAT, then there will be a mod number in the stream
    if(typeOfTable == 1 || typeOfTable == 2){
        cin >> modNum;
    }

    //takes in first search key
    cin >> searchKey;
    //data input sequence ends when there is a * in the stream
    while(searchKey != -1){
        //adds search key to search key vector
        searchKeys.push_back(searchKey);
        //gets next search key
        cin >> searchKey;
    }

    //based on what table was asked to be created...
    switch(typeOfTable){
        //if table is a DAT...
        case 0: {
            //DAT created
            DirectAddressTable* dat = new DirectAddressTable();
            //for the length of the key vector...
            for(int i = 0; i < keys.size(); ++i){
                //create new Item with the corresponding key and datapt
                Item* item = new Item(keys.at(i), data.at(i));
                //add item to DAT
                dat->add_item(item);
            }
            //for the length of the search keys vector...
            for(int j = 0; j < searchKeys.size(); ++j){
                //print out the data that was found
                cout << dat->search_dat(searchKeys.at(j));
            }
            break;
        }
        //if table was not a DAT
        case !0: {
            //ModTable created with the appriopriate modNum and collision resolution
            ModTable* modTable = new ModTable(modNum, typeOfTable);
            //for the length of the key vector...
            for(int i = 0; i < keys.size(); ++i){
                //create new Item with the corresponding key and datapt
                Item* item = new Item(keys.at(i), data.at(i));
                //add item to ModTable
                modTable->add_item(item);
            }
            //for the length of the search keys vector...
            for(int j = 0; j < searchKeys.size(); ++j){
                //print out the data that was found
                cout << modTable->search_mod_table(searchKeys.at(j));
            }
            break;
        }
    }

}