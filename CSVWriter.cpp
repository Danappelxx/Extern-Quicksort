//
//  CSVWriter.cpp
//  ISGAssignment
//
//  Created by Dan Appel on 2/22/19.
//  Copyright © 2019 Dan Appel. All rights reserved.
//

#include "CSVWriter.h"

CSVWriter::CSVWriter(const std::string& fileName) {
    stream_ = std::ofstream(fileName);
}

void CSVWriter::writeLine(std::shared_ptr<CSVLine> line) {
    auto values = line->getValues();

    stream_ << **values.begin();
    for (auto it = values.begin() + 1; it != values.end(); it++) {
        stream_ << "," << **it;
    }
    stream_ << std::endl;
}

void CSVWriter::writeLines(const std::vector<std::shared_ptr<CSVLine>>& lines) {
    for (const auto& line: lines) {
        writeLine(line);
    }
}
