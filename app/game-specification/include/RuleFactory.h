#pragma once

#include <cpp-tree-sitter.h>
#include "dataVariant.h"
#include <string_view>
#include <vector>
#include <memory>
#include "Rule.h"
#include "RulesParser.h"

class RuleFactory {
public:
    virtual std::unique_ptr<Rule> create(const ts::Node& ruleNode, 
                                        const std::string_view& sourceCode, 
                                        RulesParser& rulesParser) const = 0;
    virtual ~RuleFactory() = default;
};

class RuleFactoryRegistry {
public:
    void registerFactory(const std::string_view& key, std::unique_ptr<RuleFactory> factory);

    std::unique_ptr<Rule> createRule(const ts::Node& ruleNode, 
                                    const std::string_view& sourceCode, 
                                    RulesParser& rulesParser) const;

private:
    std::unordered_map<std::string_view, std::unique_ptr<RuleFactory>> factories;
};

/* Control Structure Factories */

class ForLoopFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(const ts::Node& ruleNode, 
                                const std::string_view& sourceCode, 
                                RulesParser& rulesParser) const override;
};

class WhileLoopFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(const ts::Node& ruleNode, 
                                const std::string_view& sourceCode, 
                                RulesParser& rulesParser) const override;
};

class MatchFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(const ts::Node& ruleNode, 
                                const std::string_view& sourceCode, 
                                RulesParser& rulesParser) const override;
};

/* List Operation Factories */

class DiscardFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(const ts::Node& ruleNode, 
                                const std::string_view& sourceCode, 
                                RulesParser& rulesParser) const override;
};

class ExtendFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(const ts::Node& ruleNode, 
                                const std::string_view& sourceCode, 
                                RulesParser& rulesParser) const override;
};

class ReverseFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(const ts::Node& ruleNode, 
                                const std::string_view& sourceCode, 
                                RulesParser& rulesParser) const override;
};

class ShuffleFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(const ts::Node& ruleNode, 
                                const std::string_view& sourceCode, 
                                RulesParser& rulesParser) const override;
};

class DealFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(const ts::Node& ruleNode, 
                                const std::string_view& sourceCode, 
                                RulesParser& rulesParser) const override;
};

/* Human Input Factories */

class InputChoiceFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(const ts::Node& ruleNode, 
                                const std::string_view& sourceCode, 
                                RulesParser& rulesParser) const override;
};

/* Output Factories */

class MessageFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(const ts::Node& ruleNode, 
                                const std::string_view& sourceCode, 
                                RulesParser& rulesParser) const override;
};

class ScoresFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(const ts::Node& ruleNode, 
                                const std::string_view& sourceCode, 
                                RulesParser& rulesParser) const override;
};