#include "SetupRule.h"

SetupRule::SetupRule(const std::string& name, SettingKind kind, const std::string& prompt,
                     std::optional<std::pair<int, int>> range, 
                     std::optional<std::unordered_map<std::string, std::string>> choices,
                     std::optional<std::string> defaultValue)
    : name(name), kind(kind), prompt(prompt), range(range), choices(choices), defaultValue(defaultValue) {}

std::string SetupRule::getName() const {
    return name;
}

SettingKind SetupRule::getKind() const {
    return kind;
}

std::string SetupRule::getPrompt() const {
    return prompt;
}

std::optional<std::pair<int, int>> SetupRule::getRange() const {
    return range;
}

std::optional<std::unordered_map<std::string, std::string>> SetupRule::getChoices() const {
    return choices;
}

std::optional<std::string> SetupRule::getDefaultValue() const {
    return defaultValue;
}

void SetupRule::setName(std::string_view name) {
    this->name = name;
}

// ???
void SetupRule::setKind(std::string_view kind) {
    if (kind == "boolean") {
        this->kind = SettingKind::BOOLEAN;
    } else if (kind == "integer") {
        this->kind = SettingKind::INTEGER;
    } else if (kind == "string") {
        this->kind = SettingKind::STRING;
    } else if (kind == "enum") {
        this->kind = SettingKind::ENUM;
    } else if (kind == "question-answer") {
        this->kind = SettingKind::QUESTION_ANSWER;
    } else if (kind == "multiple-choice") {
        this->kind = SettingKind::MULTIPLE_CHOICE;
    } else if (kind == "json") {
        this->kind = SettingKind::JSON;
    } else {
        this->kind = SettingKind::NONE;
    }
}

void SetupRule::setPrompt(std::string_view prompt) {
    this->prompt = prompt;
}

void SetupRule::setRange(std::string_view range) {
    // ?
}

void SetupRule::setChoices(std::string_view choicesStr) {
    // ?
}

void SetupRule::setDefaultValue(std::string_view defaultValue) {
    // ?
}