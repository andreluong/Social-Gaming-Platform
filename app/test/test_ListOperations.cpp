#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "listOperations.h"
#include <algorithm>
#include <map>

using namespace testing;

class FakeRandomGenerator : public RandomGenerator {
public:
    MOCK_METHOD(unsigned int, seed, (), (override));
};

TEST(ListOperationsTests, extend) {
    std::vector<dataVariant> list = {"A", 1, "B", 2, "C", 3, true};
    std::vector<dataVariant> expression = {"D", 4, false};
    std::vector<dataVariant> expected = {"A", 1, "B", 2, "C", 3, true, "D", 4, false};

    extend(list, expression);
    EXPECT_EQ(expected.size(), list.size());
    EXPECT_THAT(list, ContainerEq(expected));
}

TEST(ListOperationsTests, reverse) {
    std::vector<dataVariant> list = {"A", 1, "B", 2, "C", 3, true, "D", 4, false};
    std::vector<dataVariant> expected1 = {false, 4, "D", true, 3, "C", 2, "B", 1, "A"};
    std::vector<dataVariant> expected2(list);

    reverse(list);
    EXPECT_EQ(expected1.size(), list.size());
    EXPECT_THAT(list, ContainerEq(expected1));

    reverse(list);
    EXPECT_EQ(expected2.size(), list.size());
    EXPECT_THAT(list, ContainerEq(expected2));
}

TEST(ListOperationsTests, shuffle) {
    FakeRandomGenerator fakeRandomGenerator;

    std::vector<dataVariant> list = {"A", 1, "B", 2, "C", 3, true, "D", 4, false};
    std::vector<dataVariant> expected1 = {false, "A", "B", 3, "D", "C", true, 2, 1, 4};
    std::vector<dataVariant> expected2 = {3, true, 2, false, "C", "D", 1, "B", 4, "A"};
        
    EXPECT_CALL(fakeRandomGenerator, seed())
        .Times(2)
        .WillOnce(Return(1))
        .WillOnce(Return(23));

    shuffle(list, fakeRandomGenerator);
    EXPECT_EQ(expected1.size(), list.size());
    EXPECT_THAT(list, ContainerEq(expected1));

    shuffle(list, fakeRandomGenerator);
    EXPECT_EQ(expected2.size(), list.size());
    EXPECT_THAT(list, ContainerEq(expected2));
}

TEST(ListOperationsTests, discard) {
    std::vector<dataVariant> list1 = {"A", 1, "B", 2, "C", 3, true, "D", 4, false};
    std::vector<dataVariant> list2(list1);
    int expectedSize = list1.size();
    std::vector<dataVariant> expected = {"A", 1, "B", 2, "C", 3, true, "D"};

    discard(list1, 0);
    EXPECT_EQ(expectedSize, list1.size());

    discard(list1, 2);
    EXPECT_EQ(expected.size(), list1.size());
    EXPECT_THAT(list1, ContainerEq(expected));

    // Discard all remaining contents of list1
    discard(list1, 8);
    EXPECT_THAT(list1, IsEmpty());

    // Discard more contents than list2
    discard(list2, 100);
    EXPECT_THAT(list2, IsEmpty());
}
