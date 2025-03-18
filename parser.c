#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAX_LINE_LENGTH 100  
#define MAX_TOKEN_LENGTH 50
#define MAX_LEXEME_LENGTH 50

enum token_list {INTEGER, MINUS_OPERATOR, PLUS_OPERATOR, MULTIPLY_OPERATOR, DIVIDE_OPERATOR, LEFT_PARENTHESIS, RIGHT_PARENTHESIS};

FILE *tokenFile;
char line[MAX_LINE_LENGTH];
enum token_list token;
char lexeme[MAX_LEXEME_LENGTH];

void GetNextToken();
void expression();
void expression_suffix();
void term();
void term_suffix();
void factor();
void integer();
void integer_suffix();
void digit();


int main() {
    tokenFile = fopen("tokens.txt", "r");

    if (tokenFile == NULL) {
        fprintf(stderr, "Cannot open file!\n");
        return 1;
    }

    GetNextToken();


    return 0;
}

// Updates global lexeme and token type
void GetNextToken() {
    char inputToken[MAX_TOKEN_LENGTH];
    const char *commaPointer;

    // Pull the line and find the comma position
    fgets(line, sizeof(line), tokenFile);
    commaPointer = strchr(line, ',');

    if (commaPointer == NULL) {
        fprintf(stderr, "There is no comma\n");
        return;
    }


    // Pull lexeme and token
    strncpy(lexeme, line, strlen(line) - strlen(commaPointer));
    strncpy(inputToken, commaPointer + 2, MAX_TOKEN_LENGTH);
    inputToken[MAX_TOKEN_LENGTH - 1] = '\n';

    // Convert token to enum representation
    if (strcmp(inputToken, "integer\n") == 0) {
        token = INTEGER;
    } else if (strcmp(inputToken, "minus_operator\n") == 0) {
        token = MINUS_OPERATOR;
    } else if (strcmp(inputToken, "plus_operator\n") == 0) {
        token = PLUS_OPERATOR;
    } else if (strcmp(inputToken, "multiply_operator\n") == 0) {
        token = MULTIPLY_OPERATOR;
    } else if (strcmp(inputToken, "divide_operator\n") == 0) {
        token = DIVIDE_OPERATOR;
    } else if (strcmp(inputToken, "left_parenthesis\n") == 0) {
        token = LEFT_PARENTHESIS;
    } else if (strcmp(inputToken, "right_parenthesis\n") == 0) {
        token = RIGHT_PARENTHESIS;
    } else {
        fprintf(stderr, "Invalid token type\n");
        return;
    }
    
    printf("%s\n%d\n", lexeme, token);
}
