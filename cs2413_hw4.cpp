#include <iostream>
#include <vector>

using namespace std;

class Item{
    private:
        int key;

        char data;

        Item* nextptr;

    public:
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

class DirectAddressTable{
    private:
        vector<Item*> dat;
    public:
        void add_item(Item* item){
            if(dat.size() < item->get_key()){
                dat.resize(item->get_key()+1, nullptr);
        }
            dat.at(item->get_key()) = item;
        }

        char search_dat(int key){
            if(dat.at(key) == nullptr){
                return '*';
            }
            else{
                return dat.at(key)->get_data();
            }
        }
};

class ModTable{
    private:
        vector<Item*> table;
        int modNum;
        int collisionResolution;
    public:
        
        ModTable(int modNum, int collisionResolution){
            this->modNum = modNum;
            this->collisionResolution = collisionResolution;
            table.resize(modNum+1, nullptr);
        }
        
        void add_item(Item* item){
            int insertIdx = item->get_key() % modNum;
            
            if(table.at(insertIdx) == nullptr){
                table.at(insertIdx) = item;
            }
            else{
                if(collisionResolution == 1){
                    this->chain(item, insertIdx);
                }
                else if (collisionResolution == 2){
                    this->quad_probe(item, insertIdx);
                }
            }
        }

        char search_mod_table(int key){
            int initIdx = key % modNum;

            if(collisionResolution == 1){
                Item* currItem = table.at(initIdx);
                while(currItem != nullptr){
                    if(currItem->get_key() == key){
                        return currItem->get_data();
                    }
                    currItem = currItem->get_next_item();
                }
                return '*';
            }
            else if (collisionResolution == 2) {
                int i = 0;
                int searchIdx = (initIdx + i + i*i) % table.size();
                
                while(table.at(searchIdx) != nullptr){
                    if(table.at(searchIdx)->get_key() == key){
                        return table.at(searchIdx)->get_data();
                    }
                    ++i;
                    searchIdx = (key + i + i*i) % table.size();
                }
                return '*';
            }
        }

        void chain(Item* item, int idx){
            table.at(idx)->set_next_item(item);
        }

        void quad_probe(Item* item, int idx){
            int i = 1;
            int calc_idx = (idx + i + i*i) % table.size();

            while(table.at(calc_idx) != nullptr){
                ++i;
                calc_idx = (idx + i + i*i) % table.size();
            }
            table.at(calc_idx) = item;
        }
};


int main(){
    int typeOfTable;
    vector<int> keys;
    vector<char> data;
    vector<int> searchKeys;
    vector<Item*> items;
    int modNum;

    int key;
    char datapt;
    int searchKey;
    cin >> key;

    while(key > -1 ){
        keys.push_back(key);
        cin >> key;
    }

    cin >> datapt;
    while(datapt != '*'){
        data.push_back(datapt);
        cin >> datapt;
    }

    cin >> typeOfTable;

    if(typeOfTable == 1 || typeOfTable == 2){
        cin >> modNum;
    }

    cin >> searchKey;

    while(searchKey != -1){
        searchKeys.push_back(searchKey);
        cin >> searchKey;
    }

    switch(typeOfTable){
        case 0:
            DirectAddressTable* dat = new DirectAddressTable();
            
            for(int i = 0; i < keys.size(); ++i){
                Item* item = new Item(keys.at(i), data.at(i));
                dat->add_item(item);
            }
            
            for(int j = 0; j < searchKeys.size(); ++j){
                cout << dat->search_dat(searchKeys.at(j));
            }
            break;
        
        case 1:
            ModTable* modTable = new ModTable(modNum, typeOfTable);
            
            for(int i = 0; i < keys.size(); ++i){
                Item* item = new Item(keys.at(i), data.at(i));
                modTable->add_item(item);
            }
            
            for(int j = 0; j < searchKeys.size(); ++j){
                cout << modTable->search_mod_table(searchKeys.at(j));
            }
            break;
    }

}