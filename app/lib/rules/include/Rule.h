#pragma once

#include "dataVariant.h"
#include <string_view>
#include <unordered_map>
#include <vector>
#include <optional>
#include <memory>
#include "GameContext.h"
#include <spdlog/spdlog.h>

// Several need work/testing
// Message and scores incomplete

class Rule {
public:
    std::unordered_map<std::string_view, GameVariant> freshVariables; // For for-each and parallel-for

    virtual void execute(GameContext* context) = 0;
    virtual void print() = 0;
    virtual ~Rule() = default;
};

// Used for InParallel, incomplete/not sure about design
class ChunkableRule {
public:
    virtual void executeChunk(GameContext* context) = 0;
    virtual bool isComplete() const = 0;
    virtual ~ChunkableRule() = default;
};

/* Control Structure */

class ForLoop : public Rule {
public:
    ForLoop(std::string_view element, std::string_view list, std::vector<std::unique_ptr<Rule>> body);
    
    void execute(GameContext* context) override;
    void print() override;

private:
    std::string_view element; // identifier
    std::string_view list; // expression
    std::vector<std::unique_ptr<Rule>> body; // body
};

class WhileLoop : public Rule {
public:
    WhileLoop(std::string_view condition, std::vector<std::unique_ptr<Rule>> body);

    void execute(GameContext* context) override;
    void print() override;

private:
    std::string_view condition; // expression
    std::vector<std::unique_ptr<Rule>> body; // body
};

class ParallelFor : public Rule {
public:
    ParallelFor(std::string_view element, std::string_view list, std::vector<std::unique_ptr<Rule>> body);

    void execute(GameContext* context) override;
    void print() override;

private:
    std::string_view element; 
    std::string_view list;
    std::vector<std::unique_ptr<Rule>> body;

    //Keep track of which statement in the body (body[currentStep]) is currently being executed for a specific element from the list.
    std::unordered_map<size_t, size_t> progress;
};

// Uses ChunkableRule instead of Rule, need to figure out a way to track progress or chunks of work for a single statement (rule)
class InParallel : public Rule {
public:
    InParallel(std::vector<std::unique_ptr<ChunkableRule>> body);

    void execute(GameContext* context) override;
    void print() override;

private:
    std::vector<std::unique_ptr<ChunkableRule>> body;
};

class MatchEntry : public Rule {
public:
    MatchEntry(std::string_view guard, std::vector<std::unique_ptr<Rule>> body);

    void execute(GameContext* context) override;
    void print() override;

    std::string_view getGuard();

private:
    std::string_view guard; // expression
    std::vector<std::unique_ptr<Rule>> body; // body
};

class Match : public Rule {
public:
    Match(std::string_view target, std::vector<std::unique_ptr<MatchEntry>> entries);

    void execute(GameContext* context) override;
    void print() override;

private:
    std::string_view target; // expression
    std::vector<std::unique_ptr<MatchEntry>> entries;
};

/* List Operations */

class Extend : public Rule {
public:
    Extend(std::string_view target, std::string_view value);

    void execute(GameContext* context) override;
    void print() override;

private:
    std::string_view target; // qualififed identifier
    std::string_view value; // expression
};

class Reverse : public Rule {
public:
    Reverse(std::string_view target);

    void execute(GameContext* context) override;
    void print() override;

private:
    std::string_view target; // qualififed identifier
};

class Shuffle : public Rule {
public:
    Shuffle(std::string_view target);

    void execute(GameContext* context) override;
    void print() override;
    
private:
    std::string_view target; // qualififed identifier
};

// Not sure
class Sort : public Rule {
public:
    Sort(std::string_view listName, std::optional<std::string_view> key = std::nullopt);

    void execute(GameContext* context);
    void print() const;

private:
    std::string listName;
    std::optional<std::string> key;
};

class Deal : public Rule {
public:
    Deal(std::string_view count, std::string_view targets, std::string_view source);

    void execute(GameContext* context) override;
    void print() override;

private:
    std::string_view count; // expression
    std::string_view targets; // player set
    std::string_view source; // expression
};

class Discard : public Rule {
public:
    Discard(std::string_view count, std::string_view source);

    void execute(GameContext* context) override;
    void print() override;

private:
    std::string_view count; // expression
    std::string_view source; // qualified identifier
};

/* Timing */

// Move/adapt "Timing" here if possible


/* Human Input */

// TODO: refactor from this to class Input if better
class InputChoice : public Rule {
public:
    InputChoice(std::string_view player, 
                std::string_view prompt, 
                std::string_view choices, 
                std::string_view target);
    
    InputChoice(std::string_view player, 
                std::string_view prompt, 
                std::string_view choices, 
                std::string_view target, 
                std::string_view timeout);

    void execute(GameContext* context) override;
    void print() override;

private:
    std::string_view player; // qualififed identifier
    std::string_view prompt; // expression
    std::string_view choices; // expression
    std::string_view target; // qualififed identifier
    std::optional<std::string_view> timeout; // expression
};

/// Used in class Input
enum class InputType {
    Choice,
    Text,
    Range,
    Vote
};

// TODO: design to handle text, range, and vote versions of input handling
class Input : public Rule {
public:
    Input(std::string_view player, 
          std::string_view prompt, 
          std::string_view target,
          InputType type,
          std::optional<std::string_view> timeout = std::nullopt,
          std::optional<std::string_view> choices = std::nullopt,
          std::optional<std::string_view> range = std::nullopt);

    void execute(GameContext* context) override;
    void print() override;

private:
    std::string_view player; // qualififed identifier
    std::string_view prompt; // expression
    std::string_view target; // qualififed identifier
    InputType inputType;
    std::optional<std::string_view> hasTimeout; // expression
    std::optional<std::string_view> choices; // expression
    std::optional<std::string_view> range; // expression


};

/* Output */

class Message : public Rule {
public:
    Message(std::string_view players, std::string_view content);

    void execute(GameContext* context) override;
    void print() override;

private:
    std::string_view players; // player_set
    std::string_view content; // expression
};

class Scores : public Rule {
public:
    Scores(std::string_view keys);

    void execute(GameContext* context) override;
    void print() override;

private:
    std::string_view keys; // list_literal
};