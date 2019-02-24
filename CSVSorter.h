//
//  CSVSorter.h
//  ISGAssignment
//
//  Created by Dan Appel on 2/22/19.
//  Copyright © 2019 Dan Appel. All rights reserved.
//

#pragma once

#include "CSVLine.h"
#include <vector>
#include <string>
#include <memory>

class CSVSorter {
public:
    CSVSorter(const std::string& inputFileName, const std::string& outputFileName, const std::vector<int>& sortColumns, int threadCount, int chunkSize);

    // reads input file, breaks it into sorted chunks, merges those chunks into output file
    void sort();
private:
    std::string inputFileName;
    std::string outputFileName;
    std::vector<int> sortColumns;
    int threadCount;
    int chunkSize;

    // sorts vector of lines in place
    void sortLines(std::vector<std::shared_ptr<CSVLine>>& lines);

    // compares two lines according to this->sortColumns
    bool compareLines(std::shared_ptr<CSVLine> a, std::shared_ptr<CSVLine> b);

    // returns an iterator to the min line according to this->compareLines
    std::vector<std::shared_ptr<CSVLine>>::const_iterator minLine(const std::vector<std::shared_ptr<CSVLine>>& lines);
};
