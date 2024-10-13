#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../lib/utils/include/list.h"
#include "../lib/utils/include/mapList.h"

// Tests utils: sharedListTests, list, mapList; used for list attributes in variables and expressions section
// TODO: assignment and numerical attributes
using namespace testing;

class ListTests : public ::testing::Test {
protected:
    List<int> list;

    ListTests() : list({1, 2, 3, 4, 5}) {}
};

// Also tests size
TEST_F(ListTests, Slice) {
    List<int> sliced = list.slice(1, 4);
    EXPECT_EQ(sliced.size(), 3);
    EXPECT_EQ(sliced.at(0), 2);
    EXPECT_EQ(sliced.at(1), 3);
    EXPECT_EQ(sliced.at(2), 4);
}

// Filter for even numbers
TEST_F(ListTests, Collect) {
    List<int> collected = list.collect([](const int& value) { return value % 2 == 0; });
    EXPECT_EQ(collected.size(), 2);
    EXPECT_EQ(collected.at(0), 2);
    EXPECT_EQ(collected.at(1), 4);
}

TEST_F(ListTests, Contains) {
    EXPECT_TRUE(list.contains(1));
    EXPECT_FALSE(list.contains(0));
}

TEST_F(ListTests, At) {
    EXPECT_EQ(list.at(0), 1);
    EXPECT_EQ(list.at(1), 2);
    EXPECT_EQ(list.at(2), 3);
    EXPECT_EQ(list.at(3), 4);
    EXPECT_EQ(list.at(4), 5);
}

class MapListTests : public ::testing::Test {
protected:
    MapList<std::string, std::string> mapList;

    MapListTests() : mapList({
        {{"name", "Duke"}, {"action", "Tax."}},
        {{"name", "Assassin"}, {"action", "Assassinate."}}
    }) {}
};

TEST_F(MapListTests, Slice) {
    // Slicing from index 0 to 2 (this should give us both elements in mapList)
    List<std::map<std::string, std::string>> sliced = mapList.slice(0, 1);
    List<std::map<std::string, std::string>> sliced_second = mapList.slice(1, 2);

    // Sliced in half
    EXPECT_EQ(sliced.size(), 1);
    EXPECT_EQ(sliced_second.size(), 1);

    // Slice starts at first element
    EXPECT_EQ(sliced.at(0).at("name"), "Duke");
    EXPECT_EQ(sliced.at(0).at("action"), "Tax.");
    
    // Slice starts at second element of original
    EXPECT_EQ(sliced_second.at(0).at("name"), "Assassin");
    EXPECT_EQ(sliced_second.at(0).at("action"), "Assassinate.");
}

TEST_F(MapListTests, Contains) {
    std::map<std::string, std::string> duke = {{"name", "Duke"}, {"action", "Tax."}};
    EXPECT_TRUE(mapList.contains(duke));

    std::map<std::string, std::string> unknown = {{"name", "Unknown"}, {"action", "Unknown"}};
    EXPECT_FALSE(mapList.contains(unknown));
}

TEST_F(MapListTests, Collect) {
    List<std::map<std::string, std::string>> collected = mapList.collect([](const std::map<std::string, std::string>& map) {
        return map.at("name") == "Duke";
    });
    EXPECT_EQ(collected.size(), 1);
    EXPECT_EQ(collected.at(0).at("name"), "Duke");
}

// Test for getValues method
TEST_F(MapListTests, GetValues) {
    List<std::string> valuesName = mapList.getValues("name");
    List<std::string> valuesAction = mapList.getValues("action");
    List<std::string> valuesUnknown = mapList.getValues("Unknown");

    EXPECT_EQ(valuesName.size(), 2);
    EXPECT_EQ(valuesName.at(0), "Duke");
    EXPECT_EQ(valuesName.at(1), "Assassin");

    EXPECT_EQ(valuesAction.size(), 2);
    EXPECT_EQ(valuesAction.at(0), "Tax.");
    EXPECT_EQ(valuesAction.at(1), "Assassinate.");


    // Expecting no values for an unknown key
    EXPECT_EQ(valuesUnknown.size(), 0);
}