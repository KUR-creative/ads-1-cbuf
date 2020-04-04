// map <F4> :wa<CR>:!rm main;g++ main.cpp cbuf.c -o main;./main<CR>

#include <iostream>
#include <chrono>
#include <numeric>   // iota
#include <algorithm> // shuffle
#include <random>    // mt, rand_dev
#include <vector>
#include <queue>
#include <cstdlib>

#include "cbuf.h"

namespace crn = std::chrono;

std::vector<Item> item_seq(Item beg, Item end){
    std::vector<Item> vec(end);
    std::iota(vec.begin(), vec.end(), beg);
    return vec;
}

std::vector<Item> item_rand_seq(Item beg, Item end){
    auto vec = item_seq(beg, end);
    //=================================================
    auto beg_clk = crn::steady_clock::now();        
    //-------------------------------------------------
    auto rng = std::default_random_engine {};
    std::shuffle(
        vec.begin(), vec.end(), rng);
    //-------------------------------------------------
    auto end_clk = crn::steady_clock::now();       
    auto run_time = end_clk - beg_clk;            
    std::cout <<                                    
        " rand.seq.shuffle: " <<                                  
        crn::duration<double,std::milli>(run_time)  
        .count() << std::endl;                      
    //=================================================
    return vec;
}

#define EXPR(key, description, code) do{            \
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
    int num_data = 10e3;
    if(argc > 1){
        if(sscanf(argv[1], "%d", &num_data) == EOF){
            std::cout << "argv error: " << '\n';
            std::cout << argv[0] << ',' << argv[1] << '\n';
        }
    }
    auto items = item_seq(0, num_data);

    // data.num: data 개수
    // data.seq: data 시퀀스
    std::cout << "data.num: " << num_data << "\n";

    {
    std::vector<Item> q_result(num_data);
    std::vector<Item> cbuf_result(num_data);

    std::queue<Item> q;
    EXPR("q.push", "Queue push 수행 시간",
        for(int i = 0; i < num_data; i++){
            q.push(items[i]);
        }
    );
    int q_idx = 0;
    EXPR("q.pop", "Queue pop 수행 시간",
        for(int i = 0; i < num_data; i++){
            q_result[q_idx++] = q.front();
            q.pop();
        }
    );

    size_t buf_size = num_data;
    CirBuf cbuf; cbuf_init(&cbuf, buf_size);
    EXPR("cbuf.push", "Circular buffer push 수행 시간",
        for(int i = 0; i < num_data; i++){
            cbuf_push(&cbuf, items[i]);
        }
    );
    int b_idx = 0;
    EXPR("cbuf.pop", "Circular buffer pop 수행 시간",
        for(int i = 0; i < num_data; i++){
            Item poped = cbuf_pop(&cbuf);
            if(poped != NONE_ITEM){
                cbuf_result[b_idx++] = poped;
            }
        }
    );
    cbuf_deinit(&cbuf);

    // q=cbuf: Circular buffer가 Queue와 동일하게 작동하는가?
    //EXPR("q=cbuf","Circular buffer가 Queue와 동일하게 작동하는가?",
        for(int i = 0; i < num_data - 1; i++){
            if(q_result[i] != cbuf_result[i]){
                printf("q_result[%d] = %d != %d = cbuf_result[%d]", 
                       i, q_result[i], cbuf_result[i], i);
                exit(1);
            }
        }
    //);
    }
    
    {
    std::vector<Item> q_result(2 * num_data);
    std::vector<Item> cbuf_result(2 * num_data);

    std::random_device rd; 
    std::mt19937 mersenne(rd()); 
    std::uniform_int_distribution<> coin(0, 1); 

    std::vector<char> pp_seq (2 * num_data);
    for(int i = 0; i < 2 * num_data; i++){
        pp_seq[i] = (coin(mersenne) ? 'u' : 'o');
        //'u' means push, 'o' means pop
    }

    //=================================================
    auto beg_clk = crn::steady_clock::now();        
    //-------------------------------------------------

    //auto items = item_rand_seq(0, 2 * num_data);
    auto items = item_seq(0, 2 * num_data);

    //-------------------------------------------------
    auto end_clk = crn::steady_clock::now();       
    auto run_time = end_clk - beg_clk;            
    //std::cout << "seq.gen: " << crn::duration<double,std::milli>(run_time).count() << std::endl;                      
    //=================================================

    
    // 
    std::queue<Item> q;
    int q_idx = 0;
    EXPR("q.mixed", "Queue push/pop 혼합 시퀀스 수행시간",
        for(int i = 0; i < 2 * num_data; i++){
            if(pp_seq[i] == 'u'){
                q.push(items[i]);
            }else{
                if(! q.empty()){
                    q_result[q_idx++] = q.front();
                    q.pop();
                }
            }
        }
    );


    size_t buf_size = num_data * 0.7;
    CirBuf cbuf; cbuf_init(&cbuf, buf_size);
    int b_idx = 0;
    EXPR("cbuf.mixed", "Queue push/pop 혼합 시퀀스 수행시간",
        for(int i = 0; i < 2 * num_data; i++){
            if(pp_seq[i] == 'u'){
                cbuf_push(&cbuf, items[i]);
            }else{
                if(! cbuf_empty(&cbuf)){
                    Item poped = cbuf_pop(&cbuf);
                    if(poped != NONE_ITEM){
                        cbuf_result[b_idx++] = poped;
                    }
                }
            }
        }
    );

    // q=cbuf: Circular buffer가 Queue와 동일하게 작동하는가?
    //EXPR("q=cbuf.mixed","Circular buffer가 Queue와 동일하게 작동하는가?",
        for(int i = 0; i < num_data - 1; i++){
            if(q_result[i] != cbuf_result[i]){
                printf("q_result[%d] = %d != %d = cbuf_result[%d]", 
                       i, q_result[i], cbuf_result[i], i);
                exit(1);
            }
        }
    //);
    }

    return 0;
}
