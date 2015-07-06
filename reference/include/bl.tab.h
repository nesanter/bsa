/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_BL_TAB_H_INCLUDED
# define YY_YY_BL_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NUMERIC = 258,
    IDENT = 259,
    STRING = 260,
    LBRACE = 261,
    RBRACE = 262,
    LPAREN = 263,
    RPAREN = 264,
    LBRACK = 265,
    RBRACK = 266,
    DOT = 267,
    SEMI = 268,
    COMMA = 269,
    EQUAL = 270,
    FUNCTION = 271,
    WHILE = 272,
    IF = 273,
    ELSE = 274,
    BLOCK = 275,
    YIELD = 276,
    RETURN = 277,
    FORK = 278,
    SCOPE = 279,
    ALWAYS = 280,
    SUCCESS = 281,
    FAILURE = 282,
    OR = 283,
    XOR = 284,
    AND = 285,
    EQUAL_EQUAL = 286,
    LANGLE = 287,
    RANGLE = 288,
    LANGLE_EQUAL = 289,
    RANGLE_EQUAL = 290,
    PLUS = 291,
    MINUS = 292,
    STAR = 293,
    FSLASH = 294,
    PERCENT = 295,
    NOT = 296,
    UNARY = 297
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 8 "bl.y" /* yacc.c:1909  */

    uint64_t llu;
    char *text;

#line 102 "bl.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_BL_TAB_H_INCLUDED  */
