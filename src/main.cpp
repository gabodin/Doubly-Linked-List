#include "../include/list.h"

int main () {
    ls::list<int> exemplo( {1, 2, 3, 4} );

    std::cout << "Tamanho:" << exemplo.size();
}