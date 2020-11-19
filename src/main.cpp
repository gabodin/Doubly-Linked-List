#include "../include/list.h"

int main () {
    ls::list<int> exemplo( {1, 2, 3, 4} );
    ls::list<int> myList = {1, 2, 3, 4, 5};

    myList.assign( {1, 2, 3} );

    std::cout << "Tamanho: " << myList.size();
    
    myList.assign(7, 15);
    myList.push_front(22);
    myList.pop_front();
    auto it = myList.front();

    std::cout << "\nTamanho: " << myList.size();

    std::cout << "\nPrimeiro elemento: " << it;

    myList.push_back(4);
    myList.pop_back();
    it = myList.back();

    std::cout << "\nUltimo elemento: " << it;

    auto x = myList.begin();
    auto y = myList.end();

    // myList.erase(x , y);

    // std::cout << "\nTamanho: " << myList.size();

    myList.insert(y, 33);

    it = myList.back();

    std::cout << "\nUltimo elemento: " << it  << "\n";

    myList.print();

    





}