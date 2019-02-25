//
//  CSVLine.h
//  ISGAssignment
//
//  Created by Dan Appel on 2/22/19.
//  Copyright © 2019 Dan Appel. All rights reserved.
//

#pragma once

#include "CSVValue.h"
#include <string>
#include <vector>
#include <memory>

class CSVLine {
public:
    CSVLine(const std::string& line);
    inline const std::vector<std::shared_ptr<CSVValue>>& getValues() const {
        return values_;
    }
private:
    std::vector<std::shared_ptr<CSVValue>> values_;
};
