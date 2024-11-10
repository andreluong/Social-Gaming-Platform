#include "SectionValueMap.h"

#ifndef CONSTANTS_H
#define CONSTANTS_H

class Constants : public ValueMap {
public:
    Constants() = default;
    std::string getSectionType() const {
        return "CONSTANTS";
    }

};

#endif