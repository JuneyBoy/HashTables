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
                dat.resize(item->get_key()+1);
        }
            dat.at(item->get_key()) = item;
        }

        char search_dat(int key){
            if(dat.at(key) == 0){
                return '*';
            }
            else{
                return dat.at(key)->get_data();
            }
        }
};


int main(){
    Item* item = new Item(3,'A');
    DirectAddressTable* dat = new DirectAddressTable();

    dat->add_item(item);
    cout << dat->search_dat(2);
}