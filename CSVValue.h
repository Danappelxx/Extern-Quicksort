//
//  CSVValue.h
//  ISGAssignment
//
//  Created by Dan Appel on 2/22/19.
//  Copyright © 2019 Dan Appel. All rights reserved.
//

#pragma once

#include <string>
#include <iostream>

enum class CSVValueType {
    // asumed that types are only strings and integers, and can fit into 32 bits.
    STRING, LONG
};

class CSVValue {
public:
    CSVValue(const std::string& value);
    inline const CSVValueType& getType() const {
        return valueType_;
    }
    inline const long& getLong() const {
        return longValue_;
    }
    inline const std::string& getString() const {
        return stringValue_;
    };
private:
    CSVValueType valueType_;
    std::string stringValue_;
    long longValue_;
};

std::ostream& operator<<(std::ostream& os, CSVValue& value);
bool operator<(CSVValue const& a, CSVValue const& b);
bool operator>(CSVValue const& a, CSVValue const& b);
