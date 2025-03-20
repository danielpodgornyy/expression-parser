%{
FILE *outputFile = fopen("tokens.txt", "w");
%}

%%
[0-9]+ {fprintf(outputFile, "%s, integer\n", yytext);}
[-] {fprintf(outputFile, "%s, minus_operator\n", yytext);}
[+] {fprintf(outputFile, "%s, plus_operator\n", yytext);}
[*] {fprintf(outputFile, "%s, multiply_operator\n", yytext);}
[/] {fprintf(outputFile, "%s, divide_operator\n", yytext);}
[(] {fprintf(outputFile, "%s, left_parenthesis\n", yytext);}
[)] {fprintf(outputFile, "%s, right_parenthesis\n", yytext);}
\n    {return 0;}
%%

/*** Code Section prints the number of
capital letter present in the given input***/
int yywrap(){}
int main(){
    FILE *inputFile;
    char filename[70];

    printf("Enter the filename: \n");
    scanf("%s",filename);
    inputFile = fopen(filename,"r");
    yyin = inputFile;

    yylex();
    fclose(outputFile);
    return 0;
}

