//
//  TempFile.h
//  ISGAssignment
//
//  Created by Dan Appel on 2/22/19.
//  Copyright © 2019 Dan Appel. All rights reserved.
//

#pragma once

#include <unistd.h>
#include <string>
#include <string.h>

class TempFile {
public:
    TempFile() {
        fileName_ = strdup("/tmp/isg.XXXXXX");
        fileDescriptor_ = mkstemp(fileName_);
        assert(fileDescriptor_ != -1 && "Failed to create temporary file.");
        // close it so others can open it with the filename
        close(fileDescriptor_);
    }

    ~TempFile() {
        // manually delete the temporary file, not done by the system
        remove(fileName_);
        free(fileName_);
    }

    const char* getFileName() const {
        return fileName_;
    }
private:
    char* fileName_;
    int fileDescriptor_;
};
