#include "StackAllocator.h"
#include <vector>
#include <iostream>



int main()
{	

    std::vector<int> coolVector = { 0, 1, 4, 5, 2 };

    auto it1 = coolVector.begin();
    

    auto it2 = coolVector.begin();
    it2++;

    coolVector.insert(it1, 7);
    (*it2);


	return 0;
}