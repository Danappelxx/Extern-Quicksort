//
//  CSVSorter.cpp
//  ISGAssignment
//
//  Created by Dan Appel on 2/22/19.
//  Copyright © 2019 Dan Appel. All rights reserved.
//

#include "CSVSorter.h"
#include "Quicksort.h"
#include "CSVReader.h"
#include "CSVWriter.h"
#include "TempFile.h"
#include <fstream>
#include <algorithm>

CSVSorter::CSVSorter(const std::string& inputFileName, const std::string& outputFileName, const std::vector<int>& sortColumns, int threadCount, int chunkSize)
    : inputFileName(inputFileName), outputFileName(outputFileName), sortColumns(sortColumns), threadCount(threadCount), chunkSize(chunkSize) {
}

void CSVSorter::sort() {
    // algorithm is as follows:
    //  - process input file chunk by chunk
    //  - for each chunk, sort the chunk and write it to a temporary file
    //  - finally, perform a line-by-line merge on the temp files into the output file

    CSVReader inputReader(inputFileName);
    std::vector<std::shared_ptr<TempFile>> tempFiles;

    while (true) {
        // read a chunk of lines
        std::vector<std::shared_ptr<CSVLine>> lines = inputReader.readLines(chunkSize);
        if (lines.empty())
            // reached eof
            break;

        // sort the chunk
        sortLines(lines);

        // write the sorted lines to a temp file
        std::shared_ptr<TempFile> tempFile = std::make_shared<TempFile>();
        // stream will automatically be closed once csv writer goes out of scope
        CSVWriter(tempFile->getFileName()).writeLines(lines);

        tempFiles.push_back(tempFile);
    }

    CSVWriter outputWriter(outputFileName);

    // store readers and the latest line from each reader
    std::vector<std::shared_ptr<CSVReader>> readers;
    std::vector<std::shared_ptr<CSVLine>> lastLines;
    for (auto tempFile: tempFiles) {
        std::shared_ptr<CSVReader> reader = std::make_shared<CSVReader>(tempFile->getFileName());
        readers.push_back(reader);
        lastLines.push_back(reader->readLine());
    }

    // merge the chunks into the output file, line by line
    while (true) {
        // if all lines are nullptr, then we have completed the merge
        if (std::count(lastLines.begin(), lastLines.end(), nullptr) == lastLines.size())
            break;

        // get the smallest line
        auto minLineIt = minLine(lastLines);
        auto minLineIndex = std::distance(lastLines.cbegin(), minLineIt);

        // write the smallest line
        outputWriter.writeLine(*minLineIt);

        // consume the next line from the reader, nullptr if eof
        lastLines[minLineIndex] = readers[minLineIndex]->readLine();
    }
}

bool CSVSorter::compareLines(std::shared_ptr<CSVLine> a, std::shared_ptr<CSVLine> b) {
    // we consider nullptr to be greater than ptr for the sake of sorting
    if (!a)
        return false;
    if (!b)
        return true;
    // comparison follows specified column order
    for (const int& col: sortColumns) {
        if (*a->getValues()[col] < *b->getValues()[col]) {
            return true;
        } else if (*a->getValues()[col] > *b->getValues()[col]) {
            return false;
        }
        // if not < or >, then equal, move on to next column
    }
    return false;
}

void CSVSorter::sortLines(std::vector<std::shared_ptr<CSVLine>>& lines) {
    auto comparator = std::bind(&CSVSorter::compareLines, this, std::placeholders::_1, std::placeholders::_2);
    Quicksort<std::shared_ptr<CSVLine>>::sort(lines.begin(), lines.end(), threadCount, comparator);
}

std::vector<std::shared_ptr<CSVLine>>::const_iterator CSVSorter::minLine(const std::vector<std::shared_ptr<CSVLine>>& lines) {
    auto comparator = std::bind(&CSVSorter::compareLines, this, std::placeholders::_1, std::placeholders::_2);
    return std::min_element(lines.begin(), lines.end(), comparator);
}
