%{
    #include "../symbol_table/lexeme/lexeme_table.h"
    #include "../parser/parser.h"

    #include "../utils/errors.h"
    #include "../utils/memory.h"
    #include "../utils/numeric_parser.h"
    #include "y.tab.h"

    #include <limits.h>

    #include "../lexer/escapes.h"

    /* Reports an unknown escape sequence such as \q. */
    static void bad_escape(const char *position) {
        set_error_type(&error, LEXICAL_ERROR);
        set_error_message(
            &error,
            "Unknown escape sequence '%.2s' at %s.\n"
            "  Supported escapes: \\n \\t \\r \\\\ \\' \\\".\n",
            position,
            get_formatted_location()
        );
        yerror(error);
    }

    extern Error error;
    extern char *yytext;
    extern int yyerror(const char *s);
%}

%option noinput nounput

number [0-9]
letter [a-zA-Z]

%%

[ \t]+                          { error.column += yyleng; }  

"//".*						    { error.column += yyleng; }  

\n                              { error.line++; error.column = 1; } // Reset column number on new line

"{"                             { error.column += yyleng; return START; }
"}"                             { error.column += yyleng; return END; }

"var"                           { error.column += yyleng; return VARIABLE; }
"proc"                          { error.column += yyleng; return PROCEDURE; }
"func"                          { error.column += yyleng; return FUNCTION; }

"return"                        { error.column += yyleng; return RETURN_VALUE; }
"->"                            { error.column += yyleng; return RETURN_TYPE; }

"type"                          { error.column += yyleng; return TYPE; }
"struct"                        { error.column += yyleng; return STRUCT; }
"fstruct"                       { error.column += yyleng; return FSTRUCT; }
"array"                         { error.column += yyleng; return ARRAY; }
"of"                            { error.column += yyleng; return OF; }

"if"                            { error.column += yyleng; return IF; }
"else"                          { error.column += yyleng; return ELSE; }
"while"                         { error.column += yyleng; return WHILE; }
"for"                           { error.column += yyleng; return FOR; }

"break"                         {error.column += yyleng; return BREAK; }
"continue"                         {error.column += yyleng; return CONTINUE; }


"int"                           { error.column += yyleng; return INTEGER_TYPE; }
"float"                         { error.column += yyleng; return FLOAT_TYPE; }
"bool"                          { error.column += yyleng; return BOOLEAN_TYPE; }
"char"                          { error.column += yyleng; return CHARACTER_TYPE; }
"string"                        { error.column += yyleng; return STRING_TYPE; }

"=="                            { error.column += yyleng; return EQUAL; }
"!="                            { error.column += yyleng; return NOT_EQUAL; }
"<"                             { error.column += yyleng; return LESS_THAN; }
">"                             { error.column += yyleng; return GREATER_THAN; }
"<="                            { error.column += yyleng; return LESS_EQUAL; }
">="                            { error.column += yyleng; return GREATER_EQUAL; }

"&&"                            { error.column += yyleng; return AND; }
"||"                            { error.column += yyleng; return OR; }
"!"                             { error.column += yyleng; return NOT; }

"++"                            { error.column += yyleng; yylval.lexicographic_index = insert_lexeme("1"); return INCREMENT; }
"--"                            { error.column += yyleng; yylval.lexicographic_index = insert_lexeme("1"); return DECREMENT; }

"+"                             { error.column += yyleng; return PLUS; }
"-"                             { error.column += yyleng; return MINUS; }
"*"                             { error.column += yyleng; return MULTIPLY; }
"/"                             { error.column += yyleng; return DIVIDE; }
"%"                             { error.column += yyleng; return MODULO; }

":="                            { error.column += yyleng; return OPAFF; }

";"                             { error.column += yyleng; return SEMICOLON; }
":"                             { error.column += yyleng; return TWO_POINTS; }
"."                             { error.column += yyleng; return DOT; }
","                             { error.column += yyleng; return COMMA; }

"["                             { error.column += yyleng; return OPEN_BRACKET; }
"]"                             { error.column += yyleng; return CLOSE_BRACKET; }
"("                             { error.column += yyleng; return OPEN_PARENTHESIS; }
")"                             { error.column += yyleng; return CLOSE_PARENTHESIS; }

"print"                         { error.column += yyleng; return PRINT; }
"input"                         { error.column += yyleng; return INPUT; }


"true" { 
    yylval.lexicographic_index = insert_lexeme("true");
    error.column += yyleng;
    return BOOLEAN_VALUE; 
}

"false" { 
    yylval.lexicographic_index = insert_lexeme("false");
    error.column += yyleng;
    return BOOLEAN_VALUE; 
}

"'"([^'\\\n]|\\[^\n])"'" {
    const char *bad = NULL;
    char *decoded = decode_escapes(yytext + 1, yyleng - 2, &bad);
    if (decoded == NULL) bad_escape(bad);

    char lexeme[4] = { '\'', decoded ? decoded[0] : '?', '\'', '\0' };
    free(decoded);
    yylval.lexicographic_index = insert_lexeme(lexeme);
    error.column += yyleng;
    return CHARACTER_VALUE;
}

\"([^\"\\]|\\[^\n])*\" {
    const char *bad = NULL;
    char *decoded = decode_escapes(yytext + 1, yyleng - 2, &bad);
    if (decoded == NULL) bad_escape(bad);

    // The lexeme keeps its quotes around the decoded text.
    size_t length = decoded ? strlen(decoded) : 0;
    char *lexeme = xmalloc(length + 3);
    lexeme[0] = '"';
    if (decoded) memcpy(lexeme + 1, decoded, length);
    lexeme[length + 1] = '"';
    lexeme[length + 2] = '\0';
    yylval.lexicographic_index = insert_lexeme(lexeme);
    free(lexeme);
    free(decoded);

    // A string literal may span several lines: keep the line numbers right for later errors.
    for (char *p = yytext; *p != '\0'; p++) {
        if (*p == '\n') { error.line++; error.column = 1; }
        else error.column++;
    }
    return STRING_VALUE;
}

{number}+"."{number}* { 
    yylval.lexicographic_index = insert_lexeme(yytext);
    error.column += yyleng;
    return FLOAT_VALUE; 
}

0|[1-9][0-9]* {
    if (!parse_integer_text(yytext, NULL)) {
        set_error_type(&error, LEXICAL_ERROR);
        set_error_message(
            &error,
            "Integer literal out of range at %s.\n"
            "  The literal '%s' does not fit in a 32-bit integer (%d to %d).\n",
            get_formatted_location(),
            yytext,
            INT_MIN, INT_MAX
        );
        yerror(error);
    }
    yylval.lexicographic_index = insert_lexeme(yytext);
    error.column += yyleng;
    return INTEGER_VALUE;
}

{letter}+({letter}|{number}|"_")* {
    yylval.lexicographic_index = insert_lexeme(yytext);
    error.column += yyleng;
    return IDENTIFIER;
}

. { 
    set_error_type(&error, LEXICAL_ERROR);
    set_error_message(
        &error,
        "Unknown character at %s.\n"
        "  The character '%s' is not recognized by the parser.\n"
        "  Ensure that the character is valid for the expected syntax.\n",
        get_formatted_location(),
        yytext
    );
    yerror(error);  
    error.column += yyleng;
} 

%%

int yywrap(void) {
    return 1;
}
