/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

#ifndef YY_SQL_PARSER_H_INCLUDED
# define YY_SQL_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef SQL_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define SQL_DEBUG 1
#  else
#   define SQL_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define SQL_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined SQL_DEBUG */
#if SQL_DEBUG
extern int sql_debug;
#endif
/* "%code requires" blocks.  */
#line 16 "parser.y" /* yacc.c:1909  */

// %code requires block	
#include "AST.h"
#include <vector>
typedef void* yyscan_t;
#define YYSTYPE SQL_STYPE
#define YYLTYPE SQL_LTYPE
struct SQL_CUST_LTYPE {
	int first_line;
	int first_column;
	int last_line;
	int last_column;
	int total_column;
};
#define SQL_LTYPE SQL_CUST_LTYPE
#define SQL_LTYPE_IS_DECLARED 1
// Auto update column and line number
#define YY_USER_ACTION \
    yylloc->first_line = yylloc->last_line; \
    yylloc->first_column = yylloc->last_column; \
    for(int i = 0; yytext[i] != '\0'; i++) { \
    	yylloc->total_column++; \
        if(yytext[i] == '\n') { \
            yylloc->last_line++; \
            yylloc->last_column = 0; \
        } \
        else { \
            yylloc->last_column++; \
        } \
    }
AST* parseSQLString(const char* sql);
AST* parseSQLString(const std::string& sql);

#line 86 "parser.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef SQL_TOKENTYPE
# define SQL_TOKENTYPE
  enum sql_tokentype
  {
    SQL_IDENTIFIER = 258,
    SQL_STRING = 259,
    SQL_FLOATVAL = 260,
    SQL_INTVAL = 261,
    SQL_NOTEQUALS = 262,
    SQL_LESSEQ = 263,
    SQL_GREATEREQ = 264,
    SQL_DATABASES = 265,
    SQL_DISTINCT = 266,
    SQL_DATABASE = 267,
    SQL_BETWEEN = 268,
    SQL_VARCHAR = 269,
    SQL_CASCADE = 270,
    SQL_COLUMNS = 271,
    SQL_CONTROL = 272,
    SQL_DEFAULT = 273,
    SQL_DECIMAL = 274,
    SQL_EXPLAIN = 275,
    SQL_HISTORY = 276,
    SQL_INTEGER = 277,
    SQL_NATURAL = 278,
    SQL_PRIMARY = 279,
    SQL_SCHEMAS = 280,
    SQL_SPATIAL = 281,
    SQL_VIRTUAL = 282,
    SQL_BEFORE = 283,
    SQL_COLUMN = 284,
    SQL_CREATE = 285,
    SQL_DELETE = 286,
    SQL_DIRECT = 287,
    SQL_DOUBLE = 288,
    SQL_ESCAPE = 289,
    SQL_EXCEPT = 290,
    SQL_EXISTS = 291,
    SQL_GLOBAL = 292,
    SQL_HAVING = 293,
    SQL_INSERT = 294,
    SQL_ISNULL = 295,
    SQL_OFFSET = 296,
    SQL_RENAME = 297,
    SQL_SCHEMA = 298,
    SQL_SELECT = 299,
    SQL_SORTED = 300,
    SQL_TABLES = 301,
    SQL_UNIQUE = 302,
    SQL_UNLOAD = 303,
    SQL_UPDATE = 304,
    SQL_VALUES = 305,
    SQL_AFTER = 306,
    SQL_ALTER = 307,
    SQL_CROSS = 308,
    SQL_DELTA = 309,
    SQL_GROUP = 310,
    SQL_INDEX = 311,
    SQL_INNER = 312,
    SQL_LOCAL = 313,
    SQL_MERGE = 314,
    SQL_MINUS = 315,
    SQL_ORDER = 316,
    SQL_CHECK = 317,
    SQL_OUTER = 318,
    SQL_RIGHT = 319,
    SQL_TABLE = 320,
    SQL_UNION = 321,
    SQL_USING = 322,
    SQL_WHERE = 323,
    SQL_CALL = 324,
    SQL_DATE = 325,
    SQL_DESC = 326,
    SQL_DROP = 327,
    SQL_FILE = 328,
    SQL_FROM = 329,
    SQL_FULL = 330,
    SQL_HASH = 331,
    SQL_HINT = 332,
    SQL_INTO = 333,
    SQL_JOIN = 334,
    SQL_LEFT = 335,
    SQL_LIKE = 336,
    SQL_LOAD = 337,
    SQL_NULL = 338,
    SQL_PART = 339,
    SQL_PLAN = 340,
    SQL_SHOW = 341,
    SQL_TEXT = 342,
    SQL_TIME = 343,
    SQL_VIEW = 344,
    SQL_WITH = 345,
    SQL_CHAR = 346,
    SQL_ADD = 347,
    SQL_ALL = 348,
    SQL_AND = 349,
    SQL_ASC = 350,
    SQL_CSV = 351,
    SQL_FOR = 352,
    SQL_INT = 353,
    SQL_KEY = 354,
    SQL_NOT = 355,
    SQL_OFF = 356,
    SQL_SET = 357,
    SQL_TBL = 358,
    SQL_TOP = 359,
    SQL_USE = 360,
    SQL_AS = 361,
    SQL_BY = 362,
    SQL_IF = 363,
    SQL_IN = 364,
    SQL_IS = 365,
    SQL_OF = 366,
    SQL_ON = 367,
    SQL_OR = 368,
    SQL_TO = 369,
    SQL_EQUALS = 370,
    SQL_LESS = 371,
    SQL_GREATER = 372,
    SQL_NOTNULL = 373,
    SQL_UMINUS = 374
  };
#endif

/* Value type.  */
#if ! defined SQL_STYPE && ! defined SQL_STYPE_IS_DECLARED
typedef union SQL_STYPE SQL_STYPE;
union SQL_STYPE
{
#line 75 "parser.y" /* yacc.c:1909  */

	double fval;
	int64_t ival;
	char* sval;
	uint64_t uval;
	bool bval;
	SQLStatement* statement;
	CreateDatabaseStatement* create_database_stmt;
	DropDatabaseStatement* drop_database_stmt;
	ShowDatabasesStatement* show_databases_stmt;
	UseStatement* use_stmt;
	ShowTablesStatement* show_tables_stmt;
	DescStatement* desc_stmt;
	CreateIndexStatement* create_index_stmt;
	DropIndexStatement* drop_index_stmt;
	SelectStatement* select_stmt;
	CreateStatement* create_stmt;
	InsertStatement* insert_stmt;
	DeleteStatement* delete_stmt;
	UpdateStatement* update_stmt;
	DropStatement* drop_stmt;
	TableRef* table;
	Expr* expr;
	ColumnDefinition* column_t;
	GroupByDescription* group_t;
	UpdateClause* update_t;
	AST* stmt_list;
	std::vector<char*>* str_vec;
	std::vector<TableRef*>* table_vec;
	std::vector<ColumnDefinition*>* column_vec;
	std::vector<UpdateClause*>* update_vec;
	std::vector<Expr*>* expr_vec;
	std::vector<std::vector<Expr*>*>* expr_vecs;

#line 253 "parser.h" /* yacc.c:1909  */
};
# define SQL_STYPE_IS_TRIVIAL 1
# define SQL_STYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined SQL_LTYPE && ! defined SQL_LTYPE_IS_DECLARED
typedef struct SQL_LTYPE SQL_LTYPE;
struct SQL_LTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define SQL_LTYPE_IS_DECLARED 1
# define SQL_LTYPE_IS_TRIVIAL 1
#endif



int sql_parse (AST** result, yyscan_t scanner);

#endif /* !YY_SQL_PARSER_H_INCLUDED  */
