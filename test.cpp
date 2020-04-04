// map <F8> :wa<CR>:!rm test;g++ test.cpp cbuf.c -o test -lgtest;./test<CR>

#include <gtest/gtest.h>
#include "cbuf.h"

TEST(test, print_item) {
    Item xyb = 1;
    print_item(&xyb);
    puts("");
}

TEST(cbuf, push_and_pop) {
    size_t BUF_SIZE = 100;
    CirBuf cbuf; cbuf_init(&cbuf, BUF_SIZE);
    Item expected[3] = {1, 3, 5};
    for(int i = 0; i < 3; i++){
        cbuf_push(&cbuf, expected[i]);
    }
    for(int i = 0; i < 3; i++){
        Item actual = cbuf_pop(&cbuf);
        ASSERT_EQ(actual, expected[i]);
    }
}

TEST(cbuf, cannot_pop_from_empty_cbuf) {
    size_t BUF_SIZE = 100;
    CirBuf cbuf; cbuf_init(&cbuf, BUF_SIZE);
    ASSERT_TRUE(cbuf_empty(&cbuf));
    Item actual = cbuf_pop(&cbuf);
    ASSERT_EQ(actual, NONE_ITEM);
}

TEST(cbuf, cannot_push_to_full_cbuf) {
    size_t BUF_SIZE = 100;
    CirBuf cbuf; cbuf_init(&cbuf, BUF_SIZE);
    Item yx = 0;
    for(int i = 1; i <= BUF_SIZE - 1; i++){
        ASSERT_EQ(cbuf_push(&cbuf, yx), SUCCESS);
    }
    ASSERT_EQ(cbuf_push(&cbuf, yx), FAILURE);
}

TEST(cbuf, circular_push) {
    size_t BUF_SIZE = 100;
    CirBuf cbuf; cbuf_init(&cbuf, BUF_SIZE);
    Item expected[BUF_SIZE - 1] = {9, 0};

    for(int i = 0; i < BUF_SIZE - 1; i++){
        cbuf_push(&cbuf, expected[i]);
    }
    for(int i = 0; i < BUF_SIZE - 1; i++){
        Item actual = cbuf_pop(&cbuf);
        ASSERT_EQ(actual, expected[i]);
    }

    for(int i = 0; i < BUF_SIZE - 1; i++){
        ASSERT_EQ(cbuf_push(&cbuf, expected[i]), SUCCESS);
    }
    for(int i = 0; i < BUF_SIZE - 1; i++){
        Item actual = cbuf_pop(&cbuf);
        ASSERT_EQ(actual, expected[i]);
    }

    Item yx = 1;
    for(int i = 0; i < BUF_SIZE * 2; i++){
        cbuf_push(&cbuf, yx);
    }
    ASSERT_TRUE(cbuf_full(&cbuf));
    for(int i = 0; i < BUF_SIZE * 2; i++){
        cbuf_pop(&cbuf);
    }
    ASSERT_TRUE(cbuf_empty(&cbuf));
}


//-------------------------------------------------------------------------------------
int main(int argc, char **argv) {
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
