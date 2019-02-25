//
//  CSVValue.cpp
//  ISGAssignment
//
//  Created by Dan Appel on 2/22/19.
//  Copyright © 2019 Dan Appel. All rights reserved.
//

#include "CSVValue.h"
#include <cassert>

CSVValue::CSVValue(const std::string& value) {
    stringValue_ = value;
    valueType_ = CSVValueType::STRING;

    // attempt to convert to long, if fails then consider it a string value
    char* temp;
    std::strtol(value.c_str(), &temp, 0);
    if (*temp == '\0') {
        longValue_ = std::stol(value);
        valueType_ = CSVValueType::LONG;
    }
}

std::ostream& operator <<(std::ostream& os, CSVValue& value) {
    os << value.getString();
    return os;
}

bool operator <(CSVValue const& a, CSVValue const& b) {
    assert(a.getType() == b.getType());
    switch (a.getType()) {
        case CSVValueType::STRING: return a.getString() < b.getString();
        case CSVValueType::LONG: return a.getLong() < b.getLong();
    }
}

bool operator >(CSVValue const& a, CSVValue const& b) {
    assert(a.getType() == b.getType());
    switch (a.getType()) {
        case CSVValueType::STRING: return a.getString() > b.getString();
        case CSVValueType::LONG: return a.getLong() > b.getLong();
    }
}
