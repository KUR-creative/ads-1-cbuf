#include <gtest/gtest.h>
#include "cbuf.h"

TEST(test, print_coord) {
    YX xyb = {1,2};
    print_coord(&xyb);
    puts("");
}

TEST(q, push_and_pop) {
    Q q; q_init(&q);
    YX expected[3] = {{1,2}, {3,4}, {5,6}};
    for(int i = 0; i < 3; i++){
        q_push(&q, expected[i]);
    }
    for(int i = 0; i < 3; i++){
        YX actual = q_pop(&q);
        ASSERT_EQ(actual.y, expected[i].y);
        ASSERT_EQ(actual.x, expected[i].x);
    }
}

TEST(q, cannot_pop_from_empty_q) {
    Q q; q_init(&q);
    ASSERT_TRUE(q_empty(&q));
    YX actual = q_pop(&q);
    ASSERT_EQ(actual.y, NONE);
    ASSERT_EQ(actual.x, NONE);
}

TEST(q, cannot_push_to_full_q) {
    Q q; q_init(&q);
    YX yx = {0,1};
    for(int i = 1; i <= Q_SIZE - 1; i++){
        ASSERT_EQ(q_push(&q, yx), SUCCESS);
    }
    ASSERT_EQ(q_push(&q, yx), FAILURE);
}

TEST(q, circular_push) {
    Q q; q_init(&q);
    YX expected[Q_SIZE - 1] = {{9,9}, {0,0}};

    for(int i = 0; i < Q_SIZE - 1; i++){
        q_push(&q, expected[i]);
    }
    for(int i = 0; i < Q_SIZE - 1; i++){
        YX actual = q_pop(&q);
        ASSERT_EQ(actual.y, expected[i].y);
        ASSERT_EQ(actual.x, expected[i].x);
    }

    for(int i = 0; i < Q_SIZE - 1; i++){
        ASSERT_EQ(q_push(&q, expected[i]), SUCCESS);
    }
    for(int i = 0; i < Q_SIZE - 1; i++){
        YX actual = q_pop(&q);
        ASSERT_EQ(actual.y, expected[i].y);
        ASSERT_EQ(actual.x, expected[i].x);
    }

    YX yx = {1,1};
    for(int i = 0; i < Q_SIZE * 2; i++){
        q_push(&q, yx);
    }
    ASSERT_TRUE(q_full(&q));
    for(int i = 0; i < Q_SIZE * 2; i++){
        q_pop(&q);
    }
    ASSERT_TRUE(q_empty(&q));
}


//-------------------------------------------------------------------------------------
int main(int argc, char **argv) {
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
