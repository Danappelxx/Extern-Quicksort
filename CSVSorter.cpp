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
    std::shared_ptr<CSVReader> inputReader = std::make_shared<CSVReader>(inputFileName);
    std::vector<std::shared_ptr<TempFile>> tempFiles;

    while (true) {
        // read a chunk of lines
        std::vector<std::shared_ptr<CSVLine>> lines = inputReader->readLines(chunkSize);
        if (lines.empty())
            break;

        // sort the chunk
        sortLines(lines);

        // write the sorted lines to a temp file
        std::shared_ptr<TempFile> tempFile = std::make_shared<TempFile>();
        // stream will automatically be closed once csv writer goes out of scope
        CSVWriter(tempFile->getFileName()).writeLines(lines);

        // store the tempfiles so we can merge them back later
        tempFiles.push_back(tempFile);
    }

    std::shared_ptr<CSVWriter> outputWriter = std::make_shared<CSVWriter>(outputFileName);

    // have the last line from each temp file read
    std::vector<std::shared_ptr<CSVReader>> readers;
    std::vector<std::shared_ptr<CSVLine>> lastLines;
    for (auto tempFile: tempFiles) {
        std::shared_ptr<CSVReader> reader = std::make_shared<CSVReader>(tempFile->getFileName());
        readers.push_back(reader);
        lastLines.push_back(reader->readLine());
    }

    // merge the chunks into the output file, line by line
    while (true) {
        if (std::count(lastLines.begin(), lastLines.end(), nullptr) == lastLines.size())
            // all lines are nullptr, meaning no lines are left to be added
            break;

        // get the smallest line
        auto minLineIt = minLine(lastLines);
        auto minLineIndex = std::distance(lastLines.cbegin(), minLineIt);

        // write the smallest line
        outputWriter->writeLine(*minLineIt);

        // consume the next line from the reader
        lastLines[minLineIndex] = readers[minLineIndex]->readLine();
    }
}

bool CSVSorter::compareLines(std::shared_ptr<CSVLine> a, std::shared_ptr<CSVLine> b) {
    // we consider nullptr to be greater than ptr for the sake of sorting
    if (!a)
        return false;
    if (!b)
        return true;
    // comparator function for lines
    const auto& aValues = a->getValues();
    const auto& bValues = b->getValues();
    for (const int& col: sortColumns) {
        if (*aValues[col] < *bValues[col]) {
            return true;
        } else if (*aValues[col] > *bValues[col]) {
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
