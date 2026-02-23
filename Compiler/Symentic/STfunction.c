int lookup(char *name) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0)
            return i;
    }
    return -1;
}

void insert(char *name, DataType type) {
    if (lookup(name) != -1) {
        printf("Semantic Error: Variable '%s' already declared\n", name);
        exit(1);
    }

    if (symbolCount >= MAX_SYMBOLS) {
        printf("Symbol Table Overflow\n");
        exit(1);
    }

    strcpy(symbolTable[symbolCount].name, name);
    symbolTable[symbolCount].type = type;
    symbolCount++;
}

DataType getType(char *name) {
    int index = lookup(name);
    if (index == -1) {
        printf("Semantic Error: Variable '%s' not declared\n", name);
        exit(1);
    }
    return symbolTable[index].type;
}