#ifndef SETUPRULE_H
#define SETUPRULE_H

#include <string>
#include <unordered_map>
#include <optional>
#include <utility>
#include <string_view>
#include <vector>
#include <variant>
#include <iostream>
#include <cpp-tree-sitter.h>

class GameSpecificationParser;

enum class SettingKind {
    BOOLEAN,
    INTEGER,
    STRING,
    ENUM,
    QUESTION_ANSWER,
    MULTIPLE_CHOICE,
    JSON,
    NONE // maybe?
};

enum class SetupField{
    NAME,
    KIND,
    PROMPT,
    RANGE, 
    CHOICES,
    DEFAULT
};

class SetupRule {
public:
    SetupRule(const std::string& name = "", SettingKind kind = SettingKind::NONE, const std::string& prompt = "",
              std::optional<std::pair<int, int>> range = std::nullopt,
              std::optional<std::unordered_map<std::string, std::string>> choices = std::nullopt,
              std::optional<std::variant<int, bool, std::string>> defaultValue = std::nullopt);

    SetupRule(const ts::Node &setupRuleNode, std::string_view sourceCode);

    std::string getName() const;
    SettingKind getKind() const;
    std::string getPrompt() const;
    std::optional<std::pair<int, int>> getRange() const;
    std::optional<std::unordered_map<std::string, std::string>> getChoices() const;
    // std::optional<std::string> getDefaultValue() const;
    std::optional<std::variant<int, bool, std::string>> getDefaultValue() const;

    const char* to_string(SettingKind SettingKind) {
    switch (SettingKind) {
        case SettingKind::BOOLEAN: return "Boolean";
        case SettingKind::INTEGER: return "Integer";
        case SettingKind::STRING: return "String";
        case SettingKind::ENUM: return "Enum";
        case SettingKind::QUESTION_ANSWER: return "Question_answer";
        case SettingKind::MULTIPLE_CHOICE: return "Multiple_choice";
        case SettingKind::JSON: return "Json";
        default: return "None";
    }
}


private:
    std::string name;
    SettingKind kind;
    std::string prompt;
    std::optional<std::pair<int, int>> range;
    std::optional<std::unordered_map<std::string, std::string>> choices; // is this necessary, will it be useful or make things harder later?
    // std::optional<std::string> defaultValue; // would have to accept multiple types
    std::optional<std::variant<int, bool, std::string>> defaultValue;

    // Something better than this?

    void setName(std::string_view name);
    void setKind(std::string_view kind);
    void setPrompt(std::string_view prompt);
    void setRange(std::string_view range);
    void setChoices(std::string_view choices);
    void setDefaultValue(std::string_view defaultValue);
    void setField(const ts::Node &node, const SetupField &setupField, std::string_view sourceCode); // used for lambda to use appropriate setter

    // Only friend class should access setters
    friend class GameSpecificationParser;
};

#endif