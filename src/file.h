#ifndef FILE_H
#define FILE_H

#include <stdbool.h>

// -- types --

typedef char File[1024];

// -- interface --

bool File_Load(File this, char* filePath);

#endif