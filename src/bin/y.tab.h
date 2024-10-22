/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SRC_BIN_Y_TAB_H_INCLUDED
# define YY_YY_SRC_BIN_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    PROG = 258,                    /* PROG  */
    SEMICOLON = 259,               /* SEMICOLON  */
    TWO_POINTS = 260,              /* TWO_POINTS  */
    COMMA = 261,                   /* COMMA  */
    DOT_DOT = 262,                 /* DOT_DOT  */
    OPEN_PARENTHESIS = 263,        /* OPEN_PARENTHESIS  */
    CLOSE_PARENTHESIS = 264,       /* CLOSE_PARENTHESIS  */
    START = 265,                   /* START  */
    END = 266,                     /* END  */
    ARRAY = 267,                   /* ARRAY  */
    OF = 268,                      /* OF  */
    OPEN_BRACKET = 269,            /* OPEN_BRACKET  */
    CLOSE_BRACKET = 270,           /* CLOSE_BRACKET  */
    IDENTIFIER = 271,              /* IDENTIFIER  */
    VARIABLE = 272,                /* VARIABLE  */
    OPAFF = 273,                   /* OPAFF  */
    STRUCT = 274,                  /* STRUCT  */
    FSTRUCT = 275,                 /* FSTRUCT  */
    TYPE = 276,                    /* TYPE  */
    INTEGER = 277,                 /* INTEGER  */
    FLOAT = 278,                   /* FLOAT  */
    BOOLEAN = 279,                 /* BOOLEAN  */
    CHARACTER = 280,               /* CHARACTER  */
    STRING = 281,                  /* STRING  */
    PROCEDURE = 282,               /* PROCEDURE  */
    FUNCTION = 283,                /* FUNCTION  */
    RETURN_TYPE = 284,             /* RETURN_TYPE  */
    RETURN_VALUE = 285,            /* RETURN_VALUE  */
    IF = 286,                      /* IF  */
    ELSE = 287,                    /* ELSE  */
    WHILE = 288,                   /* WHILE  */
    EQUAL = 289,                   /* EQUAL  */
    NOT_EQUAL = 290,               /* NOT_EQUAL  */
    LESS_THAN = 291,               /* LESS_THAN  */
    GREATER_THAN = 292,            /* GREATER_THAN  */
    LESS_EQUAL = 293,              /* LESS_EQUAL  */
    GREATER_EQUAL = 294,           /* GREATER_EQUAL  */
    AND = 295,                     /* AND  */
    OR = 296,                      /* OR  */
    NOT = 297,                     /* NOT  */
    PLUS = 298,                    /* PLUS  */
    MINUS = 299,                   /* MINUS  */
    MULTIPLY = 300,                /* MULTIPLY  */
    DIVIDE = 301                   /* DIVIDE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define PROG 258
#define SEMICOLON 259
#define TWO_POINTS 260
#define COMMA 261
#define DOT_DOT 262
#define OPEN_PARENTHESIS 263
#define CLOSE_PARENTHESIS 264
#define START 265
#define END 266
#define ARRAY 267
#define OF 268
#define OPEN_BRACKET 269
#define CLOSE_BRACKET 270
#define IDENTIFIER 271
#define VARIABLE 272
#define OPAFF 273
#define STRUCT 274
#define FSTRUCT 275
#define TYPE 276
#define INTEGER 277
#define FLOAT 278
#define BOOLEAN 279
#define CHARACTER 280
#define STRING 281
#define PROCEDURE 282
#define FUNCTION 283
#define RETURN_TYPE 284
#define RETURN_VALUE 285
#define IF 286
#define ELSE 287
#define WHILE 288
#define EQUAL 289
#define NOT_EQUAL 290
#define LESS_THAN 291
#define GREATER_THAN 292
#define LESS_EQUAL 293
#define GREATER_EQUAL 294
#define AND 295
#define OR 296
#define NOT 297
#define PLUS 298
#define MINUS 299
#define MULTIPLY 300
#define DIVIDE 301

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SRC_BIN_Y_TAB_H_INCLUDED  */
