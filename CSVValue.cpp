//
//  CSVValue.cpp
//  ISGAssignment
//
//  Created by Dan Appel on 2/22/19.
//  Copyright © 2019 Dan Appel. All rights reserved.
//

#include "CSVValue.h"

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
    if (a.getType() != b.getType()) {
        // this is really an error in the data - it means that different values in
        // the same column have different types. not much we can do about it though
        // fall back to string comparison as that is the most likely intention
        return a.getString() < b.getString();
    }
    switch (a.getType()) {
        case CSVValueType::STRING: return a.getString() < b.getString();
        case CSVValueType::LONG: return a.getLong() < b.getLong();
    }
}

bool operator >(CSVValue const& a, CSVValue const& b) {
    if (a.getType() != b.getType()) {
        // this is really an error in the data - it means that different values in
        // the same column have different types. not much we can do about it though
        // fall back to string comparison as that is the most likely intention
        return a.getString() > b.getString();
    }
    switch (a.getType()) {
        case CSVValueType::STRING: return a.getString() > b.getString();
        case CSVValueType::LONG: return a.getLong() > b.getLong();
    }
}
