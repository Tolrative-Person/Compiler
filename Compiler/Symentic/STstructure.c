#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 100

typedef enum {
    TYPE_INT,
    TYPE_FLOAT
} DataType;

typedef struct {
    char name[100];
    DataType type;
} Symbol;

Symbol symbolTable[MAX_SYMBOLS];
int symbolCount = 0;