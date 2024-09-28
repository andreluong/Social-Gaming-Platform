#pragma once

#include <variant>

#ifndef DATAVARIANT_H
#define DATAVARIANT_H

// Handles many data types for lists
// TODO: add models?
typedef std::variant<
    int, 
    float, 
    double, 
    char, 
    bool, 
    std::string
> dataVariant;

#endif DATAVARIANT_H