#ifndef ENUMDESCRIPTION_H
#define ENUMDESCRIPTION_H

#include <string>

// No .cpp

// was for SetupRule.choices, but unused?

class EnumDescription {
public:
    EnumDescription(std::string_view name, std::string_view description)
        : name(name), description(description) {}

    std::string name;
    std::string description;
};

#endif
