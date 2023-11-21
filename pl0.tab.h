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

#ifndef YY_YY_PL0_TAB_H_INCLUDED
# define YY_YY_PL0_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 7 "pl0.y"


 /* Including "ast.h" must be at the top, to define the AST type */
#include "ast.h"
#include "machine_types.h"
#include "parser_types.h"
#include "lexer.h"

    /* Report an error to the user on stderr */
extern void yyerror(const char *filename, const char *msg);


#line 62 "pl0.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    identsym = 258,                /* identsym  */
    numbersym = 259,               /* numbersym  */
    plussym = 260,                 /* "+"  */
    minussym = 261,                /* "-"  */
    multsym = 262,                 /* "*"  */
    divsym = 263,                  /* "/"  */
    periodsym = 264,               /* "."  */
    semisym = 265,                 /* ";"  */
    eqsym = 266,                   /* "="  */
    commasym = 267,                /* ","  */
    becomessym = 268,              /* ":="  */
    constsym = 269,                /* "const"  */
    varsym = 270,                  /* "var"  */
    proceduresym = 271,            /* "procedure"  */
    callsym = 272,                 /* "call"  */
    beginsym = 273,                /* "begin"  */
    endsym = 274,                  /* "end"  */
    ifsym = 275,                   /* "if"  */
    thensym = 276,                 /* "then"  */
    elsesym = 277,                 /* "else"  */
    whilesym = 278,                /* "while"  */
    dosym = 279,                   /* "do"  */
    readsym = 280,                 /* "read"  */
    writesym = 281,                /* "write"  */
    skipsym = 282,                 /* "skip"  */
    oddsym = 283,                  /* "odd"  */
    neqsym = 284,                  /* "<>"  */
    ltsym = 285,                   /* "<"  */
    leqsym = 286,                  /* "<="  */
    gtsym = 287,                   /* ">"  */
    geqsym = 288,                  /* ">="  */
    lparensym = 289,               /* "("  */
    rparensym = 290                /* ")"  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (char const *file_name);


#endif /* !YY_YY_PL0_TAB_H_INCLUDED  */
