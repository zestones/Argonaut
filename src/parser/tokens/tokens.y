
%token PROG
%token SEMICOLON TWO_POINTS COMMA DOT_DOT OPEN_PARENTHESIS CLOSE_PARENTHESIS
%token START END
%token ARRAY OF OPEN_BRACKET CLOSE_BRACKET
%token IDENTIFIER VARIABLE OPAFF
%token STRUCT FSTRUCT
%token TYPE INTEGER FLOAT BOOLEAN CHARACTER STRING 
%token PROCEDURE FUNCTION RETURN_TYPE RETURN_VALUE
%token IF ELSE WHILE
%token EQUAL NOT_EQUAL LESS_THAN GREATER_THAN LESS_EQUAL GREATER_EQUAL

%left AND OR 
%right NOT

%left PLUS MINUS
%left MULTIPLY DIVIDE
