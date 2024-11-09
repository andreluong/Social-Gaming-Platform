#include "RuleFactory.h"

void RuleFactoryRegistry::registerFactory(const std::string_view& key, std::unique_ptr<RuleFactory> factory) {
    factories[key] = std::move(factory);
}

std::unique_ptr<Rule> RuleFactoryRegistry::createRule(const ts::Node& ruleNode, 
                                                    const std::string_view& sourceCode, 
                                                    RulesParser& rulesParser) const {
    auto factory = factories.find(ruleNode.getType());
    if (factory != factories.end()) {
        return factory->second->create(ruleNode, sourceCode, rulesParser);
    }
    return nullptr;
}

std::unique_ptr<Rule> ForLoopFactory::create(const ts::Node& ruleNode, 
                                            const std::string_view& sourceCode, 
                                            RulesParser& rulesParser) const {
    std::vector<std::unique_ptr<Rule>> body;

    auto element = ruleNode.getChildByFieldName("element").getSourceRange(sourceCode);
    auto list = ruleNode.getChildByFieldName("list").getSourceRange(sourceCode);
    auto bodyNode = ruleNode.getChildByFieldName("body");
    rulesParser.parseBody(bodyNode, body);

    return std::make_unique<ForLoop>(element, list, std::move(body));
}

std::unique_ptr<Rule> WhileLoopFactory::create(const ts::Node& ruleNode, 
                                            const std::string_view& sourceCode, 
                                            RulesParser& rulesParser) const {
    std::vector<std::unique_ptr<Rule>> body;

    auto condition = ruleNode.getChildByFieldName("condition").getSourceRange(sourceCode);
    auto bodyNode = ruleNode.getChildByFieldName("body");
    rulesParser.parseBody(bodyNode, body);

    return std::make_unique<WhileLoop>(condition, std::move(body));
}

std::unique_ptr<Rule> MatchFactory::create(const ts::Node& ruleNode, 
                                            const std::string_view& sourceCode, 
                                            RulesParser& rulesParser) const {
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

std::unique_ptr<Rule> DiscardFactory::create(const ts::Node& ruleNode, 
                                            const std::string_view& sourceCode, 
                                            RulesParser& rulesParser) const {
    auto count = ruleNode.getChildByFieldName("count").getSourceRange(sourceCode);
    auto source = ruleNode.getChildByFieldName("source").getSourceRange(sourceCode);
    return std::make_unique<Discard>(count, source);
}

std::unique_ptr<Rule> ExtendFactory::create(const ts::Node& ruleNode, 
                                            const std::string_view& sourceCode, 
                                            RulesParser& rulesParser) const {
    auto target = ruleNode.getChildByFieldName("target").getSourceRange(sourceCode);
    auto value = ruleNode.getChildByFieldName("value").getSourceRange(sourceCode);
    return std::make_unique<Extend>(target, value);
}

std::unique_ptr<Rule> ReverseFactory::create(const ts::Node& ruleNode, 
                                            const std::string_view& sourceCode, 
                                            RulesParser& rulesParser) const {
    auto target = ruleNode.getChildByFieldName("target").getSourceRange(sourceCode);
    return std::make_unique<Reverse>(target);
}

std::unique_ptr<Rule> ShuffleFactory::create(const ts::Node& ruleNode, 
                                            const std::string_view& sourceCode, 
                                            RulesParser& rulesParser) const {
    auto target = ruleNode.getChildByFieldName("target").getSourceRange(sourceCode);
    return std::make_unique<Shuffle>(target);
}

std::unique_ptr<Rule> DealFactory::create(const ts::Node& ruleNode, 
                                            const std::string_view& sourceCode, 
                                            RulesParser& rulesParser) const {
    auto count = ruleNode.getChildByFieldName("count").getSourceRange(sourceCode);
    auto targets = ruleNode.getChildByFieldName("targets").getSourceRange(sourceCode);
    auto source = ruleNode.getChildByFieldName("source").getSourceRange(sourceCode);
    return std::make_unique<Deal>(count, targets, source);
}

std::unique_ptr<Rule> InputChoiceFactory::create(const ts::Node& ruleNode, 
                                            const std::string_view& sourceCode, 
                                            RulesParser& rulesParser) const {
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

std::unique_ptr<Rule> MessageFactory::create(const ts::Node& ruleNode, 
                                            const std::string_view& sourceCode, 
                                            RulesParser& rulesParser) const {
    auto players = ruleNode.getChildByFieldName("players").getSourceRange(sourceCode);
    auto content = ruleNode.getChildByFieldName("content").getSourceRange(sourceCode);
    return std::make_unique<Message>(players, content);
}

std::unique_ptr<Rule> ScoresFactory::create(const ts::Node& ruleNode, 
                                            const std::string_view& sourceCode, 
                                            RulesParser& rulesParser) const {
    auto keys = ruleNode.getChildByFieldName("keys").getSourceRange(sourceCode);
    return std::make_unique<Scores>(keys);
}
