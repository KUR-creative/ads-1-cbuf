// map <F4> :wa<CR>:!g++ main.cpp cbuf.c -o main;./main<CR>

#include <iostream>
#include <chrono>
#include <numeric>   // iota
#include <algorithm> // shuffle
#include <random>    // mt, rand_dev
#include <vector>
#include <queue>

#include "cbuf.h"

namespace crn = std::chrono;

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

#define EXPR(key, description, code) do{ \
    auto beg_clk = crn::steady_clock::now();        \
    code                                            \
    auto end_clk = crn::steady_clock::now();        \
    auto run_time = end_clk - beg_clk;              \
    std::cout <<                                    \
        key": " <<                                  \
        crn::duration<double,std::milli>(run_time)  \
        .count() << std::endl;                      \
}while(0)

// Circular buffer operates like queue.
int main(int argc, char* argv[]){
    /*
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
    */

    //int num_data = 10e8; // 14610.6 ms
    int num_data = 10e7; // 1461.6 ms
    auto seq = item_seq(0, num_data);

    // data.num: data 개수
    // data.seq: data 시퀀스
    std::cout << "data.num: " << num_data << "\n";
    /*
    std::cout << "data.seq: ";
    std::cout << "[";
    for(int i = 0; i < num_data; i++){
        std::cout << seq[i] << ',';
    }
    std::cout << "]" << std::endl;
    */

    {
        std::queue<Item> q;
        EXPR("q.push", "Queue push 수행 시간",
            for(int i = 0; i < num_data; i++){
                q.push(seq[i]);
            }
        );
        EXPR("q.pop", "Queue pop 수행 시간",
            for(int i = 0; i < num_data; i++){
                q.pop();
            }
        );
    }

    // cbuf.pop: Circular buffer pop 수행 시간

    // q=cbuf: Circular buffer가 Queue와 동일하게 작동하는가?
    
    // push/pop.seq: push/pop 혼합 시퀀스 
    // q.mixed: Queue push/pop 혼합 시퀀스 수행시간
    // cbuf.mixed: Queue push/pop 혼합 시퀀스 수행시간
    
    // q=cbuf: Circular buffer가 Queue와 동일하게 작동하는가?
    return 0;
}
