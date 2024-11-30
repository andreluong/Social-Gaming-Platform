#include "../game-specification/include/GameSpecificationParser.h"
#include "../game-specification/include/SyntaxTree.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace testing;

// Mock class for SyntaxTree
class MockSyntaxTree : public SyntaxTree {
public:
    MockSyntaxTree() : SyntaxTree("mock source", createMockLanguage()) {}

    MOCK_METHOD(std::string, getSourceCode, (), (const override));
    MOCK_METHOD(ts::Node, getRootNode, (), (const override));

private:
    static ts::Language createMockLanguage() {
        // TSLanguage
        static const TSLanguage mockTSLanguage = {};
        return ts::Language(&mockTSLanguage);
    }
};

class GameSpecificationParserTests : public Test {
protected:
    std::unique_ptr<MockSyntaxTree> mockSyntaxTree;
    std::unique_ptr<GameSpecificationParser> parser;
    std::string mockSourceCode;

    void SetUp() override {
        // Simulated source code representing some part of the `.game` structure
        mockSourceCode = R"(
            configuration {
                name: "TestGame",
                player range: (2, 4),
                audience: true,
                setup: {
                    question {
                        kind: "multiple-choice",
                        prompt: "What is your favorite color?",
                        range: (1, 3)
                    }
                }
            }
            constants {
                map: {
                    constant1: "value1",
                    constant2: "value2"
                }
            }
        )";

        mockSyntaxTree = std::make_unique<MockSyntaxTree>();

        // Mock source code
        EXPECT_CALL(*mockSyntaxTree, getSourceCode())
            .WillRepeatedly(Return(mockSourceCode));

        // Mock root node with a valid TSNode
        TSNode mockTSNode = {};
        ts::Node mockNode(mockTSNode);

        EXPECT_CALL(*mockSyntaxTree, getRootNode())
            .WillRepeatedly(Return(mockNode));

        parser = std::make_unique<GameSpecificationParser>(*mockSyntaxTree);
    }

    void TearDown() override {
        parser.reset();
        mockSyntaxTree.reset();
    }
};

// Test init
TEST_F(GameSpecificationParserTests, InitializationTest) {
    ASSERT_NE(parser, nullptr);
    EXPECT_EQ(mockSyntaxTree->getSourceCode(), mockSourceCode);
}

// Test parsing config
TEST_F(GameSpecificationParserTests, ParseConfigurationTest) {
    EXPECT_NO_THROW({
        Configuration config = parser->parseConfiguration();

        // Check specific fields in the configuration
        EXPECT_EQ(config.getValue("name"), "TestGame");
        EXPECT_EQ(config.getValue("player_range"), "(2, 4)");
        EXPECT_EQ(config.getValue("has_audience"), "true");
    });
}

// Test parsing constants section
TEST_F(GameSpecificationParserTests, ParseConstantsTest) {
    EXPECT_NO_THROW({
        auto constants = parser->parseSection(ConstantsType);

        ASSERT_NE(constants, nullptr);
        EXPECT_EQ(constants->getValue("constant1"), "value1");
        EXPECT_EQ(constants->getValue("constant2"), "value2");
    });
}

// Test parsing a boolean value
TEST_F(GameSpecificationParserTests, ParseBooleanTest) {
    TSNode boolNode = {};
    boolNode.id = 1;

    EXPECT_NO_THROW({
        std::string result = parser->parseBoolean(ts::Node(boolNode));

        // Replace with the expected value for this node
        EXPECT_EQ(result, "true");
    });
}

// Test parsing a number range
TEST_F(GameSpecificationParserTests, ParseNumberRangeTest) {
    TSNode rangeNode = {};
    rangeNode.id = 2;

    EXPECT_NO_THROW({
        auto range = parser->parseNumberRange(ts::Node(rangeNode));
        EXPECT_EQ(range.first, 2);  // Expected min value
        EXPECT_EQ(range.second, 4); // Expected max value
    });
}

// Test parsing a value map
TEST_F(GameSpecificationParserTests, ParseValueMapTest) {
    TSNode mapNode = {};
    mapNode.id = 3;

    EXPECT_NO_THROW({
        auto valueMap = parser->parseValueMap(ts::Node(mapNode));

        ASSERT_FALSE(valueMap.empty());
        EXPECT_EQ(valueMap["constant1"].get<std::string>(), "value1");
        EXPECT_EQ(valueMap["constant2"].get<std::string>(), "value2");
    });
}

// Test parsing a setup rule
TEST_F(GameSpecificationParserTests, ParseSetupRuleTest) {
    TSNode setupNode = {};
    setupNode.id = 4;

    EXPECT_NO_THROW({
        auto setup = parser->parseSection(SetupRuleType);

        ASSERT_NE(setup, nullptr);
        EXPECT_EQ(setup->getValue("kind"), "multiple-choice");
        EXPECT_EQ(setup->getValue("prompt"), "What is your favorite color?");
        EXPECT_EQ(setup->getValue("range"), "(1, 3)");
    });
}

// Test parsing a list
TEST_F(GameSpecificationParserTests, ParseListTest) {
    TSNode listNode = {};
    listNode.id = 5;

    EXPECT_NO_THROW({
        auto list = parser->parseList(ts::Node(listNode));

        ASSERT_FALSE(list.empty());
        EXPECT_EQ(std::get<std::string>(list[0]), "item1");
    });
}

// Test parsing a comparison
TEST_F(GameSpecificationParserTests, ParseComparisonTest) {
    TSNode comparisonNode = {};
    comparisonNode.id = 6;

    EXPECT_NO_THROW({
        std::string comparison = parser->parseComparison(ts::Node(comparisonNode));

        EXPECT_EQ(comparison, "lhs == rhs");
    });
}

// Test parsing a logical operation
TEST_F(GameSpecificationParserTests, ParseLogicalOperationTest) {
    TSNode logicalNode = {};
    logicalNode.id = 7;

    EXPECT_NO_THROW({
        std::string logicalOperation = parser->parseLogicalOperation(ts::Node(logicalNode));

        EXPECT_EQ(logicalOperation, "lhs && rhs");
    });
}
