// map <F4> :wa<CR>:!g++ main.cpp cbuf.c -o main;./main<CR>

#include <iostream>
#include <vector>
#include <numeric>   // iota
#include <algorithm> // shuffle
#include <random>    // mt, rand_dev
#include "cbuf.h"

std::vector<Item> item_seq(Item beg, Item end){
    std::vector<Item> vec(end);
    std::iota(vec.begin(), vec.end(), beg);
    return vec;
}

std::vector<Item> item_rand_seq(Item beg, Item end){
    auto vec = item_seq(beg, end);
    std::shuffle(
        vec.begin(), vec.end(), 
        std::mt19937{std::random_device{}()});
    return vec;
}

int main(int argc, char* argv[]){
    //auto vec = item_seq(0, 30);
    auto vec = item_rand_seq(0, 30);
    for(int i = 0; i < 30; i++){
        std::cout << vec[i] << ' ';
    }
    return 0;
}
