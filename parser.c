#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAX_LINE_LENGTH 100  
#define MAX_TOKEN_LENGTH 50
#define MAX_LEXEME_LENGTH 50
#define MAX_NUM_LEXEMES 80

struct lexeme {
    char value[MAX_NUM_LEXEMES];
    char token[MAX_TOKEN_LENGTH];
};

// Abstract Syntax Tree (AST) Node
struct ASTNode {
    struct lexeme* lex;
    struct ASTNode* left;
    struct ASTNode* right;
};


FILE *tokenFile;
char line[MAX_LINE_LENGTH];
struct lexeme lexeme_list[MAX_LEXEME_LENGTH];
struct lexeme* currLexeme;
int currLexemePos = 0;
int numLexemes = 0;

void ParseLexemes();
void GetNextLexeme();
void PrintAST(struct ASTNode* root, int depth);

struct ASTNode* expression();
//void expression_suffix();
struct ASTNode* term();
//void term_suffix();
struct ASTNode* factor();
//void integer();
//void integer_suffix();
//void digit();


int main() {
    tokenFile = fopen("tokens.txt", "r");
    if (!tokenFile) {
        fprintf(stderr, "Error: Cannot open file!\n");
        exit(1);
    }

    ParseLexemes();

    for (int i = 0; i < numLexemes; i++) {
        printf("%s %s\n", lexeme_list[i].value, lexeme_list[i].token);
    }

    struct ASTNode* AST = expression();

    PrintAST(AST, 0);

    fclose(tokenFile);
    return 0;
}

// Reads expression tokens and updates global lexeme_array
void ParseLexemes() {
    // Read each line individually
    while (fgets(line, sizeof(line), tokenFile)) {
        const char *commaPointer;

        // Find comma position
        commaPointer = strchr(line, ',');
        if (commaPointer == NULL) {
            fprintf(stderr, "Error: There is no comma\n");
            exit(1);
        }

        // Pull lexeme and set it
        strncpy(lexeme_list[currLexemePos].value, line, strlen(line) - strlen(commaPointer));
        lexeme_list[currLexemePos].value[strlen(lexeme_list[currLexemePos].value)] = '\0';  // Make sure that it has null temination

        // Pull token and set it
        strncpy(lexeme_list[currLexemePos].token, commaPointer + 2, MAX_TOKEN_LENGTH);
        lexeme_list[currLexemePos].token[strlen(lexeme_list[currLexemePos].token) - 1] = '\0'; //Ensure null termination and remove newline
        
        // Iterate to the next position
        currLexemePos++;
    }

    // Get number of lexemes and reset the token position
    numLexemes = currLexemePos;
    currLexemePos = 0;

    if (ferror(tokenFile)) {
        fprintf(stderr, "Error: Issue reading file\n");
        exit(1);
    }
}

void GetNextLexeme() {
    // If we still have lexemes left set it, else set NULL
    if (currLexemePos < numLexemes) {
        currLexeme = &lexeme_list[currLexemePos];
        currLexemePos++;
    } else {
        currLexeme = NULL;
    }
}

void PrintAST(struct ASTNode* root, int depth) {
    if (!root) return;
    for (int i = 0; i < depth; i++) printf("    ");
    printf("%s\n", root->lex->value);
    printAST(root->left, depth + 1);
    printAST(root->right, depth + 1);
}

struct ASTNode* expression() {
    struct ASTNode* left = term();

    while (currLexeme != NULL && (strcmp(currLexeme->value, "+") == 0 || strcmp(currLexeme->value, "-") == 0)) {
        struct ASTNode* tempNode = (struct ASTNode*)malloc(sizeof(struct ASTNode));
        tempNode->lex = currLexeme;

        GetNextLexeme();
        struct ASTNode* right = term();

        tempNode->left = left;
        tempNode->right = right;
        left = tempNode;  // Update left to keep building the tree
    }

    return left;  // Always return the root of the expression tree
}


struct ASTNode* term() {
    struct ASTNode* node = factor();
   
    while (currLexeme != NULL && (strcmp(currLexeme->value, "*") == 0 || strcmp(currLexeme->value, "/") == 0)) {
        getToken();  // Consume operator
        struct ASTNode* right = factor();
        struct ASTNode* tempNode = (struct ASTNode*)malloc(sizeof(struct ASTNode));
        tempNode->lex = currLexeme;
        
        tempNode->left = node;
        tempNode->right = right;
        node = tempNode;
    }

    
    return node;
}


struct ASTNode* factor() {
    GetNextLexeme();
    
    if (currLexeme == NULL) {
        fprintf(stderr, "Error: Unexpected end of input\n");
        exit(1);
    }

    if (strcmp(currLexeme->token, "integer") == 0) {
        struct ASTNode* tempNode = (struct ASTNode*)malloc(sizeof(struct ASTNode));
        tempNode->lex = currLexeme;
        tempNode->left = NULL;
        tempNode->right = NULL;  // Initialize child nodes to NULL
        return tempNode;
    } else if (strcmp(currLexeme->value, "(") == 0) {
        struct ASTNode* node = expression();

        GetNextLexeme();  // Consume next token to check for ')'
        if (currLexeme == NULL || strcmp(currLexeme->value, ")") != 0) {
            fprintf(stderr, "Error: Missing closing parenthesis\n");
            exit(1);
        }

        return node;
    }

    fprintf(stderr, "Error: Invalid syntax\n");
    exit(1);
}

