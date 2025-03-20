# expression-parser

User will store expressions in separate file. We have stored them in expressions.txt.
User will need to run lex.l through FLEX to generate tokens.txt. tokens.txt is the input file for the main parser program.
run command "FLEX lex.l"
Insert the file name for expressions.
Output is stored in tokens.txt.
User will need to compile the parser program using the command "parser.c -o parser.out".
To run compiled file use the command "./parser.out"