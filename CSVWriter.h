//
//  CSVWriter.h
//  ISGAssignment
//
//  Created by Dan Appel on 2/22/19.
//  Copyright © 2019 Dan Appel. All rights reserved.
//

#pragma once

#include "CSVLine.h"
#include <fstream>
#include <vector>
#include <string>

class CSVWriter {
public:
    CSVWriter(const std::string& fileName);
    void writeLine(std::shared_ptr<CSVLine> line);
    void writeLines(const std::vector<std::shared_ptr<CSVLine>>& lines);
private:
    std::ofstream stream;
};
