//
//  CSVLine.cpp
//  ISGAssignment
//
//  Created by Dan Appel on 2/22/19.
//  Copyright © 2019 Dan Appel. All rights reserved.
//

#include "CSVLine.h"
#include <sstream>

CSVLine::CSVLine(const std::string& line) {
    values = std::vector<std::shared_ptr<CSVValue>>();

    std::istringstream ss(line);
    std::string current;
    while(std::getline(ss, current, ',')) {
        values.push_back(std::make_shared<CSVValue>(current));
    }
}
