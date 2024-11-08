#pragma once

#include "dataVariant.h"
#include <string_view>
#include <unordered_map>
#include <vector>
#include <optional>

class Rule {
public:
    virtual void execute() = 0;
    virtual void print() = 0;
    virtual ~Rule() = default;
};

/* Control Structure */

class ForLoop : public Rule {
public:
    ForLoop(std::string_view element, std::string_view list, std::vector<std::unique_ptr<Rule>> body)
        : element(element), list(list), body(std::move(body)) {}
    
    void execute() override {
        std::cout << "For loop execute" << std::endl;
    }

    void print() override {
        std::cout << "For loop:\nelement: " << element << "\nlist: " << list << "\nbody: \n";
        for (const auto& rulePointer : body) {
            if (rulePointer) {
                rulePointer->print();
            }
        }
        std::cout << std::endl;
    }

private:
    std::string_view element; // identifier
    std::string_view list; // expression
    std::vector<std::unique_ptr<Rule>> body; // body
};

class WhileLoop : public Rule {
public:
    WhileLoop(std::string_view condition, std::vector<std::unique_ptr<Rule>> body)
        : condition(condition), body(std::move(body)) {}
    
    void execute() override {
        std::cout << "While loop execute" << std::endl;
    }

    void print() override {
        std::cout << "While loop:\ncondition: " << condition << "\nbody: \n";
        for (const auto& rulePointer : body) {
            if (rulePointer) {
                rulePointer->print();
            }
        }
        std::cout << std::endl;
    }

private:
    std::string_view condition; // expression
    std::vector<std::unique_ptr<Rule>> body; // body
};

class MatchEntry : public Rule {
public:
    MatchEntry(std::string_view guard, std::vector<std::unique_ptr<Rule>> body)
        : guard(guard), body(std::move(body)) {}
    
    void execute() override {
        std::cout << "Match entry execute" << std::endl;
    }

    void print() override {
        std::cout << "Match Entry:\nguard: " << guard << "\nbody: \n";
        for (const auto& rulePointer : body) {
            if (rulePointer) {
                rulePointer->print();
            }
        }
        std::cout << std::endl;
    }

private:
    std::string_view guard; // expression
    std::vector<std::unique_ptr<Rule>> body; // body
};

class Match : public Rule {
public:
    Match(std::string_view target, std::vector<std::unique_ptr<MatchEntry>> entries)
        : target(target), entries(std::move(entries)) {}

    void execute() override {
        std::cout << "Match execute" << std::endl;
    }

    void print() override {
        std::cout << "Match: \nguard: " << target << "\nentries: \n";
        for (const auto& entryPtr : entries) {
            if (entryPtr) {
                entryPtr->print();
            }
        }
        std::cout << std::endl;
    }

private:
    std::string_view target; // expression
    std::vector<std::unique_ptr<MatchEntry>> entries;
};

/* List Operations */

class Discard : public Rule {
public:
    Discard(std::string_view count, std::string_view source) 
        : count(count), source(source) {}

    void execute() override {
        std::cout << "Discard execute" << std::endl;
    }

    void print() override {
        std::cout << "Discard:\ncount: " << count << "\nsource: " << source << std::endl << std::endl;
    }

private:
    std::string_view count; // expression
    std::string_view source; // qualified identifier
};

class Extend : public Rule {
public:
    Extend(std::string_view target, std::string_view value) 
        : target(target), value(value) {}

    void execute() override {
        std::cout << "Extend execute" << std::endl;
    }

    void print() override {
        std::cout << "Extend:\ntarget: " << target << "\nvalue: " << value << std::endl << std::endl;
    }

private:
    std::string_view target; // qualififed identifier
    std::string_view value; // expression
};

class Reverse : public Rule {
public:
    Reverse(std::string_view target) 
        : target(target) {}

    void execute() override {
        std::cout << "Reverse execute" << std::endl;
    }

    void print() override {
        std::cout << "Reverse:\ntarget: " << target << std::endl << std::endl;
    }

private:
    std::string_view target; // qualififed identifier
};

class Shuffle : public Rule {
public:
    Shuffle(std::string_view target) : target(target) {}

    void execute() override {
        std::cout << "Shuffle execute" << std::endl;
    }

    void print() override {
        std::cout << "Shuffle:\ntarget: " << target << std::endl << std::endl;
    }
    
private:
    std::string_view target; // qualififed identifier
};

class Deal : public Rule {
public:
    Deal(std::string_view count, std::string_view targets, std::string_view source) 
        : count(count), targets(targets), source(source) {}

    void execute() override {
        std::cout << "Deal execute" << std::endl;
    }

    void print() override {
        std::cout << "Deal:\ncount: " << count << "\ntargets: " << targets << "\nsource: " << source << std::endl << std::endl;
    }

private:
    std::string_view count; // expression
    std::string_view targets; // player set
    std::string_view source; // expression
};

/* Human Input */

class InputChoice : public Rule {
public:
    InputChoice(
        std::string_view player, 
        std::string_view prompt, 
        std::string_view choices, 
        std::string_view target
    ) : player(player), prompt(prompt), choices(choices), target(target), timeout({}) {}
    
    InputChoice(
        std::string_view player, 
        std::string_view prompt, 
        std::string_view choices, 
        std::string_view target, 
        std::string_view timeout
    ) : player(player), prompt(prompt), choices(choices), target(target), timeout(timeout) {}

    void execute() override {
        std::cout << "InputChoice execute" << std::endl;
    }

    void print() override {
        std::cout << "InputChoice:\nplayer: " << player << "\nprompt: " << prompt << "\nchoices: " << choices << "\ntarget: " << target << std::endl;
        if (timeout.has_value()) {
            std::cout << "timeout: " << timeout.value() << std::endl << std::endl;
        }
    }

private:
    std::string_view player; // qualififed identifier
    std::string_view prompt; // expression
    std::string_view choices; // expression
    std::string_view target; // qualififed identifier
    std::optional<std::string_view> timeout; // expression
};

/* Output */

class Message : public Rule {
public:
    Message(std::string_view players, std::string_view content)
        : players(players), content(content) {}

    void execute() override {
        std::cout << "Message execute" << std::endl;
    }

    void print() override {
        std::cout << "Message:\nplayers: " << players << "\ncontent: " << content << std::endl << std::endl;
    }

private:
    std::string_view players; // player_set
    std::string_view content; // expression
};

class Scores : public Rule {
public:
    Scores(std::string_view keys) : keys(keys) {}

    void execute() override {
        std::cout << "Scores execute" << std::endl;
    }

    void print() override {
        std::cout << "Scores:\nkeys: " << keys << std::endl << std::endl;
    }

private:
    std::string_view keys; // list_literal
};