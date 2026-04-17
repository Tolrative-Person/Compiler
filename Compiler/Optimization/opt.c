#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 100

typedef struct {
    char result[10];
    char arg1[10];
    char op[5];
    char arg2[10];
} TAC;

TAC code[MAX];
int n;

// Utility
int isNumber(char *s) {
    for (int i = 0; s[i]; i++)
        if (!isdigit(s[i])) return 0;
    return 1;
}

int toInt(char *s) {
    return atoi(s);
}

// ----------------------
// CONSTANT FOLDING
// ----------------------
void constantFolding() {
    for (int i = 0; i < n; i++) {
        if (isNumber(code[i].arg1) && isNumber(code[i].arg2)) {

            int a = toInt(code[i].arg1);
            int b = toInt(code[i].arg2);
            int res;

            if (strcmp(code[i].op, "+") == 0) res = a + b;
            else if (strcmp(code[i].op, "-") == 0) res = a - b;
            else if (strcmp(code[i].op, "*") == 0) res = a * b;
            else if (strcmp(code[i].op, "/") == 0) res = a / b;
            else continue;

            sprintf(code[i].arg1, "%d", res);
            strcpy(code[i].op, "");
            strcpy(code[i].arg2, "");
        }
    }
}

// ----------------------
// ALGEBRAIC SIMPLIFICATION
// ----------------------
void algebraicSimplification() {
    for (int i = 0; i < n; i++) {

        if (strcmp(code[i].op, "*") == 0) {
            if (strcmp(code[i].arg2, "1") == 0) {
                strcpy(code[i].arg1, code[i].arg1);
                code[i].op[0] = '\0';
            }
        }

        if (strcmp(code[i].op, "+") == 0) {
            if (strcmp(code[i].arg2, "0") == 0) {
                code[i].op[0] = '\0';
            }
        }
    }
}

// ----------------------
// CONSTANT PROPAGATION
// ----------------------
void constantPropagation() {
    for (int i = 0; i < n; i++) {
        if (isNumber(code[i].arg1) && strlen(code[i].op) == 0) {

            for (int j = i + 1; j < n; j++) {
                if (strcmp(code[j].arg1, code[i].result) == 0)
                    strcpy(code[j].arg1, code[i].arg1);

                if (strcmp(code[j].arg2, code[i].result) == 0)
                    strcpy(code[j].arg2, code[i].arg1);
            }
        }
    }
}

// ----------------------
// DEAD CODE ELIMINATION
// ----------------------
int isUsed(char *var, int index) {
    for (int i = index + 1; i < n; i++) {
        if (strcmp(code[i].arg1, var) == 0 ||
            strcmp(code[i].arg2, var) == 0)
            return 1;
    }
    return 0;
}

void deadCodeElimination() {
    for (int i = 0; i < n; i++) {
        if (!isUsed(code[i].result, i)) {
            // remove line
            for (int j = i; j < n - 1; j++)
                code[j] = code[j + 1];
            n--;
            i--;
        }
    }
}

// ----------------------
// PRINT TAC
// ----------------------
void printCode() {
    for (int i = 0; i < n; i++) {
        if (strlen(code[i].op) == 0)
            printf("%s = %s\n", code[i].result, code[i].arg1);
        else
            printf("%s = %s %s %s\n",
                   code[i].result,
                   code[i].arg1,
                   code[i].op,
                   code[i].arg2);
    }
}

// ----------------------
// MAIN
// ----------------------
int main() {
    printf("Enter number of TAC instructions: ");
    scanf("%d", &n);

    printf("Enter TAC (result arg1 op arg2):\n");

    for (int i = 0; i < n; i++) {
        scanf("%s %s %s %s",
              code[i].result,
              code[i].arg1,
              code[i].op,
              code[i].arg2);
    }

    printf("\n--- Original Code ---\n");
    printCode();

    constantPropagation();
    constantFolding();
    algebraicSimplification();
    deadCodeElimination();

    printf("\n--- Optimized Code ---\n");
    printCode();

    return 0;
}