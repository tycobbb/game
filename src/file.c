#include "file.h"
#include <stddef.h>
#include <stdio.h>
#include <errno.h>

// -- implementation --

bool File_Load(File this, char* filePath) {
    FILE* f = fopen(filePath, "r");
    if(f == NULL) {
        printf("ERROR::FILE::LOAD_FAILED\n%s\n%d\n", filePath, errno);
        return false;
    }

    size_t bytesRead = fread(this, 1, 1024, f);
    this[bytesRead] = '\0';

    fclose(f);

    return true;
}
