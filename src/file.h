#ifndef FILE_H
#define FILE_H

#include <stdbool.h>

typedef char File[1024];

bool FileLoad(File this, char* filePath);

#endif