#include "RulesParser.h"
#include "RuleFactory.h"

RulesParser::RulesParser(const ts::Node& node, const std::string& sourceCode) 
    : sourceCode(sourceCode) {
    parseBody(node, rules);
}

// Returns a pointer to a rule based on the node's type
std::unique_ptr<Rule> RulesParser::parseRule(const ts::Node& node) {
    RuleFactoryRegistry registry;
    auto ruleNode = node.getNamedChild(0);

    // Control structure factories
    registry.registerFactory("for", std::make_unique<ForLoopFactory>());
    registry.registerFactory("while", std::make_unique<WhileLoopFactory>());
    // TODO: Change to parallel for. Enabled for testing
    registry.registerFactory("parallel_for", std::make_unique<ForLoopFactory>()); 
    registry.registerFactory("match", std::make_unique<MatchFactory>());

    // List operation factories
    registry.registerFactory("discard", std::make_unique<DiscardFactory>());
    registry.registerFactory("extend", std::make_unique<ExtendFactory>());
    registry.registerFactory("reverse", std::make_unique<ReverseFactory>());
    registry.registerFactory("shuffle", std::make_unique<ShuffleFactory>());
    registry.registerFactory("deal", std::make_unique<DealFactory>());
    
    // Human input factories
    registry.registerFactory("input_choice", std::make_unique<InputChoiceFactory>());
    
    // Output factories
    registry.registerFactory("message", std::make_unique<MessageFactory>());
    registry.registerFactory("scores", std::make_unique<ScoresFactory>());

    return registry.createRule(ruleNode, sourceCode, *this);
}

// Parses a body of a node and adds rule pointers to the vector for each rule found
void RulesParser::parseBody(const ts::Node& node, std::vector<std::unique_ptr<Rule>>& vector) {
    for (const auto& child : ts::Children{node}) {
        // Children could be rule or comment (Skip comment)
        if (child.getType() == "rule") {
            vector.push_back(std::move(RulesParser::parseRule(child)));
        }
    }
}

void RulesParser::print() {
    for (const auto& rulePointer : rules) {
        rulePointer->print();
    }
}