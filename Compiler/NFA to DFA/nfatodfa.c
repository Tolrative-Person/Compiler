#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 20
#define MAX_SYMBOLS 10

int nfa_states, nfa_symbols;
char symbols[MAX_SYMBOLS];

int nfa[MAX_STATES][MAX_SYMBOLS][MAX_STATES];
int nfa_count[MAX_STATES][MAX_SYMBOLS];

int epsilon_index = -1;

int dfa_states = 0;
int dfa[MAX_STATES][MAX_SYMBOLS][MAX_STATES];
int dfa_count[MAX_STATES][MAX_SYMBOLS];

int dfa_final[MAX_STATES];
int nfa_final[MAX_STATES];

int visited[MAX_STATES];

void epsilon_closure(int state, int *closure) {
    closure[state] = 1;

    if (epsilon_index == -1)
        return;

    for (int i = 0; i < nfa_count[state][epsilon_index]; i++) {
        int next = nfa[state][epsilon_index][i];
        if (!closure[next])
            epsilon_closure(next, closure);
    }
}

void compute_closure_set(int *input_set, int *closure_set) {
    for (int i = 0; i < nfa_states; i++) {
        if (input_set[i])
            epsilon_closure(i, closure_set);
    }
}

int compare_sets(int *a, int *b) {
    for (int i = 0; i < nfa_states; i++)
        if (a[i] != b[i])
            return 0;
    return 1;
}

void copy_set(int *dest, int *src) {
    for (int i = 0; i < nfa_states; i++)
        dest[i] = src[i];
}

void print_set(int *set) {
    printf("{ ");
    for (int i = 0; i < nfa_states; i++)
        if (set[i])
            printf("%d ", i);
    printf("}");
}

int main() {
    printf("Enter number of NFA states: ");
    scanf("%d", &nfa_states);

    printf("Enter number of symbols (including e for epsilon if any): ");
    scanf("%d", &nfa_symbols);

    printf("Enter symbols:\n");
    for (int i = 0; i < nfa_symbols; i++) {
        scanf(" %c", &symbols[i]);
        if (symbols[i] == 'e')
            epsilon_index = i;
    }

    memset(nfa_count, 0, sizeof(nfa_count));
    memset(nfa_final, 0, sizeof(nfa_final));

    printf("Enter transitions (format: from symbol to), -1 to stop\n");

    while (1) {
        int from, to;
        char sym;
        scanf("%d", &from);
        if (from == -1)
            break;

        scanf(" %c %d", &sym, &to);

        int sym_index = -1;
        for (int i = 0; i < nfa_symbols; i++)
            if (symbols[i] == sym)
                sym_index = i;

        int count = nfa_count[from][sym_index];
        nfa[from][sym_index][count] = to;
        nfa_count[from][sym_index]++;
    }

    printf("Enter number of final states: ");
    int final_count;
    scanf("%d", &final_count);

    printf("Enter final states:\n");
    for (int i = 0; i < final_count; i++) {
        int f;
        scanf("%d", &f);
        nfa_final[f] = 1;
    }

    int dfa_sets[MAX_STATES][MAX_STATES];
    memset(dfa_sets, 0, sizeof(dfa_sets));

    int start_set[MAX_STATES] = {0};
    start_set[0] = 1;

    compute_closure_set(start_set, dfa_sets[0]);
    dfa_states = 1;

    for (int i = 0; i < dfa_states; i++) {
        for (int j = 0; j < nfa_symbols; j++) {

            if (j == epsilon_index)
                continue;

            int new_set[MAX_STATES] = {0};

            for (int k = 0; k < nfa_states; k++) {
                if (dfa_sets[i][k]) {
                    for (int t = 0; t < nfa_count[k][j]; t++) {
                        int next = nfa[k][j][t];
                        new_set[next] = 1;
                    }
                }
            }

            int closure_set[MAX_STATES] = {0};
            compute_closure_set(new_set, closure_set);

            int found = -1;
            for (int s = 0; s < dfa_states; s++) {
                if (compare_sets(dfa_sets[s], closure_set)) {
                    found = s;
                    break;
                }
            }

            if (found == -1) {
                copy_set(dfa_sets[dfa_states], closure_set);
                found = dfa_states;
                dfa_states++;
            }

            dfa[i][j][0] = found;
            dfa_count[i][j] = 1;
        }
    }

    for (int i = 0; i < dfa_states; i++) {
        for (int j = 0; j < nfa_states; j++) {
            if (dfa_sets[i][j] && nfa_final[j]) {
                dfa_final[i] = 1;
            }
        }
    }

    printf("\nEquivalent DFA:\n");

    for (int i = 0; i < dfa_states; i++) {
        printf("State %d ", i);
        print_set(dfa_sets[i]);
        if (dfa_final[i])
            printf(" (Final)");
        printf("\n");

        for (int j = 0; j < nfa_symbols; j++) {
            if (j == epsilon_index)
                continue;

            printf("  On %c -> %d\n", symbols[j], dfa[i][j][0]);
        }
    }

    return 0;
}