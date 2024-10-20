#ifndef RULES_H
#define RULES_H

#include <string>

class Rules {
public:
    Rules(const std::string& body)
        : body(body) {}

    std::string getBody() const { return body; }

private:
    std::string body;
};

#endif