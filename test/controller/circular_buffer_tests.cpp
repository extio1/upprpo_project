#include <gtest/gtest.h>

#define private public
#include "executor/utils/circular_buffer.hpp"

TEST(CircularBufferCircularValueConstruct, WhenCalled_WithProperArguments_Correct) {
    for (std::size_t current_value = 0; current_value < 100; ++current_value){
        for (std::size_t max_value = current_value; max_value < 100; ++max_value) {
            auto cv = CircularBuffer<int>::CircularValue(current_value, max_value);
            cv.val = current_value;
            cv.max_value = max_value;
        }
    }
}

TEST(CircularBufferCircularValuePreIncrement, WhenWrapAround_Correct) {
    auto cv = CircularBuffer<int>::CircularValue(0, 42);
    for(int i = 0; i < 1000; ++i){
        ASSERT_EQ(++cv, (i+1)%42);
    }
}

TEST(CircularBufferCircularValuePostIncrement, WhenWrapAround_Correct) {
    auto cv = CircularBuffer<int>::CircularValue(0, 42);
    for(int i = 0; i < 1000; ++i){
        ASSERT_EQ(cv++, i%42);
    }
}

TEST(CircularBufferCircularValuePreDecrement, WhenWrapAround_Correct) {
    auto cv = CircularBuffer<int>::CircularValue(0, 42);
    ASSERT_EQ(--cv, 41);
    for(int i = 40; i >= 0; --i){
        ASSERT_EQ(--cv, i);
    }
    ASSERT_EQ(--cv, 41);
}

TEST(CircularBufferCircularValuePostDecrement, WhenWrapAround_Correct) {
    auto cv = CircularBuffer<int>::CircularValue(0, 42);
    ASSERT_EQ(cv--, 0);
    for(int i = 41; i > 0; --i){
        ASSERT_EQ(cv--, i);
    }
    ASSERT_EQ(cv--, 0);
    ASSERT_TRUE(cv==41);
}

TEST(CircularBufferLimitedValuePreIncrement, WhenWrapAround_Correct) {
    auto cv = CircularBuffer<int>::LimitedValue(0, 42);
    for(int i = 0; i < 1000; ++i){
        ASSERT_EQ(++cv, (i+1>=42) ? 42 : i+1);
    }
}

TEST(CircularBufferLimitedValuePostIncrement, WhenWrapAround_Correct) {
    auto cv = CircularBuffer<int>::LimitedValue(0, 42);
    for(int i = 0; i < 1000; ++i){
        ASSERT_EQ(cv++, (i>=42) ? 42 : i);
    }
}

TEST(CircularBufferLimitedValuePreDecrement, WhenWrapAround_Correct) {
    auto cv = CircularBuffer<int>::LimitedValue(13, 42);
    for(int i = 12; i >= -100; --i){
        ASSERT_EQ(--cv, (i>=0) ? i : 0);
    }
    ASSERT_EQ(--cv, 0);
}

TEST(CircularBufferLimitedValuePostDecrement, WhenWrapAround_Correct) {
    auto cv = CircularBuffer<int>::LimitedValue(13, 42);
    for(int i = 13; i >= -100; --i){
        ASSERT_EQ(cv--, (i>=0) ? i : 0);
    }
    ASSERT_EQ(cv--, 0);
}

TEST(CircularBufferLimitedValueConstruct, WhenCalled_WithProperArguments_Correct) {
    for (std::size_t current_value = 0; current_value < 100; ++current_value){
        for (std::size_t max_value = current_value; max_value < 100; ++max_value) {
            auto lv = CircularBuffer<int>::LimitedValue(current_value, max_value);
            lv.val = current_value;
            lv.max_value = max_value;
        }
    }
}

TEST(CircularBufferConstruct, WhenCalled_WithProperArguments_Correct) {
    for (std::size_t i = 0; i < 100; ++i){
        auto cb = CircularBuffer<int>(i);
        ASSERT_EQ(cb.buffer.size(), i);
    }
}

TEST(CircularBufferPush, WhenCalled_Wraparound_Correct) {
    auto cb = CircularBuffer<int>(5);

    // push full buffer 0 1 2 3 4
    for (int i = 0; i < 5; i++){
        cb.push(std::make_shared<int>(i));
    }

    // check that buffer is 0 1 2 3 4
    for (int i = 0; i < 5; i++){
        ASSERT_EQ(*cb.buffer[i], i);
    }

    // push 5 6 7 8 9 to buffer
    for (int i = 5; i < 10; i++){
        cb.push(std::make_shared<int>(i));
    }

    // check that buffer is 5 6 7 8 9 now
    for (int i = 5; i < 10; i++){
        ASSERT_EQ(*cb.buffer[i-5], i);
    }
}

TEST(CircularBufferPop, WhenCalled_Wraparound_Correct) {
    auto cb = CircularBuffer<int>(5);

    for (int i = 0; i < 8; ++i){
        cb.push(std::make_shared<int>(i));
    }
    // 5 6 7 3 4

    ASSERT_EQ(*cb.pop(), 7);
    ASSERT_EQ(*cb.pop(), 6);
    ASSERT_EQ(*cb.pop(), 5);
    ASSERT_EQ(*cb.pop(), 4);
    ASSERT_EQ(*cb.pop(), 3);

    ASSERT_EQ(cb.pop(), nullptr);
    ASSERT_EQ(cb.pop(), nullptr);
    ASSERT_EQ(cb.pop(), nullptr);
    ASSERT_EQ(cb.pop(), nullptr);
    ASSERT_EQ(cb.pop(), nullptr);
    ASSERT_EQ(cb.pop(), nullptr);

    ASSERT_TRUE(cb.empty());
}

TEST(CircularBufferGetNext, WhenCalled_Wraparound_Correct) {
    auto cb = CircularBuffer<int>(5);

    ASSERT_TRUE(cb.empty());

    for (int i = 0; i < 8; ++i){
        cb.push(std::make_shared<int>(i));
    }
    // 5 6 7 3 4

    ASSERT_EQ(cb.get_next(), nullptr);

    ASSERT_EQ(*cb.pop(), 7);
    // 5 6 .7 3 4

    ASSERT_EQ(*cb.get_next(), 7);
    // 5 6 7 3 4

    ASSERT_EQ(cb.get_next(), nullptr);
    ASSERT_EQ(cb.get_next(), nullptr);
    ASSERT_EQ(cb.get_next(), nullptr);
    ASSERT_EQ(cb.get_next(), nullptr);
    ASSERT_EQ(cb.get_next(), nullptr);
    ASSERT_EQ(cb.get_next(), nullptr);

    ASSERT_TRUE(!cb.empty());
}