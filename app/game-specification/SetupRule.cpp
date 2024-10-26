#include "SetupRule.h"
#include <algorithm>

std::unordered_map<std::string_view, SettingKind> kindMap = {
    {"boolean", SettingKind::BOOLEAN},
    {"integer", SettingKind::INTEGER},
    {"string", SettingKind::STRING},
    {"enum", SettingKind::ENUM},
    {"question-answer", SettingKind::QUESTION_ANSWER},
    {"multiple-choice", SettingKind::MULTIPLE_CHOICE},
    {"json", SettingKind::JSON},
};

SetupRule::SetupRule(const std::string& name, SettingKind kind, const std::string& prompt,
                     std::optional<std::pair<int, int>> range, 
                     std::optional<std::unordered_map<std::string, std::string>> choices,
                     std::optional<std::variant<int, bool, std::string>> defaultValue)
    : name(name), kind(kind), prompt(prompt), range(range), choices(choices), defaultValue(defaultValue) {
    }

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

std::optional<std::variant<int, bool, std::string>> SetupRule::getDefaultValue() const {
    return defaultValue;
}

void SetupRule::setName(std::string_view name) {
    this->name = name;
}

void SetupRule::setKind(std::string_view strKind) {
    this->kind = kindMap[strKind];
}

void SetupRule::setPrompt(std::string_view prompt) {
    this->prompt = prompt;
}

// expected string in range: "(num1, num2)"
void SetupRule::setRange(std::string_view rangeStr) {
    rangeStr = rangeStr.substr(1, rangeStr.size() - 2); // removes the brackets

    int commaIndex = rangeStr.find(',');
    if(commaIndex == std::string_view::npos){
        return;
    }

    std::string_view firstStr = rangeStr.substr(0, commaIndex);
    std::string_view secondStr = rangeStr.substr(commaIndex + 1);

    // may need to change this so that string_view does not need to be converted
    // to a char*, possibly allocating new memory? Upgrade C++ version?
    int first = std::stoi(firstStr.data());
    int second = std::stoi(secondStr.data());
    if(first > second){ // might need to be removed
        return;
    }

    this->range = {first, second};
}

// assumes that this is the format of the string received:
//   choices: {
//     'fast':     'A quick round with friends',
//     'standard': 'Standard play',
//     'long':     'A marathon battle against former friends',
//   }
void SetupRule::setChoices(std::string_view choicesStr) {
    choicesStr = choicesStr.substr(1, choicesStr.size() - 2);

    std::vector<unsigned int> apostropheLoc;
    int apostropheIndex = choicesStr.find('\'');

    // grabs the location of the '\'', which are the elements to be added to our map
    while(apostropheIndex != std::string_view::npos){
        apostropheLoc.push_back(apostropheIndex);
        apostropheIndex = choicesStr.find('\'', apostropheIndex + 1);
    }

    unsigned int count = 0;
    for(int i = 0; i < apostropheLoc.size(); i += 4){
        std::string key = std::string(choicesStr.substr(apostropheLoc[i] + 1, 
                                                        apostropheLoc[i+1] - 1));                 
        std::string value = std::string(choicesStr.substr(apostropheLoc[i+2] + 1,
                                                          apostropheLoc[i+3] - 1));
        this->choices->emplace(key, value);
    }
}

// currently, default only has bool, integer, and string types
// unsure on how to implement the other types into variant
void SetupRule::setDefaultValue(std::string_view defaultValue) {
    switch(this->kind){
        case SettingKind::BOOLEAN:
            if(defaultValue == "true"){
                this->defaultValue = true;
            }
            else if(defaultValue == "false"){
                this->defaultValue = false;
            }
            break;
        case SettingKind::INTEGER:
            this->defaultValue = std::stoi(defaultValue.data());
            break;
        case SettingKind::STRING:
            this->defaultValue = std::string(defaultValue);
            break;
        default:
            this->defaultValue = std::string(defaultValue);
            break;
    }
}