#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

static const char *src;
static int pos = 0;
static int line = 1;

void init_lexer(const char *source) {
    src = source;
    pos = 0;
    line = 1;
}

static char peek() {
    return src[pos];
}

static char advance() {
    return src[pos++];
}

static int is_keyword(const char *str) {
    if (strcmp(str, "int") == 0) return TOKEN_INT;
    if (strcmp(str, "return") == 0) return TOKEN_RETURN;
    if (strcmp(str, "if") == 0) return TOKEN_IF;
    if (strcmp(str, "else") == 0) return TOKEN_ELSE;
    if (strcmp(str, "while") == 0) return TOKEN_WHILE;
    return 0;
}

static void skip_whitespace() {
    while (isspace(peek())) {
        if (peek() == '\n')
            line++;
        advance();
    }
}

Token get_next_token() {
    Token token;
    token.lexeme[0] = '\0';
    token.line = line;

    skip_whitespace();

    char c = peek();

    if (c == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }

    // Identifiers / Keywords
    if (isalpha(c) || c == '_') {
        int start = pos;
        while (isalnum(peek()) || peek() == '_')
            advance();

        int length = pos - start;
        strncpy(token.lexeme, src + start, length);
        token.lexeme[length] = '\0';

        int keyword = is_keyword(token.lexeme);
        token.type = keyword ? keyword : TOKEN_IDENTIFIER;
        return token;
    }

    // Numbers
    if (isdigit(c)) {
        int start = pos;
        while (isdigit(peek()))
            advance();

        int length = pos - start;
        strncpy(token.lexeme, src + start, length);
        token.lexeme[length] = '\0';

        token.type = TOKEN_NUMBER;
        return token;
    }

    // Operators and Delimiters
    advance();
    switch (c) {
        case '+': token.type = TOKEN_PLUS; break;
        case '-': token.type = TOKEN_MINUS; break;
        case '*': token.type = TOKEN_STAR; break;
        case '/': token.type = TOKEN_SLASH; break;
        case ';': token.type = TOKEN_SEMICOLON; break;
        case ',': token.type = TOKEN_COMMA; break;
        case '(': token.type = TOKEN_LPAREN; break;
        case ')': token.type = TOKEN_RPAREN; break;
        case '{': token.type = TOKEN_LBRACE; break;
        case '}': token.type = TOKEN_RBRACE; break;

        case '=':
            if (peek() == '=') {
                advance();
                token.type = TOKEN_EQ;
            } else {
                token.type = TOKEN_ASSIGN;
            }
            break;

        case '!':
            if (peek() == '=') {
                advance();
                token.type = TOKEN_NEQ;
            } else {
                token.type = TOKEN_UNKNOWN;
            }
            break;

        case '<':
            if (peek() == '=') {
                advance();
                token.type = TOKEN_LTE;
            } else {
                token.type = TOKEN_LT;
            }
            break;

        case '>':
            if (peek() == '=') {
                advance();
                token.type = TOKEN_GTE;
            } else {
                token.type = TOKEN_GT;
            }
            break;

        default:
            token.type = TOKEN_UNKNOWN;
            break;
    }

    return token;
}
