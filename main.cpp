#include <iostream>
#include <utility>
#include <memory>
#include <iterator>
#include <algorithm>
#include <vector>

#include "include/BST.h"


int main(){

    using BST = BST<int,int>;
    using PairType = std::pair<const int,int>;

    BST bst{};
    PairType a{8,1};
    PairType b{3,4};
    PairType c{10,55};
    PairType d{1,55};
    PairType e{6,55};
    PairType j{4,11};
    PairType f{7,33};
    PairType g{14,33};
    PairType h{13,23};

    // Testing insert and emplace
    std::cout<<"\nInserting pairs into BST...\n\n";
    std::cout<<"Inserting L-value of std::pair<const int, int>. I expect L-value ctor to be called:\n";
    auto tmp = bst.insert(a);
    std::cout<<"\n";
    std::cout<<"Inserting R-value of std::pair<const int, int>. I expect R-value ctor to be called.\n";
    tmp = bst.insert({3,4});
    std::cout<<"\n";
    std::cout<<"Emplacing value into BST.\n";
    tmp = bst.emplace(10,55);
    tmp = bst.emplace(d);
    std::cout<<"\n";
    std::cout<<"Inserting other values...\n";
    tmp = bst.insert(e); 
    tmp = bst.insert(j);
    tmp = bst.insert(f);
    tmp = bst.insert(g);
    tmp = bst.insert(h);
    std::cout<<"\n\n"<<std::endl;

    // testing range-for loop and put-to operator
    std::cout<<"Printing the BST with range for loop:\n";
    for (auto& x: bst)
        std::cout<<x.first<<" ";
    std::cout<<std::endl;
    std::cout<<"Printing the BST using put-to operator:\n";
    std::cout<<bst<<std::endl;
    std::cout<<"\n\n"<<std::endl;
    
    // Testing find
    std::cout<<"Finding some nodes..."<<std::endl;
    { 
    auto finder = bst.find(6); 
    if(finder)
        std::cout<<"Find successful! Wanted to find 6 and found: \t"<<finder->first<<std::endl;
    else
        std::cout<<"Tried to find 6 but this key doesn't exist."<<std::endl;
    }


    { 
    auto finder = bst.find(13); 
    if(finder)
        std::cout<<"Find successful! Wanted to find 13 and found: \t"<<finder->first<<std::endl;
    else
        std::cout<<"Tried to find 13 but this key doesn't exist."<<std::endl;
    }

    { 
    auto finder = bst.find(12); 
    if(finder){ 
        std::cout<<"Find successful! Wanted to find 12 and found: \t"<<finder->first<<std::endl;
    }
    else
        std::cout<<"Tried to find 12 but this key doesn't exist."<<std::endl;

    }

    { 
    auto finder = bst.find(5); 
    if(finder){ 
        std::cout<<"Find successful! Wanted to find 5 and found: \t"<<finder->first<<std::endl;
    }
    else
        std::cout<<"Tried to find 5 but this key doesn't exist."<<std::endl;

    }

    std::cout<<"\n\n"<<std::endl;
    //Testing balance.

    std::cout<<"Balancing the tree...\n";
    bst.balance();

    // testing subscripting operator
    std::cout<<"Subscripting some values"<<std::endl;
    {
    std::cout<<"bst[3]= "<<bst[3]<<"\n";
    std::cout<<"bst[9]= "<<bst[9]<<"\n";
    auto tmpint = 10;
    std::cout<<"bst[10]= "<<bst[tmpint]<<"\n";
    }
    std::cout<<std::endl;

    // testing copy semantics
    std::cout<<"Copying the tree with copy ctor:\n"<<std::endl;
    {
    BST bst2{bst};

    std::cout<<"BST1 is: "<<bst<<"BST2 is: "<<bst2;
    std::cout<<"Inserting -2 and 11 insto BST1\n";
    std::cout<<"Inserting -5, 2 and 21 into BST2\n";

    bst.emplace(-2,2);
    bst.emplace(11,3);
    bst2.emplace(-5,22);
    bst2.emplace(2, 2);
    bst2.emplace(21,0);
    std::cout<<"after insertion:\n";
    std::cout<<"BST1 is: "<<bst<<"BST2 is: "<<bst2<<"\n\n"<<std::endl;
    }

    std::cout<<"Copying tree with copy assignment:\n"<<std::endl;

    {
    BST bst2;
    bst2 = bst;

    std::cout<<"BST1 is: "<<bst<<"BST2 is: "<<bst2;
    std::cout<<"Inserting -2 and 11 insto BST1\n";
    std::cout<<"Inserting -5, 2 and 21 into BST2\n";

    bst.emplace(-2,2);
    bst.emplace(11,3);
    bst2.emplace(-5,22);
    bst2.emplace(2, 2);
    bst2.emplace(21,0);
    std::cout<<"after insertion:\n";
    std::cout<<"BST1 is: "<<bst<<"BST2 is: "<<bst2<<"\n\n"<<std::endl;

    std::cout<<"Clearing the copy of the BST...\n"<<std::endl;
    bst2.clear();
    std::cout<<"BST2 is: "<<bst2<<std::endl;
    }


    // Testing Move semantics
    std::cout<<"Copying BST with move ctor:\n"<<std::endl;
    {
    BST bst2{bst};
    BST bst3{std::move(bst2)};
    std::cout<<"BST2 is: "<<bst3<<"\n\n"<<std::endl;

    std::cout<<"Copying BST with move assignment:\n"<<std::endl;
    BST bst4{};
    bst4 = std::move(bst3);
    std::cout<<"BST2 is: "<<bst4<<"\n\n"<<std::endl;
    bst3.clear();
    bst4.clear();
    }

    // testing erase
    std::cout<<"Copying BST1 and erasing some keys from BST and BST2\n"<<std::endl;
    BST bst2{bst};
    std::cout<<"BST1 is: "<<bst<<std::endl;
    std::cout<<"BST2 is: "<<bst2<<std::endl;
    bst.erase(3);
    bst.erase(7);
    bst.erase(11);
    bst.erase(13);
    bst.erase(-2);
    bst.erase(4);
    bst2.erase(10);
    bst2.erase(3);
    std::cout<<"After erase:\n";
    std::cout<<"BST1 is: "<<bst<<"BST2 is: "<<bst2<<"\n\n"<<std::endl;

    
    return 0;
}