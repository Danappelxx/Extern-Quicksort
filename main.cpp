//
//  main.cpp
//  ISGAssignment
//
//  Created by Dan Appel on 2/22/19.
//  Copyright © 2019 Dan Appel. All rights reserved.
//

#include "CSVSorter.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

std::vector<int> parseColumnIndices(const std::string& string) {
    std::istringstream ss(string);
    std::vector<int> indices;
    std::string current;
    while(std::getline(ss, current, ',')) {
        indices.push_back(std::stoi(current));
    }
    return indices;
}

int main(int argc, const char* argv[]) {
    if (argc != 5)
        return 1;

    const char* inputFileName = argv[1];
    const char* outputFileName = argv[2];
    std::vector<int> sortColumns = parseColumnIndices(argv[3]);
    int threadCount = std::stoi(argv[4]);

    int chunkSize = 1000000;

    CSVSorter sorter(inputFileName, outputFileName, sortColumns, threadCount, chunkSize);
    sorter.sort();
 
    return 0;
}
