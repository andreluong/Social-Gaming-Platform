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
    virtual std::unique_ptr<Rule> create(
        const ts::Node& ruleNode, 
        const std::string_view& sourceCode, 
        RulesParser& rulesParser
    ) const = 0;
    
    virtual ~RuleFactory() = default;
};

class RuleFactoryRegistry {
public:
    void registerFactory(const std::string_view& key, std::unique_ptr<RuleFactory> factory) {
        factories[key] = std::move(factory);
    }

    std::unique_ptr<Rule> createRule(
        const ts::Node& ruleNode, 
        const std::string_view& sourceCode, 
        RulesParser& rulesParser
    ) const {
        auto factory = factories.find(ruleNode.getType());
        if (factory != factories.end()) {
            return factory->second->create(ruleNode, sourceCode, rulesParser);
        }
        return nullptr;
    }

private:
    std::unordered_map<std::string_view, std::unique_ptr<RuleFactory>> factories;
};

/* Control Structure Factories */

class ForLoopFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(
        const ts::Node& ruleNode, 
        const std::string_view& sourceCode, 
        RulesParser& rulesParser
    ) const override {
        std::vector<std::unique_ptr<Rule>> body;

        auto element = ruleNode.getChildByFieldName("element").getSourceRange(sourceCode);
        auto list = ruleNode.getChildByFieldName("list").getSourceRange(sourceCode);
        auto bodyNode = ruleNode.getChildByFieldName("body");
        rulesParser.parseBody(bodyNode, body);

        return std::make_unique<ForLoop>(element, list, std::move(body));
    }
};

class WhileLoopFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(
        const ts::Node& ruleNode, 
        const std::string_view& sourceCode, 
        RulesParser& rulesParser
    ) const override {
        std::vector<std::unique_ptr<Rule>> body;

        auto condition = ruleNode.getChildByFieldName("condition").getSourceRange(sourceCode);
        auto bodyNode = ruleNode.getChildByFieldName("body");
        rulesParser.parseBody(bodyNode, body);

        return std::make_unique<WhileLoop>(condition, std::move(body));
    }
};

class MatchFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(
        const ts::Node& ruleNode, 
        const std::string_view& sourceCode, 
        RulesParser& rulesParser
    ) const override {
        std::vector<std::unique_ptr<MatchEntry>> entries;

        auto target = ruleNode.getChildByFieldName("target").getSourceRange(sourceCode);

        // Extract match entries
        for (auto i = 1; i < ruleNode.getNumNamedChildren(); i++) {
            auto matchEntryNode = ruleNode.getNamedChild(i);
            std::vector<std::unique_ptr<Rule>> entryBody;

            auto guard = matchEntryNode.getChildByFieldName("guard").getSourceRange(sourceCode);
            auto bodyNode = matchEntryNode.getChildByFieldName("body");
            rulesParser.parseBody(bodyNode, entryBody);

            entries.push_back(std::make_unique<MatchEntry>(guard, std::move(entryBody)));
        }

        return std::make_unique<Match>(target, std::move(entries));
    }
};

/* List Operation Factories */

class DiscardFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(
        const ts::Node& ruleNode, 
        const std::string_view& sourceCode, 
        RulesParser& rulesParser
    ) const override {
        auto count = ruleNode.getChildByFieldName("count").getSourceRange(sourceCode);
        auto source = ruleNode.getChildByFieldName("source").getSourceRange(sourceCode);
        return std::make_unique<Discard>(count, source);
    }
};

class ExtendFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(
        const ts::Node& ruleNode, 
        const std::string_view& sourceCode, 
        RulesParser& rulesParser
    ) const override {
        auto target = ruleNode.getChildByFieldName("target").getSourceRange(sourceCode);
        auto value = ruleNode.getChildByFieldName("value").getSourceRange(sourceCode);
        return std::make_unique<Extend>(target, value);
    }
};

class ReverseFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(
        const ts::Node& ruleNode, 
        const std::string_view& sourceCode, 
        RulesParser& rulesParser
    ) const override {
        auto target = ruleNode.getChildByFieldName("target").getSourceRange(sourceCode);
        return std::make_unique<Reverse>(target);
    }
};

class ShuffleFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(
        const ts::Node& ruleNode, 
        const std::string_view& sourceCode, 
        RulesParser& rulesParser
    ) const override {
        auto target = ruleNode.getChildByFieldName("target").getSourceRange(sourceCode);
        return std::make_unique<Shuffle>(target);
    }
};

class DealFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(
        const ts::Node& ruleNode, 
        const std::string_view& sourceCode, 
        RulesParser& rulesParser
    ) const override {
        auto count = ruleNode.getChildByFieldName("count").getSourceRange(sourceCode);
        auto targets = ruleNode.getChildByFieldName("targets").getSourceRange(sourceCode);
        auto source = ruleNode.getChildByFieldName("source").getSourceRange(sourceCode);
        return std::make_unique<Deal>(count, targets, source);
    }
};

/* Human Input Factories */

class InputChoiceFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(
        const ts::Node& ruleNode, 
        const std::string_view& sourceCode, 
        RulesParser& rulesParser
    ) const override {
        auto player = ruleNode.getChildByFieldName("player").getSourceRange(sourceCode);
        auto prompt = ruleNode.getChildByFieldName("prompt").getSourceRange(sourceCode);
        auto choices = ruleNode.getChildByFieldName("choices").getSourceRange(sourceCode);
        auto target = ruleNode.getChildByFieldName("target").getSourceRange(sourceCode);

        auto timeout = ruleNode.getChildByFieldName("timeout");
        if (!timeout.isNull()) {
            return std::make_unique<InputChoice>(player, prompt, choices, target, timeout.getSourceRange(sourceCode));
        } else {
            return std::make_unique<InputChoice>(player, prompt, choices, target);
        }
    }
};

/* Output Factories */

class MessageFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(
        const ts::Node& ruleNode, 
        const std::string_view& sourceCode, 
        RulesParser& rulesParser
    ) const override {
        auto players = ruleNode.getChildByFieldName("players").getSourceRange(sourceCode);
        auto content = ruleNode.getChildByFieldName("content").getSourceRange(sourceCode);
        return std::make_unique<Message>(players, content);
    }
};

class ScoresFactory : public RuleFactory {
public:
    std::unique_ptr<Rule> create(
        const ts::Node& ruleNode, 
        const std::string_view& sourceCode, 
        RulesParser& rulesParser
    ) const override {
        auto keys = ruleNode.getChildByFieldName("keys").getSourceRange(sourceCode);
        return std::make_unique<Scores>(keys);
    }
};