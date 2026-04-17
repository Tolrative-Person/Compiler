#include <stdio.h>
#include <string.h>
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

int regCount = 0;

// Check if number
int isNumber(char *s) {
    for (int i = 0; s[i]; i++)
        if (!isdigit(s[i])) return 0;
    return 1;
}

// Get new register
void getRegister(char *reg) {
    sprintf(reg, "R%d", regCount++);
}

// ----------------------
// CODE GENERATION
// ----------------------
void generateCode() {
    char reg[10];

    printf("\n--- Target Code ---\n");

    for (int i = 0; i < n; i++) {

        // Case 1: Simple assignment → x = 5 or x = y
        if (strlen(code[i].op) == 0) {

            if (isNumber(code[i].arg1)) {
                printf("MOV %s, %s\n", code[i].result, code[i].arg1);
            } else {
                printf("MOV %s, %s\n", code[i].result, code[i].arg1);
            }
        }

        // Case 2: Arithmetic operations
        else {
            getRegister(reg);

            // Load first operand
            printf("MOV %s, %s\n", reg, code[i].arg1);

            // Perform operation
            if (strcmp(code[i].op, "+") == 0)
                printf("ADD %s, %s\n", reg, code[i].arg2);

            else if (strcmp(code[i].op, "-") == 0)
                printf("SUB %s, %s\n", reg, code[i].arg2);

            else if (strcmp(code[i].op, "*") == 0)
                printf("MUL %s, %s\n", reg, code[i].arg2);

            else if (strcmp(code[i].op, "/") == 0)
                printf("DIV %s, %s\n", reg, code[i].arg2);

            // Store result
            printf("MOV %s, %s\n", code[i].result, reg);
        }
    }
}

// ----------------------
// MAIN (Testing)
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

    generateCode();

    return 0;
}