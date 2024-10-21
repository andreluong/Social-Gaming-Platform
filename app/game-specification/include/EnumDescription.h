#ifndef ENUMDESCRIPTION_H
#define ENUMDESCRIPTION_H

#include <string>

// No .cpp

class EnumDescription {
public:
    EnumDescription(std::string_view name, std::string_view description)
        : name(name), description(description) {}

    std::string getName() const { return name; }
    std::string getDescription() const { return description; }

private:
    std::string name;
    std::string description;
};

#endif
