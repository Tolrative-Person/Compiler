#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// External lexer function
extern Token getNextToken();

Token currentToken;

// Forward declarations
void program();
void statement_list();
void statement();
void declaration();
void assignment();
void if_statement();
void while_statement();
void block();
void expression();
void term();
void factor();

void error(const char *msg) {
    printf("Syntax Error: %s\n", msg);
    exit(1);
}

void match(TokenType expected) {
    if (currentToken.type == expected) {
        currentToken = getNextToken();
    } else {
        error("Unexpected token");
    }
}

void program() {
    statement_list();
    if (currentToken.type != TOKEN_EOF)
        error("Expected EOF");
}

void statement_list() {
    while (currentToken.type != TOKEN_RBRACE &&
           currentToken.type != TOKEN_EOF) {
        statement();
    }
}

void statement() {
    switch (currentToken.type) {
        case TOKEN_INT:
        case TOKEN_FLOAT:
            declaration();
            break;

        case TOKEN_IDENTIFIER:
            assignment();
            break;

        case TOKEN_IF:
            if_statement();
            break;

        case TOKEN_WHILE:
            while_statement();
            break;

        case TOKEN_LBRACE:
            block();
            break;

        default:
            error("Invalid statement");
    }
}

void declaration() {
    match(currentToken.type); // int or float
    match(TOKEN_IDENTIFIER);
    match(TOKEN_SEMICOLON);
}

void assignment() {
    match(TOKEN_IDENTIFIER);
    match(TOKEN_ASSIGN);
    expression();
    match(TOKEN_SEMICOLON);
}

void if_statement() {
    match(TOKEN_IF);
    match(TOKEN_LPAREN);
    expression();
    match(TOKEN_RPAREN);
    statement();
}

void while_statement() {
    match(TOKEN_WHILE);
    match(TOKEN_LPAREN);
    expression();
    match(TOKEN_RPAREN);
    statement();
}

void block() {
    match(TOKEN_LBRACE);
    statement_list();
    match(TOKEN_RBRACE);
}

void expression() {
    term();
    while (currentToken.type == TOKEN_PLUS ||
           currentToken.type == TOKEN_MINUS) {
        match(currentToken.type);
        term();
    }
}

void term() {
    factor();
    while (currentToken.type == TOKEN_MUL ||
           currentToken.type == TOKEN_DIV) {
        match(currentToken.type);
        factor();
    }
}

void factor() {
    if (currentToken.type == TOKEN_IDENTIFIER) {
        match(TOKEN_IDENTIFIER);
    }
    else if (currentToken.type == TOKEN_NUMBER) {
        match(TOKEN_NUMBER);
    }
    else if (currentToken.type == TOKEN_LPAREN) {
        match(TOKEN_LPAREN);
        expression();
        match(TOKEN_RPAREN);
    }
    else {
        error("Invalid expression");
    }
}