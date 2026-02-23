#include <stdio.h>
#include "lexer.h"

int main() {
    const char *source =
        "int main() { int x = 10; return x + 5; }";

    init_lexer(source);

    Token token;
    do {
        token = get_next_token();
        printf("Token: %-15d Lexeme: %-10s Line: %d\n",
               token.type, token.lexeme, token.line);
    } while (token.type != TOKEN_EOF);

    return 0;
}
