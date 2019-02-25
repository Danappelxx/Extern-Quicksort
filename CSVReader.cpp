//
//  CSVReader.cpp
//  ISGAssignment
//
//  Created by Dan Appel on 2/22/19.
//  Copyright © 2019 Dan Appel. All rights reserved.
//

#include "CSVReader.h"

CSVReader::CSVReader(const std::string& fileName) {
    stream_ = std::ifstream(fileName);
}

std::shared_ptr<CSVLine> CSVReader::readLine() {
    std::string line;
    if (std::getline(stream_, line)) {
        return std::make_shared<CSVLine>(line);
    }
    return nullptr;
}

std::vector<std::shared_ptr<CSVLine>> CSVReader::readLines(int count) {
    std::vector<std::shared_ptr<CSVLine>> lines;
    for (int i = 0; i < count; i++) {
        if (std::shared_ptr<CSVLine> line = readLine()) {
            lines.push_back(line);
        } else {
            // no lines left
            break;
        }
    }
    return lines;
}
