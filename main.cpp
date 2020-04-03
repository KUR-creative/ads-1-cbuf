// map <F4> :wa<CR>:!g++ main.cpp cbuf.c -o main;./main<CR>

#include <iostream>
#include <numeric>   // iota
#include <algorithm> // shuffle
#include <random>    // mt, rand_dev
#include <vector>
#include <queue>
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

// Circular buffer operates like queue.
int main(int argc, char* argv[]){
    //auto vec = item_seq(0, 30);
    auto vec = item_rand_seq(0, 30);
    for(int i = 0; i < 30; i++){
        std::cout << vec[i] << ' ';
    }
    std::cout << std::endl;

    std::queue<Item> q;
    for(int i = 0; i < 30; i++){
        q.push(vec[i]);
        std::cout << q.back() << ' ';
    }

    // data.num: data 개수
    // data.seq: data 시퀀스
    
    // q.push: Queue push 수행 시간
    // q.pop: Queue pop 수행 시간
    
    // cbuf.push: Circular buffer push 수행 시간
    // cbuf.pop: Circular buffer pop 수행 시간

    // q=cbuf: Circular buffer가 Queue와 동일하게 작동하는가?
    
    // push/pop.seq: push/pop 혼합 시퀀스 
    // q.mixed: Queue push/pop 혼합 시퀀스 수행시간
    // cbuf.mixed: Queue push/pop 혼합 시퀀스 수행시간
    
    // q=cbuf: Circular buffer가 Queue와 동일하게 작동하는가?
    return 0;
}
