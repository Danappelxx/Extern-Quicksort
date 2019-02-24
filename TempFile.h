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
        fileName = strdup("/tmp/isg.XXXXXX");
        fileDescriptor = mkstemp(fileName);
        assert(fileDescriptor != -1 && "Failed to create temporary file.");
        // close it so others can open it with the filename
        close(fileDescriptor);
    }

    ~TempFile() {
        // manually delete the temporary file, not done by the system
        remove(fileName);
        free(fileName);
    }

    const char* getFileName() const {
        return fileName;
    }
private:
    char* fileName;
    int fileDescriptor;
};
