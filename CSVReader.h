//
//  CSVReader.h
//  ISGAssignment
//
//  Created by Dan Appel on 2/22/19.
//  Copyright © 2019 Dan Appel. All rights reserved.
//

#pragma once

#include "CSVLine.h"
#include <string>
#include <vector>
#include <fstream>
#include <memory>

class CSVReader {
public:
    CSVReader(const std::string& fileName);
    // returns nullptr once it reaches eof
    std::shared_ptr<CSVLine> readLine();
    // returns a vector of lines UP TO count
    std::vector<std::shared_ptr<CSVLine>> readLines(int count);
private:
    std::ifstream stream_;
};
